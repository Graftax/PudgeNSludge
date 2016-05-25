//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CollisionVolume.h
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This is the base class for all collision Volumes
//////////////////////////////////////////////////////////////////////////////////////

#ifndef CollisionVolume_H
#define CollisionVolume_H

// BodyTypes:
// BT_NULL = A collision shape that has no body 
// BT_STATIC = A collision shape that is static and will not move
// BT_DYNAMIC = A collision shape that is able to move through the world
// BT_MAX = The max number of body types, no BodyType should be equal to or greater then this type
enum BodyType { BT_NULL, BT_STATIC, BT_DYNAMIC, BT_MAX };

// VolumeTypes:
/// VT_NULL = A collision shape that has no volume
// VT_POINT = A collision shape that has a point volume
// VT_PLANE = A collision shape that has an infinte plane
// VT_LINE = A collision shape that has a line volume
// VT_CIRCLE = A collision shape that has a circle volume
// VT_SPHERE = A collision shape that has a sphere volume
// VT_CAPSULE = A collision shape that has a capsule volume
// VT_BOX = A collision shape that has a 2D box volume
// VT_CUBE = A collision shape that has a cube volume
// VT_AABB = A collision shape that has an AABB volume
// VT_POLYGONE = A collision shape that is a polygone and can hold a max number of verts
// VT_MAX = The max number of volume types, no VolumeType should be equal to or greater then this type
enum VolumeType { VMT_NULL, VMT_POINT, VMT_PLANE, VMT_LINE, VMT_CIRCLE, VMT_SPHERE, 
				  VMT_CAPSULE, VMT_BOX, VMT_CUBE, VMT_AABB, VMT_POLYGONE, VMT_MAX};

class CCollisionVolume
{
protected:
	BodyType m_btType;
	VolumeType m_vtType;

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CCollisionVolume():	Default Constructor
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CCollisionVolume ()
	{
		m_btType = BT_NULL;
		m_vtType = VMT_NULL;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CCollisionVolume():	Default Constructor which takes in the body and volume types
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CCollisionVolume ( BodyType _btType, VolumeType _vtType) : m_btType (_btType), m_vtType (_vtType) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ~CCollisionVolume():	virtual Default Destructor
	//
	// Returns:		Void
	//
	// Mod. Name:
	// Mod. Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~CCollisionVolume () {};

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	BodyType GetBodyType () { return m_btType; }
	VolumeType GetVolumeType () { return m_vtType; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetBodyType ( BodyType _btType ) { m_btType = _btType; }
	void SetVolumeType ( VolumeType _vtType ) { m_vtType = _vtType; }
};
#endif