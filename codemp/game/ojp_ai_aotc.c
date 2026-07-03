#include "g_local.h"
#include "ai_main.h"
#include "w_saber.h"
#include "qcommon/ojp_shared.h"

void ojp_AOTC_Update(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;
	if (self->NPC->shotTime > level.time) return;

	if (self->enemy && self->enemy->health > 0) {
		vec3_t dir;
		VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
		float dist = VectorLength(dir);

		if (dist < 200.0f && (self->enemy->client->ps.saberEventFlags & SEF_HITENEMY)) {
			NPC_SetAnim(self, SETANIM_TORSO, BOTH_PAIN2, SETANIM_FLAG_OVERRIDE | SETANIM_FLAG_HOLD);
		}
	}
}

qboolean ojp_AOTC_Attack(gentity_t *self)
{
	if (!self || !self->enemy) return qfalse;

	vec3_t dir;
	VectorSubtract(self->enemy->client->ps.origin, self->client->ps.origin, dir);
	float dist = VectorLength(dir);

	if (dist < 100.0f) {
		return qtrue;
	}
	return qfalse;
}
