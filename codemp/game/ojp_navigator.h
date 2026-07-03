#pragma once

#include "qcommon/q_shared.h"
#include "game/bg_public.h"
#include "g_local.h"

#define MAX_NAVPOINTS_OJP 4096

typedef struct {
	vec3_t origin;
	int inuse;
	int index;
	float weight;
	float disttonext;
	int flags;
	int associated_entity;
	int forceJumpTo;
	int neighbornum;
	wpneighbor_t neighbors[MAX_NEIGHBOR_SIZE];
} ojp_navpoint_t;

void ojp_Nav_Init(void);
int ojp_Nav_FindClosest(vec3_t origin, float maxDist);
qboolean ojp_Nav_GetPoint(int index, vec3_t out);
qboolean ojp_Nav_GetRoute(vec3_t start, vec3_t end, int *path, int *pathLen, int maxPath);
void ojp_Nav_AddPoint(vec3_t origin, int flags);
void ojp_Nav_RemovePoint(int index);
void ojp_Nav_DrawDebug(void);
