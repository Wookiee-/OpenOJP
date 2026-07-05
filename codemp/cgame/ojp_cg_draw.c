#include "cg_local.h"
#include "qcommon/ojp_shared.h"

void ojp_CG_DrawDodge(void)
{
	int dodge = cg.predictedPlayerState.stats[STAT_DODGE_OJP];
	int maxDodge = 100;
	float frac;
	int tics;
	int i;

	if (!dodge) return;

	frac = (float)dodge / maxDodge;
	tics = (int)(frac * 8);
	if (tics > 8) tics = 8;

	for (i = 0; i < tics; i++) {
		float x, y;
		x = 10 + i * 12;
		y = 440;

		if (dodge < 30)
			CG_DrawPic(x, y, 10, 10, cgs.media.redSaberGlowShader);
		else if (dodge < 60)
			CG_DrawPic(x, y, 10, 10, cgs.media.yellowSaberGlowShader);
		else
			CG_DrawPic(x, y, 10, 10, cgs.media.greenSaberGlowShader);
	}
}

void ojp_CG_DrawBalance(void)
{
	int mishap = cg.predictedPlayerState.stats[STAT_MISHAP_LEVEL_OJP];
	int i;
	float x, y;

	if (mishap <= 0) return;

	for (i = 0; i < mishap; i++) {
		x = 10 + i * 12;
		y = 455;
		CG_DrawPic(x, y, 10, 10, cgs.media.redSaberGlowShader);
	}
}
