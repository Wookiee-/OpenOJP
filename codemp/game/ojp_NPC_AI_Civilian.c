#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"

#define CIVILIAN_FLEE_DISTANCE_OJP 300.0f
#define CIVILIAN_FLEE_SPEED_OJP 300.0f

void ojp_NPC_AI_Civilian_Update(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;

	gentity_t *threat = NULL;
	float threatDist = CIVILIAN_FLEE_DISTANCE_OJP;

	int i;`r`n	for (i = 0; i < level.num_entities; i++) {
		gentity_t *ent = &g_entities[i];
		if (!ent->inuse || !ent->client) continue;
		if (ent == self) continue;

		vec3_t dir;
		VectorSubtract(ent->client->ps.origin, self->client->ps.origin, dir);
		float dist = VectorLength(dir);

		if (dist < threatDist && ent->health > 0 && ent->client->sess.sessionTeam != self->client->sess.sessionTeam) {
			threat = ent;
			threatDist = dist;
		}
	}

	if (threat) {
		vec3_t away;
		VectorSubtract(self->client->ps.origin, threat->client->ps.origin, away);
		VectorNormalize(away);
		VectorMA(self->client->ps.velocity, CIVILIAN_FLEE_SPEED_OJP, away, self->client->ps.velocity);
	}
}

