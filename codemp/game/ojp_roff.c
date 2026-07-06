#include "g_local.h"
#include "ojp_roff.h"

// Simple hash for ROFF file names
static int ojp_ROFF_HashKey(const char *string)
{
	int hash = 0;
	int i = 0;
	while (string[i]) {
		hash += tolower(string[i]) * (i + 1);
		i++;
	}
	return hash % 64;
}

#define MAX_ROFF_FILES_OJP 32
#define MAX_ROFF_PLAYERS_OJP 32

typedef struct {
	qboolean inuse;
	int entNum;
	ojp_roff_t *roff;
	int currentFrame;
	int nextFrameTime;
	qboolean doTranslation;
	qboolean loop;
} ojp_roffPlayer_t;

static ojp_roff_t ojp_roffFiles[MAX_ROFF_FILES_OJP];
static int ojp_numRoffFiles = 0;
static ojp_roffPlayer_t ojp_roffPlayers[MAX_ROFF_PLAYERS_OJP];

void ojp_ROFF_Init(void)
{
	memset(ojp_roffFiles, 0, sizeof(ojp_roffFiles));
	memset(ojp_roffPlayers, 0, sizeof(ojp_roffPlayers));
	ojp_numRoffFiles = 0;
}

void ojp_ROFF_Cache(const char *fileName)
{
	if (!fileName || !fileName[0]) return;
	int h = ojp_ROFF_HashKey(fileName);

	int i;`r`n	for (i = 0; i < ojp_numRoffFiles; i++) {
		if (!Q_stricmp(ojp_roffFiles[i].fileName, fileName)) return;
	}

	if (ojp_numRoffFiles >= MAX_ROFF_FILES_OJP) return;

	int idx = ojp_numRoffFiles++;
	Q_strncpyz(ojp_roffFiles[idx].fileName, fileName, sizeof(ojp_roffFiles[idx].fileName));
	ojp_roffFiles[idx].data = NULL;
	ojp_roffFiles[idx].numFrames = 0;
}

qboolean ojp_ROFF_Play(int entNum, const char *fileName, qboolean doTranslation)
{
	if (!fileName || !fileName[0]) return qfalse;

	int roffIdx = -1;
	int i;`r`n	for (i = 0; i < ojp_numRoffFiles; i++) {
		if (!Q_stricmp(ojp_roffFiles[i].fileName, fileName)) {
			roffIdx = i;
			break;
		}
	}
	if (roffIdx < 0) {
		ojp_ROFF_Cache(fileName);
		return qfalse;
	}

	int slot = -1;
	int i;`r`n	for (i = 0; i < MAX_ROFF_PLAYERS_OJP; i++) {
		if (!ojp_roffPlayers[i].inuse) { slot = i; break; }
		if (ojp_roffPlayers[i].entNum == entNum) { slot = i; break; }
	}
	if (slot < 0) return qfalse;

	ojp_roffPlayers[slot].inuse = qtrue;
	ojp_roffPlayers[slot].entNum = entNum;
	ojp_roffPlayers[slot].roff = &ojp_roffFiles[roffIdx];
	ojp_roffPlayers[slot].currentFrame = 0;
	ojp_roffPlayers[slot].nextFrameTime = level.time + 100;
	ojp_roffPlayers[slot].doTranslation = doTranslation;
	ojp_roffPlayers[slot].loop = qfalse;

	return qtrue;
}

void ojp_ROFF_Update(gentity_t *ent)
{
	if (!ent) return;
	int i;`r`n	for (i = 0; i < MAX_ROFF_PLAYERS_OJP; i++) {
		if (!ojp_roffPlayers[i].inuse) continue;
		if (ojp_roffPlayers[i].entNum != ent->s.number) continue;

		ojp_roffPlayer_t *rp = &ojp_roffPlayers[i];

		if (level.time >= rp->nextFrameTime) {
			rp->currentFrame++;
			rp->nextFrameTime = level.time + 100;

			if (rp->roff && rp->currentFrame >= rp->roff->numFrames) {
				if (rp->loop) {
					rp->currentFrame = 0;
				} else {
					rp->inuse = qfalse;
				}
			}
		}
		break;
	}
}

void ojp_ROFF_Stop(int entNum)
{
	int i;`r`n	for (i = 0; i < MAX_ROFF_PLAYERS_OJP; i++) {
		if (ojp_roffPlayers[i].inuse && ojp_roffPlayers[i].entNum == entNum) {
			ojp_roffPlayers[i].inuse = qfalse;
			return;
		}
	}
}

qboolean ojp_ROFF_IsPlaying(int entNum)
{
	int i;`r`n	for (i = 0; i < MAX_ROFF_PLAYERS_OJP; i++) {
		if (ojp_roffPlayers[i].inuse && ojp_roffPlayers[i].entNum == entNum) {
			return qtrue;
		}
	}
	return qfalse;
}

