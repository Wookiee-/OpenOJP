#include "g_local.h"
#include "qcommon/ojp_shared.h"

#define DOM_SPAWN_INTERVAL_OJP 5000

typedef struct {
	vec3_t origin;
	vec3_t angles;
	int team;
	qboolean inuse;
} ojp_domSpawn_t;

#define MAX_DOM_SPAWNS_OJP 32
static ojp_domSpawn_t ojp_domSpawns[MAX_DOM_SPAWNS_OJP];
static int ojp_numDomSpawns = 0;

void ojp_DomSpawn_Init(void)
{
	memset(ojp_domSpawns, 0, sizeof(ojp_domSpawns));
	ojp_numDomSpawns = 0;
}

void ojp_DomSpawn_Add(vec3_t origin, vec3_t angles, int team)
{
	if (ojp_numDomSpawns >= MAX_DOM_SPAWNS_OJP) return;
	VectorCopy(origin, ojp_domSpawns[ojp_numDomSpawns].origin);
	VectorCopy(angles, ojp_domSpawns[ojp_numDomSpawns].angles);
	ojp_domSpawns[ojp_numDomSpawns].team = team;
	ojp_domSpawns[ojp_numDomSpawns].inuse = qtrue;
	ojp_numDomSpawns++;
}

void ojp_DomSpawn_Respawn(gentity_t *ent)
{
	if (!ent || !ent->client) return;

	int myTeam = ent->client->sess.sessionTeam;
	int closestSpawn = -1;
	float closestDist = 999999.0f;

	int i;`r`n	for (i = 0; i < ojp_numDomSpawns; i++) {
		if (!ojp_domSpawns[i].inuse) continue;
		if (ojp_domSpawns[i].team != TEAM_FREE && ojp_domSpawns[i].team != myTeam) continue;

		vec3_t dir;
		VectorSubtract(ojp_domSpawns[i].origin, ent->client->ps.origin, dir);
		float dist = VectorLength(dir);
		if (dist < closestDist) {
			closestDist = dist;
			closestSpawn = i;
		}
	}

	if (closestSpawn >= 0) {
		VectorCopy(ojp_domSpawns[closestSpawn].origin, ent->s.origin);
		VectorCopy(ojp_domSpawns[closestSpawn].origin, ent->client->ps.origin);
		VectorCopy(ojp_domSpawns[closestSpawn].angles, ent->client->ps.viewangles);
	}
}

