#pragma once

#include "qcommon/q_shared.h"
#include "game/bg_public.h"

void QDECL G_Printf(const char *fmt, ...);

class CVec3
{
public:
	CVec3() : x(0), y(0), z(0) {}
	CVec3(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_) {}
	CVec3(const vec3_t a);

	float& operator[](int i);
	float operator[](int i) const;
	CVec3& operator=(const CVec3& b);
	friend CVec3 operator+(const CVec3& a, const CVec3& b);
	friend CVec3 operator-(const CVec3& a, const CVec3& b);
	CVec3& operator*=(CVec3 b);
	CVec3& operator*=(float b);
	CVec3& operator+=(CVec3 b);
	CVec3& operator-=(CVec3 b);
	bool operator!=(vec3_t b);
	CVec3& operator/=(vec3_t b);
	CVec3& operator/=(float b);
	bool operator<(CVec3 b);
	bool operator>(CVec3 b);
	friend CVec3 operator*(CVec3 a, float b);

	union
	{
		float data[3];
		struct { float x, y, z; };
		vec3_t v;
	};

	float Dist(CVec3 b) const;
	float Dist2(CVec3 b) const;
	float DistToLine(CVec3 a, CVec3 b) const;
	float DistToLine2(CVec3 a, CVec3 b) const;
	void ToStr(char *dest);

	float NormalizeSafe();
	float Normalize();
	float Dot(CVec3 b) const;
	float Length() const;
	float Length2() const;
	CVec3 Cross(CVec3 b) const;
	void Zero();
	void Copy(vec3_t v);
	void Copy(float a, float b, float c);
	qboolean Compare(CVec3 v, float tolerance) const;

	static float *GetData(CVec3 &v) { return v.data; }
	static CVec3 Average(CVec3 a, CVec3 b);
	static CVec3 MidPoint(CVec3 a, CVec3 b);
	static float Dist(CVec3 a, CVec3 b);
	static float Dist2(CVec3 a, CVec3 b);

	static bool RayIntersectsTriangle(CVec3 rayOrigin, CVec3 rayVector, CVec3 *triangle, CVec3 &outIntersectionPoint);
	static bool RayIntersectsTri(CVec3 o, CVec3 d, CVec3 &v0, CVec3 &v1, CVec3 &v2, float &t, float &u, float &v);
};
