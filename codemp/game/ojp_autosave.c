#include "g_local.h"

static int ojp_lastAutoSaveTime = 0;
#define OJP_AUTOSAVE_INTERVAL 60000

void ojp_AutoSave(void)
{
	if (!level.time) return;
	if (level.time - ojp_lastAutoSaveTime < OJP_AUTOSAVE_INTERVAL) return;
	if (g_gametype.integer >= GT_TEAM) return;

	ojp_lastAutoSaveTime = level.time;

	trap_SendConsoleCommand(EXEC_APPEND, "save auto\n");
}

void ojp_AutoSaveInit(void)
{
	ojp_lastAutoSaveTime = 0;
}
