#include "g_local.h"
#include "qcommon/ojp_shared.h"

// ============================================================
// OJP Client Plugin Detection
// ============================================================
qboolean ojp_AllPlayersHaveClientPlugin(void)
{
	int i;
	for (i = 0; i < level.maxclients; i++) {
		gentity_t *ent = &g_entities[i];
		if (!ent->inuse || !ent->client) continue;
		if (ent->r.svFlags & SVF_BOT) continue;
		if (!ent->client->pers.ojpClientPlugIn) return qfalse;
	}
	return qtrue;
}

// ============================================================
// OJP Point Spread - controls score spread for balancing
// ============================================================
int ojp_PointSpread(void)
{
	return 100;
}

// ============================================================
// OJP Spectator - dedicated spectator mode after death
// ============================================================
qboolean ojp_OJP_Spectator(gentity_t *ent)
{
	if (!ent || !ent->client) return qfalse;
	if (ojp_lastmanstanding.integer && ent->health <= 0) return qtrue;
	return qfalse;
}

// ============================================================
// LMS - Last Man Standing
// ============================================================
void ojp_LMS_Respawn(gentity_t *ent)
{
	if (!ent || !ent->client) return;
	if (!ojp_lastmanstanding.integer) return;

	if (ent->client->pers.ojpLMSLives <= 0) {
		ent->client->sess.sessionTeam = TEAM_SPECTATOR;
		ent->client->ps.pm_type = PM_SPECTATOR;
		return;
	}

	ent->health = 100;
	ent->client->ps.stats[STAT_HEALTH] = 100;
	ent->client->pers.ojpLMSLives--;
}

void ojp_LMS_Init(gentity_t *ent)
{
	if (!ent || !ent->client) return;
	if (!ojp_lastmanstanding.integer) return;
	ent->client->pers.ojpLMSLives = ojp_lmslives.integer;
}

// ============================================================
// FFA Respawn Timer
// ============================================================
qboolean ojp_FFARespawnTimer_Active(void)
{
	return (ojp_ffaRespawnTimer.integer && level.gametype == GT_FFA) ? qtrue : qfalse;
}

// ============================================================
// Skip Cutscenes
// ============================================================
qboolean ojp_SkipCutscenes(void)
{
	return (ojp_skipcutscenes.integer > 0) ? qtrue : qfalse;
}

// ============================================================
// MOTD Display
// ============================================================
void ojp_SendMOTD(gentity_t *ent)
{
	if (!ent || !ent->client) return;

	if (ent->client->pers.ojpClientPlugIn) {
		if (ojp_clientMOTD.string[0]) {
			trap_SendServerCommand(ent - g_entities, va("cp \"%s\"", ojp_clientMOTD.string));
		}
	} else {
		if (ojp_MOTD.string[0]) {
			trap_SendServerCommand(ent - g_entities, va("print \"%s\n\"", ojp_MOTD.string));
		}
	}
}

// ============================================================
// True Balance - force power balancing
// ============================================================
qboolean ojp_TrueBalance_Enabled(void)
{
	return (ojp_trueBalance.integer) ? qtrue : qfalse;
}

// ============================================================
// Model Scale
// ============================================================
qboolean ojp_ModelScale_Enabled(void)
{
	return (ojp_modelscaleEnabled.integer) ? qtrue : qfalse;
}
