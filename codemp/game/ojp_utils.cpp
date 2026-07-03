#include "g_local.h"
#include "qcommon/ojp_shared.h"

vec3_t ojp_vec3_origin = {0, 0, 0};

float ojp_Q_flrand(float min, float max)
{
	return min + (max - min) * ((float)rand() / (float)RAND_MAX);
}

int ojp_Q_irand(int min, int max)
{
	if (min >= max) return min;
	return min + rand() % (max - min + 1);
}

void ojp_VectorNormalize(vec3_t v)
{
	float length = VectorLength(v);
	if (length) {
		float ilength = 1.0f / length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}
}

float ojp_VectorNormalize2(const vec3_t v, vec3_t out)
{
	VectorCopy(v, out);
	float length = VectorLength(v);
	if (length) {
		float ilength = 1.0f / length;
		out[0] *= ilength;
		out[1] *= ilength;
		out[2] *= ilength;
	}
	return length;
}
