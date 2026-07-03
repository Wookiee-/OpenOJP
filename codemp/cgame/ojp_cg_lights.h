#pragma once

#include "cg_local.h"

#define MAX_OJP_LIGHTS 64

typedef struct {
	vec3_t origin;
	vec3_t color;
	float radius;
	float intensity;
	qboolean active;
	int lifeTime;
	int startTime;
} ojp_cgLight_t;

void ojp_CG_AddLight(vec3_t origin, vec3_t color, float radius, float intensity, int duration);
void ojp_CG_UpdateLights(void);
