#include "cg_local.h"

// ============================================================
// TrueView: Eye position shift
// Called after camera origin is computed
// ============================================================
void ojp_TrueView_AdjustEye(vec3_t vieworg, vec3_t eyeAxis)
{
	if (cg_trueeyeposition.value != 0.0f) {
		VectorMA(vieworg, cg_trueeyeposition.value, eyeAxis, vieworg);
	}
}

// ============================================================
// Holster weapon: Load weapon model into ghoul2
// ============================================================
void ojp_Holster_LoadWeapon(clientInfo_t *ci, int saberNum)
{
	if (!ci || !ci->ghoul2Model) return;
	if (ci->ghoul2HolsterWeapons[saberNum]) return;

	trap->G2API_InitGhoul2Model(&ci->ghoul2HolsterWeapons[saberNum],
		ci->saber[saberNum].model, 0, ci->saber[saberNum].skin, 0, 0, 0);

	if (ci->ghoul2HolsterWeapons[saberNum]) {
		trap->G2API_SetSkin(ci->ghoul2HolsterWeapons[saberNum], 0, ci->saber[saberNum].skin, 0);
	}
}

// ============================================================
// TrueView: Check if we should force thirdperson for saber
// ============================================================
qboolean ojp_TrueView_ForceThirdPerson(void)
{
	if (!cg_trueinvertsaber.integer) return qfalse;
	if (cg.predictedPlayerState.weapon != WP_SABER && cg.predictedPlayerState.weapon != WP_MELEE) return qfalse;
	return (cg_trueinvertsaber.integer == 2) ? qtrue : qfalse;
}

// ============================================================
// TrueView: Check if guns should render in thirdperson
// ============================================================
qboolean ojp_TrueView_ShowGuns(void)
{
	if (!cg_trueguns.integer) return qfalse;
	return (cg.predictedPlayerState.weapon == WP_SABER || cg.predictedPlayerState.weapon == WP_MELEE);
}
