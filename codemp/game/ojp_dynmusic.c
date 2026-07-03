#include "g_local.h"
#include "ojp_dynmusic.h"

ojp_DynamicMusicGroup_t ojp_DMSData;

static void LoadDMSSongData(char *buffer, char *song, ojp_DynamicMusicSet_t *songData, char *mapname)
{
	char SongGroup[DMS_INFO_SIZE_OJP];
	char transitionGroup[DMS_INFO_SIZE_OJP];
	char Value[MAX_QPATH];
	int numTransitions = 0;
	int numExits = 0;
	char *transition;

	strcpy(songData->fileName, va("music/%s/%s.mp3", mapname, song));
	songData->numTransitions = 0;

	transition = strstr(buffer, "exit");
	while (transition) {
		if (numTransitions >= MAX_DMS_TRANSITIONS_OJP) {
			G_Printf("LoadDMSSongData Error: Too many transitions found.\n");
			return;
		}
		songData->Transitions[numTransitions].numExitPoints = 0;

		sscanf(transition, "exit %s", songData->Transitions[numTransitions].fileName);

		numTransitions++;
		transition = strstr(transition + 4, "exit");
	}
	songData->numTransitions = numTransitions;
	songData->valid = qtrue;
}

static void LoadDynamicMusicGroup(char *mapname, char *buffer)
{
	ojp_DMSData.valid = qtrue;

	LoadDMSSongData(buffer, "explore", &ojp_DMSData.exploreMusic, mapname);
	LoadDMSSongData(buffer, "action", &ojp_DMSData.actionMusic, mapname);
	LoadDMSSongData(buffer, "boss", &ojp_DMSData.bossMusic, mapname);

	ojp_DMSData.dmState = DM_EXPLORE_OJP;
	ojp_DMSData.olddmState = DM_EXPLORE_OJP;
	ojp_DMSData.dmDebounceTime = 0;
	ojp_DMSData.dmBeatTime = 0;
	ojp_DMSData.dmStartTime = 0;
}

void ojp_LoadDynamicMusic(void)
{
	int len = 0;
	fileHandle_t f;
	char buffer[DMS_INFO_SIZE_OJP];
	vmCvar_t mapname;

	len = trap_FS_FOpenFile("ext_data/dms.dat", &f, FS_READ);
	if (!f) {
		G_Printf("LoadDynamicMusic() Error: Couldn't open ext_data/dms.dat\n");
		return;
	}
	if (len >= DMS_INFO_SIZE_OJP) {
		G_Printf("LoadDynamicMusic() Error: dms.dat too big.\n");
		trap_FS_FCloseFile(f);
		return;
	}

	trap_FS_Read(buffer, len, f);
	trap_FS_FCloseFile(f);

	trap_Cvar_Register(&mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM);
	LoadDynamicMusicGroup(mapname.string, buffer);
}

void ojp_SetDMSState(int DMSState)
{
	ojp_DMSData.dmState = DMSState;
}

void ojp_G_DynamicMusicUpdate(void)
{
	if (!ojp_DMSData.valid) return;

	if (ojp_DMSData.dmState != ojp_DMSData.olddmState) {
		ojp_DMSData.olddmState = ojp_DMSData.dmState;
	}
}
