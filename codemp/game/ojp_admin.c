#include "g_local.h"
#include "qcommon/ojp_shared.h"

extern vmCvar_t ojp_chatProtectTime;

// Chat spam protection
int ojp_lastChatTime[MAX_CLIENTS];

void ojp_InitAdmin(void)
{
	memset(ojp_lastChatTime, 0, sizeof(ojp_lastChatTime));
}

qboolean ojp_CheckChatSpam(gentity_t *ent)
{
	if (!ent || !ent->client) return qfalse;
	if (ent->r.svFlags & SVF_BOT) return qfalse;
	if (level.time < ojp_lastChatTime[ent->s.number] + ojp_chatProtectTime.integer) {
		int waitTime = (ojp_lastChatTime[ent->s.number] + ojp_chatProtectTime.integer - level.time) / 1000 + 1;
		trap_SendServerCommand(ent - g_entities, va("cp \"Please Don't Spam. Wait %i Seconds Before Trying Again.\"", waitTime));
		return qtrue;
	}
	ojp_lastChatTime[ent->s.number] = level.time;
	return qfalse;
}

// Team kill tracking
void ojp_CheckTKAutoKickBan(gentity_t *ent)
{
	if (!ent || !ent->client) return;
	ent->client->sess.sessionTKCount++;

	if (g_autoBanTKSpammers.integer > 0 && ent->client->sess.sessionTKCount >= g_autoBanTKSpammers.integer) {
		trap_SendConsoleCommand(EXEC_INSERT, va("addip %s\n", ent->client->sess.IP));
		trap_SendServerCommand(-1, va("print \"%s^7 was banned for team killing\n\"", ent->client->pers.netname));
		trap_SendConsoleCommand(EXEC_INSERT, va("clientkick %i\n", ent->s.number));
	} else if (g_autoKickTKSpammers.integer > 0 && ent->client->sess.sessionTKCount >= g_autoKickTKSpammers.integer) {
		trap_SendConsoleCommand(EXEC_INSERT, va("clientkick %i\n", ent->s.number));
		trap_SendServerCommand(-1, va("print \"%s^7 was kicked for team killing\n\"", ent->client->pers.netname));
	} else if (g_autoKickTKSpammers.integer > 0 || g_autoBanTKSpammers.integer > 0) {
		int remaining = (g_autoKickTKSpammers.integer > 0 ? g_autoKickTKSpammers.integer : g_autoBanTKSpammers.integer) - ent->client->sess.sessionTKCount;
		trap_SendServerCommand(ent - g_entities, va("cp \"Warning: TK %i more time(s) and you will be %s\"", remaining,
			g_autoBanTKSpammers.integer > 0 ? "banned" : "kicked"));
	}
}

// Kill spam tracking
void ojp_CheckKillSpam(gentity_t *ent)
{
	if (!ent || !ent->client) return;
	if (ent->client->sess.sessionTeam == TEAM_SPECTATOR) return;

	ent->client->sess.sessionKillCount++;

	if (g_autoBanKillSpammers.integer > 0 && ent->client->sess.sessionKillCount >= g_autoBanKillSpammers.integer) {
		trap_SendConsoleCommand(EXEC_INSERT, va("addip %s\n", ent->client->sess.IP));
		trap_SendServerCommand(-1, va("print \"%s^7 was banned for kill spamming\n\"", ent->client->pers.netname));
		trap_SendConsoleCommand(EXEC_INSERT, va("clientkick %i\n", ent->s.number));
	} else if (g_autoKickKillSpammers.integer > 0 && ent->client->sess.sessionKillCount >= g_autoKickKillSpammers.integer) {
		trap_SendConsoleCommand(EXEC_INSERT, va("clientkick %i\n", ent->s.number));
		trap_SendServerCommand(-1, va("print \"%s^7 was kicked for kill spamming\n\"", ent->client->pers.netname));
	} else if (g_autoKickKillSpammers.integer > 0 || g_autoBanKillSpammers.integer > 0) {
		int remaining = (g_autoKickKillSpammers.integer > 0 ? g_autoKickKillSpammers.integer : g_autoBanKillSpammers.integer) - ent->client->sess.sessionKillCount;
		trap_SendServerCommand(ent - g_entities, va("cp \"Warning: %i more kill(s) and you will be %s\"", remaining,
			g_autoBanKillSpammers.integer > 0 ? "banned" : "kicked"));
	}
}

// Centersay command handler
void ojp_CenterSay(void)
{
	char text[1024];
	int i;

	trap_Argv(1, text, sizeof(text));
	if (!text[0]) {
		Com_Printf("Usage: centersay <text>\n");
		return;
	}

	for (i = 0; i < level.numConnectedClients; i++) {
		int clientNum = level.sortedClients[i];
		trap_SendServerCommand(clientNum, va("cp \"%s\"", text));
	}

	Com_Printf(va("centersay: %s\n", text));
}

// IP ban commands
void ojp_AddIP(void)
{
	char ip[64];
	trap_Argv(1, ip, sizeof(ip));
	if (!ip[0]) {
		Com_Printf("Usage: addip <ip-mask>\n");
		return;
	}
	trap_SendConsoleCommand(EXEC_APPEND, va("addip %s\n", ip));
}

void ojp_RemoveIP(void)
{
	char ip[64];
	trap_Argv(1, ip, sizeof(ip));
	if (!ip[0]) {
		Com_Printf("Usage: removeip <ip-mask>\n");
		return;
	}
	trap_SendConsoleCommand(EXEC_APPEND, va("removeip %s\n", ip));
}

void ojp_ListIP(void)
{
	trap_SendConsoleCommand(EXEC_APPEND, "listip\n");
}
