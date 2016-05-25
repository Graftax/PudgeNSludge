//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionAABB.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds on to the information for an AABB
//////////////////////////////////////////////////////////////////////////////////////

#include "CollisionAABB.h"
#include "CollisionShapes.h"
#include "../../RenderEngine/DebugShapes.h"
#include "CollisionLib.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCollisionAABB():	Default Constructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCollisionAABB::CCollisionAABB ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CCollisionAABB():	Default Destructor
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CCollisionAABB::~CCollisionAABB ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AABBToAABBWorld():	Takes in an AABB (aabb) and an output pramater vec2f (CPA). This will check if both aabbs
//						against eachother and does not tranform the passed in aabb but transforms the calling aabb.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionAABB::AABBToAABBWorld (CCollisionAABB aabb, vec2f& CPA)
{

	// calculate this min and max points in world position.
	vec2f ThisMin = m_cptMin.Get2DCentorid() + m_cptWorldPos.Get2DCentorid();
	vec2f ThisMax = m_cptMax.Get2DCentorid() + m_cptWorldPos.Get2DCentorid();
	// hold on to the passed in aabbs min and max and do not put them in world space
	// for they should already be there.
	vec2f OtherMin = aabb.GetMinPoint2D();
	vec2f OtherMax = aabb.GetMaxPoint2D();

	vec2f buffer[4];
	vec3f CubeOne[4];
	vec3f CubeTwo[4];

	buffer[0] = ThisMax;
	buffer[1] = vec2f( ThisMax.x, ThisMin.y );
	buffer[2] = ThisMin;
	buffer[3] = vec2f( ThisMin.x, ThisMax.y );

	CubeTwo[0] = vec3f( buffer[1].x, buffer[1].y, 0.0f );
	CubeTwo[1] = vec3f( buffer[2].x, buffer[2].y, 0.0f );
	CubeTwo[2] = vec3f( buffer[3].x, buffer[3].y, 0.0f );
	CubeTwo[3] = vec3f( buffer[0].x, buffer[0].y, 0.0f );

	buffer[0] = OtherMax;
	buffer[1] = vec2f( OtherMax.x,OtherMin.y );
	buffer[2] = OtherMin;
	buffer[3] = vec2f( OtherMin.x, OtherMax.y );

	CubeOne[0] = vec3f( buffer[0].x, buffer[0].y, 0.0f ); 
	CubeOne[1] = vec3f( buffer[1].x, buffer[1].y, 0.0f ); 
	CubeOne[2] = vec3f( buffer[2].x, buffer[2].y, 0.0f ); 
	CubeOne[3] = vec3f( buffer[3].x, buffer[3].y, 0.0f ); 

	vec2f garbage;
	return CollisionLib::CheckCubeCollision( CubeOne, CubeTwo, garbage, CPA );

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AABBToAABB():	Takes in an AABB (aabb) and an output pramater vec2f (CPA). This will check if both aabbs
//						against eachother and does tranform both aabbs.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionAABB::AABBToAABB (CCollisionAABB aabb, vec2f& CPA)
{
	vec2f ThisMin = m_cptMin.Get2DCentorid() + m_cptWorldPos.Get2DCentorid();
	vec2f ThisMax = m_cptMax.Get2DCentorid() + m_cptWorldPos.Get2DCentorid();

	vec2f buffer[4];
	buffer[0] = ThisMax;
	buffer[1] = vec2f( ThisMax.x, ThisMin.y );
	buffer[2] = ThisMin;
	buffer[3] = vec2f( ThisMin.x, ThisMax.y );

	vec3f CubeOne[4];
	vec3f CubeTwo[4];

	CubeTwo[0] = vec3f( buffer[1].x, buffer[1].y, 0.0f );
	CubeTwo[1] = vec3f( buffer[2].x, buffer[2].y, 0.0f );
	CubeTwo[2] = vec3f( buffer[3].x, buffer[3].y, 0.0f );
	CubeTwo[3] = vec3f( buffer[0].x, buffer[0].y, 0.0f );
				 
	buffer[0] = aabb.GetMax2DTransformed();
	buffer[1] = vec2f( aabb.GetMax2DTransformed().x, aabb.GetMin2DTransformed().y );
	buffer[2] = aabb.GetMin2DTransformed();
	buffer[3] = vec2f( aabb.GetMin2DTransformed().x, aabb.GetMax2DTransformed().y );

	CubeOne[0] = vec3f( buffer[0].x, buffer[0].y, 0.0f ); 
	CubeOne[1] = vec3f( buffer[1].x, buffer[1].y, 0.0f ); 
	CubeOne[2] = vec3f( buffer[2].x, buffer[2].y, 0.0f ); 
	CubeOne[3] = vec3f( buffer[3].x, buffer[3].y, 0.0f ); 

	vec3f garbage;
	vec3f fOut;
	//CollisionLib::CheckCubeCollision( CubeOne, CubeTwo, garbage, CPA );
	bool returnval = CollisionLib::CheckCubeCollision2( CubeOne, 4, CubeTwo, 4, garbage, fOut );
	CPA.x = -fOut.x;
	CPA.y = -fOut.y;
	return returnval;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AABBToAABB():	Takes in an AABB (aabb) and an output pramater vec2f (CPA). This will check if both aabbs
//						against eachother and does tranform both aabbs.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCollisionAABB::AABBToAABB (CCollisionAABB aabb, vec3f& ClosestPoint, vec2f& CPA)
{
	// calculate this min and max points in world position.
	vec2f ThisMin = m_cptMin.Get2DCentorid() + m_cptWorldPos.Get2DCentorid();
	vec2f ThisMax = m_cptMax.Get2DCentorid() + m_cptWorldPos.Get2DCentorid();
	// calculate the passed in aabbs min and max points in world position.
	vec2f OtherMin = aabb.GetMin2DTransformed();
	vec2f OtherMax = aabb.GetMax2DTransformed();

	// calculate the X and Y widths for this aabb.
	float XWidth, YWidth;
	float aabbXWidth, aabbYWidth;
	XWidth = (ThisMax.x - ThisMin.x) * 0.5f;
	YWidth = (ThisMax.y - ThisMin.y) * 0.5f;

	// calculate the X and Y widths for the passed in aabbs.
	aabbXWidth = (OtherMax.x - OtherMin.x) * 0.5f;
	aabbYWidth = (OtherMax.y - OtherMin.y) * 0.5f;

	// eary out test to make sure both aabbs are close enough to be over lapping
	if (ThisMin.x > OtherMax.x ||
		ThisMax.x < OtherMin.x )
	{
		return false;
	}
	
	if (ThisMin.y > OtherMax.y ||
		ThisMax.y < OtherMin.y )
	{
		return false;
	}

	// find the closest point on the passed in aabb using this aabbs center in world space.
	vec2f ClosetsPoint = aabb.GetWorldPoint2D();

	if (m_cptWorldPos.Get2DCentorid().x - XWidth < OtherMin.x)
	{
		ClosetsPoint.x = OtherMin.x;
	}
	else if (m_cptWorldPos.Get2DCentorid().x + XWidth > OtherMax.x)
	{
		ClosetsPoint.x = OtherMax.x;
	}
	else
	{
		ClosetsPoint.x = m_cptWorldPos.Get2DCentorid().x;
	}


	if (m_cptWorldPos.Get2DCentorid().y - YWidth < OtherMin.y)
	{
		ClosetsPoint.y = OtherMin.y;
	}
	else if (m_cptWorldPos.Get2DCentorid().y + YWidth > OtherMax.y)
	{
		ClosetsPoint.y = OtherMax.y;
	}
	else
	{
		ClosetsPoint.y = m_cptWorldPos.Get2DCentorid().y;
	}

	ClosestPoint = vec3f(ClosetsPoint.x, ClosetsPoint.y, m_cptWorldPos.Get3DCentorid().z) ;

	// get the direction we need to move this aabb.
	vec2f Direction = m_cptWorldPos.Get2DCentorid() - ClosetsPoint;
	if( Direction.x != 0 || Direction.y != 0)
	Direction.normalize ();

	// calculate the overlapping lines
	// take the min of both max points
	vec2f LineEnd =  vec2f (min (ThisMax.x, OtherMax.x), min (ThisMax.y, OtherMax.y));
	// take the max of both min points
	vec2f LineStart = vec2f (max (ThisMin.x, OtherMin.x), max (ThisMin.y, OtherMin.y) );

	//get the vector of the over lapping lines
	vec2f Overlap = LineEnd - LineStart;

	// make sure both over lapping lines are the same sign (there may be an better way to do this)
	if (Overlap.x < 0.0f)
	{
		Overlap.x = -Overlap.x;
	}
	if (Overlap.y < 0.0f)
	{
		Overlap.y = -Overlap.y;
	}

	// if the Overlap.x is less then the Overlap.y then we need to push out in the direction of the x
	if (Overlap.x < Overlap.y)
	{
		CPA.y = 0.0f;
		CPA.x = Direction.x * Overlap.x;
		return true;
	}

	// if the Overlap.y is less then the Overlap.x then we need to push out in the direction of the y
	if ( Overlap.y < Overlap.x)
	{
		CPA.x = 0.0f;
		CPA.y = Direction.y * Overlap.y;
		return true;
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Build():	Takes in an array of Points and an unsigned int (vPoints, unCount). This function will build and aabb around
//			the points given.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCollisionAABB::Build (CCollisionPoint* vpoints, size_t uncount)
{
	// set the min and max points to the smallest and greats float values possible.
	vec3f vMax, vMin;
	vMax = vec3f (-FLT_MAX, -FLT_MAX, -FLT_MAX);
	vMin = vec3f (FLT_MAX, FLT_MAX, FLT_MAX);

	// loop through all the points in the passed in array
	for (size_t unindex = 0; unindex < uncount; ++unindex)
	{
		// lopp through the three axis to find the largest one and the smallest one.
		for (size_t unaxis = 0; unaxis < 3; ++unaxis)
		{
			// if the points max axis is greater then the max axis then save that points axis as the max
			if (vpoints[unindex].Get3DCentorid().v[unaxis] > vMax.v[unaxis])
			{
				vMax.v[unaxis] = vpoints[unindex].Get3DCentorid().v[unaxis];
			}

			// if the points min axis is less then the min axis then save that points axis as the min
			if (vpoints[unindex].Get3DCentorid().v[unaxis] < vMin.v[unaxis])
			{
				vMin.v[unaxis] = vpoints[unindex].Get3DCentorid().v[unaxis];
			}
		}
	}

	// save both the min and max in this min and max
	m_cptMax.Set3DCentorid (vMax);
	m_cptMin.Set3DCentorid (vMin);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Build():	Takes in an array of vec3f and an unsigned int (vPoints, unCount). This function will build and aabb around
//			the points given.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCollisionAABB::Build (vec3f* vpoints, size_t uncount)
{
	// set the min and max points to the smallest and greats float values possible.
	vec3f vmax, vmin;
	vmax = vec3f (-FLT_MAX, -FLT_MAX, -FLT_MAX);
	vmin = vec3f (FLT_MAX, FLT_MAX, FLT_MAX);

	// loop through all the points in the passed in array
	for (size_t unindex = 0; unindex < uncount; ++unindex)
	{
		// lopp through the three axis to find the largest one and the smallest one.
		for (size_t unaxis = 0; unaxis < 3; ++unaxis)
		{
			// if the points max axis is greater then the max axis then save that points axis as the max
			if (vpoints[unindex].v[unaxis] > vmax.v[unaxis])
			{
				vmax.v[unaxis] = vpoints[unindex].v[unaxis];
			}

			// if the points min axis is less then the min axis then save that points axis as the min
			if (vpoints[unindex].v[unaxis] < vmin.v[unaxis])
			{
				vmin.v[unaxis] = vpoints[unindex].v[unaxis];
			}
		}
	}

	// save both the min and max in this min and max
	m_cptMax.Set3DCentorid (vmax);
	m_cptMin.Set3DCentorid (vmin);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Build():	Takes in an array of vec2f and an unsigned int (vPoints, unCount). This function will build and aabb around
//			the points given.
//
// Returns:		Void
//
// Mod. Name:
// Mod. Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCollisionAABB::Build (vec2f* vpoints, size_t uncount)
{
	// set the min and max points to the smallest and greats float values possible.
	vec3f vmax, vmin;
	vmax = vec3f (-FLT_MAX, -FLT_MAX, -FLT_MAX);
	vmin = vec3f (FLT_MAX, FLT_MAX, FLT_MAX);

	// loop through all the points in the passed in array
	for (size_t unindex = 0; unindex < uncount; ++unindex)
	{
		// lopp through the three axis to find the largest one and the smallest one.
		for (size_t unaxis = 0; unaxis < 2; ++unaxis)
		{
			// if the points max axis is greater then the max axis then save that points axis as the max
			if (vpoints[unindex].v[unaxis] > vmax.v[unaxis])
			{
				vmax.v[unaxis] = vpoints[unindex].v[unaxis];
			}

			// if the points min axis is less then the min axis then save that points axis as the min
			if (vpoints[unindex].v[unaxis] < vmin.v[unaxis])
			{
				vmin.v[unaxis] = vpoints[unindex].v[unaxis];
			}
		}
	}

	// save both the min and max in this min and max
	m_cptMax.Set3DCentorid (vmax);
	m_cptMin.Set3DCentorid (vmin);
}