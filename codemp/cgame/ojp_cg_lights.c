#include "cg_local.h"
#include "ojp_cg_lights.h"

extern void trap_R_AddLightToScene(const vec3_t org, float intensity, float r, float g, float b);

static ojp_cgLight_t ojp_cgLights[MAX_OJP_LIGHTS];

void ojp_CG_AddLight(vec3_t origin, vec3_t color, float radius, float intensity, int duration)
{
	int i;
for (i = 0; i < MAX_OJP_LIGHTS; i++) {
		if (!ojp_cgLights[i].active) {
			VectorCopy(origin, ojp_cgLights[i].origin);
			VectorCopy(color, ojp_cgLights[i].color);
			ojp_cgLights[i].radius = radius;
			ojp_cgLights[i].intensity = intensity;
			ojp_cgLights[i].active = qtrue;
			ojp_cgLights[i].lifeTime = duration;
			ojp_cgLights[i].startTime = cg.time;
			break;
		}
	}
}

void ojp_CG_UpdateLights(void)
{
	int i;
for (i = 0; i < MAX_OJP_LIGHTS; i++) {
		if (!ojp_cgLights[i].active) continue;
		if (ojp_cgLights[i].lifeTime > 0 && cg.time - ojp_cgLights[i].startTime > ojp_cgLights[i].lifeTime) {
			ojp_cgLights[i].active = qfalse;
			continue;
		}
		trap_R_AddLightToScene(ojp_cgLights[i].origin, ojp_cgLights[i].radius * ojp_cgLights[i].intensity,
			ojp_cgLights[i].color[0], ojp_cgLights[i].color[1], ojp_cgLights[i].color[2]);
	}
}


