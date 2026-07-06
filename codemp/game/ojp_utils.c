#include "g_local.h"
#include "qcommon/ojp_shared.h"

extern void trap_Trace( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask );
void ojp_AutoSave(void);

qboolean ojp_G_ValidSaberStyle(saber_styles_t style, gentity_t *self)
{
	if (!self || !self->client) return qfalse;
	if (style <= SS_NONE || style >= SS_NUM_SABER_STYLES) return qfalse;
	if (style == SS_DESANN && self->client->ps.fd.forcePowerLevel[FP_SABER_OFFENSE] < FORCE_LEVEL_3) return qfalse;
	if (style == SS_TAVION && self->client->ps.fd.forcePowerLevel[FP_SABER_OFFENSE] < FORCE_LEVEL_2) return qfalse;
	if (style == SS_DUAL && !self->client->saber[1].model[0]) return qfalse;
	if (style == SS_STAFF && self->client->saber[0].type != SABER_STAFF) return qfalse;
	return qtrue;
}

qboolean ojp_InPlayersFOV(gentity_t *self, gentity_t *target, float fov)
{
	vec3_t dir, angles;
	if (!self || !target) return qfalse;
	VectorSubtract(target->r.currentOrigin, self->client->ps.origin, dir);
	vectoangles(dir, angles);
	float delta = AngleDelta(angles[YAW], self->client->ps.viewangles[YAW]);
	return (fabs(delta) <= fov) ? qtrue : qfalse;
}

qboolean ojp_InPlayersPVS(gentity_t *self, gentity_t *target)
{
	if (!self || !target) return qfalse;
	return qtrue;
}

qboolean ojp_Jedi_StopKnockdown(gentity_t *self, gentity_t *pusher, const vec3_t pushDir)
{
	if (!self || !self->client) return qfalse;
	if (self->client->ps.fd.forcePowerLevel[FP_LEVITATION] < FORCE_LEVEL_1) return qfalse;
	if (self->client->ps.eFlags2 & EF2_FLYING) return qfalse;
	return qfalse;
}

float ojp_G_GroundDistance(gentity_t *self)
{
	vec3_t down, end;
	trace_t tr;
	if (!self) return 999999.0f;
	VectorCopy(self->r.currentOrigin, down);
	down[2] -= 4;
	VectorCopy(down, end);
	end[2] -= 1024;
	trap_Trace(&tr, down, self->r.mins, self->r.maxs, end, self->s.number, MASK_SOLID);
	return tr.fraction * 1024;
}

qboolean ojp_AdvancedWouldTelefrag(const vec3_t spawnPoint)
{
	int i;
	for (i = 0; i < MAX_GENTITIES; i++) {
		gentity_t *ent = &g_entities[i];
		if (!ent->inuse) continue;
		if (ent->health <= 0) continue;
		if (ent->r.contents & CONTENTS_CORPSE) continue;
		float dist = Distance(spawnPoint, ent->r.currentOrigin);
		if (dist < 20.0f) return qtrue;
	}
	return qfalse;
}

qboolean ojp_CheckForGoodSpawnPoint(vec3_t spawnPoint, vec3_t mins, vec3_t maxs)
{
	trace_t tr;
	trap_Trace(&tr, spawnPoint, mins, maxs, spawnPoint, -1, MASK_PLAYERSOLID);
	return (tr.fraction >= 1.0f) ? qtrue : qfalse;
}

void ojp_UpdatePlayerCameraPos(gentity_t *player, vec3_t origin, vec3_t angles)
{
	if (!player || !player->client) return;
}

void ojp_Use_Autosave(gentity_t *ent, gentity_t *other, gentity_t *activator)
{
	ojp_AutoSave();
}

void ojp_Create_Autosave(void)
{
	ojp_AutoSave();
}

void ojp_Delete_Autosaves(void)
{
}
