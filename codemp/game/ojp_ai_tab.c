#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"

#define TAB_ATTACK_RANGE_OJP 200.0f
#define TAB_RETREAT_HEALTH_OJP 30

void ojp_Tab_Update(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;

	if (!self->enemy) return;

	vec3_t dir;
	VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
	float dist = VectorLength(dir);

	if (self->health < TAB_RETREAT_HEALTH_OJP) {
		VectorMA(self->client->ps.velocity, -400.0f, dir, self->client->ps.velocity);
		if (self->client->ps.groundEntityNum != ENTITYNUM_NONE) {
			self->client->ps.velocity[2] = 200.0f;
		}
	} else if (dist < TAB_ATTACK_RANGE_OJP) {
		self->NPC->goalEntity = self->enemy;
		self->NPC->combatPoint = qtrue;
	}
}

qboolean ojp_Tab_Attack(gentity_t *self)
{
	if (!self || !self->enemy) return qfalse;

	vec3_t dir;
	VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
	float dist = VectorLength(dir);

	return (dist < TAB_ATTACK_RANGE_OJP) ? qtrue : qfalse;
}
