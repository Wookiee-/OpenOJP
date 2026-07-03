#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"

void ojp_NPC_Vehicle_Update(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;

	if (!self->enemy) return;

	vec3_t dir;
	VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
	float dist = VectorLength(dir);

	if (dist > 200.0f) {
		VectorNormalize(dir);
		VectorMA(self->client->ps.velocity, 400.0f, dir, self->client->ps.velocity);
	}

	if (dist < 500.0f && level.time > self->NPC->shotTime + 1500) {
		vec3_t fwd;
		AngleVectors(self->client->ps.viewangles, fwd, NULL, NULL);
		G_Damage(self->enemy, self, self, fwd, self->enemy->client->ps.origin, 20, DAMAGE_NO_ARMOR, MOD_VEHICLE);
		self->NPC->shotTime = level.time;
	}
}

void ojp_NPC_Vehicle_Init(gentity_t *self)
{
	if (!self) return;
	self->flags |= FL_SHIELDED;
	self->health = 200;
	self->maxHealth = 200;
}
