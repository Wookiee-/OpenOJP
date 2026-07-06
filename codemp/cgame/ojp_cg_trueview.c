#include "cg_local.h"
#include "game/bg_saga.h"

vec3_t ojp_cameramins = { -4, -4, -4 };
vec3_t ojp_cameramaxs = { 4, 4, 4 };

static qboolean ojp_true_view_valid = qfalse;
static char ojp_true_view_info[8192];

void ojp_CG_AdjustEyePos(const char *modelName)
{
    if (ojp_true_view_valid) {
        char eyepos[MAX_QPATH];
        if (BG_SiegeGetPairedValue(ojp_true_view_info, (char *)modelName, eyepos)) {
            trap->Cvar_Set("cg_trueeyeposition", eyepos);
        } else {
            trap->Cvar_Set("cg_trueeyeposition", "0");
        }
    } else {
        trap->Cvar_Set("cg_trueeyeposition", "0");
    }
}

void ojp_CheckCameraLocation(vec3_t OldeyeOrigin)
{
    trace_t trace;
    CG_Trace(&trace, OldeyeOrigin, ojp_cameramins, ojp_cameramaxs, cg.refdef.vieworg, cg.snap->ps.clientNum, MASK_SOLID);
    if (trace.fraction <= 1.0f) {
        VectorCopy(trace.endpos, cg.refdef.vieworg);
    }
}

void ojp_TrueViewInit(void)
{
    fileHandle_t f;
    int len;

    memset(ojp_true_view_info, 0, sizeof(ojp_true_view_info));
    ojp_true_view_valid = qfalse;

    len = trap->FS_Open("trueview.cfg", &f, FS_READ);
    if (f && len > 0) {
        if (len >= (int)sizeof(ojp_true_view_info)) {
            len = sizeof(ojp_true_view_info) - 1;
        }
        trap->FS_Read(ojp_true_view_info, len, f);
        ojp_true_view_info[len] = '\0';
        ojp_true_view_valid = qtrue;
        trap->FS_Close(f);
    }
}
