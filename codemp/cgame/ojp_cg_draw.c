#include "cg_local.h"
#include "qcommon/ojp_shared.h"

#define DODGE_MAX_OJP 100
#define DODGEBAR_H 65.0f
#define DODGEBAR_W 13.0f
#define DODGEBAR_X 538.0f
#define DODGEBAR_Y 367.0f

#define MISHAPBAR_H 65.0f
#define MISHAPBAR_W 13.0f
#define MISHAPBAR_X 555.0f
#define MISHAPBAR_Y 367.0f

void ojp_CG_DrawDodge(void)
{
	vec4_t aColor;
	float percent;
	int dodgeStat;

	dodgeStat = cg.predictedPlayerState.stats[STAT_DODGE_OJP];
	if (dodgeStat < 0) dodgeStat = 0;
	if (dodgeStat > DODGE_MAX_OJP) dodgeStat = DODGE_MAX_OJP;

	percent = ((float)dodgeStat / DODGE_MAX_OJP) * DODGEBAR_H;

	// Default color: green
	aColor[0] = 0.0f;
	aColor[1] = 0.613f;
	aColor[2] = 0.097f;
	aColor[3] = 0.8f;

	// Critical dodge - flash red
	if (dodgeStat < DODGE_CRITICALLEVEL_OJP)
	{
		aColor[0] = 1.0f;
		aColor[1] = 0.0f;
		aColor[2] = 0.0f;
		aColor[3] = 0.8f;
		if (cg.dodgeHUDNextFlashTime < cg.time)
		{
			cg.dodgeHUDNextFlashTime = cg.time + 400;
			trap->S_StartSound(NULL, 0, CHAN_LOCAL, cgs.media.noforceSound);
		}
	}
	else
	{
		cg.dodgeHUDNextFlashTime = 0;
	}

	if (percent > DODGEBAR_H) percent = DODGEBAR_H;
	if (percent < 0.1f) percent = 0.1f;

	// Draw the filled portion of the bar
	CG_FillRect(DODGEBAR_X, DODGEBAR_Y + (DODGEBAR_H - percent), DODGEBAR_W, DODGEBAR_H - (DODGEBAR_H - percent), aColor);
}

void ojp_CG_DrawBalance(void)
{
	float percent;
	int mishapLevel;

	mishapLevel = cg.predictedPlayerState.stats[STAT_MISHAP_LEVEL_OJP];

	percent = ((float)mishapLevel / MISHAPLEVEL_MAX_OJP) * MISHAPBAR_H;
	if (percent > MISHAPBAR_H) percent = MISHAPBAR_H;
	if (percent < 0.1f) percent = 0.1f;

	{
		vec4_t mColor;
		// Color based on mishap severity
		if (mishapLevel >= MISHAPLEVEL_FULL_OJP) {
			mColor[0] = 1.0f; mColor[1] = 0.0f; mColor[2] = 0.0f;
		} else if (mishapLevel >= MISHAPLEVEL_HEAVY_OJP) {
			mColor[0] = 1.0f; mColor[1] = 0.5f; mColor[2] = 0.0f;
		} else {
			mColor[0] = 1.0f; mColor[1] = 1.0f; mColor[2] = 0.0f;
		}
		mColor[3] = 0.8f;

		CG_FillRect(MISHAPBAR_X, MISHAPBAR_Y + (MISHAPBAR_H - percent), MISHAPBAR_W, MISHAPBAR_H - (MISHAPBAR_H - percent), mColor);
	}
}
