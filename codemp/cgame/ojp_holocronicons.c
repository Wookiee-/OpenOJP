#include "cg_local.h"
#include "ojp_holocronicons.h"

qhandle_t ojp_holocronIconShaders[6];

void ojp_CG_LoadHolocronIcons(void)
{
	ojp_holocronIconShaders[0] = trap_R_RegisterShader("holocons/icon_blue");
	ojp_holocronIconShaders[1] = trap_R_RegisterShader("holocons/icon_red");
	ojp_holocronIconShaders[2] = trap_R_RegisterShader("holocons/icon_green");
	ojp_holocronIconShaders[3] = trap_R_RegisterShader("holocons/icon_purple");
	ojp_holocronIconShaders[4] = trap_R_RegisterShader("holocons/icon_yellow");
	ojp_holocronIconShaders[5] = trap_R_RegisterShader("holocons/icon_white");
}

void ojp_CG_DrawHolocronIcon(int iconIndex, float x, float y, float w, float h)
{
	if (iconIndex < 0 || iconIndex >= 6) return;
	if (!ojp_holocronIconShaders[iconIndex]) return;
	CG_DrawPic(x, y, w, h, ojp_holocronIconShaders[iconIndex]);
}
