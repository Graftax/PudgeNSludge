#ifndef CCOMPONENT_BREAKABLEWALLCOLLISION_H
#define	CCOMPONENT_BREAKABLEWALLCOLLISION_H

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Kevin Clouden
// Date:                5/16/12
// Purpose:             Defines the collisions and physics for the breakable wall.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_Collision.h"
#include "../HUD/CIcon.h"
//#include "../IBaseObject"

//#include "../PhysicsEnvironment/PhysicsEnvironment.h"
//#include "../Utilities/CollisionLib/CollisionShapes.h"
//#include "../RenderEngine/DebugShapes.h"


class CComponent_Sound;
__declspec(align(32))
class CComponent_BreakableWallCollision : public CComponent_Collision
{

private:
	std::vector<IBaseObject*> m_vAssociatedObjs;
	bool IsBroken;
	CComponent_Sound* m_pSoundComponent;

	// Pionter to the collision volume
	AABB* m_pCollisionObject;

	// Icon
	CIcon* m_pIcon;
	int m_nIconTexture;
	int m_nIconTexture2;
	short m_sCount;


public:

	CComponent_BreakableWallCollision();
	~CComponent_BreakableWallCollision(void);

	bool Initialize();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	std::vector<IBaseObject*>& GetAssociatedObjects () { return m_vAssociatedObjs; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void AddAssociatedObj ( IBaseObject* _pObject ) { m_vAssociatedObjs.push_back (_pObject); }


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():	Updates the collison component, incrementing its physics and detecting collision with other objects.
	//
	// Returns:		Void
	//
	// Mod. Name:   Josh Fields
	// Mod. Date:   5/16/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Shutdown():	destories all class objects
	//
	// Returns:		Void
	//
	// Mod. Name:   Josh Fields
	// Mod. Date:   5/16/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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