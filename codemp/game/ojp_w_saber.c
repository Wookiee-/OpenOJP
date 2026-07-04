#include "g_local.h"
#include "w_saber.h"
#include "qcommon/ojp_shared.h"
#include "ojp_saberbeh.h"

#define REALTRACE_SABERBLOCKHIT 2

extern int RealTraceContent_ent[8];
extern int RealTraceContent_val[8];

extern qboolean G_G2TraceCollide(trace_t *tr, vec3_t lastValidStart, vec3_t lastValidEnd, vec3_t traceMins, vec3_t traceMaxs);
extern qboolean G_SaberCollide(gentity_t *atk, gentity_t *def, vec3_t atkStart, vec3_t atkEnd, vec3_t atkMins, vec3_t atkMaxs, vec3_t impactPoint);

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

int ojp_G_RealTrace(gentity_t *attacker, trace_t *tr, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passEntityNum, int contentmask, int rSaberNum, int rBladeNum)
{
	vec3_t currentStart;
	trace_t closestTrace;
	float closestFraction = 1.1f;
	qboolean atkIsSaberer = (attacker && attacker->client && attacker->client->ps.weapon == WP_SABER) ? qtrue : qfalse;

	ojp_InitRealTraceContent();

	if (atkIsSaberer) {
		attacker->client->lastSaberCollided = -1;
		attacker->client->lastBladeCollided = -1;
	}

	ojp_TraceClear(&closestTrace, end);
	VectorCopy(start, currentStart);

	for (int misses = 0; misses < 8; misses++) {
		vec3_t currentEndPos;
		int currentEntityNum;

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

		gentity_t *currentEnt = &g_entities[tr->entityNum];

		if (currentEnt->inuse && currentEnt->client) {
			if (attacker && ojp_SaberCanBlock(currentEnt, attacker, qtrue, tr->endpos, rSaberNum, rBladeNum)) {
				if (!VectorCompare(start, currentStart))
					tr->fraction = ojp_CalcTraceFraction(start, end, tr->endpos);
				if (tr->fraction < closestFraction) {
					ojp_RestoreRealTraceContent();
					tr->entityNum = currentEnt->client->saberStoredIndex;
					return REALTRACE_SABERBLOCKHIT;
				} else {
					if (closestTrace.fraction < tr->fraction) ojp_TraceCopy(&closestTrace, tr);
					ojp_RestoreRealTraceContent();
					return tr->entityNum;
				}
			}
			G_G2TraceCollide(tr, currentStart, end, mins, maxs);
		} else if ((currentEnt->r.contents & CONTENTS_LIGHTSABER) && currentEnt->r.contents != -1 && currentEnt->inuse) {
			gentity_t *saberOwner = &g_entities[currentEnt->r.ownerNum];
			G_SaberCollide((atkIsSaberer ? attacker : NULL), saberOwner, currentStart, end, mins, maxs, tr);
		} else if (tr->entityNum < ENTITYNUM_WORLD) {
			if (currentEnt->inuse && currentEnt->ghoul2) {
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
	vec3_t diff, fwdangles = { 0, 0, 0 }, right, clEye;
	float rightdot, zdiff;
	qboolean inFront = InFront(hitloc, self->client->ps.origin, self->client->ps.viewangles, -0.2f);

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

	if (missileBlock)
		self->client->ps.saberBlocked = WP_MissileBlockForBlock(self->client->ps.saberBlocked);

	self->client->ps.userInt3 &= ~(1 << FLAG_PREBLOCK_OJP);
}
