#include "g_local.h"
#include "qcommon/ojp_shared.h"
#include "ojp_dom_flags.h"

void ojp_DomSys_Init(void)
{
	ojp_Dom_Init();
}

void ojp_DomSys_Update(void)
{
	ojp_Dom_Update();

	if ((level.time % 2000) == 0) {
		int redScore = ojp_Dom_GetTeamScore(TEAM_RED);
		int blueScore = ojp_Dom_GetTeamScore(TEAM_BLUE);

		if (redScore > blueScore) {
			level.teamScores[TEAM_RED] += 1;
		} else if (blueScore > redScore) {
			level.teamScores[TEAM_BLUE] += 1;
		}
	}
}

void ojp_DomSys_SpawnFlags(void)
{
	gentity_t *ent = NULL;

	while ((ent = G_Find(ent, FOFS(classname), "team_CTF_redflag")) != NULL) {
		ojp_Dom_AddFlag(ent->s.origin);
	}
	while ((ent = G_Find(ent, FOFS(classname), "team_CTF_blueflag")) != NULL) {
		ojp_Dom_AddFlag(ent->s.origin);
	}
}
