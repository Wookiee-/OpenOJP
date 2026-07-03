#pragma once

#include "qcommon/q_shared.h"
#include "game/bg_public.h"

#define OJP_ROFF_HASH_SIZE 64

typedef struct {
	char fileName[MAX_QPATH];
	int numFrames;
	int frameSize;
	byte *data;
} ojp_roff_t;

void ojp_ROFF_Cache(const char *fileName);
qboolean ojp_ROFF_Play(int entNum, const char *fileName, qboolean doTranslation);
void ojp_ROFF_Update(gentity_t *ent);
void ojp_ROFF_Stop(int entNum);
qboolean ojp_ROFF_IsPlaying(int entNum);
void ojp_ROFF_Init(void);
