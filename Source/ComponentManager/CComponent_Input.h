#ifndef CCOMPONENT_INPUT_H_
#define CCOMPONENT_INPUT_H_

#include "IComponent.h"
#include "../PhysicsEnvironment/PhysicsEnvironment.h"
#include "../PS/CGame.h"
#include "../InputLib/InputManager.h"
#include "../Utilities/CollisionLib/CollisionShapes.h"
#include "../AudioLib/AudioManager.h"
#include "../HUD/CIcon.h"

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
class CTrajectory;

enum eVacState{ VACS_NULL, VACS_PUSH, VACS_PULL, VACS_HASOBJECT, VACS_MAX };

class CPM_ParEmitter;

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

	void Reset()
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

	CPM_ParEmitter* m_pJumpParticleEffect;
	float m_fJumpParticleTimer;
	float m_fWalkParticleTimer;

	CPM_ParEmitter* m_PSparkleParticleEffect;
	CPM_ParEmitter* m_pGooDripParticleEffect;
	CPM_ParEmitter* m_pPullSmokeParticleEffect;
	CPM_ParEmitter* m_pPullSparkleParticleEffect;

	// Variables for movement
	bool m_bJumping;
	bool m_bDoubleJump;
	bool m_bWallJump;
	bool m_bWallJumping;
	bool m_bRunning;
	bool m_bPreviouslyOnGround;
	bool m_bStopInput;
	float m_fPlayerSpeedMod;
	float m_fWalkTimer;

	eVacState m_vsWeaponState;

	float m_fDropTimer;

	// Icon
	CIcon* m_pIcon;
	int m_nPullIconTexture;
	int m_nPullIconTexture2;
	int m_nPushIconTexture;
	int m_nPushIconTexture2;


	// Line for the laser sight
	Line m_LineToCursor;

	bool RotatedLeft;
	bool RotatedRight;

	CComponent_Emitter* m_pEmitterComponent;
	CComponent_Emitter* m_pPullEmitterComponent;
	CComponent_Emitter* m_pPushEmitterComponent;

	PulledObj m_tPulledObj;
	IBaseObject* m_pCurrentTarget;

	bool m_bCreatePullOnce;

	float m_fRotationTimer;		// The timer which increases when the player is rotating from right to left or vice-versa.

	IBaseObject* SelectionMesh;	// The reticle Mesh Effect.

	// Trajectory timer
	float m_fTrajectoryTimer;
	CTrajectory* m_pTrajectory;

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
	void SetStopInput( bool _bStop ) { m_bStopInput = _bStop; }

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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CheckForInbetweens
	// Purpose: This function tells us if there's any AABBs between the player and what he's trying to pull. Returns true
	//			if there's something blocking my pull.
	// Original Author: Josh Morgan
	// Creation Date: 8/16/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool CheckForInbetweens(IBaseObject* _pTarget);

	vec3f GetValidPlacement( float _fMaxRange );

	void HandleMovementInput( float fDT );

	void ResetPulledObject(void )
	{
		m_tPulledObj.Reset();
		m_vsWeaponState = VACS_NULL;
	}
};

#endif //CCOMPONENT_INPUT_H_