#include "g_local.h"

#define OJP_CRASH_SPEED_THRESHOLD 400.0f
#define OJP_CRASH_DAMAGE_FACTOR 0.1f

void ojp_Crash_Check(gentity_t *self)
{
	float speed;
	vec3_t vel;

	if (!self || !self->client) return;
	if (self->client->ps.groundEntityNum != ENTITYNUM_NONE) return;

	VectorCopy(self->client->ps.velocity, vel);
	speed = VectorLength(vel);

	if (speed > OJP_CRASH_SPEED_THRESHOLD) {
		int damage = (int)((speed - OJP_CRASH_SPEED_THRESHOLD) * OJP_CRASH_DAMAGE_FACTOR);
		if (damage > 0) {
			G_Damage(self, self, self, vel, self->client->ps.origin, damage, DAMAGE_NO_ARMOR, MOD_FALLING);
		}
	}
}

void ojp_Crash_Init(gentity_t *ent)
{
}
