//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionLine.h
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds on to the information for a Line
//////////////////////////////////////////////////////////////////////////////////////
#ifndef CollisionLine_H
#define CollisionLine_H

#include "CollisionVolume.h"
#include "CollisionPoint.h"

class CCollisionCircle;
class CCollisionSphere;
class CCollisionPlane;
class CCollisionCapsule;
class CCollisionBox;
class CCollisionAABB;

class CCollisionLine : public CCollisionVolume
{
protected:
	CCollisionPoint m_cptStart;
	CCollisionPoint m_cptEnd;

private:
	bool LineToAABB1D (float Point, float Dir, float Min, float Max, float& EnScale);

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CCollisionLine():	Default Constructor
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CCollisionLine ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ~CCollisionLine():	Default Destructor
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~CCollisionLine ();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	vec2f GetStartPoint2D () { return m_cptStart.Get2DCentorid(); }
	vec3f GetStartPoint3D () { return m_cptStart.Get3DCentorid(); }

	vec2f GetEndPoint2D () { return m_cptEnd.Get2DCentorid(); }
	vec3f GetEndPoint3D () { return m_cptEnd.Get3DCentorid(); }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetStartPoint ( vec2f _v2Start ) { m_cptStart.Set2DCentorid (_v2Start); }
	void SetStartPoint ( vec3f _v3Start ) { m_cptStart.Set3DCentorid (_v3Start); }

	void SetEndPoint ( vec2f _v2End ) { m_cptEnd.Set2DCentorid (_v2End); }
	void SetEndPoint ( vec3f _v3End ) { m_cptEnd.Set3DCentorid (_v3End); }


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ClosestPointOnA2DLine():	Takes in a vec2f Point (vPointA) and an output vec2f (ptA).
	//							Finds the closets 2D point on the line then returns that point in the output pramater.
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ClosestPointOnA2DLine ( vec2f vPointA, vec2f& CPA );
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LineToCircle():	Takes in a Circle (cirA) and an output vec2f (ptA).
	//					Checks collision between the Line and Circle and returns true
	//					if they are colliding and puts the point in the output vec2f.
	//
	// Returns:		bool
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LineToCircle(CCollisionCircle cirA, vec2f& vlnCP );
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ClosestPointOnALine():	Takes in a vec3f Point (vPointA) and an output vec3f (ClosestPoint).
	//							Finds the closets point on the line then returns that point in the output
	//							pramater.
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ClosestPointOnALine( vec3f vPointA, vec3f& ClosestPoint );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LineToSphere():	Takes in a Sphere (spA) and an output vec3f (ptA).
	//					Finds the cloests point on the line then returns that point in the output pramater.
	//					And returns true if that point is inside the sphere.
	//
	// Returns:		bool
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LineToSphere( CCollisionSphere spA, vec3f& pta );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LineToCapsule():	Takes in a Capsule (cpA) and two output vec3f (CPA, CPB).
	//					Checks collision between the line and the capsule. Returns true if they are
	//					colliding and fills out the output vec3f with correct collision points. CPA is the closest point
	//					on the Line and CPB is the closest point on the capsule.
	//
	// Returns:		bool
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LineToCapsule ( CCollisionCapsule cpa, vec3f& CPA, vec3f& cpb );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LineToBox():	Takes in a Box (box) and an output vec3f (CPA).
	//				Checks collision between the line and  the box. Returns true if the line is colliding with the box and
	//				fills out the output vec3f correct collision points. CPA is the closest point on the line.
	//
	// Returns:		bool
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LineToBox (CCollisionBox& box, vec3f& CPA);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LineToAABB():Takes in an AABB and an output vec3f (CPA).
	//				Checks collision between the line and  the AABB. Returns true if the line is colliding with the AABB and
	//				fills out the output vec3f correct collision points. CPA is the closest point on the line.
	//
	// Returns:		bool
	//
	// Mod. Name:	JM
	// Mod. Date:	6/7/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LineToAABB(CCollisionAABB& aabb, vec3f& CPA);

	bool LineToAABB(CCollisionAABB& aabb, vec3f& CPA, vec3f& CPB);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LineToAABB():Takes in an line and an output vec3f (CPA).
	//				Checks collision between the line and the line. Returns true if the line is colliding with the line and
	//				fills out the output vec3f correct collision points. CPA is the closest point on the line.
	//
	// Returns:		float - the distance between the closest points
	//
	// Mod. Name:	JM
	// Mod. Date:	6/7/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float LineToLine(CCollisionLine& lineIn, vec2f& c1, vec2f& c2);
};
#endif