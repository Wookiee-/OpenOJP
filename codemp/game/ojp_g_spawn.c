#include "g_local.h"
#include "qcommon/ojp_shared.h"

extern void trap_Trace(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask);

void ojp_InitSpawnScriptValues(void)
{
}

qboolean ojp_CheckAboveOK_Player(vec3_t spawnPoint, gentity_t *ent)
{
	trace_t tr;
	vec3_t start, end;
	VectorCopy(spawnPoint, start);
	VectorCopy(spawnPoint, end);
	end[2] += 72;
	trap_Trace(&tr, start, ent->r.mins, ent->r.maxs, end, ent->s.number, MASK_PLAYERSOLID);
	return (tr.fraction >= 1.0f) ? qtrue : qfalse;
}

qboolean ojp_CheckBelowOK(vec3_t spawnPoint, float minDist)
{
	trace_t tr;
	vec3_t end;
	VectorCopy(spawnPoint, end);
	end[2] -= minDist;
	trap_Trace(&tr, spawnPoint, NULL, NULL, end, -1, MASK_SOLID);
	return (tr.fraction >= 1.0f) ? qtrue : qfalse;
}
