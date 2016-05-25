//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionCapsule.h
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds on to the information for a Capsule
//////////////////////////////////////////////////////////////////////////////////////
#ifndef CollisionCapsule_H
#define CollisionCapsule_H

#include "CollisionVolume.h"
#include "CollisionLine.h"

class CCollisionCapsule : public CCollisionVolume
{
protected:
	CCollisionLine m_clnCenterLine;
	float m_fRadius;

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CCollisionCapsule():	Default Constructor
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CCollisionCapsule ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ~CCollisionCapsule():	Default Destructor
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~CCollisionCapsule ();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	CCollisionLine& GetCenterLine () { return m_clnCenterLine; }
	vec2f GetCenterLineStart2D () { return m_clnCenterLine.GetStartPoint2D(); }
	vec3f GetCenterLineStart3D () { return m_clnCenterLine.GetStartPoint3D(); }

	vec2f GetCenterLineEnd2D () { return m_clnCenterLine.GetEndPoint2D(); }
	vec3f GetCenterLineEnd3D () { return m_clnCenterLine.GetEndPoint3D(); }

	float GetRadius () { return m_fRadius; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetCenterLine ( CCollisionLine _clnCenterLine ) { m_clnCenterLine = _clnCenterLine; }
	void SetCenterLineStart ( vec2f _v2Start ) { m_clnCenterLine.SetStartPoint (_v2Start); }
	void SetCenterLineStart ( vec3f _v3Start ) { m_clnCenterLine.SetStartPoint (_v3Start); }

	void SetCenterLineEnd ( vec2f _v2End ) { m_clnCenterLine.SetEndPoint (_v2End); }
	void SetCenterLineEnd ( vec3f _v3End ) { m_clnCenterLine.SetEndPoint (_v3End); }

	void SetRadius ( float _fRadius ) { m_fRadius = _fRadius; }
};
#endif