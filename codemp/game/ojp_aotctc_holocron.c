#include "g_local.h"
#include "qcommon/ojp_shared.h"

#define AOTC_MAX_HOLOCRONS_OJP 8
#define AOTC_HOLOCRON_RADIUS_OJP 50.0f

typedef struct {
	vec3_t origin;
	int forcePower;
	int count;
	qboolean active;
} ojp_aotcHolocron_t;

static ojp_aotcHolocron_t ojp_aotcHolocrons[AOTC_MAX_HOLOCRONS_OJP];
static int ojp_numAotcHolocrons = 0;

void ojp_AOTC_Init(void)
{
	memset(ojp_aotcHolocrons, 0, sizeof(ojp_aotcHolocrons));
	ojp_numAotcHolocrons = 0;
}

int ojp_AOTC_AddHolocron(vec3_t origin, int forcePower, int count)
{
	if (ojp_numAotcHolocrons >= AOTC_MAX_HOLOCRONS_OJP) return -1;
	int idx = ojp_numAotcHolocrons++;
	VectorCopy(origin, ojp_aotcHolocrons[idx].origin);
	ojp_aotcHolocrons[idx].forcePower = forcePower;
	ojp_aotcHolocrons[idx].count = count;
	ojp_aotcHolocrons[idx].active = qtrue;
	return idx;
}

void ojp_AOTC_HolocronUpdate(void)
{
	int i;
for (i = 0; i < ojp_numAotcHolocrons; i++) {
		if (!ojp_aotcHolocrons[i].active) continue;

		int j;
for (j = 0; j < level.num_entities; j++) {
			gentity_t *ent = &g_entities[j];
			if (!ent->inuse || !ent->client) continue;
			if (ent->health <= 0) continue;

			vec3_t dir;
			VectorSubtract(ojp_aotcHolocrons[i].origin, ent->client->ps.origin, dir);
			float dist = VectorLength(dir);

			if (dist < AOTC_HOLOCRON_RADIUS_OJP) {
				ent->client->ps.holocronsCarried[ojp_aotcHolocrons[i].forcePower] += ojp_aotcHolocrons[i].count;
				ojp_aotcHolocrons[i].active = qfalse;
				break;
			}
		}
	}
}


