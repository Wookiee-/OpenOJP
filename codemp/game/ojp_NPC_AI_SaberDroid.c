#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"
#include "ojp_saberbeh.h"

#define SABER_DROID_ATTACK_RANGE_OJP 100.0f

void ojp_NPC_AI_SaberDroid_Update(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;
	if (!self->enemy) return;

	vec3_t dir;
	VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
	float dist = VectorLength(dir);

	if (dist > SABER_DROID_ATTACK_RANGE_OJP) {
		VectorNormalize(dir);
		VectorMA(self->client->ps.velocity, 300.0f, dir, self->client->ps.velocity);
	} else {
		if (level.time > self->NPC->shotTime + 1500) {
			G_Damage(self->enemy, self, self, dir, self->enemy->client->ps.origin, 15, DAMAGE_NO_ARMOR, MOD_SABER);
			self->NPC->shotTime = level.time;
		}
	}
}
