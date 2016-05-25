#ifndef CCOMPONENT_PLAYERCOLLISION_H
#define	CCOMPONENT_PLAYERCOLLISION_H

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
#include "../EffectsLib/ParticleManager.h"

class CComponent_Sound;
class CComponent_Animation;
class CInputManager;
class CComponent_Health;
class CComponent_Input;

#define WALLSLIDETIMER 0.2f

__declspec(align(32))
class CComponent_PlayerCollision : public CComponent_Collision
{

private:
	CComponent_Sound* m_pSoundComponent;
	CComponent_Animation* m_pAnimationComponent;
	CComponent_Health* m_pHealthComponent;
	CComponent_Input* m_pInputComp;
	AABB* m_pPlayerCollisionVolume;

	CPM_ParEmitter* m_pWallSlideEffect;

	CInputManager* m_InputMng;
	vec3f m_WallJumpPushOutVector;	// This vector from the wall to the Player which is used to push out.
	float m_fInitiateWallSlideTimer; // This float will be used to determine if we have been running into a wall long enough to wall slide.

	bool _CollidingWallRight;
	bool _CollidingWallLeft;
	bool _isWallSliding;

public:

	CComponent_PlayerCollision();
	~CComponent_PlayerCollision(void);

	bool Initialize();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	vec3f GetWallJumpPushOutVector(){ return m_WallJumpPushOutVector; }
	bool IsCollidingRight() { return _CollidingWallRight; }
	bool IsCollidingLeft() { return _CollidingWallLeft; }
	bool GetIsWallSliding() { return _isWallSliding; }
	float GetWallTimer() { return m_fInitiateWallSlideTimer; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void ResetWallJumpTimer() { m_fInitiateWallSlideTimer = 0.0f;}
	//void SetSplashed ( bool _bSplashed ) { m_bSplashed = _bSplashed; }

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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckDynamicCollisions()
	// Purpose: Check collisions with all dynamic objects
	// Original Author: Rueben Massey
	// Creation Date: 6/11/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckStaticCollisions()
	// Purpose: Check collisions with all static objects
	// Original Author: Rueben Massey
	// Creation Date: 6/11/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckStaticCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );
};

#endif