#include "g_local.h"
#include "bg_local.h"
#include "qcommon/ojp_shared.h"
#include "ojp_saberbeh.h"

qboolean ojp_PM_InForceGetUp(playerState_t *ps);

#define MAX_REAL_PASSTHRU 8
#define REALTRACEDATADEFAULT -9999

static int RealTraceContent_ent[MAX_REAL_PASSTHRU];
static int RealTraceContent_val[MAX_REAL_PASSTHRU];

void ojp_BG_LetGoofLedge(playerState_t *ps)
{
	ps->pm_flags &= ~PMF_STUCK_TO_WALL;
	ps->torsoTimer = 0;
	ps->legsTimer = 0;
}

int ojp_PM_MinGetUpTime(playerState_t *ps)
{
	if (ps->legsAnim == BOTH_PLAYER_PA_3_FLY || ps->legsAnim == BOTH_LK_DL_ST_T_SB_1_L || ps->legsAnim == BOTH_RELEASED)
		return 200;
	if (ps->clientNum < MAX_CLIENTS) {
		int getUpTime = 400;
		if (ps->fd.forcePowerLevel[FP_LEVITATION] >= FORCE_LEVEL_3) return getUpTime + 400;
		if (ps->fd.forcePowerLevel[FP_LEVITATION] == FORCE_LEVEL_2) return getUpTime + 200;
		if (ps->fd.forcePowerLevel[FP_LEVITATION] == FORCE_LEVEL_1) return getUpTime + 100;
		return getUpTime;
	}
	return 200;
}

qboolean ojp_PM_AdjustAnglesForKnockdown(playerState_t *ps, usercmd_t *ucmd, int localAnimIndex)
{
	if (PM_InKnockDown(ps)) {
		if (!ojp_PM_InForceGetUp(ps) && (ps->legsTimer > ojp_PM_MinGetUpTime(ps) || ps->clientNum >= MAX_CLIENTS ||
			ps->legsAnim == BOTH_GETUP1 || ps->legsAnim == BOTH_GETUP2 || ps->legsAnim == BOTH_GETUP3 ||
			ps->legsAnim == BOTH_GETUP4 || ps->legsAnim == BOTH_GETUP5)) {
			ucmd->forwardmove = 0; ucmd->rightmove = 0;
		}
		if (ps->clientNum >= MAX_CLIENTS) VectorClear(ps->moveDir);
		if (!ojp_PM_InForceGetUp(ps)) {
			ucmd->angles[PITCH] = ANGLE2SHORT(ps->viewangles[PITCH]) - ps->delta_angles[PITCH];
			ucmd->angles[YAW] = ANGLE2SHORT(ps->viewangles[YAW]) - ps->delta_angles[YAW];
			return qtrue;
		}
	}
	return qfalse;
}

qboolean ojp_PM_CanForceFall(playerState_t *ps, usercmd_t *ucmd, int gametype)
{
	return (!BG_InRoll(ps, ps->legsAnim) && !PM_InKnockDown(ps) && !BG_InDeathAnim(ps->legsAnim) &&
		!BG_SaberInSpecialAttack(ps->torsoAnim) && !BG_SaberInAttack(ps->saberMove) &&
		BG_CanUseFPNow(gametype, ps, ucmd->serverTime, FP_HEAL) && !(ps->pm_flags & PMF_JUMP_HELD) &&
		ucmd->upmove > 10 && ps->velocity[2] < -200 && ps->groundEntityNum == ENTITYNUM_NONE &&
		ps->fd.forcePowerLevel[FP_LEVITATION] > FORCE_LEVEL_1 && ps->fd.forcePower > 5);
}

int ojp_BlockedforQuad(int quad)
{
	switch (quad) {
	case Q_BR: return BLOCKED_LOWER_RIGHT;
	case Q_R: return BLOCKED_UPPER_RIGHT;
	case Q_TR: return BLOCKED_UPPER_RIGHT;
	case Q_T: return BLOCKED_TOP;
	case Q_TL: return BLOCKED_UPPER_LEFT;
	case Q_L: return BLOCKED_UPPER_LEFT;
	case Q_BL: return BLOCKED_LOWER_LEFT;
	case Q_B: return BLOCKED_LOWER_LEFT;
	default: return BLOCKED_TOP;
	}
}

int ojp_InvertQuad(int quad)
{
	switch (quad) {
	case Q_B: return Q_B;
	case Q_BR: return Q_BL;
	case Q_R: return Q_L;
	case Q_TR: return Q_TL;
	case Q_T: return Q_T;
	case Q_TL: return Q_TR;
	case Q_L: return Q_R;
	case Q_BL: return Q_BR;
	default: return quad;
	}
}

void ojp_InitRealTraceContent(void)
{
	int i;
	for (i = 0; i < MAX_REAL_PASSTHRU; i++) {
		RealTraceContent_ent[i] = REALTRACEDATADEFAULT;
		RealTraceContent_val[i] = REALTRACEDATADEFAULT;
	}
}

qboolean ojp_AddRealTraceContent(int entityNum)
{
	int i;
	if (entityNum == ENTITYNUM_WORLD || entityNum == ENTITYNUM_NONE) return qtrue;
	for (i = 0; i < MAX_REAL_PASSTHRU; i++) {
		if (RealTraceContent_ent[i] == REALTRACEDATADEFAULT && RealTraceContent_val[i] == REALTRACEDATADEFAULT) {
			RealTraceContent_ent[i] = entityNum;
			RealTraceContent_val[i] = g_entities[entityNum].r.contents;
			g_entities[entityNum].r.contents = 0;
			return qtrue;
		}
	}
	return qfalse;
}

void ojp_RestoreRealTraceContent(void)
{
	int i;
	for (i = 0; i < MAX_REAL_PASSTHRU; i++) {
		if (RealTraceContent_ent[i] != REALTRACEDATADEFAULT) {
			if (RealTraceContent_val[i] != REALTRACEDATADEFAULT) {
				g_entities[RealTraceContent_ent[i]].r.contents = RealTraceContent_val[i];
				RealTraceContent_ent[i] = REALTRACEDATADEFAULT;
				RealTraceContent_val[i] = REALTRACEDATADEFAULT;
			}
		} else break;
	}
}

qboolean ojp_PM_InForceGetUp(playerState_t *ps)
{
	switch (ps->legsAnim) {
	case BOTH_FORCE_GETUP_F1: case BOTH_FORCE_GETUP_F2:
	case BOTH_FORCE_GETUP_B1: case BOTH_FORCE_GETUP_B2: case BOTH_FORCE_GETUP_B3:
	case BOTH_FORCE_GETUP_B4: case BOTH_FORCE_GETUP_B5: case BOTH_FORCE_GETUP_B6:
	case BOTH_GETUP_BROLL_B: case BOTH_GETUP_BROLL_F: case BOTH_GETUP_BROLL_L: case BOTH_GETUP_BROLL_R:
	case BOTH_GETUP_FROLL_B: case BOTH_GETUP_FROLL_F: case BOTH_GETUP_FROLL_L: case BOTH_GETUP_FROLL_R:
		if (ps->legsTimer) return qtrue;
	}
	return qfalse;
}

qboolean ojp_PM_InAirKickingAnim(int anim)
{
	switch (anim) {
	case BOTH_A7_KICK_F_AIR: case BOTH_A7_KICK_B_AIR:
	case BOTH_A7_KICK_R_AIR: case BOTH_A7_KICK_L_AIR:
		return qtrue;
	}
	return qfalse;
}
