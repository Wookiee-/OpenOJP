#include "cg_local.h"
extern vmCvar_t cg_trueroll;
extern vmCvar_t cg_trueflip;
extern vmCvar_t cg_truespin;
extern vmCvar_t cg_truemoveroll;
extern vmCvar_t cg_truesaberonly;
extern vmCvar_t cg_trueeyeposition;
extern vmCvar_t cg_trueguns;

qboolean ojp_TrueView_RollEnabled(void) { return cg_trueroll.integer ? qtrue : qfalse; }
qboolean ojp_TrueView_FlipEnabled(void) { return cg_trueflip.integer ? qtrue : qfalse; }
qboolean ojp_TrueView_SpinEnabled(void) { return cg_truespin.integer ? qtrue : qfalse; }
qboolean ojp_TrueView_MoveRollEnabled(void) { return cg_truemoveroll.integer ? qtrue : qfalse; }
qboolean ojp_TrueView_SaberOnly(void) { return cg_truesaberonly.integer ? qtrue : qfalse; }
float ojp_TrueView_EyePosition(void) { return cg_trueeyeposition.value; }
qboolean ojp_TrueView_GunsEnabled(void) { return cg_trueguns.integer ? qtrue : qfalse; }
