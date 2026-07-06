#include "cg_local.h"

#define ZONE_SIZE 16

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
		if (dist > 800) continue; // only within range

		float yawToTarget = vectoyaw(dir);
		float yawDiff = AngleDelta(yawToTarget, cg.predictedPlayerState.viewangles[YAW]);

		// Show zone based on enemy position
		if (dist < 400) {
			float angleFromForward = AngleNormalize180(yawDiff);
			if (angleFromForward > -70 && angleFromForward < 70) {
				if (angleFromForward > 20) threatZone = 1;
				else if (angleFromForward < -20) threatZone = 2;
				else threatZone = 0;
			} else if (angleFromForward > 110 || angleFromForward < -110) {
				threatZone = 4;
			} else if (angleFromForward > 0) {
				threatZone = 3;
			} else {
				threatZone = 4;
			}
		}
	}

	if (threatZone < 0) return;

	// Draw 5 zones around crosshair
	float angles[5] = { -90, -35, -145, 35, 145 };
	const float *zoneColor;
	qboolean playerBlocking = cg.predictedPlayerState.saberBlocking ? qtrue : qfalse;

	for (j = 0; j < 5; j++) {
		float rad = angles[j] * M_PI / 180.0f;
		float px = cx + cos(rad) * 50 - ZONE_SIZE/2;
		float py = cy - sin(rad) * 50 - ZONE_SIZE/2;

		if (j == threatZone)
			zoneColor = playerBlocking ? colorTable[CT_HUD_GREEN] : colorTable[CT_YELLOW];
		else
			zoneColor = colorTable[CT_LTGREY];

		CG_DrawPic(px, py, ZONE_SIZE, ZONE_SIZE, cgs.media.whiteShader);
	}
}
