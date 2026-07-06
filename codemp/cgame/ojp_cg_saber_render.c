#include "cg_local.h"
extern vmCvar_t cg_MovieSaberType;

void ojp_CG_DoPrequelSaber(vec3_t blade_muz, vec3_t blade_tip, vec3_t trail_tip, vec3_t trail_muz,
	float lengthMax, float radius, saber_colors_t color, int rfx, qboolean doLight, qboolean doTrail,
	int cnum, int bnum, int saberType)
{
	vec3_t dif, mid, blade_dir, end_dir, trail_dir, base_dir;
	float radiusmult, effectradius, coreradius, effectalpha, AngleScale;
	float blade_len, end_len, base_len, DisTip, DisMuz, DisDif;
	float glowscale = 0.5f;
	float v1, v2, len;
	vec3_t rgb = { 1, 1, 1 };
	float ignite_len, ignite_radius;
	qhandle_t glow = 0, ignite = 0, blade = 0;
	refEntity_t saber;

	VectorSubtract(blade_tip, blade_muz, blade_dir);
	VectorSubtract(trail_tip, trail_muz, trail_dir);
	blade_len = VectorLength(blade_dir);
	VectorNormalize(blade_dir);
	VectorNormalize(trail_dir);

	VectorSubtract(trail_tip, blade_tip, end_dir);
	VectorSubtract(trail_muz, blade_muz, base_dir);
	end_len = VectorLength(end_dir);
	base_len = VectorLength(base_dir);
	VectorNormalize(end_dir);
	VectorNormalize(base_dir);

	switch (color) {
	case SABER_RED:
		switch (saberType) {
		case 1: glow = cgs.media.redEp1GlowShader; blade = cgs.media.ep1SaberCoreShader; break;
		case 2: glow = cgs.media.redEp2GlowShader; blade = cgs.media.ep2SaberCoreShader; ignite = cgs.media.whiteIgniteFlare; rgb[0] = 255; rgb[1] = 0; rgb[2] = 0; break;
		case 3: glow = cgs.media.redEp3GlowShader; blade = cgs.media.ep3SaberCoreShader; ignite = cgs.media.redIgniteFlare; break;
		default: return;
		} break;
	case SABER_ORANGE:
		switch (saberType) {
		case 1: glow = cgs.media.orangeEp1GlowShader; blade = cgs.media.ep1SaberCoreShader; break;
		case 2: glow = cgs.media.orangeEp2GlowShader; blade = cgs.media.ep2SaberCoreShader; ignite = cgs.media.whiteIgniteFlare; rgb[0] = 255; rgb[1] = 128; rgb[2] = 0; break;
		case 3: glow = cgs.media.orangeEp3GlowShader; blade = cgs.media.ep3SaberCoreShader; ignite = cgs.media.orangeIgniteFlare; break;
		default: return;
		} break;
	case SABER_YELLOW:
		switch (saberType) {
		case 1: glow = cgs.media.yellowEp1GlowShader; blade = cgs.media.ep1SaberCoreShader; break;
		case 2: glow = cgs.media.yellowEp2GlowShader; blade = cgs.media.ep2SaberCoreShader; ignite = cgs.media.whiteIgniteFlare; rgb[0] = 255; rgb[1] = 200; rgb[2] = 0; break;
		case 3: glow = cgs.media.yellowEp3GlowShader; blade = cgs.media.ep3SaberCoreShader; ignite = cgs.media.yellowIgniteFlare; break;
		default: return;
		} break;
	case SABER_GREEN:
		switch (saberType) {
		case 1: glow = cgs.media.greenEp1GlowShader; blade = cgs.media.ep1SaberCoreShader; break;
		case 2: glow = cgs.media.greenEp2GlowShader; blade = cgs.media.ep2SaberCoreShader; ignite = cgs.media.whiteIgniteFlare; rgb[0] = 63; rgb[1] = 255; rgb[2] = 0; break;
		case 3: glow = cgs.media.greenEp3GlowShader; blade = cgs.media.ep3SaberCoreShader; ignite = cgs.media.greenIgniteFlare; break;
		default: return;
		} break;
	case SABER_PURPLE:
		switch (saberType) {
		case 1: glow = cgs.media.purpleEp1GlowShader; blade = cgs.media.ep1SaberCoreShader; break;
		case 2: glow = cgs.media.purpleEp2GlowShader; blade = cgs.media.ep2SaberCoreShader; ignite = cgs.media.whiteIgniteFlare; rgb[0] = 128; rgb[1] = 0; rgb[2] = 255; break;
		case 3: glow = cgs.media.purpleEp3GlowShader; blade = cgs.media.ep3SaberCoreShader; ignite = cgs.media.purpleIgniteFlare; break;
		default: return;
		} break;
	case SABER_BLUE:
		switch (saberType) {
		case 1: glow = cgs.media.blueEp1GlowShader; blade = cgs.media.ep1SaberCoreShader; break;
		case 2: glow = cgs.media.blueEp2GlowShader; blade = cgs.media.ep2SaberCoreShader; ignite = cgs.media.whiteIgniteFlare; rgb[0] = 0; rgb[1] = 65; rgb[2] = 255; break;
		case 3: glow = cgs.media.blueEp3GlowShader; blade = cgs.media.ep3SaberCoreShader; ignite = cgs.media.blueIgniteFlare; break;
		default: return;
		} break;
	default:
		switch (saberType) {
		case 1: glow = cgs.media.blueEp1GlowShader; blade = cgs.media.ep1SaberCoreShader; break;
		case 2: glow = cgs.media.blueEp2GlowShader; blade = cgs.media.ep2SaberCoreShader; ignite = cgs.media.whiteIgniteFlare; rgb[0] = 0; rgb[1] = 65; rgb[2] = 255; break;
		case 3: glow = cgs.media.blueEp3GlowShader; blade = cgs.media.ep3SaberCoreShader; ignite = cgs.media.blueIgniteFlare; break;
		default: return;
		} break;
	}

	VectorMA(blade_muz, blade_len * 0.5f, blade_dir, mid);

	if (doLight) {
		VectorScale(rgb, 0.66f, rgb);
		trap->R_AddLightToScene(mid, (blade_len * 2.0f) + ((rand() % 100) / 100.0f * 10.0f), rgb[0], rgb[1], rgb[2]);
	}

	VectorSubtract(mid, cg.refdef.vieworg, dif);
	len = VectorLength(dif);
	if (len > 4000) len = 4000;
	else if (len < 1) len = 1;
	v1 = (len + 400) / 400;
	v2 = (len + 4000) / 4000;

	if (end_len > 1 || base_len > 1) {
		if (end_len > base_len) glowscale = (end_len + 4) * 0.1f;
		else glowscale = (base_len + 4) * 0.1f;
		if (glowscale > 1.0f) glowscale = 1.0f;
		effectalpha = glowscale;
	} else effectalpha = glowscale;

	VectorSubtract(blade_tip, cg.refdef.vieworg, dif);
	DisTip = VectorLength(dif);
	VectorSubtract(blade_muz, cg.refdef.vieworg, dif);
	DisMuz = VectorLength(dif);
	DisDif = (DisTip > DisMuz) ? DisTip - DisMuz : DisMuz - DisTip;

	if (blade_len > 0) {
		float ratio = DisDif / blade_len;
		AngleScale = 1.2f - ratio * ratio;
	} else AngleScale = 1.0f;

	if (AngleScale > 1.0f) AngleScale = 1.0f;
	if (AngleScale < 0.2f) AngleScale = 0.2f;
	effectalpha *= AngleScale;
	AngleScale += 0.3f;
	if (AngleScale > 1.0f) AngleScale = 1.0f;
	if (AngleScale < 0.4f) AngleScale = 0.4f;

	memset(&saber, 0, sizeof(refEntity_t));

	if (blade_len < lengthMax) radiusmult = 0.5f + ((blade_len / lengthMax) / 2);
	else radiusmult = 1.0f;

	effectradius = ((radius * 1.6f * v1) + ((rand() % 20) / 100.0f * 0.1f)) * radiusmult;
	coreradius = ((radius * 0.4f * v2) + ((rand() % 20) / 100.0f * 0.1f)) * radiusmult;

	ignite_len = lengthMax * 0.25f;
	ignite_radius = effectradius * effectradius * 1.5f - blade_len;
	if (saberType == 2) ignite_radius *= 2.0f;

	if (blade && glow) {
	VectorCopy(blade_muz, saber.origin);
		VectorCopy(blade_tip, saber.oldorigin);
		saber.customShader = glow;
		saber.reType = RT_LINE;
		saber.radius = effectradius * effectalpha * AngleScale;
		if (blade_len >= 8.0f) {
			trap->R_AddRefEntityToScene(&saber);
		}

		saber.customShader = blade;
		saber.reType = RT_LINE;
		saber.radius = coreradius * AngleScale;
		trap->R_AddRefEntityToScene(&saber);
	}

	if (ignite && doTrail && cg_MovieSaberType.integer) {
		saber.customShader = ignite;
		saber.reType = RT_LINE;
		saber.radius = 1.5f;
		VectorCopy(blade_muz, saber.origin);
		VectorMA(blade_muz, ignite_len, blade_dir, saber.oldorigin);
		trap->R_AddRefEntityToScene(&saber);
	}
}
