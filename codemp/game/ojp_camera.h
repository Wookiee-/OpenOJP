#pragma once

#include "qcommon/q_shared.h"
#include "game/bg_public.h"

#define MAX_CAMERA_GROUP_SUBJECTS_OJP 16
#define MAX_ACCEL_PER_FRAME_OJP 10.0f
#define MAX_SHAKE_INTENSITY_OJP 16.0f
#define CAMERA_DEFAULT_FOV_OJP 90.0f
#define CAMERA_WIDESCREEN_FOV_OJP 120.0f
#define BAR_DURATION_OJP 1000.0f
#define BAR_RATIO_OJP 48.0f

#define CAMERA_MOVING_OJP 0x00000001
#define CAMERA_PANNING_OJP 0x00000002
#define CAMERA_ZOOMING_OJP 0x00000004
#define CAMERA_BAR_FADING_OJP 0x00000008
#define CAMERA_FADING_OJP 0x00000010
#define CAMERA_FOLLOWING_OJP 0x00000020
#define CAMERA_TRACKING_OJP 0x00000040
#define CAMERA_ROFFING_OJP 0x00000080
#define CAMERA_SMOOTHING_OJP 0x00000100
#define CAMERA_CUT_OJP 0x00000200
#define CAMERA_ACCEL_OJP 0x00000400

typedef struct ojp_camera_s {
	vec3_t origin;
	vec3_t angles;
	vec3_t origin2;
	vec3_t angles2;

	float move_duration;
	float move_time;
	int move_type;

	float FOV;
	float FOV2;
	float FOV_duration;
	float FOV_time;
	float FOV_vel;
	float FOV_acc;

	float pan_time;
	float pan_duration;

	char cameraGroup[MAX_QPATH];
	float cameraGroupZOfs;
	char cameraGroupTag[MAX_QPATH];
	vec3_t subjectPos;
	float subjectSpeed;
	float followSpeed;
	qboolean followInitLerp;
	float distance;
	qboolean distanceInitLerp;

	int trackEntNum;
	vec3_t trackToOrg;
	vec3_t moveDir;
	float speed;
	float initSpeed;
	float trackInitLerp;
	int nextTrackEntUpdateTime;

	float bar_alpha;
	float bar_alpha_source;
	float bar_alpha_dest;
	float bar_time;
	float bar_height_source;
	float bar_height_dest;
	float bar_height;

	vec4_t fade_color;
	vec4_t fade_source;
	vec4_t fade_dest;
	float fade_time;
	float fade_duration;

	int info_state;

	float shake_intensity;
	int shake_duration;
	int shake_start;

	float smooth_intensity;
	int smooth_duration;
	int smooth_start;
	vec3_t smooth_origin;
	qboolean smooth_active;

	char sRoff[MAX_QPATH];
	int roff_frame;
	int next_roff_time;

} ojp_camera_t;

extern qboolean ojp_in_camera;
extern ojp_camera_t ojp_client_camera;

void ojp_GCam_Init(void);
void ojp_GCam_Enable(void);
void ojp_GCam_Disable(void);
void ojp_GCam_SetPosition(vec3_t org);
void ojp_GCam_SetAngles(vec3_t ang);
void ojp_GCam_SetFOV(float FOV);
void ojp_GCam_Zoom(float FOV, float duration);
void ojp_GCam_Pan(vec3_t dest, vec3_t panDirection, float duration);
void ojp_GCam_Move(vec3_t dest, float duration);
void ojp_GCam_Fade(vec4_t source, vec4_t dest, float duration);
void ojp_GCam_UpdateFade(void);
void ojp_GCam_Update(void);
void ojp_GCam_RenderScene(void);
void ojp_GCam_DrawWideScreen(void);
void ojp_GCam_Shake(float intensity, int duration);
void ojp_GCam_UpdateShake(vec3_t origin, vec3_t angles);
void ojp_GCam_Follow(int cameraGroup[MAX_CAMERA_GROUP_SUBJECTS_OJP], float speed, float initLerp);
void ojp_GCam_Track(const char *trackName, float speed, float initLerp);
void ojp_GCam_Distance(float distance, float initLerp);
void ojp_GCam_Roll(float dest, float duration);
void ojp_GCam_StartRoff(char *roff);
void ojp_GCam_Smooth(float intensity, int duration);
void ojp_GCam_UpdateSmooth(vec3_t origin, vec3_t angles);
void ojp_GCam_FollowUpdate(void);
void ojp_DisablePlayerCameraPos(void);
