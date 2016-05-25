#ifndef CCOMPONENT_GOOPITCOLLISION_H
#define	CCOMPONENT_GOOPITCOLLISION_H

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Josh Fields
// Date:                5/16/12
// Purpose:             Defines the collisions and physics for the player.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_Collision.h"
//#include "../PhysicsEnvironment/PhysicsEnvironment.h"
//#include "../Utilities/CollisionLib/CollisionShapes.h"
//#include "../RenderEngine/DebugShapes.h"

class CComponent_Sound;

__declspec(align(32))
class CComponent_GooPitCollision : public CComponent_Collision
{

private:
	//float m_fDamageTimer;
	CComponent_Sound* m_pSoundComponent;
	float m_fElapsedTime;
	bool m_bSoundLoop;
	bool m_bSoundSplash;
	bool m_bInRange;
	float m_fSoundTimer;

private:
	void StaticCollision ( IBaseObject* Collider, IBaseObject* Collide );
	void DynamicCollision ( IBaseObject* Collider, IBaseObject* Collide );
	void PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction );

public:

	CComponent_GooPitCollision();
	~CComponent_GooPitCollision(void);

	bool Initialize();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	bool GetInRange(void) { return m_bInRange; }
	
	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetInRange(bool _inRange) { m_bInRange = _inRange; }


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():	Updates the collison component, incrementing its physics and detecting collision with other objects.
	//
	// Returns:		Void
	//
	// Mod. Name:   Josh Fields
	// Mod. Date:   5/16/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );

	void Shutdown(void);

};

#endif