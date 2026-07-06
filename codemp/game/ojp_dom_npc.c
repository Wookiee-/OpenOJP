#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"
#include "ojp_dom_flags.h"

void ojp_DomNPC_Update(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;

	int closestFlag = -1;
	float closestDist = 999999.0f;
	int myTeam = self->client->sess.sessionTeam;

	int numFlags = ojp_Dom_GetNumFlags();
	int i;`r`n	for (i = 0; i < numFlags; i++) {
		int flagTeam = ojp_Dom_GetFlagTeam(i);
		vec3_t flagOrigin;
		ojp_Dom_GetFlagOrigin(i, flagOrigin);

		if (flagTeam != myTeam) {
			vec3_t dir;
			VectorSubtract(flagOrigin, self->client->ps.origin, dir);
			float dist = VectorLength(dir);
			if (dist < closestDist) {
				closestDist = dist;
				closestFlag = i;
			}
		}
	}

	if (closestFlag >= 0) {
		vec3_t flagOrigin;
		ojp_Dom_GetFlagOrigin(closestFlag, flagOrigin);
		if (self->NPC) {
			self->NPC->goalEntity = NULL;
		}
	}
}

