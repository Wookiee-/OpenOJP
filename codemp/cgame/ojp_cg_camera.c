#include "cg_local.h"
#include "ojp_cg_camera.h"

static qboolean ojp_cgCamEnabled = qfalse;
static vec3_t ojp_cgCamOrigin = {0, 0, 0};
static vec3_t ojp_cgCamAngles = {0, 0, 0};
static float ojp_cgCamFOV = 90.0f;
static int ojp_cgCamInfoState = 0;
static float ojp_cgCamMoveTime = 0;
static float ojp_cgCamMoveDuration = 0;
static vec3_t ojp_cgCamDestOrigin = {0, 0, 0};
static vec3_t ojp_cgCamDestAngles = {0, 0, 0};

void ojp_CGCam_Init(void)
{
	ojp_cgCamEnabled = qfalse;
	VectorClear(ojp_cgCamOrigin);
	VectorClear(ojp_cgCamAngles);
	ojp_cgCamFOV = 90.0f;
	ojp_cgCamInfoState = 0;
}

void ojp_CGCam_Enable(void)
{
	ojp_cgCamEnabled = qtrue;
}

void ojp_CGCam_Disable(void)
{
	ojp_cgCamEnabled = qfalse;
}

qboolean ojp_CGCam_IsEnabled(void)
{
	return ojp_cgCamEnabled;
}

void ojp_CGCam_SetPosition(vec3_t org)
{
	VectorCopy(org, ojp_cgCamOrigin);
}

void ojp_CGCam_SetAngles(vec3_t ang)
{
	VectorCopy(ang, ojp_cgCamAngles);
}

void ojp_CGCam_SetFOV(float FOV)
{
	ojp_cgCamFOV = FOV;
}

void ojp_CGCam_Zoom(float FOV, float duration)
{
}

void ojp_CGCam_Move(vec3_t dest, float duration)
{
	VectorCopy(dest, ojp_cgCamDestOrigin);
	ojp_cgCamMoveDuration = duration;
	ojp_cgCamMoveTime = cg.time;
	ojp_cgCamInfoState |= 0x00000004;
}

void ojp_CGCam_Pan(vec3_t dest, vec3_t panDirection, float duration)
{
	VectorCopy(dest, ojp_cgCamDestAngles);
	ojp_cgCamMoveDuration = duration;
	ojp_cgCamMoveTime = cg.time;
	ojp_cgCamInfoState |= 0x00000002;
}

void ojp_CGCam_Fade(vec4_t source, vec4_t dest, float duration)
{
}

void ojp_CGCam_Update(void)
{
	if (!ojp_cgCamEnabled) return;

	if (ojp_cgCamInfoState & 0x00000004) {
		float t = (cg.time - ojp_cgCamMoveTime) / (ojp_cgCamMoveDuration * 1000.0f);
		if (t >= 1.0f) {
			VectorCopy(ojp_cgCamDestOrigin, ojp_cgCamOrigin);
			ojp_cgCamInfoState &= ~0x00000004;
		} else {
			ojp_cgCamOrigin[0] += (ojp_cgCamDestOrigin[0] - ojp_cgCamOrigin[0]) * t;
			ojp_cgCamOrigin[1] += (ojp_cgCamDestOrigin[1] - ojp_cgCamOrigin[1]) * t;
			ojp_cgCamOrigin[2] += (ojp_cgCamDestOrigin[2] - ojp_cgCamOrigin[2]) * t;
		}
	}

	if (ojp_cgCamInfoState & 0x00000002) {
		float t = (cg.time - ojp_cgCamMoveTime) / (ojp_cgCamMoveDuration * 1000.0f);
		if (t >= 1.0f) {
			VectorCopy(ojp_cgCamDestAngles, ojp_cgCamAngles);
			ojp_cgCamInfoState &= ~0x00000002;
		} else {
			ojp_cgCamAngles[0] += (ojp_cgCamDestAngles[0] - ojp_cgCamAngles[0]) * t;
			ojp_cgCamAngles[1] += (ojp_cgCamDestAngles[1] - ojp_cgCamAngles[1]) * t;
			ojp_cgCamAngles[2] += (ojp_cgCamDestAngles[2] - ojp_cgCamAngles[2]) * t;
		}
	}
}

void ojp_CGCam_RenderScene(void)
{
}

void ojp_CGCam_DrawWideScreen(void)
{
}

void ojp_CGCam_Shake(float intensity, int duration)
{
}

void ojp_CGCam_Follow(int cameraGroup[16], float speed, float initLerp)
{
}

void ojp_CGCam_Track(const char *trackName, float speed, float initLerp)
{
}

void ojp_CGCam_Distance(float distance, float initLerp)
{
}

void ojp_CGCam_Roll(float dest, float duration)
{
}

void ojp_CGCam_StartRoff(char *roff)
{
}

void ojp_CGCam_Smooth(float intensity, int duration)
{
}

void ojp_CGCam_FollowUpdate(void)
{
}
