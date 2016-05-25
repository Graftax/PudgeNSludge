//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionCircle.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds on to the information for a Circle
//////////////////////////////////////////////////////////////////////////////////////
#include "CollisionCircle.h"
#include "CollisionShapes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCollisionCircle():	Default Constructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionCircle::CCollisionCircle ()
{
	m_btType = BT_NULL;
	m_vtType = VMT_CIRCLE;
	m_fRadius = 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CCollisionCircle():	Default Destructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionCircle::~CCollisionCircle ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CircleToCircle():	Takes in a Circle (cirB) and an Output vec2f
//						(vNewPosB). Check collision between the cirlce passed in and the circle that called the function
//						ingoring the Z axis. Places the new center position for cirlce B in the output vec2f (vNewPosB)
//						and returns true if they are colliding.
//
// Returns:		bool
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionCircle::CircleToCircle( CCollisionCircle cirB, vec2f& vNewPosA)
{
	// calculate the vector between both circles
	vec2f vAtoB = m_cptCenter.Get2DCentorid() - cirB.GetCenter2D();

	// calcualte the distance of the vector between both circles
	float fDistance = vAtoB.magnitude ();
	// calcualte the normal between both cirlcles
	vAtoB.normalize ();

	//if the distance between both circles is less then or egual to both radi combined then there is a collision
	if (fDistance <= (m_fRadius + cirB.GetRadius()))
	{
		// calculate the new position for circleA so there is no collision
		vNewPosA = m_cptCenter.Get2DCentorid() + vAtoB * ((m_fRadius + cirB.GetRadius()) - fDistance);
		return true;
	}
	return false;
}