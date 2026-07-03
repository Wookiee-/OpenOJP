#pragma once

#include "cg_local.h"

void ojp_CGCam_Init(void);
void ojp_CGCam_Enable(void);
void ojp_CGCam_Disable(void);
void ojp_CGCam_SetPosition(vec3_t org);
void ojp_CGCam_SetAngles(vec3_t ang);
void ojp_CGCam_SetFOV(float FOV);
void ojp_CGCam_Zoom(float FOV, float duration);
void ojp_CGCam_Move(vec3_t dest, float duration);
void ojp_CGCam_Pan(vec3_t dest, vec3_t panDirection, float duration);
void ojp_CGCam_Fade(vec4_t source, vec4_t dest, float duration);
void ojp_CGCam_Update(void);
void ojp_CGCam_RenderScene(void);
void ojp_CGCam_DrawWideScreen(void);
void ojp_CGCam_Shake(float intensity, int duration);
void ojp_CGCam_Follow(int cameraGroup[16], float speed, float initLerp);
void ojp_CGCam_Track(const char *trackName, float speed, float initLerp);
void ojp_CGCam_Distance(float distance, float initLerp);
void ojp_CGCam_Roll(float dest, float duration);
void ojp_CGCam_StartRoff(char *roff);
void ojp_CGCam_Smooth(float intensity, int duration);
void ojp_CGCam_FollowUpdate(void);
qboolean ojp_CGCam_IsEnabled(void);
