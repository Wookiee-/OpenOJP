#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"

void ojp_AIMod_Jump(gentity_t *self)
{
	if (!self || !self->client) return;
	self->client->ps.velocity[2] = 250;
	self->NPC->shotTime = level.time + 1000;
}

qboolean ojp_ScanForEnemies(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return qfalse;
	for (int i = 0; i < level.num_entities; i++) {
		gentity_t *ent = &g_entities[i];
		if (!ent->inuse || !ent->client) continue;
		if (ent == self) continue;
		if (ent->health <= 0) continue;
		if (OnSameTeam(self, ent)) continue;
		float dist = Distance(self->r.currentOrigin, ent->r.currentOrigin);
		if (dist < 1000.0f) {
			self->enemy = ent;
			return qtrue;
		}
	}
	return qfalse;
}

float ojp_TargetDistance(gentity_t *self, gentity_t *target, vec3_t targetorigin)
{
	if (target) return Distance(self->r.currentOrigin, target->r.currentOrigin);
	if (targetorigin) return Distance(self->r.currentOrigin, targetorigin);
	return 0.0f;
}

void ojp_ClearRoute(gentity_t *self)
{
	if (!self || !self->NPC) return;
	self->NPC->goalEntity = NULL;
	self->NPC->lastGoalEntity = NULL;
}

static void ojp_FavoriteWeapon(gentity_t *self)
{
	if (!self || !self->client) return;
	int bestWeapon = WP_SABER;
	float bestDist = 999999.0f;

	if (!self->enemy) return;
	float dist = Distance(self->r.currentOrigin, self->enemy->r.currentOrigin);

	if (dist < 200.0f && self->client->ps.weapon == WP_SABER) {
		bestWeapon = WP_SABER;
	} else if (dist < 500.0f) {
		bestWeapon = WP_BLASTER;
	} else {
		bestWeapon = WP_BLASTER;
	}

	if (self->client->ps.weapon != bestWeapon) {
		self->client->ps.weapon = bestWeapon;
	}
}

void ojp_SaberCombatHandling(gentity_t *self)
{
	if (!self || !self->client || !self->enemy) return;

	float dist = Distance(self->r.currentOrigin, self->enemy->r.currentOrigin);
	if (dist < 150.0f && level.time > self->NPC->shotTime + 1000) {
		G_Damage(self->enemy, self, self, NULL, self->enemy->r.currentOrigin, Q_irand(5, 15), DAMAGE_NO_ARMOR, MOD_SABER);
		self->NPC->shotTime = level.time;
	}
}

void ojp_MeleeCombatHandling(gentity_t *self)
{
	if (!self || !self->client || !self->enemy) return;

	float dist = Distance(self->r.currentOrigin, self->enemy->r.currentOrigin);
	if (dist < 80.0f && level.time > self->NPC->shotTime + 1500) {
		G_Damage(self->enemy, self, self, NULL, self->enemy->r.currentOrigin, Q_irand(3, 8), DAMAGE_NO_ARMOR, MOD_MELEE);
		self->NPC->shotTime = level.time;
	}
}

qboolean ojp_PassStandardEnemyChecks(gentity_t *self, gentity_t *enemy)
{
	if (!self || !enemy) return qfalse;
	if (enemy->health <= 0) return qfalse;
	if (OnSameTeam(self, enemy)) return qfalse;
	return qtrue;
}

void ojp_MoveTowardIdealAngles(gentity_t *self)
{
	if (!self || !self->client || !self->NPC) return;
}

void ojp_BotAimLeading(gentity_t *self)
{
	if (!self || !self->client || !self->enemy) return;
}
