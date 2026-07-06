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
