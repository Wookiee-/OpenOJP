#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"

#define ROCKET_TROOPER_MIN_RANGE_OJP 300.0f
#define ROCKET_TROOPER_MAX_RANGE_OJP 1000.0f

void ojp_NPC_AI_RocketTrooper_Update(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;
	if (!self->enemy) return;

	vec3_t dir;
	VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
	float dist = VectorLength(dir);

	if (dist > ROCKET_TROOPER_MAX_RANGE_OJP) {
		VectorNormalize(dir);
		VectorMA(self->client->ps.velocity, 200.0f, dir, self->client->ps.velocity);
	} else if (dist < ROCKET_TROOPER_MIN_RANGE_OJP) {
		VectorNormalize(dir);
		VectorMA(self->client->ps.velocity, -200.0f, dir, self->client->ps.velocity);
	}

	if (dist < ROCKET_TROOPER_MAX_RANGE_OJP && dist > ROCKET_TROOPER_MIN_RANGE_OJP) {
		if (level.time > self->NPC->shotTime + 2000) {
			vec3_t fwd;
			AngleVectors(self->client->ps.viewangles, fwd, NULL, NULL);
			G_Damage(self->enemy, self, self, fwd, self->enemy->client->ps.origin, 25, DAMAGE_NO_ARMOR, MOD_ROCKET);
			self->NPC->shotTime = level.time;
		}
	}
}
