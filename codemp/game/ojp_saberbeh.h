#pragma once

#include "qcommon/q_shared.h"
#include "game/bg_public.h"
#include "g_local.h"

#define BOT_ATTACKPARRYRATE_OJP 20
#define MPCOST_PARRIED_OJP 3
#define MPCOST_PARRIED_ATTACKFAKE_OJP 6
#define MPCOST_PARRYING_OJP -3
#define MPCOST_PARRYING_ATTACKFAKE_OJP -4
#define FPCOST_PARRYING_PURE_OJP -10

// FLAG constants for userInt3
#define FLAG_ATTACKFAKE_OJP 0
#define FLAG_SABERLOCK_ATTACKER_OJP 1
#define FLAG_PARRIED_OJP 2
#define FLAG_QUICKPARRY_OJP 3
#define FLAG_SLOWBOUNCE_OJP 4
#define FLAG_OLDSLOWBOUNCE_OJP 5

typedef struct {
	qboolean doStun;
	qboolean doKnockdown;
	qboolean doButterFingers;
	qboolean doParry;
	qboolean doSlowBounce;
	qboolean doHeavySlowBounce;
#ifdef _DEBUG
	int behaveMode;
#endif
} ojp_sabmech_t;

#ifdef _DEBUG
typedef enum {
	SABBEHAVE_NONE_OJP = 0,
	SABBEHAVE_ATTACK_OJP,
	SABBEHAVE_ATTACKPARRIED_OJP,
	SABBEHAVE_ATTACKBLOCKED_OJP,
	SABBEHAVE_BLOCK_OJP,
	SABBEHAVE_BLOCKFAKED_OJP,
	SABBEHAVE_PARRY_OJP,
} ojp_sabBehave_t;
#endif

// Core saber behavior functions
void ojp_SabBeh_RunSaberBehavior(gentity_t *self, ojp_sabmech_t *mechSelf,
	gentity_t *otherOwner, ojp_sabmech_t *mechOther, vec3_t hitLoc,
	qboolean *didHit, qboolean otherHitSaberBlade);

qboolean ojp_SabBeh_RollBalance(gentity_t *self, ojp_sabmech_t *mechSelf, qboolean forceMishap);
void ojp_G_RollBalance(gentity_t *self, gentity_t *inflictor, qboolean forceMishap);
void ojp_SabBeh_AddBalance(gentity_t *self, ojp_sabmech_t *mechSelf, int amount, qboolean attack);
void ojp_G_AddMercBalance(gentity_t *self, int amount);
void ojp_SabBeh_AnimateSlowBounce(gentity_t *self, gentity_t *inflictor);
void ojp_SabBeh_AnimateHeavySlowBounce(gentity_t *self, gentity_t *inflictor);
qboolean ojp_SabBeh_ButtonforSaberLock(gentity_t *self);
void ojp_BG_ReduceMishapLevel(playerState_t *ps);

// Referenced OJP functions that need to exist
int ojp_SaberBlockCost(gentity_t *defender, gentity_t *attacker, vec3_t hitLoc);
int ojp_SaberCanBlock(gentity_t *self, gentity_t *atk, qboolean checkBBoxBlock, vec3_t point, int rSaberNum, int rBladeNum);
qboolean ojp_BlockIsParry(gentity_t *self, gentity_t *attacker, vec3_t hitLoc);
qboolean ojp_BlockIsQuickParry(gentity_t *self, gentity_t *attacker, vec3_t hitLoc);
qboolean ojp_InAttackParry(gentity_t *self);
void ojp_BG_AddFatigue(playerState_t *ps, int fatigue);
void ojp_G_DodgeDrain(gentity_t *blocker, gentity_t *attacker, int amount);
