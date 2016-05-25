//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionPoint.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds on to the information for a plane
//////////////////////////////////////////////////////////////////////////////////////
#include "CollisionPlane.h"
#include "CollisionShapes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCollisionPlane():	Default Constructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionPlane::CCollisionPlane ()
{
	m_btType = BT_NULL;
	m_vtType = VMT_PLANE;
	m_fOffset = 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CCollisionPlane():	Default Destructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionPlane::~CCollisionPlane ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ComputePlane():	Takes in 3 Points (pointA, pointB, pointC). This computes a plane based on the 3 passed in point and
//					places the new plane in the plane that called this function.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCollisionPlane::ComputePlane( CCollisionPoint pointA, CCollisionPoint pointB, CCollisionPoint pointC )
{
	// calculate the normal of the plane
	vec3f Normal;
	cross_product (Normal, ( pointA.Get3DCentorid() - pointB.Get3DCentorid()), 
												(pointC.Get3DCentorid() - pointB.Get3DCentorid()));
	// normalize the normal to be unit length
	Normal.normalize();
	m_cptNormal.Set3DCentorid ( Normal );
	
	// calculate the offset from the origin
	m_fOffset = dot_product (m_cptNormal.Get3DCentorid(), pointA.Get3DCentorid());
	// a point that is on the plane
	m_cptPoint = pointA;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PositiveHalfSpaceTest():	This function returns true if the vec3f point is on the positive side or on the plane.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionPlane::PositiveHalfSpaceTest ( vec3f vPoint )
{
	// calculate the distance the point is from the plane
	float fDistance = dot_product (m_cptNormal.Get3DCentorid(), vPoint) - m_fOffset;
	// if the distance is less then 0 the point is on the negative side of the plane
	if (fDistance < 0.0f)
	{
		return false;
	}
	return true;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NegativeHalfSpaceTest():	Takes in a vec3f (vPoint). This function returns true if the vec3f point is on the negative
//							side of the plane
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionPlane::NegativeHalfSpaceTest ( vec3f vPointA )
{
	// calculate the distance the point is from the plane
	float fDistance = dot_product (m_cptNormal.Get3DCentorid(), vPointA) - m_fOffset;
	// if the distance is less then 0 the point is on the negative side of the plane
	if (fDistance < 0.0f)
	{
		return true;
	}
	return false;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SphereToPlaneHalfSpaceTest():	Takes in a Sphere (spA).  Returns true if the sphere is in front of or on the plane.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionPlane::SphereToPlaneHalfSpaceTest (CCollisionSphere spA)
{
	// if the dot product of the plane normal and the spheres center minus the planes offset
	// is less then the negative spheres radius then the sphere is on the negative side of the plane
	if(dot_product(m_cptNormal.Get3DCentorid(), spA.GetCenter3D()) - m_fOffset < - spA.GetRadius())
	{
		return false;
	}
	return true;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CapsuleToPlaneHalfSpaceTest():	Takes in a Capsule(cpA).  Returns true if the capsule is in front of or on the plane.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionPlane::CapsuleToPlaneHalfSpaceTest (CCollisionCapsule cpA)
{
	// if the dot product of the plane normal and the capsules start minus the planes offset
	// is less then the negative capsules radius && the dot product of the plane normal and the 
	// capsules end minus the planes offset is less then the negative capsules radius
	// then the capsule is on the negative side of the plane
	if (( dot_product ( m_cptNormal.Get3DCentorid(), cpA.GetCenterLine().GetStartPoint3D() ) - m_fOffset) < -cpA.GetRadius() &&
		( dot_product ( m_cptNormal.Get3DCentorid(), cpA.GetCenterLine().GetEndPoint3D() ) - m_fOffset) < -cpA.GetRadius())
	{
		return false;
	}
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LineToPlane():	Takes in a Line(lnA) and an output vec3f (ptA).
//			Returns true if the line is colliding with the plane and fills the 
//					output vec3f with the collision point.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionPlane::LineToPlane ( CCollisionLine lnA, vec3f& ptA )
{
	// early out test to see if the line is on the negative side of the plane
	if (dot_product (lnA.GetStartPoint3D(), m_cptNormal.Get3DCentorid()) - dot_product (m_cptPoint.Get3DCentorid(), m_cptNormal.Get3DCentorid()) < 0
		&& dot_product (lnA.GetEndPoint3D(), m_cptNormal.Get3DCentorid()) - dot_product (m_cptPoint.Get3DCentorid(), m_cptNormal.Get3DCentorid()) < 0)
	{
		return false;
	}
	
	// early out test to see if the line is on the positive side of the plane
	if (dot_product (lnA.GetStartPoint3D(), m_cptNormal.Get3DCentorid()) - dot_product (m_cptPoint.Get3DCentorid(), m_cptNormal.Get3DCentorid()) > 0
		&& dot_product (lnA.GetEndPoint3D(), m_cptNormal.Get3DCentorid()) - dot_product (m_cptPoint.Get3DCentorid(), m_cptNormal.Get3DCentorid()) > 0)
	{
		return false;
	}
	
	// calculate the distance the line start is from the origin in the direction of the plane normal
	float fDistance0 = dot_product (m_cptNormal.Get3DCentorid(), lnA.GetStartPoint3D());
	// calculate the distance the plane point is from the origin in the direction of the plane normal
	float fDistance1 = dot_product (m_cptNormal.Get3DCentorid(), m_cptPoint.Get3DCentorid());
	// find the diference of the two distances
	float fDistance2 = fDistance0 - fDistance1;
	
	// calculate the line vector
	vec3f vLineNormal = lnA.GetEndPoint3D() - lnA.GetStartPoint3D();
	
	// calculate the length the line vector in the direction of the normal
	float fDistance3 = dot_product (m_cptNormal.Get3DCentorid(), vLineNormal);
	
	// calculate overall distance by dividing the 3rd distance from the 4th distance and then negate the value
	float fOverallDistance = -(fDistance2 / fDistance3);
	
	// add the line start point to the line vector times the overall distance
	ptA = lnA.GetStartPoint3D() + vLineNormal * fOverallDistance;
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// Function Name: ClassifyAABBToPlane
// Purpose: Takes in a AABB and tests it against the invoking object which is
//			a plane and determines whether it is infront of the plane, behind
//			or straddeling.
// Original Author: Andy Madruga
// Creation Date: 5/25/2012
// Last Modification By:	
// Last Modification Date:	
///////////////////////////////////////////////////////////////////////////////////
int CCollisionPlane::ClassifyAABBToPlane( CCollisionAABB& aabb )
{
	// Add the Min and the Max and then Multiply the result by half to get
	// the center point.
	vec3f CenterPoint = (aabb.GetMin3DTransformed() + aabb.GetMax3DTransformed()) * 0.5f;

	// Calculate Extents
	vec3f Extent = aabb.GetMax3DTransformed() - CenterPoint;

	// Projected Radius = DotProduct( Extent , abs(Normal) );
	// Calculate the radius in the direction of the absolute normal to avoid
	// failure when the normal is negative.
	vec3f PlaneNormal = this->GetNormal3D();
	float ProjectedRadius = (Extent.x * abs( PlaneNormal.x)) + (Extent.y * abs(PlaneNormal.y)) + (Extent.z * abs(PlaneNormal.z));

	// Calculate distance of AABB to the Plane.
	float AABB_Distance_To_Plane = dot_product( PlaneNormal, CenterPoint) - this->GetOffset();

	// If AABB distance is greater than the Projected radius, AABB is in front of
	// the plane and has passed the halfspace test.
	if( AABB_Distance_To_Plane > ProjectedRadius )
		return 1;
	// If AABB distance is less than or equal to the Projected Radius, AABB is
	// behind the plane.
	else if( AABB_Distance_To_Plane < (-ProjectedRadius) )
		return 2;
	// Otherwise the AABB is stradeling in the plane.
	else
		return 3;
}