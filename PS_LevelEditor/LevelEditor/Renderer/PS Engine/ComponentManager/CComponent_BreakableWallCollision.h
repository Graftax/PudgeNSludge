#ifndef CCOMPONENT_BREAKABLEWALLCOLLISION_H
#define	CCOMPONENT_BREAKABLEWALLCOLLISION_H

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Kevin Clouden
// Date:                5/16/12
// Purpose:             Defines the collisions and physics for the breakable wall.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_Collision.h"
//#include "../PhysicsEnvironment/PhysicsEnvironment.h"
//#include "../Utilities/CollisionLib/CollisionShapes.h"
//#include "../RenderEngine/DebugShapes.h"


class CComponent_Sound;
__declspec(align(32))
class CComponent_BreakableWallCollision : public CComponent_Collision
{

private:
	bool IsBroken;
	CComponent_Sound* m_pSoundComponent;

	// Pionter to the collision volume
	AABB* m_pCollisionObject;

public:

	CComponent_BreakableWallCollision();
	~CComponent_BreakableWallCollision(void);

	bool Initialize();

	/////////////////////////////
	//  Accessors
	/////////////////////////////

	/////////////////////////////
	//  Mutators
	/////////////////////////////


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
	// Function Name: CheckCollisions()
	// Purpose: Check for collision	and react appropriately
	// Original Author: Rueben Massey
	// Creation Date: 6/13/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckCollisions( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckDynamicCollisions()
	// Purpose: Check collisions with all dynamic objects
	// Original Author: Rueben Massey
	// Creation Date: 6/13/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );
};

#endif