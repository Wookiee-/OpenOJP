#include "g_local.h"

void ojp_BlockSystem_Init(void)
{
}

qboolean ojp_CanBlock(gentity_t *ent)
{
	if (!ent || !ent->client) return qtrue;
	if (!ojp_manualBlock.integer) return qtrue;
	// Manual block: only block when alt_attack button is held
	return (ent->client->pers.cmd.buttons & BUTTON_ALT_ATTACK) ? qtrue : qfalse;
}
