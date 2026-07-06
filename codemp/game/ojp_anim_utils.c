#include "g_local.h"
#include "bg_local.h"
#include "bg_public.h"
#include "qcommon/ojp_shared.h"
#include "ojp_saberbeh.h"

qboolean ojp_BG_SaberInNonIdleDamageMove(playerState_t *ps, int AnimIndex)
{
	if (!ps) return qfalse;
	int move = ps->saberMove;
	return (move == LS_A_TL2BR || move == LS_A_BL2TR ||
		move == LS_A_R2L || move == LS_A_T2B ||
		move == LS_A_BACKSTAB || move == LS_A_BACK ||
		move == LS_A_FLIP_STAB || move == LS_A_FLIP_SLASH ||
		move == LS_A_JUMP_T__B_ || move == LS_A_LUNGE ||
		move == LS_A_BACKFLIP_ATK);
}

qboolean ojp_BG_SaberInFullDamageMove(playerState_t *ps)
{
	if (!ps) return qfalse;
	int move = ps->saberMove;
	return (move == LS_A_TL2BR || move == LS_A_BL2TR ||
		move == LS_A_R2L || move == LS_A_T2B);
}

qboolean ojp_BG_SaberInTransitionDamageMove(playerState_t *ps)
{
	if (!ps) return qfalse;
	int move = ps->saberMove;
	return qfalse;
}

qboolean ojp_BG_InSlowBounce(playerState_t *ps)
{
	if (!ps) return qfalse;
	return (ps->userInt3 & (1 << FLAG_SLOWBOUNCE_OJP)) ? qtrue : qfalse;
}

qboolean PM_SaberInBounce(int move)
{
	return (move >= LS_BOUNCE && move < LS_BOUNCE + 16) ? qtrue : qfalse;
}

qboolean BG_SaberInTransitionAny(int move)
{
	return (move >= LS_TRANSITION && move < LS_TRANSITION + 32) ? qtrue : qfalse;
}

qboolean PM_SaberInBrokenParry(int move)
{
	return (move == LS_PARRY_BROKEN) ? qtrue : qfalse;
}

qboolean BG_InKnockDown(int anim)
{
	switch (anim) {
	case BOTH_KNOCKDOWN1: case BOTH_KNOCKDOWN2: case BOTH_KNOCKDOWN3:
	case BOTH_KNOCKDOWN4: case BOTH_KNOCKDOWN5:
		return qtrue;
	}
	return qfalse;
}

void G_Knockdown(gentity_t *self, gentity_t *attacker, const vec3_t pushDir, float strength, qboolean breakSaberLock)
{
	if (!self || !self->client) return;
	self->client->ps.forceHandExtend = HANDEXTEND_KNOCKDOWN;
	self->client->ps.forceHandExtendTime = level.time + 1300;
	self->client->ps.otherKiller = self->s.number;
	self->client->ps.otherKillerTime = level.time + 8000;
	self->client->ps.otherKillerDebounceTime = level.time + 100;
}
