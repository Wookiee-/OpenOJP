#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"

void ojp_Hybrid_Update(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;

	if (self->enemy && self->enemy->health > 0) {
		vec3_t dir;
		VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
		float dist = VectorLength(dir);

		if (dist > 300.0f && self->client->ps.stats[STAT_WEAPONS] & (1 << WP_BOWCASTER)) {
			self->NPC->goalEntity = self->enemy;
			self->NPC->combatPoint = qtrue;
		}

		if (dist < 150.0f) {
			G_Damage(self->enemy, self, self, dir, self->enemy->client->ps.origin, 5, DAMAGE_NO_ARMOR, MOD_MELEE);
		}
	}
}

qboolean ojp_Hybrid_Attack(gentity_t *self)
{
	if (!self || !self->enemy) return qfalse;

	vec3_t dir;
	VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
	float dist = VectorLength(dir);

	return (dist < 500.0f) ? qtrue : qfalse;
}
