#include "g_local.h"
#include "ai_main.h"
#include "qcommon/ojp_shared.h"

extern bot_state_t *botstates[MAX_CLIENTS];

int ojp_FindBotType(int clientNum)
{
	if (!(g_entities[clientNum].r.svFlags & SVF_BOT))
		return -1;
	return 0;
}

float ojp_VectorDistance(vec3_t v1, vec3_t v2)
{
	vec3_t dir;
	VectorSubtract(v2, v1, dir);
	return VectorLength(dir);
}

float ojp_DistancetoClosestPlayer(vec3_t position, int enemyTeam)
{
	int i;
	float bestdist = 9999;
	for (i = 0; i < MAX_CLIENTS; i++) {
		gentity_t *player = &g_entities[i];
		if (!player || !player->client || !player->inuse ||
			player->client->pers.connected != CON_CONNECTED ||
			player->client->sess.sessionTeam == TEAM_SPECTATOR)
			continue;
		if (enemyTeam != -1 && player->client->playerTeam != enemyTeam)
			continue;
		if (player->health <= 0 || (player->s.eFlags & EF_DEAD))
			continue;
		float dist = Distance(player->client->ps.origin, position);
		if (dist < bestdist) bestdist = dist;
	}
	return bestdist;
}

qboolean ojp_PassStandardEnemyChecks_AotC(bot_state_t *bs, gentity_t *en)
{
	if (!bs || !en || !en->client) return qfalse;
	if (en->health < 1 || !en->takedamage) return qfalse;

	if (en->s.weapon == WP_SABER &&
		((en->client->ps.saberHolstered >= 2 && en->client->saber[0].numBlades >= 2) ||
			(en->client->ps.saberHolstered >= 2 && en->client->saber[1].type) ||
			(en->client->ps.saberHolstered && !en->client->saber[1].type &&
				en->client->saber[0].numBlades <= 1)))
		return qfalse;

	return qtrue;
}

qboolean ojp_DontBlockAllies(bot_state_t *bs)
{
	if (!bs) return qfalse;
	if (bs->squadLeader && bs->squadLeader->health > 0 &&
		Distance(bs->origin, bs->squadLeader->r.currentOrigin) < 64)
		return qtrue;
	if (bs->lovednum > 0) {
		int i;
		for (i = 0; i < bs->lovednum; i++) {
			if (bs->loved[i].level >= 2) return qtrue;
		}
	}
	return qfalse;
}

void ojp_ResetWPTimers(bot_state_t *bs)
{
	if (!bs) return;
	bs->wpDestSwitchTime = 0;
}

qboolean ojp_AttackLocalBreakable(gentity_t *self)
{
	if (!self || !self->NPC) return qfalse;
	return qfalse;
}
