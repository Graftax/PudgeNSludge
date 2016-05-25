#ifndef CCOMPONENT_INPUT_H_
#define CCOMPONENT_INPUT_H_

#include "IComponent.h"
#include "../PhysicsEnvironment/PhysicsEnvironment.h"
#include "../PS/CGame.h"
#include "../InputLib/InputManager.h"
#include "../Utilities/CollisionLib/CollisionShapes.h"
#include "../AudioLib/AudioManager.h"

#define PLAYER_MOVE_SPEED 50.0f
#define MAX_PULL_DIST 488.0f
#define MAX_PUSH_DIST 488.0f
#define THROW_SPEED ( 1000.0f * fDT )

class CComponent_Emitter;
class CEffects;
class CComponent_Animation;
class CComponent_Collision;
class CComponent_PlayerCollision;
class CComponent_Sound;
class CObjectManager;

enum eVacState{ VACS_NULL, VACS_PUSH, VACS_PULL, VACS_HASOBJECT, VACS_MAX };

struct PulledObj
{
	bool			m_bIsAnObjectPulled;	// The bool determining whether an object is pulled or not.
	eOBJECT_TYPE	ObjectPulledType;		// The Type of object that was pulled.
	IBaseObject*	m_Object;				// A pointer to the object currently being pulled. Used to prevent pulling in two objects.

	PulledObj()
	{
		m_bIsAnObjectPulled = false;
		ObjectPulledType = OBJ_NULL;
		m_Object = NULL;
	}
};

__declspec(align(32))
class CComponent_Input : public IComponent
{
private:
	CGame* m_pGame;
	CInputManager* m_pInput;
	CCamera* m_pCamera;
	CObjectManager* m_pObjectManager;
	CComponent_Sound * m_pSoundComponent;
	CComponent_Collision* pPlayerCol;
	CComponent_PlayerCollision* pColComponent;
	CComponent_Animation*	pAnimComponent;

	// Variables for movement
	bool m_bJumping;
	bool m_bDoubleJump;
	bool m_bWallJump;
	bool m_bWallJumping;
	bool m_bRunning;
	bool m_bPreviouslyOnGround;
	float m_fPlayerSpeedMod;
	float m_fWalkTimer;

	eVacState m_vsWeaponState;

	float m_fDropTimer;
	int m_nGlowHandle;

	// Line for the laser sight
	Line m_LineToCursor;

	bool RotatedLeft;
	bool RotatedRight;

	CComponent_Emitter* m_pEmitterComponent;
	CComponent_Emitter* m_pPullEmitterComponent;
	CComponent_Emitter* m_pPushEmitterComponent;
	CEffects* m_pEffects;

	PulledObj m_tPulledObj;
	IBaseObject* m_pCurrentTarget;

	bool m_bCreatePullOnce;

public:

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	float GetSpeedMod() { return m_fPlayerSpeedMod; }
	bool GetRotatedRight () { return RotatedRight; }
	bool _IsObjectPulled() { return m_tPulledObj.m_bIsAnObjectPulled; }
	eVacState GetWeaponState() const { return m_vsWeaponState; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetSpeedMod( float _fSpeedMod ) { m_fPlayerSpeedMod = _fSpeedMod; }
	void SetPulledObjectIsPulled(bool _value) { m_tPulledObj.m_bIsAnObjectPulled = _value; }
	void SetPulledObjectType( eOBJECT_TYPE _type ) { m_tPulledObj.ObjectPulledType = _type; }
	void SetObjectPulled( IBaseObject* _object ) { m_tPulledObj.m_Object = _object; }
	eOBJECT_TYPE GetPulledObjectType(void) { return m_tPulledObj.ObjectPulledType; }
	bool GetPulling();
	void SetPullEmitter( CComponent_Emitter* _component ) { m_pPullEmitterComponent = _component; }
	void SetPushEmitter( CComponent_Emitter* _component ) { m_pPushEmitterComponent = _component; }
	void SetWeaponState(eVacState val) { m_vsWeaponState = val; }

	CComponent_Input(void);

	bool Initialize(void);
	void Update(float fDT);
	void Render();
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: PlayerMovement_Right()
	// Purpose: Move the player to the right
	// Original Author: Rueben Massey
	// Creation Date: 5/12/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void PlayerMovement_Right( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: PlayerMovement_Left()
	// Purpose: Move the player to the left
	// Original Author: Rueben Massey
	// Creation Date: 5/12/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void PlayerMovement_Left( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: GetClosestObject()
	// Purpose: returns the closest object along the laser sight
	// Original Author: Rueben Massey
	// Creation Date: 5/15/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IBaseObject* GetClosestObject( vec3f _start, vec3f _end );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// GetClosestWall():	This function will get the nearest wall and 
	//						return it.
	//
	// Returns:				IBaseObject* - the closest wall, or null if no
	//										walls have been collided with.
	//
	// Mod Date:			6/7/12
	// Mod Initials:		JM
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IBaseObject* GetClosestWall(float &fDist);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: PlayerPush()
	// Purpose: Apply force to the closest object
	// Original Author: Rueben Massey
	// Creation Date: 5/15/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void PlayerPush( float fDT );

	////////////////////////////////////////////////////////////////////////
	// PlayerPull():	This function is to find the nearest object that
	//					the player is pointing at, and set it to go towards
	//					the player.
	//
	// Returns:			void
	//
	// Mod Date:		6/7/12
	// Mod Initials:	JM
	////////////////////////////////////////////////////////////////////////
	void PlayerPull( IBaseObject* toPull );
	void PlayerJump( float fDT );
	void Shutdown(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: HalfSpaceTest
	// Purpose: Determine which side of the plane the point is on.
	// Original Author: Andy Madruga
	// Creation Date: 5/17/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool HalfSpaceTest( vec3f PlanePos, vec3f PlaneNormal, vec3f TestPoint );

	vec3f GetValidPlacement( float _fMaxRange );

	void HandleMovementInput( float fDT );
};

#endif //CCOMPONENT_INPUT_H_