#ifndef CCOMPONENT_CORKCOLLISION_H
#define	CCOMPONENT_CORKCOLLISION_H

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
class CComponent_CorkCollision : public CComponent_Collision
{

private:
	bool m_bPulled;
	bool m_bCorked;
	bool m_bPlayed;
	float m_fPlayDelay;
	vec3f m_tPrevPos;
	CComponent_Sound* m_pSoundComponent;

public:
	CComponent_CorkCollision();
	~CComponent_CorkCollision(void);
	/////////////////////////////
	//  Accessors
	/////////////////////////////
	bool GetPulled() const { return m_bPulled; }
	bool GetCorked() const { return m_bCorked; }
	
	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetPulled(bool val) { m_bPulled = val; }
	void SetCorked(bool val) { m_bCorked = val; }

	bool Initialize();

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