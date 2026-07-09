#include "g_local.h"
#include "b_local.h"
#include "ojp_saberbeh.h"
#include "w_saber.h"
#include "qcommon/ojp_shared.h"
#include "ai_main.h"

extern void ojp_BG_AddFatigue(playerState_t *ps, int fatigue);
extern void WP_SaberBlockNonRandom(gentity_t *self, vec3_t hitloc, qboolean missileBlock);
extern int ojp_SaberBlockCost(gentity_t *defender, gentity_t *attacker, vec3_t hitLoc);
extern qboolean WP_SabersCheckLock(gentity_t *ent1, gentity_t *ent2);
extern qboolean BG_SuperBreakWinAnim(int anim);
extern qboolean ojp_BG_SaberInNonIdleDamageMove(playerState_t *ps, int AnimIndex);
extern void NPC_Pain(gentity_t *self, gentity_t *attacker, int damage);

static void ClearSabMech(ojp_sabmech_t *sabmech)
{
	sabmech->doStun = qfalse;
	sabmech->doKnockdown = qfalse;
	sabmech->doButterFingers = qfalse;
	sabmech->doParry = qfalse;
	sabmech->doSlowBounce = qfalse;
	sabmech->doHeavySlowBounce = qfalse;
}

qboolean ojp_SabBeh_RollBalance(gentity_t *self, ojp_sabmech_t *mechSelf, qboolean forceMishap)
{
	int randNum;
	int mishap = self->client->ps.stats[STAT_MISHAP_LEVEL_OJP];
	if (mishap >= MISHAPLEVEL_FULL_OJP) {
		mechSelf->doButterFingers = qtrue;
		return qtrue;
	}
	else if (self->client->ps.stats[STAT_DODGE_OJP] < DODGE_CRITICALLEVEL_OJP) {
		randNum = Q_irand(0, 99);
		if (randNum < 0 || forceMishap) {
			mechSelf->doHeavySlowBounce = qtrue;
			return qtrue;
		}
	}
	else if (mishap >= MISHAPLEVEL_HEAVY_OJP) {
		randNum = Q_irand(0, 99);
		if (randNum < 0 || forceMishap) {
			mechSelf->doHeavySlowBounce = qtrue;
			return qtrue;
		}
	}
	else if (mishap >= MISHAPLEVEL_LIGHT_OJP) {
		randNum = Q_irand(0, 99);
		if (randNum < 0 || forceMishap) {
			mechSelf->doSlowBounce = qtrue;
			return qtrue;
		}
	}
	else if (forceMishap) {
		mechSelf->doSlowBounce = qtrue;
		return qtrue;
	}
	return qfalse;
}

void ojp_SabBeh_AddBalance(gentity_t *self, ojp_sabmech_t *mechSelf, int amount, qboolean attack)
{
	if (!self || !self->client) return;

	self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] += amount;

	if (self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] < MISHAPLEVEL_NONE_OJP)
	{
		self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] = MISHAPLEVEL_NONE_OJP;
	}
	else if (self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] > MISHAPLEVEL_MAX_OJP)
	{
		int randNum = Q_irand(0, 2);
		switch (randNum)
		{
		case 0:
			mechSelf->doButterFingers = qtrue;
			break;
		case 1:
			mechSelf->doKnockdown = qtrue;
			break;
		};
		self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] = MISHAPLEVEL_HEAVY_OJP;
	}
}

void ojp_BG_ReduceMishapLevel(playerState_t *ps)
{
	if (!ps) return;
	if (ps->stats[STAT_MISHAP_LEVEL_OJP] > MISHAPLEVEL_NONE_OJP)
	{
		ps->stats[STAT_MISHAP_LEVEL_OJP]--;
	}
}

void ojp_SabBeh_AttackVsAttack(gentity_t *self, ojp_sabmech_t *mechSelf,
	gentity_t *otherOwner, ojp_sabmech_t *mechOther)
{
	qboolean atkfake = (self->client->ps.userInt3 & (1 << FLAG_ATTACKFAKE_OJP)) ? qtrue : qfalse;
	qboolean otherfake = (otherOwner->client->ps.userInt3 & (1 << FLAG_ATTACKFAKE_OJP)) ? qtrue : qfalse;

	if (atkfake && !otherfake)
	{
		ojp_SabBeh_AddBalance(self, mechSelf, 1, qtrue);
		if (WP_SabersCheckLock(self, otherOwner))
		{
			self->client->ps.userInt3 |= (1 << FLAG_SABERLOCK_ATTACKER_OJP);
			self->client->ps.saberBlocked = BLOCKED_NONE;
			otherOwner->client->ps.saberBlocked = BLOCKED_NONE;
		}
		ojp_SabBeh_AddBalance(otherOwner, mechOther, -1, qtrue);
	}
	else if (otherfake && !atkfake)
	{
		if (WP_SabersCheckLock(otherOwner, self))
		{
			self->client->ps.saberBlocked = BLOCKED_NONE;
			otherOwner->client->ps.userInt3 |= (1 << FLAG_SABERLOCK_ATTACKER_OJP);
			otherOwner->client->ps.saberBlocked = BLOCKED_NONE;
		}
		ojp_SabBeh_AddBalance(self, mechSelf, -1, qtrue);
		ojp_SabBeh_AddBalance(otherOwner, mechOther, 1, qtrue);
	}
	else
	{
		ojp_SabBeh_AddBalance(self, mechSelf, 1, qtrue);
		ojp_SabBeh_AddBalance(otherOwner, mechOther, 1, qtrue);
	}
}

void ojp_SabBeh_AttackVsBlock(gentity_t *attacker, ojp_sabmech_t *mechAttacker,
	gentity_t *blocker, ojp_sabmech_t *mechBlocker, vec3_t hitLoc, qboolean hitSaberBlade,
	qboolean *attackerMishap, qboolean *blockerMishap)
{
	qboolean startSaberLock = qfalse;
	qboolean parried = ojp_BlockIsParry(blocker, attacker, hitLoc);
	qboolean atkparry = ojp_InAttackParry(blocker);
	qboolean atkfake = (attacker->client->ps.userInt3 & (1 << FLAG_ATTACKFAKE_OJP)) ? qtrue : qfalse;

	if (BG_SuperBreakWinAnim(attacker->client->ps.torsoAnim))
	{
		*attackerMishap = ojp_SabBeh_RollBalance(attacker, mechAttacker, qtrue);
		ojp_SabBeh_AddBalance(attacker, mechAttacker, 2, qtrue);
		ojp_SabBeh_AddBalance(blocker, mechBlocker, -1, qfalse);
	}
	else if (atkfake)
	{
		if (parried)
		{
			*attackerMishap = ojp_SabBeh_RollBalance(attacker, mechAttacker, atkparry);
			ojp_SabBeh_AddBalance(attacker, mechAttacker, MPCOST_PARRIED_ATTACKFAKE_OJP, qtrue);
			if (blocker->client->pers.blockButton)
			{
				attacker->client->ps.userInt3 |= (1 << FLAG_QUICKPARRY_OJP);
			}
			else
			{
				attacker->client->ps.userInt3 |= (1 << FLAG_PARRIED_OJP);
			}
			ojp_SabBeh_AddBalance(blocker, mechBlocker, MPCOST_PARRYING_ATTACKFAKE_OJP, qfalse);
			ojp_BG_AddFatigue(&blocker->client->ps, FPCOST_PARRYING_PURE_OJP);
		}
		else
		{
			ojp_SabBeh_AddBalance(attacker, mechAttacker, -1, qtrue);
			if (attacker->client->ps.fd.saberAnimLevel == SS_DESANN)
				ojp_SabBeh_AddBalance(blocker, mechBlocker, 2, qfalse);
			if (WP_SabersCheckLock(attacker, blocker))
			{
				attacker->client->ps.userInt3 |= (1 << FLAG_SABERLOCK_ATTACKER_OJP);
				attacker->client->ps.saberBlocked = BLOCKED_NONE;
				blocker->client->ps.saberBlocked = BLOCKED_NONE;
				startSaberLock = qtrue;
			}
		}
	}
	else if (hitSaberBlade && ojp_BG_InSlowBounce(&blocker->client->ps)
		&& blocker->client->ps.userInt3 & (1 << FLAG_OLDSLOWBOUNCE_OJP)
		&& attacker->client->ps.fd.saberAnimLevel == SS_TAVION)
	{
		mechBlocker->doButterFingers = qtrue;
		blocker->client->ps.stats[STAT_MISHAP_LEVEL_OJP] = MISHAPLEVEL_NONE_OJP;
		ojp_SabBeh_AddBalance(attacker, mechAttacker, -3, qtrue);
	}
	else
	{
		if (parried)
		{
			if (attacker->client->ps.saberMove == LS_A_LUNGE
				|| attacker->client->ps.saberMove == LS_SPINATTACK
				|| attacker->client->ps.saberMove == LS_SPINATTACK_DUAL)
			{
				*attackerMishap = ojp_SabBeh_RollBalance(attacker, mechAttacker, qtrue);
			}
			else
			{
				*attackerMishap = ojp_SabBeh_RollBalance(attacker, mechAttacker, atkparry);
			}
			ojp_SabBeh_AddBalance(attacker, mechAttacker, MPCOST_PARRIED_OJP, qtrue);
			if (blocker->client->pers.blockButton)
			{
				attacker->client->ps.userInt3 |= (1 << FLAG_QUICKPARRY_OJP);
			}
			else
			{
				attacker->client->ps.userInt3 |= (1 << FLAG_PARRIED_OJP);
			}
			ojp_SabBeh_AddBalance(blocker, mechBlocker, MPCOST_PARRYING_OJP, qfalse);
			ojp_BG_AddFatigue(&blocker->client->ps, FPCOST_PARRYING_PURE_OJP);
		}
		else
		{
			ojp_SabBeh_AddBalance(attacker, mechAttacker, -1, qtrue);
			if (attacker->client->ps.fd.saberAnimLevel == SS_TAVION)
				ojp_SabBeh_AddBalance(blocker, mechBlocker, 2, qfalse);
			else if (attacker->client->ps.fd.saberAnimLevel == SS_STRONG)
				blocker->client->ps.fd.forcePower -= 2;
		}
	}

	if (!OnSameTeam(attacker, blocker) || g_friendlySaber.integer)
	{
		if (parried)
		{
			mechBlocker->doParry = qtrue;
		}
		else if (!startSaberLock)
		{
			blocker->client->ps.saberLockFrame = 0;
			WP_SaberBlockNonRandom(blocker, hitLoc, qfalse);
		}
	}

	ojp_G_DodgeDrain(blocker, attacker, ojp_SaberBlockCost(blocker, attacker, hitLoc));
	ojp_BG_AddFatigue(&blocker->client->ps, 1);
}

void ojp_SabBeh_AnimateSlowBounce(gentity_t *self, gentity_t *inflictor)
{
	if (!self || !self->client) return;
	self->client->ps.userInt3 |= (1 << FLAG_SLOWBOUNCE_OJP);
	if (self->s.number < MAX_CLIENTS)
	{
		G_AddEvent(self, Q_irand(EV_PUSHED1, EV_PUSHED3), 0);
	}
	else
	{
		NPC_Pain(self, inflictor, 0);
	}
	self->client->ps.saberBlocked = BLOCKED_ATK_BOUNCE;
}

void ojp_SabBeh_AnimateHeavySlowBounce(gentity_t *self, gentity_t *inflictor)
{
	if (!self || !self->client) return;
	self->client->ps.userInt3 |= (1 << FLAG_SLOWBOUNCE_OJP);
	self->client->ps.userInt3 |= (1 << FLAG_OLDSLOWBOUNCE_OJP);
	if (self->s.number < MAX_CLIENTS)
	{
		G_AddEvent(self, Q_irand(EV_PUSHED1, EV_PUSHED3), 0);
	}
	else
	{
		NPC_Pain(self, inflictor, 0);
	}
	self->client->ps.saberBlocked = BLOCKED_ATK_BOUNCE;
}

void ojp_SabBeh_RunSaberBehavior(gentity_t *self, ojp_sabmech_t *mechSelf,
	gentity_t *otherOwner, ojp_sabmech_t *mechOther, vec3_t hitLoc,
	qboolean *didHit, qboolean otherHitSaberBlade)
{
	qboolean selfMishap = qfalse;
	qboolean otherMishap = qfalse;

	if (!self || !self->client) return;

	ClearSabMech(mechSelf);
	if (mechOther) ClearSabMech(mechOther);

	if (!otherOwner || !otherOwner->client)
		return;

	if (ojp_BG_SaberInNonIdleDamageMove(&self->client->ps, self->localAnimIndex))
	{
		if (ojp_BG_SaberInNonIdleDamageMove(&otherOwner->client->ps, otherOwner->localAnimIndex))
		{
			ojp_SabBeh_AttackVsAttack(self, mechSelf, otherOwner, mechOther);
		}
		else if (ojp_SaberCanBlock(otherOwner, self, qfalse, hitLoc, -1, -1))
		{
			ojp_SabBeh_AttackVsBlock(self, mechSelf, otherOwner, mechOther, hitLoc, otherHitSaberBlade,
				&selfMishap, &otherMishap);
			if (didHit) *didHit = qfalse;
		}
	}
	else if (ojp_SaberCanBlock(self, otherOwner, qfalse, hitLoc, -1, -1))
	{
		if (ojp_BG_SaberInNonIdleDamageMove(&otherOwner->client->ps, otherOwner->localAnimIndex))
		{
			ojp_SabBeh_AttackVsBlock(otherOwner, mechOther, self, mechSelf, hitLoc, qtrue,
				&otherMishap, &selfMishap);
		}
	}
}

void ojp_G_RollBalance(gentity_t *self, gentity_t *inflictor, qboolean forceMishap)
{
	ojp_sabmech_t mechSelf;
	if (!self || !self->client) return;
	ClearSabMech(&mechSelf);

	if (ojp_SabBeh_RollBalance(self, &mechSelf, forceMishap))
	{
		if (mechSelf.doButterFingers)
		{
			extern qboolean saberKnockOutOfHand(gentity_t *saberent, gentity_t *saberOwner, vec3_t velocity);
			saberKnockOutOfHand(&g_entities[self->client->ps.saberEntityNum], self, vec3_origin);
		}
		if (mechSelf.doKnockdown)
		{
			ojp_AnimateKnockdown(self, inflictor);
		}
		else if (mechSelf.doStun)
		{
			ojp_AnimateStun(self, inflictor, self->client->ps.origin);
		}
		else if (mechSelf.doSlowBounce)
			ojp_SabBeh_AnimateSlowBounce(self, inflictor);
		else if (mechSelf.doHeavySlowBounce)
			ojp_SabBeh_AnimateHeavySlowBounce(self, inflictor);
	}
}

qboolean ojp_SabBeh_ButtonforSaberLock(gentity_t *self)
{
	if (!self || !self->client) return qfalse;
	return (self->client->pers.cmd.buttons & BUTTON_ATTACK) ? qtrue : qfalse;
}


