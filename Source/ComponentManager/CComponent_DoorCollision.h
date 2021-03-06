/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CComponent_DoorCollision
//
// Purpose: Handle all collisions for the bridge object
//
// Original Author: Rueben Massey
//
// Creation Date: 6/15/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CCOMPONENT_DOORCOLLISION_H_
#define CCOMPONENT_DOORCOLLISION_H_

#include "CComponent_Collision.h"

// FORWARD DECLARATIONS
class IBaseObject;
class CComponent_Sound;
class CComponent_Emitter;
class CEffects;
class CComponent_Animation;

__declspec(align(32))
class CComponent_DoorCollision : public CComponent_Collision
{

private:

	// Pointer to the collision volume
	AABB* m_pCollisionObject;

	// collision volume for opening the door
	AABB m_DoorOpenAABB;
	// flag for collision checks
	bool m_bIsOpen;
	bool m_bButtonPress;		//this bool is set by the button to tell me I should open.
	bool m_bPrevPress;		//buffer for the button press. was I down last frame?
	bool m_bOpening;

	bool m_bHasAssociatedButton;	// Bool to determine whether the Door has an Associated Button.

	// Sound
	CComponent_Sound* m_pSoundComponent;
	// Particles and effects
	CComponent_Emitter* m_pEmitterComponent;
	// Animation
	CComponent_Animation* m_pAnimationComponent;

public:

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	bool IsDoorOpen() { return m_bIsOpen; }
	bool GetPressed(void) { return m_bButtonPress; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetPressed(bool _bPressed) { m_bButtonPress = _bPressed; }
	void SetHasAssociatedButton( bool _val ) { m_bHasAssociatedButton = _val; }

	CComponent_DoorCollision();
	~CComponent_DoorCollision();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Initialize()
	// Purpose: Initialize all data members
	// Original Author: Rueben Massey
	// Creation Date: 6/15/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Initialize( );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Update()
	// Purpose: Update all data members
	// Original Author: Rueben Massey
	// Creation Date: 6/15/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Shutdown()
	// Purpose: clean up memory
	// Original Author: Rueben Massey
	// Creation Date: 6/15/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckCollisions()
	// Purpose: Check for collision	and react appropriately
	// Original Author: Rueben Massey
	// Creation Date: 6/15/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckCollisions( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckOpenCollision()
	// Purpose: Check for collision	with the player and open the door
	// Original Author: Rueben Massey
	// Creation Date: 6/15/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckOpenCollision( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckDynamicCollisions()
	// Purpose: Check collisions with all dynamic objects
	// Original Author: Rueben Massey
	// Creation Date: 6/15/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );
};
#endif //CCOMPONENT_DOORCOLLISION_H_