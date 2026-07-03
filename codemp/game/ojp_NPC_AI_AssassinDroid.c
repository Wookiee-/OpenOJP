#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"

#define ASSASSIN_DROID_ATTACK_RANGE_OJP 800.0f
#define ASSASSIN_DROID_STRAFE_SPEED_OJP 200.0f

void ojp_NPC_AI_AssassinDroid_Update(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;
	if (!self->enemy) return;

	vec3_t dir;
	VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
	float dist = VectorLength(dir);

	if (dist < ASSASSIN_DROID_ATTACK_RANGE_OJP) {
		vec3_t right;
		CrossProduct(dir, self->client->ps.velocity, right);
		VectorNormalize(right);
		VectorMA(self->client->ps.velocity, ASSASSIN_DROID_STRAFE_SPEED_OJP, right, self->client->ps.velocity);

		if (level.time > self->NPC->shotTime + 1000) {
			vec3_t fwd;
			AngleVectors(self->client->ps.viewangles, fwd, NULL, NULL);
			G_Damage(self->enemy, self, self, fwd, self->enemy->client->ps.origin, 10, DAMAGE_NO_ARMOR, MOD_BLASTER);
			self->NPC->shotTime = level.time;
		}
	}
}
