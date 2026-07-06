#include "cg_local.h"
#include "qcommon/ojp_shared.h"

extern qboolean trap_G2API_GetBoltMatrix(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale);
extern qboolean trap_G2API_SetBoneAngles(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime, int currentTime);
extern qboolean trap_G2API_SetBoneAnim(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame, const int blendTime);
extern void trap_TrueMalloc(void **ptr, int size);
extern void trap_TrueFree(void **ptr);

qboolean ojp_cg_strap_G2API_GetBoltMatrix(void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix,
	const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale)
{
	return trap_G2API_GetBoltMatrix(ghoul2, modelIndex, boltIndex, matrix, angles, position, frameNum, modelList, scale);
}

qboolean ojp_cg_strap_G2API_SetBoneAngles(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags,
	const int up, const int right, const int forward, qhandle_t *modelList, int blendTime, int currentTime)
{
	return trap_G2API_SetBoneAngles(ghoul2, modelIndex, boneName, angles, flags, up, right, forward, modelList, blendTime, currentTime);
}

qboolean ojp_cg_strap_G2API_SetBoneAnim(void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame,
	const int flags, const float animSpeed, const int currentTime, const float setFrame, const int blendTime)
{
	return trap_G2API_SetBoneAnim(ghoul2, modelIndex, boneName, startFrame, endFrame, flags, animSpeed, currentTime, setFrame, blendTime);
}

void ojp_cg_strap_TrueMalloc(void **ptr, int size)
{
	trap_TrueMalloc(ptr, size);
}

void ojp_cg_strap_TrueFree(void **ptr)
{
	trap_TrueFree(ptr);
}
