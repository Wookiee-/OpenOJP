#include "cg_local.h"
#include "qcommon/ojp_shared.h"

extern vmCvar_t ojp_drawHUDBars;

void ojp_CG_DrawDodge(void)
{
	int dodge = cg.predictedPlayerState.stats[STAT_DODGE_OJP];
	int i, tics;

	if (!ojp_drawHUDBars.integer) return;
	if (!dodge) return;

	tics = dodge / 12;
	if (tics > 8) tics = 8;

	for (i = 0; i < tics; i++) {
		CG_DrawPic(48 + i * 8, SCREEN_HEIGHT - 16, 6, 6, cgs.media.whiteShader);
	}
}

void ojp_CG_DrawBalance(void)
{
	int mishap = cg.predictedPlayerState.stats[STAT_MISHAP_LEVEL_OJP];
	int i;

	if (!ojp_drawHUDBars.integer) return;
	if (mishap <= 0) return;

	for (i = 0; i < mishap; i++) {
		CG_DrawPic(48 + i * 8, SCREEN_HEIGHT - 8, 6, 6, cgs.media.whiteShader);
	}
}
