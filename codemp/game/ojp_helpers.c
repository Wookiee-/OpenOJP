#include "g_local.h"
#include "bg_local.h"
#include "qcommon/ojp_shared.h"

qboolean ojp_BG_InLedgeMove(int anim)
{
	switch (anim) {
	case BOTH_LEDGE_GRAB:
	case BOTH_LEDGE_HOLD:
	case BOTH_LEDGE_LEFT:
	case BOTH_LEDGE_RIGHT:
	case BOTH_LEDGE_MERCPULL:
		return qtrue;
	}
	return qfalse;
}

qboolean ojp_PM_LockedAnim(int anim)
{
	switch (anim) {
	case BOTH_KYLE_PA_1: case BOTH_KYLE_PA_2: case BOTH_KYLE_PA_3:
	case BOTH_PLAYER_PA_1: case BOTH_PLAYER_PA_2: case BOTH_PLAYER_PA_3: case BOTH_PLAYER_PA_3_FLY:
	case BOTH_TAVION_SCEPTERGROUND: case BOTH_TAVION_SWORDPOWER:
	case BOTH_SCEPTER_START: case BOTH_SCEPTER_HOLD: case BOTH_SCEPTER_STOP:
	case BOTH_GRABBED: case BOTH_RELEASED:
	case BOTH_HANG_IDLE: case BOTH_HANG_ATTACK: case BOTH_HANG_PAIN:
		return qtrue;
	}
	return qfalse;
}

float ojp_BG_GetLegsAnimPoint(playerState_t *ps, int AnimIndex)
{
	float attackAnimLength = 0;
	float currentPoint = 0;
	float animSpeedFactor = 1.0f;
	float animPercentage = 0;

	if (AnimIndex < 0 || AnimIndex >= MAX_ANIMATIONS)
		return 0;

	if (bgAllAnims[AnimIndex].anims[ps->legsAnim].numFrames < 2) {
		attackAnimLength = fabs((float)(bgAllAnims[AnimIndex].anims[ps->legsAnim].frameLerp)) * (1.0f / animSpeedFactor);
	} else {
		attackAnimLength = (bgAllAnims[AnimIndex].anims[ps->legsAnim].numFrames - 1) *
			fabs((float)(bgAllAnims[AnimIndex].anims[ps->legsAnim].frameLerp)) * (1.0f / animSpeedFactor);
	}
	if (attackAnimLength > 1) attackAnimLength--;

	currentPoint = (float)ps->legsTimer;
	animPercentage = currentPoint / attackAnimLength;
	return animPercentage;
}

int ojp_ForceFallBrakeRate[NUM_FORCE_POWER_LEVELS] = { 0, 50, 60, 70 };

qboolean ojp_PM_KnockDownAnimExtended(int anim)
{
	switch (anim) {
	case BOTH_KNOCKDOWN1: case BOTH_KNOCKDOWN2: case BOTH_KNOCKDOWN3:
	case BOTH_KNOCKDOWN4: case BOTH_KNOCKDOWN5: case BOTH_RELEASED:
	case BOTH_LK_DL_ST_T_SB_1_L: case BOTH_PLAYER_PA_3_FLY:
		return qtrue;
	}
	return qfalse;
}

qboolean ojp_PM_InAttackRoll(int anim)
{
	switch (anim) {
	case BOTH_GETUP_BROLL_B: case BOTH_GETUP_BROLL_F:
	case BOTH_GETUP_FROLL_B: case BOTH_GETUP_FROLL_F:
		return qtrue;
	}
	return qfalse;
}

qboolean ojp_PM_GoingToAttackDown(playerState_t *ps)
{
	if (BG_StabDownAnim(ps->torsoAnim) || ps->saberMove == LS_A_LUNGE ||
		ps->saberMove == LS_A_JUMP_T__B_ || ps->saberMove == LS_A_T2B)
		return qtrue;
	return qfalse;
}

void ojp_PM_DoPunch(gentity_t *self)
{
	int desTAnim = (self->client->ps.torsoAnim == BOTH_MELEE1) ? BOTH_MELEE2 : BOTH_MELEE1;
	NPC_SetAnim(self, SETANIM_TORSO, desTAnim, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD | SETANIM_FLAG_HOLDLESS);
	if (self->client->ps.torsoAnim == desTAnim)
		self->client->ps.weaponTime = self->client->ps.torsoTimer;
}

int ojp_NumberOfWeapons(playerState_t *ps)
{
	int num = 0;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_BRYAR_PISTOL)) num++;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_BLASTER)) num++;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_DISRUPTOR)) num++;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_BOWCASTER)) num++;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_REPEATER)) num++;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_DEMP2)) num++;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_FLECHETTE)) num++;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_ROCKET_LAUNCHER)) num++;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_THERMAL)) num++;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_TRIP_MINE)) num++;
	if (ps->stats[STAT_WEAPONS] & (1 << WP_DET_PACK)) num++;
	return num;
}

int ojp_JumpHeightDeduction(playerState_t *ps)
{
	if (!ojp_NumberOfWeapons(ps)) return 0;
	if (ps->fd.forcePowerLevel[FP_LEVITATION] == FORCE_LEVEL_0) return 0;
	if (BG_InBackFlip(ps->legsAnim)) return 200;
	return 0;
}
