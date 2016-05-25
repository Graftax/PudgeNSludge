#ifndef CCOMPONENT_COLLISION_H
#define	CCOMPONENT_COLLISION_H

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#include "IComponent.h"
#include "../PhysicsEnvironment/PhysicsEnvironment.h"
#include "../Utilities/CollisionLib/CollisionShapes.h"
#include "../RenderEngine/DebugShapes.h"

enum eCollisionCMPType {	CCMP_NULL, 
							CCMP_PLAYER,
							CCMP_BOSS,
							CCMP_PLAYERARM, 
							CCMP_GOO, 
							CCMP_SLIME, 
							CCMP_HULK, 
							CCMP_CRATE, 
							CCMP_CORK, 
							CCMP_GOOPIT, 
							CCMP_BREAKABLE, 
							CCMP_STEAMPIPE, 
							CCMP_BOXPIPE,
							CCMP_BRIDGE,
							CCMP_OUTLET,
							CCMP_PLUG,
							CCMP_POOL,
							CCMP_CRUSHER,
							CCMP_DOOR,
							CCMP_BUTTON,
							WALLCCMP_MAX,
						};

__declspec(align(32))
class CComponent_Collision : public IComponent
{
protected:

	eCollisionCMPType	m_nCollType;

	bool			m_bCollidingWall;
	bool			m_bCollidingGround;
	bool			m_bCollidingGroundLast;
	tPhyObject*		m_pPhysicsBox;	
	vec3f			m_fPos;
	vec3f			m_fVel;
	vec3f			m_fLastOffset;

	float			m_fTimeLeftToIgnorePlayer;
	bool			m_bPrevInGoo;
	bool			m_bInGoo;

	//AABB m_CheckPoint;

public:


	CComponent_Collision ();
	~CComponent_Collision (void);

	bool Initialize( /*CCollisionVolume* _cvVolume*/ );

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	unsigned int GetCollType() { return m_nCollType; }
	tPhyObject* GetPhysicsBox() { return m_pPhysicsBox; }

	// These wont do anything if it has a physics component.
	vec3f& GetPos() { return m_fPos; }
	vec2f GetPos2DClean();
	vec3f& GetVel();
	bool CollidingWall() { return m_bCollidingWall; }
	bool CollidingGround() { return m_bCollidingGround || m_bCollidingGroundLast; }
	vec3f GetLastOffset() { return m_fLastOffset; }
	bool GetCollidingGroundLast() const { return m_bCollidingGroundLast; }
	bool GetCollidingGroundAlone() const { return m_bCollidingGround; }
	float GetTimeLeft() { return m_fTimeLeftToIgnorePlayer; }
	bool GetInGoo() { return m_bInGoo; }
	bool GetPrevInGoo() { return m_bPrevInGoo; }
	
	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetCollType( eCollisionCMPType _type ) { m_nCollType = _type; }
	void SetPhysicsBox( tPhyObject* _pNewBox ) { m_pPhysicsBox = _pNewBox; }
	void SetTimeLeft( float _time ) { m_fTimeLeftToIgnorePlayer = _time; }
	void SetCollidingWall ( bool _Colliding ) { m_bCollidingWall = _Colliding; }
	void SetCollidingGroundAlone(bool val) { m_bCollidingGround = val; }
	void SetCollidingGroundLast(bool val) { m_bCollidingGroundLast = val; }
	void SetInGoo(bool _inGoo) { m_bInGoo = _inGoo; }
	void SetPrevInGoo(bool _prevInGoo) { m_bPrevInGoo = _prevInGoo; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():	Updates the collison component, incrementing its physics and detecting collision with other objects.
	//
	// Returns:		Void
	//
	// Mod. Name:   EP
	// Mod. Date:   5/9/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );

	void Shutdown(void);

	float3 prjPoint2Line( float3 pt, float3 p1, float3 p2 );

	void CleanForce( vec2f _fForce );
	void CleanOffset( vec2f _fOffset );
	void CleanRemoveVelocity();
};

#endif // CCOMPONENT_COLLISION_H