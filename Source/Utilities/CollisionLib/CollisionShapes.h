#ifndef SHAPES_H_
#define SHAPES_H_

#include <math.h>

#include "CollisionPlane.h"
#include "CollisionPoint.h"
#include "CollisionLine.h"
#include "CollisionRay.h"
#include "CollisionCircle.h"
#include "CollisionSphere.h"
#include "CollisionCapsule.h"
#include "CollisionBox.h"
#include "../MathLib/matrix3.h"
#include "../MathLib/matrix4.h"
#include "../MathLib/vec2.h"
#include "../MathLib/vec3.h"
#include "../MathLib/vec4.h"


#include "CollisionCube.h"
#include "CollisionAABB.h"
#include "CollisionPolygone.h"
#include "CollisionFrustum.h"

typedef CCollisionPoint Point;
typedef CCollisionPlane Plane;
typedef CCollisionLine Line;
typedef CCollisionRay Ray;
typedef CCollisionCircle Circle;
typedef CCollisionSphere Sphere;
typedef CCollisionCapsule Capsule;
typedef CCollisionBox Box;
typedef CCollisionCube Cube;
typedef CCollisionAABB AABB;
typedef CCollisionPolygone Polygone;
typedef CCollisionFrustum Frustum;

#ifndef SameSign
#define SameSign(a,b) ( ((*(unsigned int*)&(a)) & 0x80000000) == ((*(unsigned int*)&(b)) & 0x80000000) )
#endif 

#ifndef SameSignLessThen
#define SameSignLessThen(a,b) ( ((*(unsigned int*)&(a)) & 0x80000000) < ((*(unsigned int*)&(b)) & 0x80000000) )
#endif SameSignLessThen

struct Rotation
{
	float sine, cosine;
};

//struct Transform
//{
//	vec2f position;
//	Rotation rot;
//};
//

//static void Mul (vec2f& out, float sine, float cosine, vec2f vec)
//{
//	out.x = (cosine * vec.x + sine * vec.y);
//	out.y = (sine * vec.x - cosine * vec.y);
//}
//
//static void Mull (vec2f& out, CCollisionBox::Transform trans, vec2f vec)
//{
//	out.x = (trans.m_fCosine * vec.x + trans.m_fSine * vec.y) + trans.m_v2Position.x;
//	out.y = (trans.m_fSine * vec.x - trans.m_fCosine * vec.y) + trans.m_v2Position.y;
//}
//
//static vec2f Mul (float sine, float cosine, vec2f vec)
//{
//	float x = (cosine * vec.x + sine * vec.y);
//	float y = (sine * vec.x - cosine * vec.y);
//	return vec2f (x, y);
//}
//
//static vec2f MulT(float sine, float cosine, const vec2f& v)
//{
//	return vec2f(cosine * v.x - sine * v.y, sine * v.x + cosine * v.y);
//}
//
//static vec2f Mul (CCollisionBox::Transform trans, vec2f vec)
//{
//	float x = (trans.m_fCosine * vec.x + trans.m_fSine * vec.y) + trans.m_v2Position.x;
//	float y = (trans.m_fSine * vec.x - trans.m_fCosine * vec.y) + trans.m_v2Position.y;
//
//	return vec2f (x, y);
//}

static float Dot(const vec2f& a, const vec2f& b)
{
	return a.x * b.x + a.y * b.y;
}



//static vec2f MulT(const CCollisionBox::Transform& T, const vec2f& v)
//{
//	float px = v.x - T.m_v2Position.x;
//	float py = v.y - T.m_v2Position.y;
//	float x = (T.m_fCosine * px - T.m_fSine * py);
//	float y = (T.m_fSine * px + T.m_fCosine * py);
//
//	return vec2f(x, y);
//}

template <typename T>
inline T Min(T a, T b)
{
	return a < b ? a : b;
}

inline vec2f Min(const vec2f& a, const vec2f& b)
{
	return vec2f(Min(a.x, b.x), Min(a.y, b.y));
}

template <typename T>
inline T Max(T a, T b)
{
	return a > b ? a : b;
}

inline vec2f Max(const vec2f& a, const vec2f& b)
{
	return vec2f(Max(a.x, b.x), Max(a.y, b.y));
}

template <typename T>
inline T Clamp(T a, T low, T high)
{
	return Max(low, Min(a, high));
}

inline vec2f b2Clamp(const vec2f& a, const vec2f& low, const vec2f& high)
{
	return Max(low, Min(a, high));
}

#define timeToSleep				0.5f

/// A body cannot sleep if its linear velocity is above this tolerance.
#define linearSleepTolerance		0.01f

/// A body cannot sleep if its angular velocity is above this tolerance.
#define angularSleepTolerance	(2.0f / 180.0f * 3.14159265359f)



#define MaxTranslation			2.0f
#define MaxTranslationSquared	(MaxTranslation * MaxTranslation)
#define MaxRotation				(0.5f * 3.14159265359f)
#define MaxRotationSquared		(MaxRotation * MaxRotation)
#define VelocityThreshold		1.0f

template <typename T>
inline T Abs(T a)
{
	return a > T(0) ? a : -a;
}

inline vec2f Abs(const vec2f& a)
{
	return vec2f(Abs(a.x), Abs(a.y));
}

inline float Cross(const vec2f& a, const vec2f& b)
{
	return a.x * b.y - a.y * b.x;
}

inline vec2f Cross(float s, const vec2f& a)
{
	return vec2f(s * a.y, -s * a.x);
}

inline float Distance(const vec2f& a, const vec2f& b)
{
	vec2f c = a - b;
	return c.magnitude();
}

inline float DistanceSquared(const vec2f& a, const vec2f& b)
{
	vec2f c = a - b;
	return Dot(c, c);
}

//inline vec2f Mul(const Mat22& A, const vec2f& v)
//{
//	return vec2f(A.ex.x * v.x + A.ey.x * v.y, A.ex.y * v.x + A.ey.y * v.y);
//}


#endif