//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionRay.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds on to the information for a Ray
//////////////////////////////////////////////////////////////////////////////////////
#include "CollisionRay.h"
#include "CollisionShapes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCollisionRay():	Default Constructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionRay::CCollisionRay ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCollisionRay():	Default Destructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionRay::~CCollisionRay ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClosestPointOnRay():	Takes in a vec3f (vPoint) and an output vec3f (ptA). This will find the closest point on the
//						ray using the point passed in and save the point on the ray in the output pramater.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCollisionRay::ClosestPointOnRay( vec3f vPointA, vec3f& pta )
{
	// find the direction from the point to start of the ray.
	vec3f vvectopoint = vPointA - m_cptStart.Get3DCentorid();

	// calculate the dot product of the direction and above direction
	float fdistance = dot_product (m_cptDirection.Get3DCentorid(), vvectopoint);

	// if the dot product is less then 0 then the closest point is the start point
	if ( fdistance < 0.0f)
	{
		pta = m_cptStart.Get3DCentorid();
		return;
	}
	
	// calculate the normal with the length of the direction
	vec3f vnormalprime = m_cptDirection.Get3DCentorid() * fdistance;
	// calculate the new point on the line using the start point
	pta = m_cptStart.Get3DCentorid() + vnormalprime;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RayToPlane():	Takes in a Plane (plA) and an output vec3f (ptA). This will find the closest point on the plane
//					using the ray and save the point in the output pramater.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionRay::RayToPlane ( CCollisionPlane pla, vec3f& pta)
{
	// early out test to see if the ray is faceing away from the plane or facing perpendicular
	if (!dot_product (pla.GetNormal3D(), m_cptDirection.Get3DCentorid()))
	{
		return false;
	}

	if (pla.PositiveHalfSpaceTest (m_cptStart.Get3DCentorid()))
	{
		if (dot_product (pla.GetNormal3D(), m_cptDirection.Get3DCentorid()) > 0)
		{
			return false;
		}
	}
	else
	{
		if (dot_product (pla.GetNormal3D(), m_cptDirection.Get3DCentorid()) < 0)
		{
			return false;
		}
	}

	// find the time of intersection on the ray
	float ftime = (pla.GetOffset() - (dot_product (pla.GetNormal3D(), m_cptStart.Get3DCentorid()))) / (dot_product (pla.GetNormal3D(), m_cptDirection.Get3DCentorid()));
	// calculate the point at that time of intersection.
	pta = m_cptStart.Get3DCentorid() + m_cptDirection.Get3DCentorid() * ftime;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RayToSphere():	Takes in a Sphere (spA) and an output vec3f (ptA). This function will find the closest point on the
//					sphere and put the point on the sphere in the output pramater.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionRay::RayToSphere ( CCollisionSphere spa, vec3f& pta)
{
	// calculate the vector from the start of the ray to the sphere's center
	vec3f vdirtosphere = m_cptStart.Get3DCentorid() - spa.GetCenter3D();
	// get the distance of the vector in the direction of the ray's normal
	float fb = dot_product (vdirtosphere, m_cptDirection.Get3DCentorid());
	// calculate the distance of the vector minus the sphere's radius squared
	float fc = dot_product (vdirtosphere, vdirtosphere) - spa.GetRadius() * spa.GetRadius();
		
	// if either of these is greater then 0 return false
	if (fc > 0.0f && fb > 0.0f)
		return false;

	// calculate the discriminant
	float fdiscr = fb*fb - fc;

	// if the discriminant
	if (fdiscr < 0.0f)
		return false;

	// subtract the square root of the discriminant and negative fb
	float ft = -fb - sqrt (fdiscr);

	// if ft is less then 0 then clamp ft to 0
	if (ft < 0.0f) 
	{
		ft = 0.0f;
	}

	// claculate the point on the ray by adding the ray's start to the ray's normal times ft
	pta = m_cptStart.Get3DCentorid() + m_cptDirection.Get3DCentorid() * ft;

	return true;
}