#include "cg_local.h"

#define BLOCK_ZONES 7

void ojp_CG_DrawBlockHUD(void)
{
	int i, j, j2;
	float cx = SCREEN_WIDTH / 2;
	float cy = SCREEN_HEIGHT / 2;
	int threatZone = -1;
	float bestDist = 999;
	centity_t *cent;

	// Scan for nearest enemy threat
	for (i = 0; i < MAX_CLIENTS; i++) {
		cent = &cg_entities[i];
		if (!cent->currentState.number) continue;
		if (cent->currentState.number == cg.predictedPlayerState.clientNum) continue;
		if (cent->currentState.eType != ET_PLAYER) continue;
		if (cent->currentState.weapon != WP_SABER) continue;

		vec3_t dir;
		VectorSubtract(cent->lerpOrigin, cg.predictedPlayerState.origin, dir);
		float dist = VectorLength(dir);
		if (dist > 800 || dist >= bestDist) continue;

		bestDist = dist;
		float yawToTarget = vectoyaw(dir);
		float yawDiff = AngleDelta(yawToTarget, cg.predictedPlayerState.viewangles[YAW]);
		float a = AngleNormalize180(yawDiff);

		if (a > -20 && a < 20) threatZone = 0;
		else if (a >= 20 && a < 65) threatZone = 3;
		else if (a >= 65 && a < 110) threatZone = 1;
		else if (a >= 110) threatZone = 2;
		else if (a > -65 && a <= -20) threatZone = 6;
		else if (a > -110 && a <= -65) threatZone = 4;
		else threatZone = 5;
	}

	if (threatZone < 0) return;

	// 7 angles evenly spaced around top half + sides (no bottom)
	float angles[BLOCK_ZONES] = { -90, -50, -130, -10, -170, 10, 170 };
	const float *zoneColor;
	qboolean playerBlocking = cg.predictedPlayerState.saberBlocking ? qtrue : qfalse;
	float ringRadius = 28;

	for (j = 0; j < BLOCK_ZONES; j++) {
		if (j == threatZone)
			zoneColor = colorTable[playerBlocking ? CT_HUD_GREEN : CT_YELLOW];
		else
			zoneColor = colorTable[CT_DKGREY];

		trap->R_SetColor(zoneColor);

		for (j2 = -1; j2 <= 1; j2 += 2) {
			float a = (angles[j] + j2 * 6) * M_PI / 180.0f;
			float px = cx + cos(a) * ringRadius - 3;
			float py = cy - sin(a) * ringRadius - 3;
			CG_DrawPic(px, py, 6, 6, cgs.media.whiteShader);
		}
	}
	trap->R_SetColor(colorTable[CT_WHITE]);
}
