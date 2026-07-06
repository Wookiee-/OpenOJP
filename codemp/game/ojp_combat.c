#include "g_local.h"
#include "b_local.h"
#include "w_saber.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"
#include "ojp_saberbeh.h"

#ifndef BUTTON_15
#define BUTTON_15 BUTTON_ALT_ATTACK
#endif

#define SABER_ELASTIC_RATIO_OJP 0.5f
#define DODGE_BOLTBLOCK_OJP 8
#define DODGE_MINDAM_OJP 5
#define DODGE_KICKCOST_OJP 5
#define STAFF_KICK_RANGE_OJP 48.0f

// Basic dodge costs by damage mod
static int BasicDodgeCosts_OJP[MOD_UNKNOWN + 1] = { 0 };
static qboolean dodgeCostsInit_OJP = qfalse;
static void InitDodgeCosts_OJP(void)
{
	if (dodgeCostsInit_OJP) return;
	memset(BasicDodgeCosts_OJP, 0, sizeof(BasicDodgeCosts_OJP));
	BasicDodgeCosts_OJP[MOD_SABER] = 40;
	BasicDodgeCosts_OJP[MOD_BLASTER] = 15;
	BasicDodgeCosts_OJP[MOD_DISRUPTOR] = 25;
	BasicDodgeCosts_OJP[MOD_BOWCASTER] = 20;
	BasicDodgeCosts_OJP[MOD_REPEATER] = 10;
	BasicDodgeCosts_OJP[MOD_REPEATER_ALT] = 30;
	BasicDodgeCosts_OJP[MOD_FLECHETTE] = 15;
	BasicDodgeCosts_OJP[MOD_THERMAL] = 20;
	BasicDodgeCosts_OJP[MOD_ROCKET] = 30;
	BasicDodgeCosts_OJP[MOD_ROCKET_SPLASH] = 15;
	dodgeCostsInit_OJP = qtrue;
}

qboolean ojp_WalkCheck(gentity_t *self)
{
	if (!self || !self->client) return qtrue;
	if (self->client->ps.groundEntityNum == ENTITYNUM_NONE) return qfalse;
	if ((self->client->ps.pm_flags & PMF_DUCKED)) return qtrue;
	float speed = VectorLength(self->client->ps.velocity);
	return (speed < 100.0f) ? qtrue : qfalse;
}

int ojp_BasicSaberBlockCost(int attackerStyle)
{
	switch (attackerStyle) {
	case SS_DUAL: return 13;
	case SS_STAFF: return 13;
	case SS_TAVION: return 14;
	case SS_FAST: return 12;
	case SS_MEDIUM: return 15;
	case SS_DESANN: return 16;
	case SS_STRONG: return 17;
	default: return 0;
	}
}

int ojp_SaberBlockCost(gentity_t *defender, gentity_t *attacker, vec3_t hitLoc)
{
	float saberBlockCost = 0;
	if (!attacker || !attacker->client || attacker->client->ps.weapon != WP_SABER) {
		saberBlockCost = DODGE_BOLTBLOCK_OJP;
	} else if (attacker->client->ps.saberMove == LS_A_LUNGE || attacker->client->ps.saberMove == LS_SPINATTACK || attacker->client->ps.saberMove == LS_SPINATTACK_DUAL) {
		saberBlockCost = 0.75f * ojp_BasicSaberBlockCost(attacker->client->ps.fd.saberAnimLevel);
	} else if (attacker->client->ps.saberMove == LS_ROLL_STAB) {
		saberBlockCost = 2.0f * ojp_BasicSaberBlockCost(attacker->client->ps.fd.saberAnimLevel);
	} else if (attacker->client->ps.saberMove == LS_A_JUMP_T__B_) {
		saberBlockCost = 4.0f * ojp_BasicSaberBlockCost(attacker->client->ps.fd.saberAnimLevel);
	} else {
		saberBlockCost = ojp_BasicSaberBlockCost(attacker->client->ps.fd.saberAnimLevel);
		if (!ojp_WalkCheck(attacker)) {
			saberBlockCost *= 1.5f;
		}
	}
	return (int)saberBlockCost;
}

qboolean ojp_BlockIsParry(gentity_t *self, gentity_t *attacker, vec3_t hitLoc)
{
	vec3_t pAngles, pRight, parrierMove, hitPos, hitFlat;
	float blockDot;
	if (!InFront(attacker->client->ps.origin, self->client->ps.origin, self->client->ps.viewangles, 0.0f)) return qfalse;
	if (PM_SaberInKnockaway(self->client->ps.saberMove)) return qtrue;
	if (ojp_BG_SaberInNonIdleDamageMove(&self->client->ps, self->localAnimIndex) || PM_SaberInBounce(self->client->ps.saberMove)) return qfalse;
	if (self->client->ps.pm_flags & PMF_DUCKED) return qfalse;

	VectorSubtract(hitLoc, self->client->ps.origin, hitPos);
	VectorSet(pAngles, 0, self->client->ps.viewangles[YAW], 0);
	AngleVectors(pAngles, NULL, pRight, NULL);
	hitFlat[0] = 0;
	hitFlat[1] = DotProduct(pRight, hitPos);
	hitFlat[2] = hitPos[2] - 10;
	VectorNormalize(hitFlat);

	parrierMove[0] = 0;
	parrierMove[1] = (float)(self->client->pers.cmd.rightmove);
	parrierMove[2] = -(float)(self->client->pers.cmd.forwardmove);
	VectorNormalize(parrierMove);

	blockDot = DotProduct(hitFlat, parrierMove);
	return (blockDot >= 0.4f) ? qtrue : qfalse;
}

qboolean ojp_BlockIsQuickParry(gentity_t *self, gentity_t *attacker, vec3_t hitLoc)
{
	if (!(self->client->pers.cmd.buttons & BUTTON_15)) return qfalse;
	return ojp_BlockIsParry(self, attacker, hitLoc);
}

qboolean ojp_InAttackParry(gentity_t *self)
{
	if ((self->client->pers.cmd.buttons & BUTTON_ATTACK) || (self->client->pers.cmd.buttons & BUTTON_ALT_ATTACK)) return qfalse;
	if (self->client->ps.userInt3 & (1 << FLAG_PARRIED_OJP)) return qfalse;
	if (BG_SaberInTransitionAny(self->client->ps.saberMove)) return qtrue;
	return qfalse;
}

qboolean ojp_ButterFingers(gentity_t *saberent, gentity_t *saberOwner, gentity_t *other, trace_t *tr)
{
	vec3_t svelocity, ovelocity, sswing, oswing, dir;
	VectorClear(svelocity); VectorClear(ovelocity);
	VectorClear(sswing); VectorClear(oswing);

	if (!saberOwner->client->ps.saberEntityNum || saberOwner->client->ps.saberInFlight) return qfalse;

	if (other && other->client && other->inuse) {
		VectorCopy(other->client->ps.velocity, oswing);
	} else {
		VectorScale(sswing, SABER_ELASTIC_RATIO_OJP, sswing);
		if (DotProduct(tr->plane.normal, sswing) > 0)
			VectorScale(tr->plane.normal, -(VectorLength(sswing)), oswing);
		else
			VectorScale(tr->plane.normal, VectorLength(sswing), oswing);
	}

	if (DotProduct(sswing, oswing) > 0)
		VectorSubtract(oswing, sswing, dir);
	else
		VectorAdd(oswing, sswing, dir);

	return qtrue;
}

void ojp_AnimateKnockdown(gentity_t *self, gentity_t *inflictor)
{
	if (self->health > 0 && !PM_InKnockDown(&self->client->ps)) {
		int throwStr = Q_irand(50, 75);
		vec3_t pushDir;
		if (inflictor) {
			VectorSubtract(self->client->ps.origin, inflictor->r.currentOrigin, pushDir);
		} else {
			AngleVectors(self->client->ps.viewangles, pushDir, NULL, NULL);
			VectorScale(pushDir, -1, pushDir);
		}
		pushDir[2] = 0;
		VectorNormalize(pushDir);
		G_Throw(self, pushDir, throwStr);
		G_Knockdown(self, inflictor, pushDir, throwStr, qtrue);
	}
}

void ojp_AnimateStun(gentity_t *self, gentity_t *inflictor, vec3_t impact)
{
	if (self->client->ps.weapon != WP_SABER) {
		G_Knockdown(self, inflictor, vec3_origin, 300, qtrue);
	} else if (!PM_SaberInBrokenParry(self->client->ps.saberMove) && !PM_InKnockDown(&self->client->ps)) {
		self->client->ps.saberMove = BG_BrokenParryForParry((self->client->ps.saberBlocked));
		self->client->ps.saberBlocked = BLOCKED_PARRY_BROKEN;
		if (self->s.number < MAX_CLIENTS)
			G_AddEvent(self, Q_irand(EV_PUSHED1, EV_PUSHED3), 0);
		else
			NPC_Pain(self, inflictor, 0);
	}
}

void ojp_DoNormalDodge(gentity_t *self, int dodgeAnim, qboolean partial)
{
	self->client->ps.forceHandExtend = HANDEXTEND_DODGE;
	self->client->ps.forceDodgeAnim = dodgeAnim;
	self->client->ps.forceHandExtendTime = level.time + 300;
	self->client->ps.weaponTime = 300;
	self->client->ps.saberMove = LS_NONE;
	if (!partial)
		self->client->ps.powerups[PW_SPEEDBURST] = level.time + 100;
	G_Sound(self, CHAN_BODY, G_SoundIndex("sound/weapons/force/speed.wav"));
}

qboolean ojp_G_DoDodge(gentity_t *self, gentity_t *shooter, vec3_t dmgOrigin, int hitLoc, int *dmg, int mod)
{
	InitDodgeCosts_OJP();
	int dpcost = BasicDodgeCosts_OJP[mod];
	qboolean partial = qfalse;
	qboolean NoAction = qfalse;

	if (!self || !self->client || !self->inuse || self->health <= 0) return qfalse;
	if (dpcost == -1) return qfalse;
	if (self->client->ps.stats[STAT_DODGE_OJP] < 30) return qfalse;
	if (self->client->ps.forceHandExtend == HANDEXTEND_DODGE) return qtrue;
	if (self->client->ps.groundEntityNum == ENTITYNUM_NONE) return qfalse;
	if (self->client->ps.forceHandExtend == HANDEXTEND_CHOKE || BG_InGrappleMove(self->client->ps.torsoAnim) > 1) return qfalse;

	if (*dmg <= DODGE_MINDAM_OJP && mod != MOD_REPEATER) {
		dpcost = (int)(dpcost * ((float)*dmg / DODGE_MINDAM_OJP));
		NoAction = qtrue;
	}

	if (dpcost < self->client->ps.stats[STAT_DODGE_OJP]) {
		ojp_G_DodgeDrain(self, shooter, dpcost);
	} else if (dpcost != 0 && self->client->ps.stats[STAT_DODGE_OJP]) {
		*dmg = (int)(*dmg * (self->client->ps.stats[STAT_DODGE_OJP] / (float)dpcost));
		ojp_G_DodgeDrain(self, shooter, self->client->ps.stats[STAT_DODGE_OJP]);
		partial = qtrue;
	} else {
		return qfalse;
	}

	if (NoAction) return qtrue;

	if (hitLoc == HL_NONE) return qfalse;

	int dodgeAnim = BOTH_DODGE_FL;
	if (self->client->ps.forceHandExtend != HANDEXTEND_DODGE && !PM_InKnockDown(&self->client->ps)) {
		ojp_DoNormalDodge(self, dodgeAnim, partial);
	}

	if (partial && *dmg > 0) return qfalse;
	return qtrue;
}

qboolean ojp_EnemyInKickRange(gentity_t *self, gentity_t *enemy)
{
	if (!self || !enemy) return qfalse;
	if (fabs(self->r.currentOrigin[2] - enemy->r.currentOrigin[2]) < 32) {
		if (DistanceHorizontal(self->r.currentOrigin, enemy->r.currentOrigin) <= (STAFF_KICK_RANGE_OJP + 8.0f + (self->r.maxs[0] * 1.5f) + (enemy->r.maxs[0] * 1.5f)))
			return qtrue;
	}
	return qfalse;
}

qboolean ojp_CanKickEntity(gentity_t *self, gentity_t *target)
{
	if (target && target->client && !BG_InKnockDown(target->client->ps.legsAnim) && ojp_EnemyInKickRange(self, target))
		return qtrue;
	return qfalse;
}

void ojp_G_AddMercBalance(gentity_t *self, int amount)
{
	self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] -= amount;
	if (self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] < 0)
		self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] = MISHAPLEVEL_NONE_OJP;
	else if (self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] > MISHAPLEVEL_MAX_OJP)
		self->client->ps.stats[STAT_MISHAP_LEVEL_OJP] = MISHAPLEVEL_MAX_OJP;
}

void ojp_BG_AddFatigue(playerState_t *ps, int fatigue)
{
	ps->stats[STAT_DODGE_OJP] -= fatigue;
	if (ps->stats[STAT_DODGE_OJP] < 0) ps->stats[STAT_DODGE_OJP] = 0;
}

void ojp_G_DodgeDrain(gentity_t *blocker, gentity_t *attacker, int amount)
{
	if (!blocker || !blocker->client) return;
	blocker->client->ps.stats[STAT_DODGE_OJP] -= amount;
	if (blocker->client->ps.stats[STAT_DODGE_OJP] < 0)
		blocker->client->ps.stats[STAT_DODGE_OJP] = 0;
}

qboolean ojp_G_StandardHumanoid(gentity_t *self)
{
	if (!self || !self->client) return qfalse;
	if (self->client->ps.weapon == WP_SABER) return qtrue;
	return qfalse;
}
