#include "g_local.h"
#include "qcommon/ojp_shared.h"
#include "ojp_dom_flags.h"

#define MAX_DOM_FLAGS_OJP 8
#define DOM_FLAG_CAPTURE_TIME_OJP 5000
#define DOM_FLAG_RESPAWN_TIME_OJP 30000

typedef struct {
	int index;
	vec3_t origin;
	int team;
	int owner;
	int captureTime;
	int respawnTime;
	qboolean active;
	qboolean captured;
} ojp_domFlag_t;

static ojp_domFlag_t ojp_domFlags[MAX_DOM_FLAGS_OJP];
static int ojp_numDomFlags = 0;

void ojp_Dom_Init(void)
{
	memset(ojp_domFlags, 0, sizeof(ojp_domFlags));
	ojp_numDomFlags = 0;
}

int ojp_Dom_AddFlag(vec3_t origin)
{
	if (ojp_numDomFlags >= MAX_DOM_FLAGS_OJP) return -1;
	int idx = ojp_numDomFlags++;
	ojp_domFlags[idx].index = idx;
	VectorCopy(origin, ojp_domFlags[idx].origin);
	ojp_domFlags[idx].team = TEAM_FREE;
	ojp_domFlags[idx].owner = ENTITYNUM_NONE;
	ojp_domFlags[idx].active = qtrue;
	ojp_domFlags[idx].captured = qfalse;
	return idx;
}

int ojp_Dom_GetFlagTeam(int flagIndex)
{
	if (flagIndex < 0 || flagIndex >= ojp_numDomFlags) return TEAM_FREE;
	if (!ojp_domFlags[flagIndex].active) return TEAM_FREE;
	return ojp_domFlags[flagIndex].team;
}

int ojp_Dom_GetNumFlags(void)
{
	return ojp_numDomFlags;
}

qboolean ojp_Dom_GetFlagOrigin(int flagIndex, vec3_t out)
{
	if (flagIndex < 0 || flagIndex >= ojp_numDomFlags) return qfalse;
	VectorCopy(ojp_domFlags[flagIndex].origin, out);
	return qtrue;
}

qboolean ojp_Dom_CaptureFlag(int flagIndex, int entNum, int team)
{
	if (flagIndex < 0 || flagIndex >= ojp_numDomFlags) return qfalse;
	if (!ojp_domFlags[flagIndex].active) return qfalse;

	ojp_domFlags[flagIndex].team = team;
	ojp_domFlags[flagIndex].owner = entNum;
	ojp_domFlags[flagIndex].captureTime = level.time;
	ojp_domFlags[flagIndex].captured = qtrue;
	return qtrue;
}

void ojp_Dom_Update(void)
{
	for (int i = 0; i < ojp_numDomFlags; i++) {
		if (!ojp_domFlags[i].active) {
			if (ojp_domFlags[i].respawnTime && level.time > ojp_domFlags[i].respawnTime) {
				ojp_domFlags[i].active = qtrue;
				ojp_domFlags[i].team = TEAM_FREE;
				ojp_domFlags[i].respawnTime = 0;
			}
			continue;
		}

		for (int j = 0; j < level.num_entities; j++) {
			gentity_t *ent = &g_entities[j];
			if (!ent->inuse || !ent->client) continue;
			if (ent->health <= 0) continue;

			vec3_t dir;
			VectorSubtract(ojp_domFlags[i].origin, ent->client->ps.origin, dir);
			float dist = VectorLength(dir);

			if (dist < 50.0f) {
				ojp_Dom_CaptureFlag(i, j, ent->client->sess.sessionTeam);
			}
		}
	}
}

int ojp_Dom_GetTeamScore(int team)
{
	int score = 0;
	for (int i = 0; i < ojp_numDomFlags; i++) {
		if (ojp_domFlags[i].active && ojp_domFlags[i].team == team) {
			score++;
		}
	}
	return score;
}
