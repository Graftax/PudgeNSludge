//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionAABB.h
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds on to the information for an AABB
//////////////////////////////////////////////////////////////////////////////////////
#ifndef CollisionAABB_H
#define CollisionAABB_H

#include "CollisionVolume.h"
#include "CollisionPoint.h"

class CCollisionAABB : public CCollisionVolume
{

private:

	CCollisionPoint m_cptMax;
	CCollisionPoint m_cptMin;
	CCollisionPoint m_cptCenter;
	CCollisionPoint m_cptWorldPos;

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CCollisionAABB():	Default Constructor
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CCollisionAABB ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ~CCollisionAABB():	Default Destructor
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~CCollisionAABB ();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	CCollisionPoint GetMaxPoint () { return m_cptMax; }
	vec2f GetMaxPoint2D () { return m_cptMax.Get2DCentorid(); }
	vec3f GetMaxPoint3D () { return m_cptMax.Get3DCentorid(); }
	vec2f GetMax2DTransformed () { return m_cptMax.Get2DCentorid() + m_cptWorldPos.Get2DCentorid(); }
	vec3f GetMax3DTransformed () { return m_cptMax.Get3DCentorid() + m_cptWorldPos.Get3DCentorid(); }

	CCollisionPoint GetMinPoint () { return m_cptMin; }
	vec2f GetMinPoint2D () { return m_cptMin.Get2DCentorid(); }
	vec3f GetMinPoint3D () { return m_cptMin.Get3DCentorid(); }
	vec2f GetMin2DTransformed () { return m_cptMin.Get2DCentorid() + m_cptWorldPos.Get2DCentorid(); }
	vec3f GetMin3DTransformed () { return m_cptMin.Get3DCentorid() + m_cptWorldPos.Get3DCentorid(); }

	CCollisionPoint GetWorldPoint () { return m_cptWorldPos; }
	vec2f GetWorldPoint2D () { return m_cptWorldPos.Get2DCentorid(); }
	vec3f GetWorldPoint3D () { return m_cptWorldPos.Get3DCentorid(); }

	CCollisionPoint GetCenterPoint () { return m_cptCenter; }
	vec2f GetCenterPoint2D () { return m_cptCenter.Get2DCentorid(); }
	vec3f GetCenterPoint3D () { return m_cptCenter.Get3DCentorid(); }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetMaxPoint ( CCollisionPoint _cptMax ) { m_cptMax = _cptMax; }
	void SetMaxPoint ( vec2f _v2Max ) { m_cptMax.Set2DCentorid (_v2Max); }
	void SetMaxPoint ( vec3f _v3Max ) { m_cptMax.Set3DCentorid (_v3Max); }
	
	void SetMinPoint ( CCollisionPoint _cptMin ) { m_cptMin = _cptMin; }
	void SetMinPoint ( vec2f _v2Min ) { m_cptMin.Set2DCentorid (_v2Min); }
	void SetMinPoint ( vec3f _v3Min ) { m_cptMin.Set3DCentorid (_v3Min); }

	void SetWorldPoint ( CCollisionPoint _cptWorld ) { m_cptWorldPos = _cptWorld; }
	void SetWorldPoint ( vec2f _v2World ) { m_cptWorldPos.Set2DCentorid (_v2World); }
	void SetWorldPoint ( vec3f _v3World ) { m_cptWorldPos.Set3DCentorid (_v3World); }

	void SetCenterPoint ( CCollisionPoint _cptCenter ) { m_cptCenter = _cptCenter; }
	void SetCenterPoint ( vec2f _v2Center ) { m_cptCenter.Set2DCentorid (_v2Center); }
	void SetCenterPoint ( vec3f _v3Center ) { m_cptCenter.Set3DCentorid (_v3Center); }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Build():	Takes in an array of Points and an unsigned int (vPoints, unCount). This function will build and aabb around
	//			the points given.
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Build ( CCollisionPoint* vPoints, size_t uncount);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Build():	Takes in an array of vec3f and an unsigned int (vPoints, unCount). This function will build and aabb around
	//			the points given.
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Build ( vec3f* vPoints, size_t uncount);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Build():	Takes in an array of vec2f and an unsigned int (vPoints, unCount). This function will build and aabb around
	//			the points given.
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Build ( vec2f* vPoints, size_t uncount);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AABBToAABBWorld():	Takes in an AABB (aabb) and an output pramater vec2f (CPA). This will check if both aabbs
	//						against eachother and does not tranform the passed in aabb but transforms the calling aabb.
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool AABBToAABBWorld (CCollisionAABB aabb, vec2f& CPA);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AABBToAABB():	Takes in an AABB (aabb) and an output pramater vec2f (CPA). This will check if both aabbs
	//						against eachother and does tranform both aabbs.
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool AABBToAABB (CCollisionAABB aabb, vec2f& CPA);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AABBToAABB():	Takes in an AABB (aabb) and an output pramater vec2f (CPA). This will check if both aabbs
	//						against eachother and does tranform both aabbs.
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool AABBToAABB (CCollisionAABB aabb, vec3f& ClosestPoint ,vec2f& CPA);

};
#endif