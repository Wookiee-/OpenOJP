#include "g_local.h"

#define OJP_MAX_BREAKABLE_HEALTH 200

void ojp_Breakable_Die(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
{
	if (!self) return;

	G_UseTargets(self, attacker);

	if (self->splashDamage && self->splashRadius) {
		G_RadiusDamage(self->r.currentOrigin, attacker, self->splashDamage, self->splashRadius, self, self, mod);
	}

	if (self->s.eFlags & EF_NODRAW) return;

	self->s.eFlags |= EF_NODRAW;
	self->r.contents = 0;
	self->takedamage = qfalse;

	if (self->s.pos.trType != TR_STATIONARY) {
	}
}

void ojp_Breakable_TakeDamage(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod)
{
	if (!self || !self->takedamage) return;

	self->health -= damage;

	if (self->health <= 0) {
		ojp_Breakable_Die(self, inflictor, attacker, damage, mod);
	}
}

void ojp_Breakable_Init(gentity_t *ent)
{
	if (!ent) return;

	ent->takedamage = qtrue;
	ent->health = OJP_MAX_BREAKABLE_HEALTH;
	ent->splashDamage = 0;
	ent->splashRadius = 0;
}
