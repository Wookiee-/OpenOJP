#pragma once

// OJP-specific shared definitions
// NOTE: Include g_local.h or cg_local.h BEFORE this header

// Integer vector types
typedef int ojp_ivec3_t[3];
typedef int ojp_ivec4_t[4];
typedef int ojp_ivec5_t[5];

// Additional SEF flags (extend the ones in w_saber.h)
#define SEF_KNOCKDOWN_OJP 0x200
#define SEF_DISARMED_OJP 0x400
#define SEF_PUSHED_OJP 0x800

// Additional STAT indices
#define STAT_MISHAP_LEVEL_OJP 8
#define STAT_DODGE_OJP 9

// Mishap / Balance system
#define MISHAPLEVEL_NONE_OJP 0
#define MISHAPLEVEL_LIGHT_OJP 1
#define MISHAPLEVEL_HEAVY_OJP 2
#define MISHAPLEVEL_FULL_OJP 3
#define MISHAPLEVEL_MAX_OJP 3

#define DODGE_CRITICALLEVEL_OJP 25

// Saber blocked type extension
#define BLOCKED_BACK_OJP 14

// Extended saber colors (indices 7-11)
#define SABER_WHITE_OJP 7
#define SABER_BLACK_OJP 8
#define SABER_RGB_OJP 9
#define SABER_PIMP_OJP 10
#define SABER_SCRIPTED_OJP 11

// Skills system
typedef enum {
	SK_JETPACK_OJP,
	SK_PISTOL_OJP,
	SK_BLASTER_OJP,
	SK_THERMAL_OJP,
	SK_ROCKET_OJP,
	SK_BACTA_OJP,
	SK_FLAMETHROWER_OJP,
	SK_BOWCASTER_OJP,
	SK_FORCEFIELD_OJP,
	SK_CLOAK_OJP,
	SK_SEEKER_OJP,
	SK_SENTRY_OJP,
	SK_DETPACK_OJP,
	SK_REPEATER_OJP,
	SK_DISRUPTOR_OJP,
	SK_BLUESTYLE_OJP,
	SK_REDSTYLE_OJP,
	SK_PURPLESTYLE_OJP,
	SK_GREENSTYLE_OJP,
	SK_DUALSTYLE_OJP,
	SK_STAFFSTYLE_OJP,
	SK_REPEATERUPGRADE_OJP,
	SK_FLECHETTE_OJP,
	SK_BLASTERRATEOFFIREUPGRADE_OJP,
	NUM_SKILLS_OJP
} ojp_skills_t;

#define NUM_TOTAL_SKILLS_OJP (NUM_FORCE_POWERS + NUM_SKILLS_OJP)

// Vector4Copy macro (not defined in OpenJK)
#ifndef Vector4Copy
#define Vector4Copy(a,b) ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])
#endif

// Additional FLAG constants for userInt3
#define FLAG_PREBLOCK_OJP 9
#define FLAG_FATIGUED_HEAVY_OJP 8
#define FLAG_DODGEROLL_OJP 10

// GAME_INLINE
#ifndef GAME_INLINE
#if defined(_WIN32)
#define GAME_INLINE __inline
#else
#define GAME_INLINE inline
#endif
#endif
