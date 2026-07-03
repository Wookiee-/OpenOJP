#include "g_local.h"

gentity_t *ojp_podium1;
gentity_t *ojp_podium2;
gentity_t *ojp_podium3;

void ojp_UpdateTournamentInfo(void)
{
	int i;
	gentity_t *player;
	int playerClientNum;
	int n, accuracy, perfect, msglen;
	int buflen;
	int score1, score2;
	qboolean won;
	char buf[32];
	char msg[MAX_STRING_CHARS];

	player = NULL;
	for (i = 0; i < level.maxclients; i++) {
		player = &g_entities[i];
		if (!player->inuse) continue;
		if (!(player->r.svFlags & SVF_BOT)) break;
	}
	if (!player || i == level.maxclients) return;
	playerClientNum = i;

	CalculateRanks();

	if (level.clients[playerClientNum].sess.sessionTeam == TEAM_SPECTATOR) {
		Com_sprintf(msg, sizeof(msg), "postgame %i %i 0 0 0 0 0 0 0 0 0 0 0",
			level.numNonSpectatorClients, playerClientNum);
	} else {
		if (player->client->accuracy_shots) {
			accuracy = player->client->accuracy_hits * 100 / player->client->accuracy_shots;
		} else {
			accuracy = 0;
		}
		won = qfalse;
		if (g_gametype.integer >= GT_CTF) {
			score1 = level.teamScores[TEAM_RED];
			score2 = level.teamScores[TEAM_BLUE];
			if (level.clients[playerClientNum].sess.sessionTeam == TEAM_RED) {
				won = (level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE]);
			} else {
				won = (level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED]);
			}
		} else {
			if (&level.clients[playerClientNum] == &level.clients[level.sortedClients[0]]) {
				won = qtrue;
				score1 = level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE];
				score2 = level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE];
			} else {
				score2 = level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE];
				score1 = level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE];
			}
		}
		if (won && player->client->ps.persistant[PERS_KILLED] == 0) {
			perfect = 1;
		} else {
			perfect = 0;
		}
		Com_sprintf(msg, sizeof(msg),
			"postgame %i %i %i %i %i %i %i %i %i %i %i %i %i %i",
			level.numNonSpectatorClients, playerClientNum, accuracy,
			player->client->ps.persistant[PERS_IMPRESSIVE_COUNT],
			player->client->ps.persistant[PERS_EXCELLENT_COUNT],
			player->client->ps.persistant[PERS_DEFEND_COUNT],
			player->client->ps.persistant[PERS_ASSIST_COUNT],
			player->client->ps.persistant[PERS_GAUNTLET_FRAG_COUNT],
			player->client->ps.persistant[PERS_SCORE],
			perfect, score1, score2, level.time,
			player->client->ps.persistant[PERS_CAPTURES]);
	}

	msglen = strlen(msg);
	for (i = 0; i < level.numNonSpectatorClients; i++) {
		n = level.sortedClients[i];
		Com_sprintf(buf, sizeof(buf), " %i %i %i", n,
			level.clients[n].ps.persistant[PERS_RANK],
			level.clients[n].ps.persistant[PERS_SCORE]);
		buflen = strlen(buf);
		if (msglen + buflen + 1 >= sizeof(msg)) break;
		strcat(msg, buf);
	}
	trap_SendConsoleCommand(EXEC_APPEND, msg);
}

void ojp_ScoreboardMessage(gentity_t *ent)
{
	char entry[1024];
	char string[14000];
	int stringlength = 0;
	int i, j;
	int numSorted;

	CalculateRanks();

	numSorted = level.numNonSpectatorClients;
	if (numSorted > MAX_CLIENTS) numSorted = MAX_CLIENTS;

	Com_sprintf(string, sizeof(string),
		"{\"playerteam\":[");
	stringlength = strlen(string);

	for (i = 0; i < numSorted; i++) {
		int cl = level.sortedClients[i];
		int team = level.clients[cl].sess.sessionTeam;
		if (team == TEAM_SPECTATOR) continue;

		Com_sprintf(entry, sizeof(entry),
			"{\"n\":%i,\"s\":%i,\"t\":%i},",
			cl, level.clients[cl].ps.persistant[PERS_SCORE], team);
		if (stringlength + strlen(entry) >= sizeof(string)) break;
		strcat(string, entry);
		stringlength += strlen(entry);
	}

	if (string[stringlength - 1] == ',') string[stringlength - 1] = ']';
	else strcat(string, "]");
	strcat(string, "}");

	trap_SendServerCommand(ent - g_entities, string);
}
