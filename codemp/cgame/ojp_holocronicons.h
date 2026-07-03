#pragma once

#define OJP_HOLOCRON_ICON_BLUE 0
#define OJP_HOLOCRON_ICON_RED 1
#define OJP_HOLOCRON_ICON_GREEN 2
#define OJP_HOLOCRON_ICON_PURPLE 3
#define OJP_HOLOCRON_ICON_YELLOW 4
#define OJP_HOLOCRON_ICON_WHITE 5

extern qhandle_t ojp_holocronIconShaders[6];

void ojp_CG_LoadHolocronIcons(void);
void ojp_CG_DrawHolocronIcon(int iconIndex, float x, float y, float w, float h);
