#include "qcommon/q_shared.h"
#include "g_local.h"
#include "qcommon/ojp_shared.h"
#include "ojp_camera.h"

qboolean ojp_in_camera = qfalse;
ojp_camera_t ojp_client_camera = {{0}};

vec3_t ojp_camerapos;
vec3_t ojp_cameraang;

void ojp_GCam_Update(void)
{
	int i;
	qboolean checkFollow = qfalse;
	qboolean checkTrack = qfalse;

	if (ojp_client_camera.info_state & CAMERA_ZOOMING_OJP) {
		float actualFOV_X = ojp_client_camera.FOV;
		float t = (level.time - ojp_client_camera.FOV_time) * 0.001f;
		float fovDuration = ojp_client_camera.FOV_duration;

		if (ojp_client_camera.FOV_time + fovDuration < level.time) {
			ojp_client_camera.info_state &= ~CAMERA_ZOOMING_OJP;
		}
	}

	if (ojp_client_camera.info_state & CAMERA_PANNING_OJP) {
		if (ojp_client_camera.pan_time + ojp_client_camera.pan_duration < level.time) {
			ojp_client_camera.info_state &= ~CAMERA_PANNING_OJP;
		}
	}

	if (ojp_client_camera.info_state & CAMERA_MOVING_OJP) {
		float t = (level.time - ojp_client_camera.move_time) * 0.001f;
		float moveDuration = ojp_client_camera.move_duration;

		if (ojp_client_camera.move_time + moveDuration * 1000 < level.time) {
			VectorCopy(ojp_client_camera.origin2, ojp_client_camera.origin);
			VectorCopy(ojp_client_camera.angles2, ojp_client_camera.angles);
			ojp_client_camera.info_state &= ~CAMERA_MOVING_OJP;
		} else {
			float frac = t / moveDuration;
			if (frac > 1.0f) frac = 1.0f;
			ojp_client_camera.origin[0] = ojp_client_camera.origin[0] + (ojp_client_camera.origin2[0] - ojp_client_camera.origin[0]) * frac;
			ojp_client_camera.origin[1] = ojp_client_camera.origin[1] + (ojp_client_camera.origin2[1] - ojp_client_camera.origin[1]) * frac;
			ojp_client_camera.origin[2] = ojp_client_camera.origin[2] + (ojp_client_camera.origin2[2] - ojp_client_camera.origin[2]) * frac;
			ojp_client_camera.angles[0] = ojp_client_camera.angles[0] + (ojp_client_camera.angles2[0] - ojp_client_camera.angles[0]) * frac;
			ojp_client_camera.angles[1] = ojp_client_camera.angles[1] + (ojp_client_camera.angles2[1] - ojp_client_camera.angles[1]) * frac;
			ojp_client_camera.angles[2] = ojp_client_camera.angles[2] + (ojp_client_camera.angles2[2] - ojp_client_camera.angles[2]) * frac;
		}
	}

	if (ojp_client_camera.info_state & CAMERA_FOLLOWING_OJP) {
		checkFollow = qtrue;
	}

	if (ojp_client_camera.info_state & CAMERA_TRACKING_OJP) {
		checkTrack = qtrue;
	}

	if (ojp_client_camera.info_state & CAMERA_FADING_OJP) {
		ojp_GCam_UpdateFade();
	}

	if (ojp_client_camera.info_state & CAMERA_SMOOTHING_OJP) {
		ojp_GCam_UpdateSmooth(ojp_client_camera.origin, ojp_client_camera.angles);
	}

	ojp_GCam_UpdateShake(ojp_client_camera.origin, ojp_client_camera.angles);
}

void ojp_GCam_Init(void)
{
	memset(&ojp_client_camera, 0, sizeof(ojp_client_camera));
	ojp_client_camera.FOV = CAMERA_DEFAULT_FOV_OJP;
	ojp_client_camera.info_state = 0;
	ojp_in_camera = qfalse;
}

void ojp_GCam_Enable(void)
{
	ojp_in_camera = qtrue;
	ojp_GCam_Init();
}

void ojp_GCam_Disable(void)
{
	ojp_in_camera = qfalse;
}

void ojp_GCam_SetPosition(vec3_t org)
{
	VectorCopy(org, ojp_client_camera.origin);
}

void ojp_GCam_SetAngles(vec3_t ang)
{
	VectorCopy(ang, ojp_client_camera.angles);
}

void ojp_GCam_SetFOV(float FOV)
{
	ojp_client_camera.FOV = FOV;
}

void ojp_GCam_Zoom(float FOV, float duration)
{
	ojp_client_camera.FOV2 = FOV;
	ojp_client_camera.FOV_duration = duration;
	ojp_client_camera.FOV_time = level.time;
	ojp_client_camera.info_state |= CAMERA_ZOOMING_OJP;
}

void ojp_GCam_Pan(vec3_t dest, vec3_t panDirection, float duration)
{
	VectorCopy(dest, ojp_client_camera.origin2);
	ojp_client_camera.pan_duration = duration;
	ojp_client_camera.pan_time = level.time;
	ojp_client_camera.info_state |= CAMERA_PANNING_OJP;
}

void ojp_GCam_Move(vec3_t dest, float duration)
{
	VectorCopy(dest, ojp_client_camera.origin2);
	ojp_client_camera.move_duration = duration;
	ojp_client_camera.move_time = level.time;
	ojp_client_camera.info_state |= CAMERA_MOVING_OJP;
}

void ojp_GCam_Fade(vec4_t source, vec4_t dest, float duration)
{
	Vector4Copy(source, ojp_client_camera.fade_source);
	Vector4Copy(dest, ojp_client_camera.fade_dest);
	ojp_client_camera.fade_duration = duration;
	ojp_client_camera.fade_time = level.time;
	ojp_client_camera.info_state |= CAMERA_FADING_OJP;
}

void ojp_GCam_UpdateFade(void)
{
	float t = (level.time - ojp_client_camera.fade_time) / ojp_client_camera.fade_duration;
	if (t >= 1.0f) {
		t = 1.0f;
		ojp_client_camera.info_state &= ~CAMERA_FADING_OJP;
	}
	ojp_client_camera.fade_color[0] = ojp_client_camera.fade_source[0] + (ojp_client_camera.fade_dest[0] - ojp_client_camera.fade_source[0]) * t;
	ojp_client_camera.fade_color[1] = ojp_client_camera.fade_source[1] + (ojp_client_camera.fade_dest[1] - ojp_client_camera.fade_source[1]) * t;
	ojp_client_camera.fade_color[2] = ojp_client_camera.fade_source[2] + (ojp_client_camera.fade_dest[2] - ojp_client_camera.fade_source[2]) * t;
	ojp_client_camera.fade_color[3] = ojp_client_camera.fade_source[3] + (ojp_client_camera.fade_dest[3] - ojp_client_camera.fade_source[3]) * t;
}

void ojp_GCam_Shake(float intensity, int duration)
{
	ojp_client_camera.shake_intensity = intensity;
	ojp_client_camera.shake_duration = duration;
	ojp_client_camera.shake_start = level.time;
}

void ojp_GCam_UpdateShake(vec3_t origin, vec3_t angles)
{
	if (level.time - ojp_client_camera.shake_start < ojp_client_camera.shake_duration) {
		float intensity = ojp_client_camera.shake_intensity * (1.0f - (float)(level.time - ojp_client_camera.shake_start) / ojp_client_camera.shake_duration);
		angles[0] += (Q_irand(0, 100) / 100.0f * 2 - 1) * intensity;
		angles[1] += (Q_irand(0, 100) / 100.0f * 2 - 1) * intensity;
	}
}

void ojp_GCam_RenderScene(void)
{
}

void ojp_GCam_DrawWideScreen(void)
{
}

void ojp_GCam_Follow(int cameraGroup[MAX_CAMERA_GROUP_SUBJECTS_OJP], float speed, float initLerp)
{
	ojp_client_camera.followSpeed = speed;
	ojp_client_camera.followInitLerp = qtrue;
	ojp_client_camera.info_state |= CAMERA_FOLLOWING_OJP;
}

void ojp_GCam_Track(const char *trackName, float speed, float initLerp)
{
	ojp_client_camera.speed = speed;
	ojp_client_camera.trackInitLerp = initLerp;
	ojp_client_camera.info_state |= CAMERA_TRACKING_OJP;
}

void ojp_GCam_Distance(float distance, float initLerp)
{
	ojp_client_camera.distance = distance;
	ojp_client_camera.distanceInitLerp = qtrue;
}

void ojp_GCam_Roll(float dest, float duration)
{
}

void ojp_GCam_StartRoff(char *roff)
{
}

void ojp_GCam_Smooth(float intensity, int duration)
{
	ojp_client_camera.smooth_intensity = intensity;
	ojp_client_camera.smooth_duration = duration;
	ojp_client_camera.smooth_start = level.time;
	ojp_client_camera.info_state |= CAMERA_SMOOTHING_OJP;
}

void ojp_GCam_UpdateSmooth(vec3_t origin, vec3_t angles)
{
	if (level.time - ojp_client_camera.smooth_start < ojp_client_camera.smooth_duration) {
		VectorCopy(origin, ojp_client_camera.smooth_origin);
		ojp_client_camera.smooth_active = qtrue;
	} else {
		ojp_client_camera.smooth_active = qfalse;
		ojp_client_camera.info_state &= ~CAMERA_SMOOTHING_OJP;
	}
}

void ojp_GCam_FollowUpdate(void)
{
}

void ojp_DisablePlayerCameraPos(void)
{
}
