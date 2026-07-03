#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"

#define SAND_CREATURE_ATTACK_RANGE_OJP 150.0f
#define SAND_CREATURE_BURROW_TIME_OJP 5000

void ojp_NPC_AI_Sand_creature_Update(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;

	if (self->health > 0 && self->enemy) {
		vec3_t dir;
		VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
		float dist = VectorLength(dir);

		if (dist < SAND_CREATURE_ATTACK_RANGE_OJP) {
			if (level.time > self->NPC->shotTime + 2000) {
				G_Damage(self->enemy, self, self, dir, self->enemy->client->ps.origin, 30, DAMAGE_NO_ARMOR, MOD_MELEE);
				self->NPC->shotTime = level.time;
			}
		} else {
			VectorNormalize(dir);
			VectorMA(self->client->ps.velocity, 200.0f, dir, self->client->ps.velocity);
		}
	}
}
