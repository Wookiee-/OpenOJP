#include "ojp_CVec.h"
#include "qcommon/q_shared.h"
#include "game/bg_public.h"

CVec3::CVec3(const vec3_t a)
{
	x = a[0];
	y = a[1];
	z = a[2];
}

float& CVec3::operator[](int i) { return data[i]; }
float CVec3::operator[](int i) const { return data[i]; }

CVec3& CVec3::operator=(const CVec3& b)
{
	x = b.x; y = b.y; z = b.z;
	return *this;
}

CVec3 operator+(const CVec3& a, const CVec3& b)
{
	return CVec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

CVec3 operator-(const CVec3& a, const CVec3& b)
{
	return CVec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

CVec3& CVec3::operator*=(CVec3 b)
{
	x *= b.x; y *= b.y; z *= b.z;
	return *this;
}

CVec3& CVec3::operator*=(float b)
{
	x *= b; y *= b; z *= b;
	return *this;
}

CVec3& CVec3::operator+=(CVec3 b)
{
	x += b.x; y += b.y; z += b.z;
	return *this;
}

CVec3& CVec3::operator-=(CVec3 b)
{
	x -= b.x; y -= b.y; z -= b.z;
	return *this;
}

bool CVec3::operator!=(vec3_t b)
{
	return (x != b[0] || y != b[1] || z != b[2]);
}

CVec3& CVec3::operator/=(vec3_t b)
{
	if (b[0]) x /= b[0];
	if (b[1]) y /= b[1];
	if (b[2]) z /= b[2];
	return *this;
}

CVec3& CVec3::operator/=(float b)
{
	if (b) { x /= b; y /= b; z /= b; }
	return *this;
}

bool CVec3::operator<(CVec3 b)
{
	return (Length2() < b.Length2());
}

bool CVec3::operator>(CVec3 b)
{
	return (Length2() > b.Length2());
}

CVec3 operator*(CVec3 a, float b)
{
	return CVec3(a.x * b, a.y * b, a.z * b);
}

float CVec3::Dist(CVec3 b) const
{
	CVec3 c = *this - b;
	return sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
}

float CVec3::Dist2(CVec3 b) const
{
	CVec3 c = *this - b;
	return (c.x * c.x + c.y * c.y + c.z * c.z);
}

float CVec3::DistToLine(CVec3 a, CVec3 b) const
{
	return sqrt(DistToLine2(a, b));
}

float CVec3::DistToLine2(CVec3 a, CVec3 b) const
{
	CVec3 ab = b - a;
	CVec3 ac = *this - a;
	float f = ac.Dot(ab);
	if (f < 0) return Dist2(a);
	if (f > ab.Dot(ab)) return Dist2(b);
	return Dist2(a + (ab * (f / ab.Dot(ab))));
}

void CVec3::ToStr(char *dest)
{
	sprintf(dest, "%f %f %f", x, y, z);
}

float CVec3::NormalizeSafe()
{
	float lensq = x * x + y * y + z * z;
	if (lensq == 0) return 0;
	float length = sqrt(lensq);
	float invLength = 1.0f / length;
	x *= invLength; y *= invLength; z *= invLength;
	return length;
}

float CVec3::Normalize()
{
	float length = sqrt(x * x + y * y + z * z);
	if (length > 0) {
		float invLength = 1.0f / length;
		x *= invLength; y *= invLength; z *= invLength;
	}
	return length;
}

float CVec3::Dot(CVec3 b) const
{
	return x * b.x + y * b.y + z * b.z;
}

float CVec3::Length() const
{
	return sqrt(x * x + y * y + z * z);
}

float CVec3::Length2() const
{
	return x * x + y * y + z * z;
}

CVec3 CVec3::Cross(CVec3 b) const
{
	return CVec3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
}

void CVec3::Zero()
{
	x = y = z = 0;
}

void CVec3::Copy(vec3_t v)
{
	v[0] = x; v[1] = y; v[2] = z;
}

void CVec3::Copy(float a, float b, float c)
{
	x = a; y = b; z = c;
}

qboolean CVec3::Compare(CVec3 v, float tolerance) const
{
	return (qboolean)(fabs(x - v.x) <= tolerance &&
		fabs(y - v.y) <= tolerance &&
		fabs(z - v.z) <= tolerance);
}

CVec3 CVec3::Average(CVec3 a, CVec3 b)
{
	return CVec3((a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f, (a.z + b.z) * 0.5f);
}

CVec3 CVec3::MidPoint(CVec3 a, CVec3 b)
{
	return Average(a, b);
}

float CVec3::Dist(CVec3 a, CVec3 b)
{
	return a.Dist(b);
}

float CVec3::Dist2(CVec3 a, CVec3 b)
{
	return a.Dist2(b);
}

bool CVec3::RayIntersectsTriangle(CVec3 rayOrigin, CVec3 rayVector, CVec3 *triangle, CVec3 &outIntersectionPoint)
{
	float t, u, v;
	if (RayIntersectsTri(rayOrigin, rayVector, triangle[0], triangle[1], triangle[2], t, u, v)) {
		outIntersectionPoint = rayOrigin + rayVector * t;
		return true;
	}
	return false;
}

bool CVec3::RayIntersectsTri(CVec3 o, CVec3 d, CVec3 &v0, CVec3 &v1, CVec3 &v2, float &t, float &u, float &v)
{
	CVec3 e1 = v1 - v0;
	CVec3 e2 = v2 - v0;
	CVec3 h = d.Cross(e2);
	float a = e1.Dot(h);
	if (a > -0.00001f && a < 0.00001f) return false;
	float f = 1.0f / a;
	CVec3 s = o - v0;
	u = f * s.Dot(h);
	if (u < 0.0f || u > 1.0f) return false;
	CVec3 q = s.Cross(e1);
	v = f * d.Dot(q);
	if (v < 0.0f || u + v > 1.0f) return false;
	t = f * e2.Dot(q);
	return t > 0.00001f;
}
