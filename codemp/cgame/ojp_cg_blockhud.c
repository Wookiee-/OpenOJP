#include "cg_local.h"

#define BLOCK_ZONES 7
#define ZONE_SIZE 12

void ojp_CG_DrawBlockHUD(void)
{
	int i, j;
	float cx = SCREEN_WIDTH / 2;
	float cy = SCREEN_HEIGHT / 2;
	int threatZone = -1;
	float bestYawDiff = 999;
	centity_t *cent;

	// Scan for nearby enemies with sabers
	for (i = 0; i < MAX_CLIENTS; i++) {
		cent = &cg_entities[i];
		if (!cent->currentState.number) continue;
		if (cent->currentState.number == cg.predictedPlayerState.clientNum) continue;
		if (cent->currentState.eType != ET_PLAYER) continue;
		if (cent->currentState.weapon != WP_SABER) continue;

		vec3_t dir;
		VectorSubtract(cent->lerpOrigin, cg.predictedPlayerState.origin, dir);
		float dist = VectorLength(dir);
		if (dist > 800) continue;

		float yawToTarget = vectoyaw(dir);
		float yawDiff = AngleDelta(yawToTarget, cg.predictedPlayerState.viewangles[YAW]);

		if (dist < 400) {
			float a = AngleNormalize180(yawDiff);
			// Map yaw offset to 7 zones
			int zones[BLOCK_ZONES];
			if (a > -20 && a < 20) threatZone = 0;         // top
			else if (a >= 20 && a < 65) threatZone = 3;    // right
			else if (a >= 65 && a < 110) threatZone = 1;   // upper right
			else if (a >= 110) threatZone = 2;              // lower right
			else if (a > -65 && a <= -20) threatZone = 6;   // left
			else if (a > -110 && a <= -65) threatZone = 4;  // upper left
			else if (a <= -110) threatZone = 5;              // lower left
		}
	}

	if (threatZone < 0) return;

	// 7 zones around crosshair (no straight down)
	// Angles: top, upper-right, lower-right, right, upper-left, lower-left, left
	float angles[BLOCK_ZONES] = { -90, -45, 45, 0, -135, 135, 180 };
	const float *zoneColor;
	qboolean playerBlocking = cg.predictedPlayerState.saberBlocking ? qtrue : qfalse;

	for (j = 0; j < BLOCK_ZONES; j++) {
		float rad = angles[j] * M_PI / 180.0f;
		float px = cx + cos(rad) * 48 - ZONE_SIZE/2;
		float py = cy - sin(rad) * 48 - ZONE_SIZE/2;

		if (j == threatZone)
			zoneColor = playerBlocking ? colorTable[CT_HUD_GREEN] : colorTable[CT_YELLOW];
		else
			zoneColor = colorTable[CT_DKGREY2];

		CG_DrawPic(px, py, ZONE_SIZE, ZONE_SIZE, cgs.media.whiteShader);
	}
}
