//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionSphere.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds on to the information for a Sphere
//////////////////////////////////////////////////////////////////////////////////////
#include "CollisionSphere.h"
#include "CollisionShapes.h"

#include "../../RenderEngine/DebugShapes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCollisionSphere():	Default Constructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionSphere::CCollisionSphere ()
{
	m_btType = BT_NULL;
	m_vtType = VMT_SPHERE;
	m_fRadius = 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCollisionSphere():	Copy Constructor. Thakes in a const reference to a Sphere and copies all the data over.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionSphere::CCollisionSphere ( const CCollisionSphere& _csSphere)
{
	m_fRadius = _csSphere.m_fRadius;
	m_cptCenter = _csSphere.m_cptCenter;
	m_btType = _csSphere.m_btType;
	m_vtType = _csSphere.m_vtType;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CCollisionSphere():	Default Destructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionSphere::~CCollisionSphere ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SphereToSphere():	Takes in a Sphere (spB). Checks collision between the passed in sphere and the calling sphere.
//						Returns true if both spheres are over lapping.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionSphere::SphereToSphere ( CCollisionSphere spb)
{
	// calculate a vector between both spheres
	vec3f vdistance = m_cptCenter.Get3DCentorid() - spb.GetCenter3D();

	// save how long the vector is
	float ftempmove = vdistance.magnitude ();

	// if the length of the vector between both spheres is less then or equal to both sphere's radius added together
	// then there is a collision
	if (ftempmove <= (m_fRadius + spb.GetRadius()))
	{
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SphereToSphere():	Takes in a Sphere (spB) and an output vec3f (v3NewPosB). Checks collision between the passed in 
//						sphere and the calling sphere. Returns true if both spheres are over lapping and puts the new 
//						position for spB in the output vec3f.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionSphere::SphereToSphere (CCollisionSphere spb, vec3f& spamoveoutdis)
{
	// calculate a vector between both spheres
	vec3f vdistance = m_cptCenter.Get3DCentorid() - spb.GetCenter3D();

	// save how long the vector is
	float ftempmove = vdistance.magnitude ();

	// normalize the vector between both spheres
	vdistance.normalize ();

	// if the length of the vector between both spheres is less then or equal to both sphere's radius added together
	// then there is a collision
	if (ftempmove <= (m_fRadius + spb.GetRadius()))
	{
		// calculate the new position for spherea so there is no longer a collision
		spamoveoutdis = vdistance * ((m_fRadius + spb.GetRadius()) - ftempmove);
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SphereToCapsule():	Takes in a Capsule (cpA) and an output vec3f (v3NewPos). Checks collision between the passed in 
//						capsule and the calling sphere. Returns true if they are over lapping and puts the new 
//						position for cpA in the output vec3f.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionSphere::SphereToCapsule ( CCollisionCapsule cpa, vec3f& vmovedistance)
{
	// calculate the normal of the capsule
	vec3f vcapnormal = cpa.GetCenterLine().GetStartPoint3D() - cpa.GetCenterLine().GetEndPoint3D();
	float fcaplength = vcapnormal.magnitude();
	vcapnormal /= fcaplength;

	// calculate a vector from the capsule's end point to the sphere's center
	vec3f vsphtocapend = m_cptCenter.Get3DCentorid() - cpa.GetCenterLine().GetEndPoint3D();
		
	// calculate the distance of the above vector in the direction of the capsule's normal
	float fdistance = dot_product (vcapnormal, vsphtocapend);

	// create a temp sphere to use for the capsule
	CCollisionSphere capsphere;
	capsphere.SetRadius (cpa.GetRadius());

	// if the distance of the of the sphere's center and the capsule's end is less then 0 
	// then the temp sphere's center is the capsule's end point
	if ( fdistance < 0)
	{
		capsphere.SetCenter (cpa.GetCenterLine().GetEndPoint3D());
	}
	// if the distance is greater then the capsule's length then the temp sphere's center is the capsule's start point
	else if ( fdistance > fcaplength)
	{
		capsphere.SetCenter (cpa.GetCenterLine().GetStartPoint3D());
	}
	// else calculate the temp sphere's center point
	else
	{
		// scale the capsule's normal by the distance
		vec3f vscalednormal = vcapnormal * fdistance;
		// add the scaled normal to the capsule's end point
		capsphere.SetCenter (cpa.GetCenterLine().GetEndPoint3D() + vscalednormal );
	}

	// check the capsule's temp sphere and the sphere passed in to see if they are colliding
	return SphereToSphere (capsphere);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SphereToAABB():	Takes in a AABB (aabb) and an output vec2f (v3NewPos). Checks collision between the passed in 
//					aabb and the calling sphere. Returns true if they are over lapping and puts the new 
//					position for the aabb in the output vec2f.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionSphere::SphereToAABB (CCollisionAABB aabb, vec2f& pushout)
{
	// find the closest point on the aabb using the spheres center
	vec2f ClosetsPoint;

	if (m_cptCenter.Get2DCentorid().x < aabb.GetMin2DTransformed().x)
	{
		ClosetsPoint.x = aabb.GetMin2DTransformed().x;
	}
	else if (m_cptCenter.Get2DCentorid().x > aabb.GetMax2DTransformed().x)
	{
		ClosetsPoint.x = aabb.GetMax2DTransformed().x;
	}
	else
	{
		ClosetsPoint.x = m_cptCenter.Get2DCentorid().x;
	}


	if (m_cptCenter.Get2DCentorid().y < aabb.GetMin2DTransformed().y)
	{
		ClosetsPoint.y = aabb.GetMin2DTransformed().y;
	}
	else if (m_cptCenter.Get2DCentorid().y > aabb.GetMax2DTransformed().y)
	{
		ClosetsPoint.y = aabb.GetMax2DTransformed().y;
	}
	else
	{
		ClosetsPoint.y = m_cptCenter.Get2DCentorid().y;
	}

	// find the direction from the closest point to the spheres center
	vec2f Direction = m_cptCenter.Get2DCentorid() - ClosetsPoint;
	// get the length of the vector
	float moveDis = Direction.magnitude();

	// if the length is less then the radius then there is a collision
	if (moveDis < GetRadius())
	{
		// get the normal of the direction
		if (moveDis)
			Direction /= moveDis;

		vec2f center = m_cptCenter.Get2DCentorid();

		// calculate the new center point moving out in the direction
		center = center + Direction * (m_fRadius - moveDis);
		// save that direction and the ammount needed to move out of the collision
		pushout = Direction * (m_fRadius - moveDis);
		return true;
	}


	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SphereToAABB():	Takes in a AABB (aabb) and an output vec2f (v3NewPos). Checks collision between the passed in 
//					aabb and the calling sphere. Returns true if they are over lapping and puts the new 
//					position for the aabb in the output vec2f.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionSphere::SphereToAABB (CCollisionAABB aabb, vec2f& pushout, vec2f& CP)
{
	// find the closest point on the aabb using the spheres center
	vec2f ClosetsPoint;

	if (m_cptCenter.Get2DCentorid().x < aabb.GetMin2DTransformed().x)
	{
		ClosetsPoint.x = aabb.GetMin2DTransformed().x;
	}
	else if (m_cptCenter.Get2DCentorid().x > aabb.GetMax2DTransformed().x)
	{
		ClosetsPoint.x = aabb.GetMax2DTransformed().x;
	}
	else
	{
		ClosetsPoint.x = m_cptCenter.Get2DCentorid().x;
	}


	if (m_cptCenter.Get2DCentorid().y < aabb.GetMin2DTransformed().y)
	{
		ClosetsPoint.y = aabb.GetMin2DTransformed().y;
	}
	else if (m_cptCenter.Get2DCentorid().y > aabb.GetMax2DTransformed().y)
	{
		ClosetsPoint.y = aabb.GetMax2DTransformed().y;
	}
	else
	{
		ClosetsPoint.y = m_cptCenter.Get2DCentorid().y;
	}

	// find the direction from the closest point to the spheres center
	vec2f Direction = m_cptCenter.Get2DCentorid() - ClosetsPoint;
	// get the length of the vector
	float moveDis = Direction.magnitude();

	// if the length is less then the radius then there is a collision
	if (moveDis < GetRadius())
	{
		// get the normal of the direction
		if (moveDis)
			Direction /= moveDis;

		vec2f center = m_cptCenter.Get2DCentorid();

		// calculate the new center point moving out in the direction
		center = center + Direction * (m_fRadius - moveDis);
		// save that direction and the ammount needed to move out of the collision
		pushout = Direction * (m_fRadius - moveDis);
		CP = ClosetsPoint;
		return true;
	}


	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SphereToBox():	Takes in a Box (box) and an output vec2f (v3NewPos). Checks collision between the passed in 
//					box and the calling sphere. Returns true if they are over lapping and puts the new 
//					position for the box in the output vec2f.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionSphere::SphereToBox (CCollisionBox box, vec2f& pushout)
{
	// create a line for each edge of the box in a clockwise motion.
	CCollisionLine Lines[4];
	vec2f trasformedVert;
	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[3].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[0].SetStartPoint (trasformedVert);
	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[0].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[0].SetEndPoint (trasformedVert);

	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[0].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[1].SetEndPoint (trasformedVert);
	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[1].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[1].SetStartPoint (trasformedVert);

	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[1].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[2].SetEndPoint (trasformedVert);
	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[2].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[2].SetStartPoint (trasformedVert);

	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[2].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[3].SetEndPoint (trasformedVert);
	trasformedVert =  box.Rotate ((float)sin(box.GetAngle()), (float)cos(box.GetAngle()), box.GetVertices()[3].Get2DCentorid() + box.GetWorld2D(), box.GetWorld2D());
	Lines[3].SetStartPoint (trasformedVert);

	//// loop thorugh all the edges and check collision between them and the sphere.
	//for (int i = 0; i < 4; i++)
	//{
	//	// find the closest point on the line using the spheres center.
	//	vec2f closestPointA;
	//	Lines[i].ClosestPointOnA2DLine (m_cptCenter.Get2DCentorid(), closestPointA);

	//	// find the squared distance between the spheres center and the closest point
	//	float distance = dot_product (m_cptCenter.Get2DCentorid() - closestPointA, m_cptCenter.Get2DCentorid() - closestPointA);

	//	// calculate the direction need to move out of the collision.
	//	vec2f Direction = m_cptCenter.Get2DCentorid() - closestPointA;
	//	distance = Direction.magnitude();

	//	//if the distance is less then the radius of the sphere there is a collision
	//	if (distance <= m_fRadius)
	//	{
	//		// pass back the direction along with how much to push out
	//		Direction.normalize();
	//		pushout =  Direction * (m_fRadius - distance);
	//		return true;
	//	}
	//}

	float fLastDistance = FLT_MAX;
	vec2f fLastPoint;

	D3DXMATRIX mat;
	D3DXMatrixIdentity( &mat );

	for( unsigned int i = 0; i < 4; ++i )
	{
		vec2f closestPointA;
		Lines[i].ClosestPointOnA2DLine (m_cptCenter.Get2DCentorid(), closestPointA);
		float currDist = ( m_cptCenter.Get2DCentorid() - closestPointA ).magnitude();
		if( currDist < fLastDistance )
		{
			fLastPoint = closestPointA;
			fLastDistance = currDist;
		}
	}

	if( fLastDistance <= m_fRadius )
	{
		pushout = m_cptCenter.Get2DCentorid() - fLastPoint; 
		return true;
	}

	return false;
}