#ifndef CCOMPONENT_CRATECOLLISION_H
#define	CCOMPONENT_CRATECOLLISION_H

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
class CComponent_CrateCollision : public CComponent_Collision
{

private:
	IBaseObject* m_pSpawnObject;

	CComponent_Sound* m_pSoundComponent;

	bool m_bPlayed;
	float m_fPlayTimer;
	vec3f m_tPrevPos;
	unsigned int m_nCollisionCount;
	bool m_bCollideSoundCap;

	bool m_bCol;
	bool m_bPrevCol;
	IBaseObject* m_pCurCollisionObj;
	IBaseObject* m_pPrevCollisionObj;

public:

	CComponent_CrateCollision();
	~CComponent_CrateCollision(void);

	bool Initialize();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	
	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetSpawnObject ( IBaseObject* _pSpawnObject ) { m_pSpawnObject = _pSpawnObject; }


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
	// Original Author: Ethan Pendergraft
	// Creation Date: 6/13/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckStaticCollisions()
	// Purpose: Check collisions with all static objects
	// Original Author: Ethan Pendergraft
	// Creation Date: 6/13/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckStaticCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );

};

#endif