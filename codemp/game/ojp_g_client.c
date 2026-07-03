#include "g_local.h"
#include "qcommon/ojp_shared.h"

qboolean ojp_G_ChangePlayerModel(gentity_t *ent, const char *modelName)
{
	if (!ent || !ent->client || !modelName) return qfalse;
	return qtrue;
}

void ojp_G_RemovePlayerModel(gentity_t *ent)
{
	if (!ent || !ent->client) return;
}

void ojp_G_RemoveWeaponModels(gentity_t *ent)
{
	if (!ent || !ent->client) return;
	ent->client->ps.weapon = WP_NONE;
}

qboolean ojp_G_CheckCharmed(gentity_t *self, gentity_t *target)
{
	if (!self || !self->client || !target) return qfalse;
	if (self->client->ps.fd.forcePowersActive & (1 << FP_TELEPATHY)) return qtrue;
	return qfalse;
}
