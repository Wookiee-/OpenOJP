#pragma once

#include "cg_local.h"

qboolean ojp_CG_HolsterWeapon(int clientNum, int holsterIndex);
qboolean ojp_CG_UnholsterWeapon(int clientNum);
qboolean ojp_CG_IsHolstered(int clientNum);
int ojp_CG_GetHolsterIndex(int clientNum);
void ojp_CG_SetHolsterIndex(int clientNum, int index);
