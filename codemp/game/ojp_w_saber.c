#include "g_local.h"
#include "w_saber.h"

#define OJP_MAX_REAL_PASSTHRU 8
#define OJP_REALTRACEDATADEFAULT -9999
#define OJP_REALTRACE_SABERBLOCKHIT 2

static int ojp_RealTraceContent_ent[OJP_MAX_REAL_PASSTHRU];
static int ojp_RealTraceContent_val[OJP_MAX_REAL_PASSTHRU];

static void ojp_InitRealTraceContent(void)
{
	int i;
	for (i = 0; i < OJP_MAX_REAL_PASSTHRU; i++) {
		ojp_RealTraceContent_ent[i] = OJP_REALTRACEDATADEFAULT;
		ojp_RealTraceContent_val[i] = OJP_REALTRACEDATADEFAULT;
	}
}

static qboolean ojp_AddRealTraceContent(int entityNum)
{
	int i;
	if (entityNum == ENTITYNUM_WORLD || entityNum == ENTITYNUM_NONE) return qtrue;
	for (i = 0; i < OJP_MAX_REAL_PASSTHRU; i++) {
		if (ojp_RealTraceContent_ent[i] == OJP_REALTRACEDATADEFAULT && ojp_RealTraceContent_val[i] == OJP_REALTRACEDATADEFAULT) {
			ojp_RealTraceContent_ent[i] = entityNum;
			ojp_RealTraceContent_val[i] = g_entities[entityNum].r.contents;
			g_entities[entityNum].r.contents = 0;
			return qtrue;
		}
	}
	return qfalse;
}

static void ojp_RestoreRealTraceContent(void)
{
	int i;
	for (i = 0; i < OJP_MAX_REAL_PASSTHRU; i++) {
		if (ojp_RealTraceContent_ent[i] != OJP_REALTRACEDATADEFAULT) {
			if (ojp_RealTraceContent_val[i] != OJP_REALTRACEDATADEFAULT) {
				g_entities[ojp_RealTraceContent_ent[i]].r.contents = ojp_RealTraceContent_val[i];
				ojp_RealTraceContent_ent[i] = OJP_REALTRACEDATADEFAULT;
				ojp_RealTraceContent_val[i] = OJP_REALTRACEDATADEFAULT;
			}
		} else break;
	}
}

static float ojp_CalcTraceFraction(vec3_t Start, vec3_t End, vec3_t Endpos)
{
	float fulldist = Distance(Start, End);
	float dist = Distance(Start, Endpos);
	if (fulldist > 0) {
		if (dist > 0) return dist / fulldist;
		return 0;
	}
	return 1;
}

static void ojp_TraceCopy(trace_t *a, trace_t *b)
{
	b->allsolid = a->allsolid;
	b->contents = a->contents;
	VectorCopy(a->endpos, b->endpos);
	b->entityNum = a->entityNum;
	b->fraction = a->fraction;
	VectorCopy(a->plane.normal, b->plane.normal);
	b->startsolid = a->startsolid;
	b->surfaceFlags = a->surfaceFlags;
}

static void ojp_TraceClear(trace_t *tr, vec3_t end)
{
	memset(tr, 0, sizeof(*tr));
	tr->fraction = 1.0f;
	tr->entityNum = ENTITYNUM_NONE;
	VectorCopy(end, tr->endpos);
}

qboolean ojp_SaberCanBlock(gentity_t *self, gentity_t *atk, qboolean checkBBoxBlock, vec3_t point, int rSaberNum, int rBladeNum)
{
	if (!self || !self->client || !atk) return qfalse;
	if (!self->client->ps.saberEntityNum || self->client->ps.saberInFlight) return qfalse;
	if (self->client->ps.saberHolstered >= 2) return qfalse;
	if (self->client->ps.weapon != WP_SABER) return qfalse;
	if (PM_InKnockDown(&self->client->ps)) return qfalse;
	if (self->client->ps.forceHandExtend == HANDEXTEND_DODGE) return qfalse;
	if (!InFront(point, self->client->ps.origin, self->client->ps.viewangles, -0.2f)) {
		if (self->client->ps.fd.forcePowerLevel[FP_SABER_DEFENSE] >= FORCE_LEVEL_3) return qtrue;
		return qfalse;
	}
	return qtrue;
}

int ojp_G_RealTrace(gentity_t *attacker, trace_t *tr, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passEntityNum, int contentmask, int rSaberNum, int rBladeNum)
{
	vec3_t currentStart, currentEndPos, impactPoint;
	trace_t closestTrace;
	float closestFraction = 1.1f;
	int misses, currentEntityNum;
	gentity_t *currentEnt, *saberOwner;
	qboolean atkIsSaberer;

	atkIsSaberer = (attacker && attacker->client && attacker->client->ps.weapon == WP_SABER) ? qtrue : qfalse;

	ojp_InitRealTraceContent();

	if (atkIsSaberer) {
		attacker->client->lastSaberCollided = -1;
		attacker->client->lastBladeCollided = -1;
	}

	ojp_TraceClear(&closestTrace, end);
	VectorCopy(start, currentStart);

	for (misses = 0; misses < OJP_MAX_REAL_PASSTHRU; misses++) {

		trap_Trace(tr, currentStart, mins, maxs, end, passEntityNum, contentmask);
		VectorCopy(tr->endpos, currentEndPos);
		currentEntityNum = tr->entityNum;

		if (tr->startsolid) VectorCopy(currentStart, tr->endpos);

		if (tr->entityNum == ENTITYNUM_NONE) {
			if (!VectorCompare(start, currentStart))
				tr->fraction = ojp_CalcTraceFraction(start, end, tr->endpos);
			if (tr->fraction < closestFraction) {
				ojp_TraceCopy(tr, &closestTrace);
				closestFraction = tr->fraction;
			}
			ojp_RestoreRealTraceContent();
			return tr->entityNum;
		}

		currentEnt = &g_entities[tr->entityNum];

		if (currentEnt->inuse && currentEnt->client) {
			if (attacker && ojp_SaberCanBlock(currentEnt, attacker, qtrue, tr->endpos, rSaberNum, rBladeNum)) {
				if (!VectorCompare(start, currentStart))
					tr->fraction = ojp_CalcTraceFraction(start, end, tr->endpos);
				if (tr->fraction < closestFraction) {
					ojp_RestoreRealTraceContent();
					tr->entityNum = currentEnt->client->saberStoredIndex;
					return OJP_REALTRACE_SABERBLOCKHIT;
				} else {
					if (closestTrace.fraction < tr->fraction) ojp_TraceCopy(&closestTrace, tr);
					ojp_RestoreRealTraceContent();
					return tr->entityNum;
				}
			}
			extern qboolean G_G2TraceCollide(trace_t *t, vec3_t s, vec3_t e, vec3_t mi, vec3_t ma);
			G_G2TraceCollide(tr, currentStart, end, mins, maxs);
		} else if ((currentEnt->r.contents & CONTENTS_LIGHTSABER) && currentEnt->r.contents != -1 && currentEnt->inuse) {
			saberOwner = &g_entities[currentEnt->r.ownerNum];
			VectorCopy(tr->endpos, impactPoint);
			extern qboolean G_SaberCollide(gentity_t *a, gentity_t *d, vec3_t s, vec3_t e, vec3_t mi, vec3_t ma, vec3_t ip);
			G_SaberCollide((atkIsSaberer ? attacker : NULL), saberOwner, currentStart, end, mins, maxs, impactPoint);
		} else if (tr->entityNum < ENTITYNUM_WORLD) {
			if (currentEnt->inuse && currentEnt->ghoul2) {
				extern qboolean G_G2TraceCollide(trace_t *t, vec3_t s, vec3_t e, vec3_t mi, vec3_t ma);
				G_G2TraceCollide(tr, currentStart, end, mins, maxs);
			} else {
				if (!VectorCompare(start, currentStart))
					tr->fraction = ojp_CalcTraceFraction(start, end, tr->endpos);
				if (tr->fraction < closestFraction) { ojp_TraceCopy(tr, &closestTrace); closestFraction = tr->fraction; }
				if (closestTrace.fraction < tr->fraction) ojp_TraceCopy(&closestTrace, tr);
				ojp_RestoreRealTraceContent();
				return tr->entityNum;
			}
		} else {
			if (!VectorCompare(start, currentStart))
				tr->fraction = ojp_CalcTraceFraction(start, end, tr->endpos);
			if (tr->fraction < closestFraction) { ojp_TraceCopy(tr, &closestTrace); closestFraction = tr->fraction; }
			if (closestTrace.fraction < tr->fraction) ojp_TraceCopy(&closestTrace, tr);
			ojp_RestoreRealTraceContent();
			return tr->entityNum;
		}

		if (!VectorCompare(start, currentStart))
			tr->fraction = ojp_CalcTraceFraction(start, end, tr->endpos);
		if (tr->fraction < closestFraction) { ojp_TraceCopy(tr, &closestTrace); closestFraction = tr->fraction; }
		if (!ojp_AddRealTraceContent(currentEntityNum)) break;
		VectorCopy(currentEndPos, currentStart);
	}

	if (closestTrace.fraction < tr->fraction) ojp_TraceCopy(&closestTrace, tr);
	ojp_RestoreRealTraceContent();
	return tr->entityNum;
}

void ojp_WP_SaberBlockNonRandom(gentity_t *self, vec3_t hitloc, qboolean missileBlock)
{
	vec3_t diff, fwdangles, right, clEye;
	float rightdot, zdiff;

	VectorClear(fwdangles);
	VectorCopy(self->client->ps.origin, clEye);
	clEye[2] += self->client->ps.viewheight;
	VectorSubtract(hitloc, clEye, diff);
	diff[2] = 0;
	VectorNormalize(diff);
	fwdangles[1] = self->client->ps.viewangles[1];
	AngleVectors(fwdangles, NULL, right, NULL);
	rightdot = DotProduct(right, diff);
	zdiff = hitloc[2] - clEye[2];

	if (zdiff > 0) {
		if (rightdot > 0.3f) self->client->ps.saberBlocked = BLOCKED_UPPER_RIGHT;
		else if (rightdot < -0.3f) self->client->ps.saberBlocked = BLOCKED_UPPER_LEFT;
		else self->client->ps.saberBlocked = BLOCKED_TOP;
	} else if (zdiff > -20) {
		if (rightdot > 0.1f) self->client->ps.saberBlocked = BLOCKED_UPPER_RIGHT;
		else if (rightdot < -0.1f) self->client->ps.saberBlocked = BLOCKED_UPPER_LEFT;
		else self->client->ps.saberBlocked = BLOCKED_TOP;
	} else {
		if (rightdot >= 0) self->client->ps.saberBlocked = BLOCKED_LOWER_RIGHT;
		else self->client->ps.saberBlocked = BLOCKED_LOWER_LEFT;
	}
	if (missileBlock) {
		extern int WP_MissileBlockForBlock(int sb);
		self->client->ps.saberBlocked = WP_MissileBlockForBlock(self->client->ps.saberBlocked);
	}
}
