#pragma once

#include "qcommon/q_shared.h"

#define DMS_INFO_SIZE_OJP 30000
#define DMS_DEATH_MUSIC_OJP "music/death_music.mp3"
#define DMS_DEATH_MUSIC_TIME_OJP 5000
#define MAX_DMS_TRANSITIONS_OJP 4
#define MAX_DMS_EXITPOINTS_OJP 8
#define DMS_MUSICLENGTH_FILENAME_OJP "ext_data/musiclength.dat"
#define DMS_MUSICFILE_DEFAULT_OJP 21000
#define DMS_TRANSITIONFUDGEFACTOR_OJP 500

typedef enum {
	DM_AUTO_OJP,
	DM_SILENCE_OJP,
	DM_EXPLORE_OJP,
	DM_ACTION_OJP,
	DM_BOSS_OJP,
	DM_DEATH_OJP
} ojp_dynamicMusic_t;

typedef struct ojp_DynamicTransition_s {
	char fileName[MAX_QPATH];
	int fileLength;
	int exitPoints[MAX_DMS_EXITPOINTS_OJP];
	int numExitPoints;
} ojp_DynamicTransition_t;

typedef struct ojp_DynamicMusicSet_s {
	char fileName[MAX_QPATH];
	int fileLength;
	ojp_DynamicTransition_t Transitions[MAX_DMS_TRANSITIONS_OJP];
	int numTransitions;
	qboolean valid;
} ojp_DynamicMusicSet_t;

typedef struct ojp_DynamicMusicGroup_s {
	ojp_DynamicMusicSet_t exploreMusic;
	ojp_DynamicMusicSet_t actionMusic;
	ojp_DynamicMusicSet_t bossMusic;
	int dmState;
	int olddmState;
	int dmDebounceTime;
	int dmBeatTime;
	int dmStartTime;
	qboolean valid;
} ojp_DynamicMusicGroup_t;

extern ojp_DynamicMusicGroup_t ojp_DMSData;

void ojp_LoadDynamicMusic(void);
void ojp_SetDMSState(int DMSState);
void ojp_G_DynamicMusicUpdate(void);
