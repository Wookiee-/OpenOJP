#include "g_local.h"
#include "ojp_navigator.h"
#include "ojp_CVec.h"

#define MAX_NAV_PATHS_OJP 64

static ojp_navpoint_t ojp_navPoints[MAX_NAVPOINTS_OJP];
static int ojp_numNavPoints = 0;

void ojp_Nav_Init(void)
{
	memset(ojp_navPoints, 0, sizeof(ojp_navPoints));
	ojp_numNavPoints = 0;
}

int ojp_Nav_FindClosest(vec3_t origin, float maxDist)
{
	int closest = -1;
	float closestDist = maxDist * maxDist;

	for (int i = 0; i < MAX_NAVPOINTS_OJP; i++) {
		if (!ojp_navPoints[i].inuse) continue;
		CVec3 p1(origin);
		CVec3 p2(ojp_navPoints[i].origin);
		float dist = p1.Dist2(p2);
		if (dist < closestDist) {
			closestDist = dist;
			closest = i;
		}
	}
	return closest;
}

qboolean ojp_Nav_GetPoint(int index, vec3_t out)
{
	if (index < 0 || index >= MAX_NAVPOINTS_OJP) return qfalse;
	if (!ojp_navPoints[index].inuse) return qfalse;
	VectorCopy(ojp_navPoints[index].origin, out);
	return qtrue;
}

static qboolean Nav_FindPath(int startIdx, int endIdx, int *path, int *pathLen, int maxPath)
{
	if (startIdx < 0 || endIdx < 0) return qfalse;
	if (startIdx >= MAX_NAVPOINTS_OJP || endIdx >= MAX_NAVPOINTS_OJP) return qfalse;
	if (!ojp_navPoints[startIdx].inuse || !ojp_navPoints[endIdx].inuse) return qfalse;

	int pathCount = 0;
	int current = startIdx;
	int visited[MAX_NAVPOINTS_OJP];
	memset(visited, 0, sizeof(visited));

	for (int i = 0; i < maxPath && current != endIdx; i++) {
		if (pathCount >= maxPath) break;
		visited[current] = 1;
		path[pathCount++] = current;

		int bestNeighbor = -1;
		float bestDist = 999999.0f;
		CVec3 endPos(ojp_navPoints[endIdx].origin);

		for (int j = 0; j < ojp_navPoints[current].neighbornum; j++) {
			int nidx = ojp_navPoints[current].neighbors[j].num;
			if (nidx < 0 || nidx >= MAX_NAVPOINTS_OJP) continue;
			if (visited[nidx]) continue;
			if (!ojp_navPoints[nidx].inuse) continue;

			CVec3 np(ojp_navPoints[nidx].origin);
			float dist = np.Dist2(endPos);
			if (dist < bestDist) {
				bestDist = dist;
				bestNeighbor = nidx;
			}
		}

		if (bestNeighbor < 0) break;
		current = bestNeighbor;
	}

	if (current == endIdx && pathCount < maxPath) {
		path[pathCount++] = endIdx;
		*pathLen = pathCount;
		return qtrue;
	}

	*pathLen = 0;
	return qfalse;
}

qboolean ojp_Nav_GetRoute(vec3_t start, vec3_t end, int *path, int *pathLen, int maxPath)
{
	int startIdx = ojp_Nav_FindClosest(start, 999999.0f);
	int endIdx = ojp_Nav_FindClosest(end, 999999.0f);

	if (startIdx < 0 || endIdx < 0) {
		*pathLen = 0;
		return qfalse;
	}

	return Nav_FindPath(startIdx, endIdx, path, pathLen, maxPath);
}

void ojp_Nav_AddPoint(vec3_t origin, int flags)
{
	if (ojp_numNavPoints >= MAX_NAVPOINTS_OJP) return;
	int idx = ojp_numNavPoints++;
	ojp_navPoints[idx].inuse = qtrue;
	VectorCopy(origin, ojp_navPoints[idx].origin);
	ojp_navPoints[idx].index = idx;
	ojp_navPoints[idx].flags = flags;
	ojp_navPoints[idx].neighbornum = 0;
}

void ojp_Nav_RemovePoint(int index)
{
	if (index < 0 || index >= MAX_NAVPOINTS_OJP) return;
	ojp_navPoints[index].inuse = qfalse;
}

void ojp_Nav_DrawDebug(void)
{
}
