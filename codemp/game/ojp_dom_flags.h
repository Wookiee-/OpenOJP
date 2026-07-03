#pragma once

#include "qcommon/q_shared.h"

void ojp_Dom_Init(void);
int ojp_Dom_AddFlag(vec3_t origin);
int ojp_Dom_GetFlagTeam(int flagIndex);
int ojp_Dom_GetNumFlags(void);
qboolean ojp_Dom_GetFlagOrigin(int flagIndex, vec3_t out);
qboolean ojp_Dom_CaptureFlag(int flagIndex, int entNum, int team);
void ojp_Dom_Update(void);
int ojp_Dom_GetTeamScore(int team);
