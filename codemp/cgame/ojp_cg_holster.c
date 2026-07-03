#include "cg_local.h"
#include "ojp_cg_holster.h"

#define MAX_HOLSTER_CLIENTS_OJP 32

typedef struct {
	qboolean holstered;
	int holsterIndex;
} ojp_holsterInfo_t;

static ojp_holsterInfo_t ojp_holsterInfo[MAX_HOLSTER_CLIENTS_OJP];

qboolean ojp_CG_HolsterWeapon(int clientNum, int holsterIndex)
{
	if (clientNum < 0 || clientNum >= MAX_HOLSTER_CLIENTS_OJP) return qfalse;
	ojp_holsterInfo[clientNum].holstered = qtrue;
	ojp_holsterInfo[clientNum].holsterIndex = holsterIndex;
	return qtrue;
}

qboolean ojp_CG_UnholsterWeapon(int clientNum)
{
	if (clientNum < 0 || clientNum >= MAX_HOLSTER_CLIENTS_OJP) return qfalse;
	ojp_holsterInfo[clientNum].holstered = qfalse;
	ojp_holsterInfo[clientNum].holsterIndex = 0;
	return qtrue;
}

qboolean ojp_CG_IsHolstered(int clientNum)
{
	if (clientNum < 0 || clientNum >= MAX_HOLSTER_CLIENTS_OJP) return qfalse;
	return ojp_holsterInfo[clientNum].holstered;
}

int ojp_CG_GetHolsterIndex(int clientNum)
{
	if (clientNum < 0 || clientNum >= MAX_HOLSTER_CLIENTS_OJP) return 0;
	return ojp_holsterInfo[clientNum].holsterIndex;
}

void ojp_CG_SetHolsterIndex(int clientNum, int index)
{
	if (clientNum < 0 || clientNum >= MAX_HOLSTER_CLIENTS_OJP) return;
	ojp_holsterInfo[clientNum].holsterIndex = index;
}
