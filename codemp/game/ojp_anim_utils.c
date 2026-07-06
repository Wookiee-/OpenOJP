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
	return qfalse;
}

qboolean ojp_BG_InSlowBounce(playerState_t *ps)
{
	if (!ps) return qfalse;
	return (ps->userInt3 & (1 << FLAG_SLOWBOUNCE_OJP)) ? qtrue : qfalse;
}

