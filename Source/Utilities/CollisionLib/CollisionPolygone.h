//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionPolygone.h
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds on to the information for a Polygone
//////////////////////////////////////////////////////////////////////////////////////
#ifndef CollisionPolygone_H
#define CollisionPolygone_H

#include "CollisionVolume.h"
#include "CollisionPoint.h"

class CCollisionPolygone : public CCollisionVolume
{
public:
	//struct Transform
	//{
	//	vec2f m_v2Position;
	//	float m_fCosine;
	//	float m_fSine;
	//	void Set (float angle)
	//	{

	//	}
	//};

	//struct Sweep
	//{
	//	vec2f m_v2CenterMass;
	//	vec2f m_v2Center;		//World center position
	//	float m_fAngle;			//World angle in radians
	//};

protected:
	CCollisionPoint m_cptCentorid;		// center of the polygone in local space
	CCollisionPoint m_cptWorld;			// center of the polygone in world space
	CCollisionPoint m_cptVertices[10];	// the vertices of the polygone
	CCollisionPoint m_cptNormals [10];	// the normals of each face on the polygone
	unsigned int m_unVertCount;			// 3 = tri, 4 = box, 8 = cube, anything else is just a poly
	float  m_fAngle;					// the rotation of the polygone in radians
	//float m_fRadius;

	//Sweep m_Sweep;
	//Transform m_Trans;

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CCollisionSphere():	Default Constructor
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CCollisionPolygone ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CCollisionSphere():	Default Constructor
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~CCollisionPolygone ();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	CCollisionPoint& GetCentorid () { return m_cptCentorid; }
	vec2f GetCentorid2D () { return m_cptCentorid.Get2DCentorid (); }
	vec3f GetCentorid3D () { return m_cptCentorid.Get3DCentorid (); }

	CCollisionPoint& GetWorld () { return m_cptWorld; }
	vec2f GetWorld2D () { return m_cptWorld.Get2DCentorid (); }
	vec3f GetWorld3D () { return m_cptWorld.Get3DCentorid (); }

	CCollisionPoint* GetVertices () { return m_cptVertices; }

	CCollisionPoint* GetNormals () { return m_cptNormals; }

	unsigned int GetVertCount () { return m_unVertCount; }

	float GetAngle () { return m_fAngle; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetCentorid ( CCollisionPoint _cptCentorid ) { m_cptCentorid = _cptCentorid; }
	void SetCentorid ( vec2f _v2Centorid ) { m_cptCentorid.Set2DCentorid (_v2Centorid); }
	void SetCentorid ( vec3f _v3Centorid ) { m_cptCentorid.Set3DCentorid (_v3Centorid); }
	
	void SetWorld ( CCollisionPoint _cptWorld ) { m_cptWorld = _cptWorld; }
	void SetWorld ( vec2f _v2World ) { m_cptWorld.Set2DCentorid (_v2World); }
	void SetWorld ( vec3f _v3World ) { m_cptWorld.Set3DCentorid (_v3World); }

	void SetVertices ( CCollisionPoint* _cptVertices ) { *m_cptVertices = *_cptVertices; }

	void SetNormals ( CCollisionPoint* _cptNormals ) { *m_cptNormals = *_cptNormals; }

	void SetVertCount ( unsigned int _unVertCount ) { m_unVertCount = _unVertCount; }

	void SetAngle ( float _fAngle ) { m_fAngle = _fAngle; }

	/*
	Sweep& GetSweep () { return m_Sweep; }
	void SetSweep ( Sweep _Sweep) { m_Sweep = _Sweep; }

	Transform& GetTransform () { return m_Trans; }
	void SetTransform ( Transform _Trans) { m_Trans = _Trans; }

	float GetRadius () { return m_fRadius; }
	void SetRadius ( float _fRadius ) { m_fRadius = _fRadius; }*/
};
#endif