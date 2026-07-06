#include "g_local.h"

void ojp_BlockSystem_Init(void)
{
}

qboolean ojp_CanBlock(gentity_t *ent)
{
	if (!ent || !ent->client) return qtrue;
	if (!ojp_manualBlock.integer) return qtrue;
	return ent->client->pers.blockButton;
}
