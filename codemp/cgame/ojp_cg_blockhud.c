#include "cg_local.h"

#define BLOCK_ZONES 5
#define ZONE_SIZE 20

void ojp_CG_DrawBlockHUD(void)
{
	int i;
	float cx = SCREEN_WIDTH / 2;
	float cy = SCREEN_HEIGHT / 2;
	qboolean threatActive = qfalse;
	int threatZone = -1;

	// Check if local player's saber is blocking
	if (!cg.predictedPlayerState.saberBlocking) return;
	if (cg.predictedPlayerState.weapon != WP_SABER) return;

	// Map player's current block direction to a zone
	int blocked = cg.predictedPlayerState.saberBlocked;
	switch (blocked) {
	case BLOCKED_TOP: threatZone = 0; threatActive = qtrue; break;
	case BLOCKED_UPPER_RIGHT: threatZone = 1; threatActive = qtrue; break;
	case BLOCKED_UPPER_LEFT: threatZone = 2; threatActive = qtrue; break;
	case BLOCKED_LOWER_RIGHT: threatZone = 3; threatActive = qtrue; break;
	case BLOCKED_LOWER_LEFT: threatZone = 4; threatActive = qtrue; break;
	}

	if (!threatActive) return;

	// Draw 5 zone indicators around crosshair
	// Spread them in a fan: top, upper-right, upper-left, lower-right, lower-left
	float angles[BLOCK_ZONES] = { -90, -35, -145, 35, 145 };
	const float *zoneColor;

	for (i = 0; i < BLOCK_ZONES; i++) {
		float rad = angles[i] * M_PI / 180.0f;
		float px = cx + cos(rad) * 55 - ZONE_SIZE/2;
		float py = cy - sin(rad) * 55 - ZONE_SIZE/2;

		if (i == threatZone)
			zoneColor = colorTable[CT_HUD_GREEN];
		else
			zoneColor = colorTable[CT_HUD_RED];

		CG_DrawPic(px, py, ZONE_SIZE, ZONE_SIZE, cgs.media.whiteShader);
	}
}
