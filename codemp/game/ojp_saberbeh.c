#include "g_local.h"
#include "b_local.h"
#include "ojp_saberbeh.h"
#include "w_saber.h"
#include "qcommon/ojp_shared.h"
#include "ai_main.h"

static void ClearSabMech(ojp_sabmech_t *sabmech)
{
	sabmech->doStun = qfalse;
	sabmech->doKnockdown = qfalse;
	sabmech->doButterFingers = qfalse;
	sabmech->doParry = qfalse;
	sabmech->doSlowBounce = qfalse;
	sabmech->doHeavySlowBounce = qfalse;
#ifdef _DEBUG
	sabmech->behaveMode = SABBEHAVE_NONE_OJP;
#endif
}

qboolean ojp_SabBeh_RollBalance(gentity_t *self, ojp_sabmech_t *mechSelf, qboolean forceMishap)
{
	int randNum;
	if (self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] <= MISHAPLEVEL_FULL_OJP) {
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
	else if (self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] <= MISHAPLEVEL_HEAVY_OJP) {
		randNum = Q_irand(0, 99);
		if (randNum < 0 || forceMishap) {
			mechSelf->doHeavySlowBounce = qtrue;
			return qtrue;
		}
	}
	else if (self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] <= MISHAPLEVEL_LIGHT_OJP) {
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

void ojp_SabBeh_AnimateSlowBounce(gentity_t *self, gentity_t *inflictor)
{
	if (!self || !self->client) return;
	NPC_SetAnim(self, SETANIM_BOTH, BOTH_PAIN2, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD);
}

void ojp_SabBeh_AnimateHeavySlowBounce(gentity_t *self, gentity_t *inflictor)
{
	if (!self || !self->client) return;
	NPC_SetAnim(self, SETANIM_BOTH, BOTH_PAIN2, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD);
}

void ojp_SabBeh_RunSaberBehavior(gentity_t *self, ojp_sabmech_t *mechSelf,
	gentity_t *otherOwner, ojp_sabmech_t *mechOther, vec3_t hitLoc,
	qboolean *didHit, qboolean otherHitSaberBlade)
{
	if (!self || !self->client || !otherOwner) return;

	if (mechSelf->doParry) {
		if (otherOwner->client) {
			otherOwner->client->ps.saberEventFlags |= SEF_PARRIED;
		}
	}

	if (mechSelf->doStun) {
		mechSelf->doKnockdown = qtrue;
	}

	if (mechSelf->doKnockdown && self->client) {
		self->client->ps.saberEventFlags |= SEF_KNOCKDOWN_OJP;
	}

	if (mechSelf->doButterFingers && self->client) {
		self->client->ps.saberEventFlags |= SEF_DISARMED_OJP;
	}

	ClearSabMech(mechSelf);
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
			extern void AnimateKnockdown(gentity_t *self, gentity_t *inflictor);
			AnimateKnockdown(self, inflictor);
		}
		else if (mechSelf.doStun)
		{
			extern void AnimateStun(gentity_t *self, gentity_t *inflictor, vec3_t impactPoint);
			AnimateStun(self, inflictor, self->client->ps.origin);
		}
		else if (mechSelf.doSlowBounce)
		{
			ojp_SabBeh_AnimateSlowBounce(self, inflictor);
		}
		else if (mechSelf.doHeavySlowBounce)
		{
			ojp_SabBeh_AnimateHeavySlowBounce(self, inflictor);
		}
	}
}

void ojp_SabBeh_AddBalance(gentity_t *self, ojp_sabmech_t *mechSelf, int amount, qboolean attack)
{
	if (!self || !self->client) return;

	self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] -= amount;

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
	if (ps->stats[STAT_MISHAP_LEVEL_OJP] > 0)
	{
		if (ps->stats[STAT_MISHAP_LEVEL_OJP] > MISHAPLEVEL_LIGHT_OJP)
		{
			ps->stats[STAT_MISHAP_LEVEL_OJP] = MISHAPLEVEL_LIGHT_OJP;
		}
		else
		{
			ps->stats[STAT_MISHAP_LEVEL_OJP]--;
		}
	}
}
