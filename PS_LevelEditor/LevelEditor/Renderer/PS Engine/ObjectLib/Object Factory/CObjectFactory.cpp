/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CObjectFactory
//
// Purpose: Allocate memory for all objects	and components		
//
// Original Author: Rueben Massey
//
// Creation Date: 5/7/2012
//
// Last Modification By: RCM
// Last Modification Date: 5/10/2012
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CObjectFactory.h"
#include "..//ObjectManager/CObjectManager.h"
#include "../../ComponentManager/CComponent_PlayerCollision.h"
#include "../../ComponentManager/CComponent_PlayerArmCollision.h"
#include "../../ComponentManager/CComponent_CrateCollision.h"
#include "../../ComponentManager/CComponent_CorkCollision.h"
#include "../../ComponentManager/CComponent_GooPitCollision.h"
#include "../../ComponentManager/CComponent_BreakableWallCollision.h"
#include "../../ComponentManager/CComponent_SteamPipeCollision.h"
#include "../../AnimationLib/AnimationIncludes.h"

// Logger syntax
//LOG(LOG_INFO, "CMainMenuState::GetInstance()", "CMainMenuState's instance creation succeeded");

// This is the memory leak debug stuff
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

CObjectFactory::CObjectFactory()
{
	
}

CObjectFactory::~CObjectFactory()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Init()
// Purpose: Initializes all object and component memory pools.
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectFactory::Init( void )
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateObject()
// Purpose: Associates objects with components and passes them off to the Object Manager
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:	JM
// Last Modification Date:	6/1/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IBaseObject* CObjectFactory::CreateObject( eOBJECT_TYPE eType, D3DXMATRIX d3dPosMat, IBaseObject* _partner )
{
	// loop through the object inventory and find
	// the first unused object memory
	for( unsigned int objindex = 0; objindex < MAX_OBJECTS; ++objindex )
	{
		if( m_cObjectInventory[objindex].GetRefCount() == 0 )
		{
			// add a reference to this object
			// since whatever called this method
			// is holding on to a reference
			m_cObjectInventory[objindex].AddRef();
			// set the type
			m_cObjectInventory[objindex].SetType( eType );
			// add all necessary components
			AddObjectComponents( &m_cObjectInventory[objindex], d3dPosMat, _partner );
			return &m_cObjectInventory[objindex];
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: AddObjectComponents()
// Purpose: Associates objects with components and passes them off to the Object Manager
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectFactory::AddObjectComponents( IBaseObject* pObject, D3DXMATRIX d3dPosMat, IBaseObject* _partner )
{
	// Get the position vector from the matrix
	vec3f vPosition = vec3f( d3dPosMat._41, d3dPosMat._42, d3dPosMat._43 );

	// adds specific components based on the object type
	// each component needs to be added individually for each case
	switch( pObject->GetType() )
	{
	case OBJ_PLAYER:
		{
#if !_ANIMATIONS
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume( "PudgeShape" );
#else
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume( "Pudge" );
#endif
			pObject->SetCollidableObject(volume);

			IComponent* compToAdd = CreatePlayerColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if !_ANIMATIONS
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "PudgeShape" ) );
#else
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "Pudge" ) );
#endif
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// this should be done in the Object Manager
			// or in GamePlayState
			//////////////////////////////////////////////////////////////////////////////////////
			//D3DXMATRIX _worldMat;// = (*(D3DXMATRIX*)(pObject->GetMat()));
			//D3DXMATRIX _rotation;
			//D3DXMatrixRotationY(&_rotation, D3DXToRadian( -90.0f ) );
			//D3DXMatrixMultiply( &_worldMat, &_rotation, (D3DXMATRIX*)(pObject->GetMat()) );
			// This is actually setting the matrix not the position.
			//pObject->SetMat( (matrix4f*)&_worldMat);
			//////////////////////////////////////////////////////////////////////////////////////
			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateInputComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateHealthComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

  			compToAdd = CreateEmitterComp(FOOT_STEPS_FX);
  			// add the component to the object's
  			// component list
  			pObject->AddComponent( compToAdd );
  			// add the component to the
  			// component manager
  			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			// Set the initial state of the animation to Idle
			((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( PudgeIdleState::GetState() );
#endif

 			compToAdd = CreateCheckPointComp(  );
 			// add the component to the object's
 			// component list
 			pObject->AddComponent( compToAdd );
 			// add the component to the
 			// component manager
 			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			break;
		}
	case OBJ_PLAYER_ARM:
		{
#if !_ANIMATIONS
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume("PudgeArm");
#else
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume("VacuumArm");
#endif
			pObject->SetCollidableObject( volume );

			IComponent* compToAdd = CreatePlayerArmColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// Set up Component for Flattening of hierarchy.
			IBaseObject* player = CObjectManager::GetInstance()->GetPlayer();
			CComponent_PlayerArmCollision* pPlayerComp = (CComponent_PlayerArmCollision*)compToAdd;
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if !_ANIMATIONS
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "PudgeArm" ));
#else
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "VacuumArm" ));
#endif
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if !_ANIMATIONS
			player->GetFrame().AddChild( (Node*)&pObject->GetFrame() );
			player->GetFrame().Update();
#else
			// Set the Objects Parent
			pPlayerComp->SetObjectsParent( player );
			// Set the Animation Component.
			pPlayerComp->SetAnimationComponent();
			// Set the Frame Index.
			//pPlayerComp->SetFrameIndex("joint13");
			pPlayerComp->SetFrameIndex("rShoulderJGR");
			// Translate the arm by the joints bind pose.
			pPlayerComp->TranslateObjectByJoint();

			compToAdd = CreateAnimationComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			// Set the initial state of the animation to Idle
			((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( VacuumArmIdleState::GetState() );
#endif
			break;
		}
	case OBJ_GOO:
		{
#if !_ANIMATIONS
			CCollisionVolume* volume = new Box (*(Sphere*)MeshManager::GetInstance()->GetCollisionVolume( "LilGoo" ));
			pObject->SetCollidableObject (volume);
			delete volume;
#else
			CCollisionVolume* volume = new Box (*(Sphere*)MeshManager::GetInstance()->GetCollisionVolume( "LilGoo" ));
			pObject->SetCollidableObject (volume);
			delete volume;
#endif

			//((Sphere*)volume)->SetRadius (20.0f);
			//((Sphere*)volume)->SetBodyType (BT_DYNAMIC);
			//pObject->SetCollidableObject(volume);

			/*tPhyObject* pAddedPhyObj = CObjectManager::GetInstance()->m_PhysicsEnvironment.AddPhyObject( vPosition, 1.0f );
			CObjectManager::GetInstance()->m_PhysicsEnvironment.AddVelConstraint( VEL_CONSTRAINT, pAddedPhyObj );*/

			IComponent* compToAdd = CreateGooColComp( vPosition, nullptr );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateGooAIComp ();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateItemComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if !_ANIMATIONS
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "LilGoo" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#else
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "LilGoo" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif

			compToAdd = CreateHealthComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			//delete volume;

			compToAdd = CreateItemComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( LilGooWalkState::GetState() );
#endif

			break;
		}
	case OBJ_MONSTER:
		{
#if !_ANIMATIONS
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume( "SlimeMonster" );
#else
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume( "SlimeMonster" );
#endif
			//((Sphere*)volume)->SetRadius (20.0f);
			//((Sphere*)volume)->SetBodyType (BT_DYNAMIC);
			pObject->SetCollidableObject(volume);

			/*tPhyObject* pAddedPhyObj = CObjectManager::GetInstance()->m_PhysicsEnvironment.AddPhyObject( vPosition, 1.0f );
			CObjectManager::GetInstance()->m_PhysicsEnvironment.AddVelConstraint( VEL_CONSTRAINT, pAddedPhyObj );*/

			IComponent* compToAdd = CreateSlimeColComp( vPosition, NULL );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSlimeAIComp ();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#if !_ANIMATIONS
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "SlimeMonster" ) );
#else
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "SlimeMonster" ) );
#endif
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateHealthComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			//delete volume;

#if _ANIMATIONS 
			// Create an Animation component to animate the Slime Monster.
			compToAdd = CreateAnimationComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( SlimeMonsterIdleState::GetState() );
#endif
			break;
		}
	case OBJ_HULK:
		{
			break;
		}
	case OBJ_BOSS:
		{
			break;
		}
	case OBJ_CRUSHER:
		{
			// Created By: Andy Madruga
			CCollisionAABB volume;
			// Create a collision volume to be added to the Object.
			MeshManager::GetInstance()->LoadHazard( "Crusher", volume, d3dPosMat );
			pObject->SetCollidableObject( &volume );

			// Create a Crusher Component.
			IComponent* compToAdd = CreateCrusherCollisionComp( vPosition );
			// Add the Component to the Objects List.
			pObject->AddComponent( compToAdd );
			// Add the Component to the Component Managers.
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create a Render Component for the Crusher.
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "Crusher" ) );
			// Add the Component to the Objects List.
			pObject->AddComponent( compToAdd );
			// Add the Component to the Component Managers.
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			break;
		}
	case OBJ_BRIDGE:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "Bridge1", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);

			// Create the component
			IComponent* compToAdd = CreateBridgeColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create a Render Component to be added
#if _ANIMATIONS
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "Bridge" ) );
#else
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "Bridge1" ) );
#endif
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS 
			// Create an Animation component to animate the Slime Monster.
			compToAdd = CreateAnimationComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( BridgeIdleState::GetState() );
#endif

			break;
		}
	case OBJ_DOOR:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
 			MeshManager::GetInstance()->LoadHazard( "Door1", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);

			// TODO
			// Change to door collsion component
			// Create the component
			IComponent* compToAdd = CreateDoorColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create a Render Component to be added
#if _ANIMATIONS
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "Door" ) );
#else
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "Door1" ) );
#endif
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
			// Create an Animation component to animate the Slime Monster.
			compToAdd = CreateAnimationComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			// TODO
			// uncomment when the state is in
			((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( DoorIdleState::GetState() );
#endif

			break;
		}
	case OBJ_PLUG:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionVolume* volume = new Box (*(AABB*)MeshManager::GetInstance()->GetCollisionVolume( "Plug" ));
			pObject->SetCollidableObject (volume);
			delete volume;

			// Create a Physics Object at the objects position
			tPhyObject* pAddedPhyObj = CObjectManager::GetInstance()->m_PhysicsEnvironment.AddPhyObject( vPosition, 1.0f );
			CObjectManager::GetInstance()->m_PhysicsEnvironment.AddVelConstraint( VEL_CONSTRAINT, pAddedPhyObj );

			IComponent* compToAdd = CreatePlugColComp( vPosition, pAddedPhyObj );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// TODO
			// Change to Plug Mesh
			// Create a Render Component to be added
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "Plug" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create and add the Item Component
			compToAdd = CreateItemComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			break;
		}
	case OBJ_OUTLET:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "Outlet", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);

			// Create the component
			IComponent* compToAdd = CreateOutletColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create a Render Component to be added to the Goo Pit
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "Outlet" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			break;
		}
	case OBJ_PIT:
		{
			// Created By: Andy Madruga

			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "Goopit", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);

			IComponent* compToAdd = CreateGooPitColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// this should be done in the Object Manager
			// or in GamePlayState
			//////////////////////////////////////////////////////////////////////////////////////
			// pObject->SetMat( (matrix4f*)&_ObjectsMatrix );
			//////////////////////////////////////////////////////////////////////////////////////

			// Create a Render Component to be added to the Goo Pit
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "GooPit" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			break;
		}
	case OBJ_WALL_BREAKABLE_VER:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "BreakableWall", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);


			IComponent* compToAdd = CreateBreakableWallColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// this should be done in the Object Manager
			// or in GamePlayState
			//////////////////////////////////////////////////////////////////////////////////////
			//pObject->SetMat( (matrix4f*)&_ObjectsMatrix );
			//////////////////////////////////////////////////////////////////////////////////////

			// Create a Render Component to be added
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "BreakableWall" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			break;
		}
	case OBJ_WALL_BREAKABLE_HOR:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "BreakableWall", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);

			IComponent* compToAdd = CreateBreakableWallColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// this should be done in the Object Manager
			// or in GamePlayState
			//////////////////////////////////////////////////////////////////////////////////////
			//pObject->SetMat( (matrix4f*)&_ObjectsMatrix );
			//////////////////////////////////////////////////////////////////////////////////////

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create a Render Component to be added
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "BreakableWall" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			break;
		}
	case OBJ_PIPE_STEAM:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;// = MeshManager::GetInstance()->GetCollisionVolume( "SteamPipe" );
			MeshManager::GetInstance()->LoadHazard( "SteamPipe", volume, d3dPosMat );			
			pObject->SetCollidableObject (&volume);

			IComponent* compToAdd = CreateSteamPipeColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			
			// this should be done in the Object Manager
			// or in GamePlayState
			//////////////////////////////////////////////////////////////////////////////////////
			//pObject->SetMat( (matrix4f*)&_ObjectsMatrix );
			//////////////////////////////////////////////////////////////////////////////////////

			// Create a Render Component to be added
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "SteamPipe" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create an Emitter Component for particles
 			compToAdd = CreateEmitterComp( STEAM_FX );
 			pObject->AddComponent( compToAdd );
 			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			break;
		}
	case OBJ_PIPE_INTAKE:
		{
			// Create a Collision Volume to be Added to the Object			
			CCollisionAABB volume;// = MeshManager::GetInstance()->GetCollisionVolume( "SteamPipe" );
			MeshManager::GetInstance()->LoadHazard( "Intake", volume, d3dPosMat );			
			pObject->SetCollidableObject (&volume);

			// Create the component
			IComponent* compToAdd = CreateIntakePipeColComp( vPosition );
			
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create a Render Component to be added
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "Intake" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create an Emitter Component for particles
// 			compToAdd = CreateEmitterComp();
// 			pObject->AddComponent( compToAdd );
// 			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			break;
		}
	case OBJ_PIPE_SPAWN:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "SpawnPipe", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);

			IComponent* compToAdd = CreateSpawnPipeColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			//compToAdd = CreateSoundComp();
			//// add the component to the object's
			//// component list
			//pObject->AddComponent( compToAdd );
			//// add the component to the
			//// component manager
			//CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// this should be done in the Object Manager
			// or in GamePlayState
			//////////////////////////////////////////////////////////////////////////////////////
			//pObject->SetMat( (matrix4f*)&_ObjectsMatrix );
			//////////////////////////////////////////////////////////////////////////////////////

			// Create a Render Component to be added
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "SpawnPipe" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create an Emitter Component for particles
			//compToAdd = CreateEmitterComp();
			//pObject->AddComponent( compToAdd );
			//CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			break;
		}
	case OBJ_PIPE_BOX:
		{
			break;
		}
	case OBJ_CRATE:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionVolume* volume = new Box (*(AABB*)MeshManager::GetInstance()->GetCollisionVolume( "Crate" ));
			pObject->SetCollidableObject (volume);
			delete volume;

			// Create a Physics Object at the objects position
			tPhyObject* pAddedPhyObj = CObjectManager::GetInstance()->m_PhysicsEnvironment.AddPhyObject( vPosition, 1.0f );
			CObjectManager::GetInstance()->m_PhysicsEnvironment.AddVelConstraint( VEL_CONSTRAINT, pAddedPhyObj );

			IComponent* compToAdd = CreateCrateColComp( vPosition, pAddedPhyObj );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create a Render Component to be added
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "Crate" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create and add the Item Component
			compToAdd = CreateItemComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			pObject->SetLocalMat( (matrix4f*)&d3dPosMat );

//  			compToAdd = CreateImpactMeshEffectComp(  );
//  			// add the component to the object's
//  			// component list
//  			pObject->AddComponent( compToAdd );
//  			// add the component to the
//  			// component manager
//  			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			 
			break;
		}
	case OBJ_CORK:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionVolume* volume = new Box (*(AABB*)MeshManager::GetInstance()->GetCollisionVolume( "CorkShape" ));
			pObject->SetCollidableObject (volume);
			delete volume;

			// Create a Physics Object at the objects position
			tPhyObject* pAddedPhyObj = CObjectManager::GetInstance()->m_PhysicsEnvironment.AddPhyObject( vPosition, 1.0f );
			CObjectManager::GetInstance()->m_PhysicsEnvironment.AddVelConstraint( VEL_CONSTRAINT, pAddedPhyObj );

			IComponent* compToAdd = CreateCorkColComp( vPosition, pAddedPhyObj );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create a Render Component to be added
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "CorkShape" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Create and add the Item Component
			compToAdd = CreateItemComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			pObject->SetLocalMat( (matrix4f*)&d3dPosMat );
			break;
		}
	case OBJ_LEVEL:
		{
			break;
		}
	case OBJ_CHKPNT:
		{
			break;
		}
	case OBJ_POOL:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "WaterPool", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);

			IComponent* compToAdd = CreateWaterPoolColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSoundComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// this should be done in the Object Manager
			// or in GamePlayState
			//////////////////////////////////////////////////////////////////////////////////////
			// pObject->SetMat( (matrix4f*)&_ObjectsMatrix );
			//////////////////////////////////////////////////////////////////////////////////////

			// Create a Render Component to be added to the Goo Pit
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "WaterPool" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			break;
		}
	case OBJ_PULL_MESHEFFECT:
		{
			IComponent* compToAdd = CreatePullMeshEffectComp( /*vPosition*/ );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "PullMesh" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
 
			compToAdd = CreateEmitterComp(PULL_FX);
 			// add the component to the object's
 			// component list
 			pObject->AddComponent( compToAdd );
 			// add the component to the
 			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			// Set the initial state of the animation to Idle
			((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( PullEffectState::GetState() );
#endif
			break;
		}
	case OBJ_PUSH_MESHEFFECT:
		{
			IComponent* compToAdd = CreatePushMeshEffectComp( /*vPosition*/ );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Set the Matrix of the Component to use for the joint.
			CComponent_PushMeshEffect* pushComp = (CComponent_PushMeshEffect*)compToAdd;

			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "PushMesh" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

 			compToAdd = CreateEmitterComp(PUSH_FX);
 			// add the component to the object's
 			// component list
 			pObject->AddComponent( compToAdd );
 			// add the component to the
 			// component manager
 			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			// Set the initial state of the animation to Idle
			((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( PushEffectState::GetState() );

			IBaseObject* playerArm = CObjectManager::GetInstance()->GetPlayerArm();
			pushComp->SetJointMatrix( ((CComponent_Animation*)playerArm->GetComponent( ECOMP_ANIMATION))->GetBindPose()->GetJoint("VacuumElbow")->m_InverseLocalMatrix );
#endif
			pObject->SetLocalMat( (matrix4f*)&d3dPosMat );

			break;
		}
case OBJ_IMPACT_MESHEFFECT:
	{
 		IComponent* compToAdd = CreateImpactMeshEffectComp( vPosition );
 		// add the component to the object's
 		// component list
 		pObject->AddComponent( compToAdd );
 		// add the component to the
 		// component manager
 		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
 
		compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "ImpactMesh" ) );
 		// add the component to the object's
 		// component list
 		pObject->AddComponent( compToAdd );
 		// add the component to the
 		// component manager
 		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
 
//  		compToAdd = CreateEmitterComp();
//  		// add the component to the object's
//  		// component list
//  		pObject->AddComponent( compToAdd );
//  		// add the component to the
//  		// component manager
//  		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
 
 #if _ANIMATIONS
  		compToAdd = CreateAnimationComp();
  		// add the component to the object's
  		// component list
  		pObject->AddComponent( compToAdd );
  		// add the component to the
  		// component manager
  		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
  		// Set the initial state of the animation to Idle
 		((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( ImpactEffectState::GetState() );
#endif

			break;
	}
case OBJ_JUMP_MESHEFFECT:
	{
		IComponent* compToAdd = CreateJumpMeshEffectComp( );
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

		compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "JumpSteamStartMesh" ) );
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

  		compToAdd = CreateEmitterComp(JUMP_FX);
  		// add the component to the object's
  		// component list
  		pObject->AddComponent( compToAdd );
  		// add the component to the
  		// component manager
  		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
		compToAdd = CreateAnimationComp();
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
		// Set the initial state of the animation to Idle
		((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( JumpIdleEffectState::GetState() );
#endif

		break;
	}
case OBJ_LAND_MESHEFFECT:
	{
		IComponent* compToAdd = CreateLandMeshEffectComp(  );
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

		compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "JumpSteamFloorMesh" ) );
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

// 		compToAdd = CreateEmitterComp();
// 		// add the component to the object's
// 		// component list
// 		pObject->AddComponent( compToAdd );
// 		// add the component to the
// 		// component manager
// 		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
		compToAdd = CreateAnimationComp();
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
		// Set the initial state of the animation to Idle
		((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( LandIdleEffectState::GetState() );
#endif

		break;
	}
case OBJ_BIGHIT_MESHEFFECT:
	{
		IComponent* compToAdd = CreateBigHitMeshEffectComp(  );
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

		compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "BigHurtMesh" ) );
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

		compToAdd = CreateEmitterComp(LARGEDMG_FX);
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
		compToAdd = CreateAnimationComp();
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
		// Set the initial state of the animation to Idle
		((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( BigHurtIdleEffectState::GetState() );
#endif

		break;
	}
case OBJ_LITTLEHIT_MESHEFFECT:
	{
		IComponent* compToAdd = CreateLittleHitMeshEffectComp(  );
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

		compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "LittleHurtMesh" ) );
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

   		compToAdd = CreateEmitterComp(SMALLDMG_FX);
   		// add the component to the object's
   		// component list
   		pObject->AddComponent( compToAdd );
   		// add the component to the
   		// component manager
   		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
		compToAdd = CreateAnimationComp();
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
		// Set the initial state of the animation to Idle
		((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( LittleHurtIdleEffectState::GetState() );
#endif

		break;
	}
case OBJ_CHECKPOINT:
	{
		IComponent* compToAdd = CreateCheckPointComp(  );
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

// 		compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "ShowerMesh" ) );
		//// add the component to the object's
		//// component list
		//pObject->AddComponent( compToAdd );
		//// add the component to the
		//// component manager
		//CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );


		break;
	}
	case OBJ_GOO_PARTICLE_EFFECT:
		{
#if _PARTICLES
			// Create a Emitter Comp using the GOO Explode as the default particle effect.
			IComponent* compToAdd = CreateEmitterComp(GOO_EXPLODE_FX);
			// Add the component to the object component list.
			pObject->AddComponent( compToAdd );
			// Add the Component to the manager.
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif
			break;
		}
	case OBJ_CRUSHER_PARTICLE_EFFECT:
		{
#if _PARTICLES
			// Create a Emitter Comp using the GOO Explode as the default particle effect.
			IComponent* compToAdd = CreateEmitterComp(CRUSHERSPARKS_FX);
			// Add the component to the object component list.
			pObject->AddComponent( compToAdd );
			// Add the Component to the manager.
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif
			break;
		}
	case OBJ_PLAYER_DEATH_EFFECT:
		{
			IComponent* compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "DeathCloud" ) );
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
		compToAdd = CreateAnimationComp();
		// add the component to the object's
		// component list
		pObject->AddComponent( compToAdd );
		// add the component to the
		// component manager
		CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
		// Set the initial state of the animation to Idle
			((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( DeathCloudPudgeDeathState::GetState() );
#endif
		break;
	}
	case OBJ_TEST_ONE:
		{
			break;
		}
	case OBJ_TEST_TWO:
		{
			break;
		}
	case OBJ_TEST_THREE:
		{
			break;
		}
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ReleaseObject()
// Purpose: Add object back to inventory and release components
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectFactory::ReleaseObject( IBaseObject* pObject )
{
	// add all this object's components back to the inventory
	for( unsigned int i = 0; i < ECOMP_MAX; ++i )
	{
		IComponent* pComp = pObject->GetComponent( i );
		if( pComp )
		{
			pComp->Shutdown();
			CObjectManager::GetInstance()->m_pComponentManager.RemoveComponent( pComp );
		}
	}
	/*if( pObject->GetCollidableObject() )
	{
		delete pObject->GetCollidableObject();
	}*/
	// release the object back to the inventory
	pObject->Release();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ReleaseComponent()
// Purpose: Not currently applicable. DO NOT USE
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectFactory::ReleaseComponent( IBaseObject* pObject, IComponent* pComponent )
{
	pObject->ReleaseComponent( pComponent );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Shutdown()
// Purpose: FREE THE MEMORY
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectFactory::Shutdown( void )
{
	// clear the Object Manager's object list
	// Object Manager should do this itself.
	CObjectManager::GetInstance()->m_pComponentManager.Clear();
}

#pragma  region OLD COMP SYSTEM
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetAvailRenderComponent()
// Purpose: return the first available component in the inventory
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//IComponent* CObjectFactory::GetAvailComponent( int nComponentType )
//{
//	// Finds the first available inventory slot for
//	// the passed in component type.
//	// Initialization of the component is handled here as well.
//	switch( nComponentType )
//	{
//	case ECOMP_COLLISION:
//		{
//			for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS; ++cindex )
//			{
//				//if( m_cCollisionComponentInventory[cindex].GetParent() == nullptr )
//				{
//					//m_cCollisionComponentInventory[cindex].SetPhysicsBox( CObjectManager::GetInstance()->m_PhysicsEnvironment.AddPhyObject( vec3f( -50.0f, 10.0f, 0.0f ), 1.0f ) );
//					//m_cCollisionComponentInventory[cindex].Initialize();
//					return m_cCollisionComponentInventory[cindex];
//				}
//			}
//			break;
//		}
//	case ECOMP_RENDER:
//		{
//			for( unsigned int cindex = 0; cindex < MAX_RENDER_COMPONENTS; ++cindex )
//			{
//				if( m_cRenderComponentInventory[cindex].GetParent() == nullptr )
//				{
//					m_cRenderComponentInventory[cindex].SetRenderNode( MeshManager::GetInstance()->GetRenderNode( "PudgeShape" ) );
//					//m_cRenderComponentInventory[cindex].Initialize();
//					return &m_cRenderComponentInventory[cindex];
//				}
//			}
//			break;
//		}
//	case ECOMP_AI:
//		{
//			/*for( unsigned int cindex = 0; cindex < MAX_AI_COMPONENTS; ++cindex )
//			{
//			if( m_cAIComponentInventory[cindex].GetParent() == nullptr )
//			{
//			m_cAIComponentInventory[cindex].Initialize();
//			return &m_cAIComponentInventory[cindex];
//			}
//
//			}*/
//			break;
//		}
//	case ECOMP_INPUT:
//		{
//			/*for( unsigned int cindex = 0; cindex < MAX_COMPONENTS; ++cindex )
//			{
//			if( m_cInputComponentInventory[cindex].GetParent() == nullptr )
//			{
//			m_cInputComponentInventory[cindex].Initialize();
//			return &m_cInputComponentInventory[cindex];
//			}
//			}*/
//
//			// We only have one input component
//			if( m_cInputComponentInventory.GetParent() == nullptr )
//			{
//				m_cInputComponentInventory.Initialize();
//				return &m_cInputComponentInventory;
//			}
//			break;
//		}
//	case ECOMP_ANIMATION:
//		{
//#if _ANIMATIONS
//#else
//			for( unsigned int cindex = 0; cindex < MAX_ANIMATION_COMPONENTS; ++cindex )
//			{
//				if( m_cAnimationComponentInventory[cindex].GetParent() == nullptr )
//				{
//					m_cAnimationComponentInventory[cindex].Initialize();
//					return &m_cAnimationComponentInventory[cindex];
//				}				
//			}
//#endif
//			break;
//		}
//	case ECOMP_ITEM:
//		{
//			for( unsigned int cindex = 0; cindex < MAX_ITEM_COMPONENTS; ++cindex )
//			{
//				if( m_cItemComponentInventory[cindex].GetParent() == nullptr )
//				{
//					m_cItemComponentInventory[cindex].Initialize();
//					return &m_cItemComponentInventory[cindex];
//				}
//
//			}
//			break;
//		}
//	case ECOMP_SPAWN:
//		{
//			/*for( unsigned int cindex = 0; cindex < MAX_COMPONENTS; ++cindex )
//			{
//			if( m_cSpawnComponentInventory[cindex].GetParent() == nullptr )
//			{
//			m_cSpawnComponentInventory[cindex].Initialize();
//			return &m_cSpawnComponentInventory[cindex];
//			}
//
//			}*/
//			break;
//		}
//	case ECOMP_EMITTER:
//		{
//			for( unsigned int cindex = 0; cindex < MAX_EMITTER_COMPONENTS; ++cindex )
//			{
//				if( m_cEmitterComponentInventory[cindex].GetParent() == nullptr )
//				{
//					m_cEmitterComponentInventory[cindex].Initialize();
//					return &m_cEmitterComponentInventory[cindex];
//				}
//			}
//			break;
//		}
//	case ECOMP_HEALTH:
//		{
//			for( unsigned int cindex = 0; cindex < MAX_HEALTH_COMPONENTS; ++cindex )
//			{
//				if( m_cHealthComponentInventory[cindex].GetParent() == nullptr )
//				{
//					m_cHealthComponentInventory[cindex].Initialize();
//				}
//				return &m_cHealthComponentInventory[cindex];
//			}
//			break;
//		}
//
//	default:
//		break;
//	}
//	return nullptr;
//}
#pragma  endregion
#pragma region OLD COL COMP
//IComponent* CObjectFactory::GetComp_Collision(IBaseObject* _pParent, vec3f _fPos, eCollisionCMPType _colliderType, tPhyObject* _phyObject/*, CCollisionVolume* _fColVolume*/)
//{
//	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS; ++cindex )
//	{
//		if (!m_cCollisionComponentInventory[cindex])
//		{
//			switch (_colliderType)
//			{
//			case CCMP_PLAYER:
//				m_cCollisionComponentInventory[cindex] = new CComponent_PlayerCollision();
//				break;
//			case CCMP_CRATE:
//				m_cCollisionComponentInventory[cindex] = new CComponent_CrateCollision();
//				break;
//			case CCMP_CORK:
//				m_cCollisionComponentInventory[cindex] = new CComponent_CorkCollision();
//				break;
//			case CCMP_GOOPIT:
//				m_cCollisionComponentInventory[cindex] = new CComponent_GooPitCollision();
//				break;
//			case CCMP_BREAKABLE:
//				m_cCollisionComponentInventory[cindex] = new CComponent_BreakableWallCollision();
//				break;
//			case CCMP_STEAMPIPE:
//				m_cCollisionComponentInventory[cindex] = new CComponent_SteamPipeCollision();
//				break;
//			}
//			m_cCollisionComponentInventory[cindex]->SetParent (_pParent);
//			m_cCollisionComponentInventory[cindex]->Initialize (/*_fColVolume*/);
//
//			m_cCollisionComponentInventory[cindex]->GetPos() = _fPos;
//			m_cCollisionComponentInventory[cindex]->SetCollType( _colliderType );
//			
//			if( _phyObject )
//				m_cCollisionComponentInventory[cindex]->SetPhysicsBox( _phyObject );
//
//			return m_cCollisionComponentInventory[cindex];
//		}
//		else if( m_cCollisionComponentInventory[cindex]->GetParent() == nullptr )
//		{
//			m_cCollisionComponentInventory[cindex]->SetParent (_pParent);
//			m_cCollisionComponentInventory[cindex]->Initialize (/*_fColVolume*/);
//			
//			m_cCollisionComponentInventory[cindex]->GetPos() = _fPos;
//			m_cCollisionComponentInventory[cindex]->SetCollType( _colliderType );
//			
//			if( _phyObject )
//				m_cCollisionComponentInventory[cindex]->SetPhysicsBox( _phyObject );
//
//			return m_cCollisionComponentInventory[cindex];
//		}
//	}
//	return nullptr;
//}

//IComponent* CObjectFactory::GetComp_Render( RenderNode* _rendernode )
//{
//	for( unsigned int cindex = 0; cindex < MAX_COMPONENTS; ++cindex )
//	{
//		if( m_cRenderComponentInventory[cindex].GetParent() == nullptr )
//		{
//			m_cRenderComponentInventory[cindex].Initialize();
//
//			if( _rendernode )
//				m_cRenderComponentInventory[cindex].SetRenderNode( _rendernode );
//
//			return &m_cRenderComponentInventory[cindex];
//		}
//	}
//
//	return nullptr;
//}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateRenderComp()
// Purpose: Finds the first available render component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateRenderComp( RenderNode* pRenderNode )
{
	// Input Safety Check
	if( pRenderNode )
	{
		// Find the first available component
		for( unsigned int cindex = 0; cindex < MAX_RENDER_COMPONENTS; ++cindex )
		{
			// If the component doesn't have a parent
			// it's available for use
			if( m_cRenderComponentInventory[cindex].GetParent() == nullptr )
			{
				m_cRenderComponentInventory[cindex].Initialize();
				m_cRenderComponentInventory[cindex].SetRenderNode( pRenderNode );
				return &m_cRenderComponentInventory[cindex];
			}
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateAIComp()
// Purpose: Finds the first available AI component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateAIComp( void )
{
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateInputComp()
// Purpose: Initializes the input component and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateInputComp( void )
{
	// We only have one input component
	// If the component doesn't have a parent
	// it's available for use
	if( m_cInputComponentInventory.GetParent() == nullptr )
	{
		m_cInputComponentInventory.Initialize();
		return &m_cInputComponentInventory;
	}
	return nullptr;
}

#if _ANIMATIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateAnimationComp()
// Purpose: Finds the first available Animation component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateAnimationComp( void )
{
	// Find the first available component
	for( unsigned int cindex = 0; cindex < MAX_ANIMATION_COMPONENTS; ++cindex )
	{
		// If the component doesn't have a parent
		// it's available for use
		if( m_cAnimationComponentInventory[cindex].GetParent() == nullptr )
		{
			m_cAnimationComponentInventory[cindex].Initialize();
			return &m_cAnimationComponentInventory[cindex];
		}				
	}
	return nullptr;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateItemComp()
// Purpose: Finds the first available Item component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateItemComp( void )
{
	// Find the first available component
	for( unsigned int cindex = 0; cindex < MAX_ITEM_COMPONENTS; ++cindex )
	{
		// If the component doesn't have a parent
		// it's available for use
		if( m_cItemComponentInventory[cindex].GetParent() == nullptr )
		{
			m_cItemComponentInventory[cindex].Initialize();
			return &m_cItemComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateSpawnComp()
// Purpose: Finds the first available Spawn component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateSpawnComp( void )
{
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateEmitterComp()
// Purpose: Finds the first available Emitter component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateEmitterComp( int nType )
{
	// Find the first available component
	for( unsigned int cindex = 0; cindex < MAX_EMITTER_COMPONENTS; ++cindex )
	{
		// If the component doesn't have a parent
		// it's available for use
		if( m_cEmitterComponentInventory[cindex].GetParent() == nullptr )
		{
			m_cEmitterComponentInventory[cindex].Initialize();
			m_cEmitterComponentInventory[cindex].SetType(nType);
			return &m_cEmitterComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreatePullMeshEffectComp()
// Purpose: Finds the first available Mesh Effect component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Kevin Clouden
// Creation Date: 5/31/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreatePullMeshEffectComp( /*vec3f pos*/ )
{
	// Find the first available component
	for( int index = 0; index < MAX_PULLMESHEFFECT_COMPONENTS; index++)
	{
		// Find the first available component
		if( m_cPullMeshEffectComponentInventory[index].GetParent() == nullptr )
		{
			m_cPullMeshEffectComponentInventory[index].Initialize();
			return &m_cPullMeshEffectComponentInventory[index];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateMeshEffectComp()
// Purpose: Finds the first available Mesh Effect component in
//			the inventory, initializes it, and returns a pointer
//			to it. Will probably be deleted
// Original Author: Kevin Clouden
// Creation Date: 5/31/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreatePushMeshEffectComp( /*vec3f pos*/ )
{
	for( int index = 0; index < MAX_PUSHMESHEFFECT_COMPONENTS; index++)
	{
	// Find the first available component
		if( m_cPushMeshEffectComponentInventory[index].GetParent() == nullptr )
	{
			m_cPushMeshEffectComponentInventory[index].Initialize();
			return &m_cPushMeshEffectComponentInventory[index];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateJumpMeshEffectComp()
// Purpose: Finds the first available Mesh Effect component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Kevin Clouden
// Creation Date: 5/31/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateJumpMeshEffectComp(  )
{
	// Find the first available component
	if( m_cJumpMeshEffectComponentInventory.GetParent() == nullptr )
	{
		m_cJumpMeshEffectComponentInventory.Initialize();
		return &m_cJumpMeshEffectComponentInventory;
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateLandMeshEffectComp()
// Purpose: Finds the first available Mesh Effect component in
//			the inventory, initializes it, and returns a pointer
//			to it. Will probably be removed
// Original Author: Kevin Clouden
// Creation Date: 5/31/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateLandMeshEffectComp( /*vec3f pos*/ )
{
	// Find the first available component
	if( m_cLandMeshEffectComponentInventory.GetParent() == nullptr )
	{
		m_cLandMeshEffectComponentInventory.Initialize();
		return &m_cLandMeshEffectComponentInventory;
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateBigHitMeshEffectComp()
// Purpose: Finds the first available Mesh Effect component in
//			the inventory, initializes it, and returns a pointer
//			to it. Will probably be removed
// Original Author: Kevin Clouden
// Creation Date: 5/31/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateBigHitMeshEffectComp( /*vec3f pos*/ )
{
	// Find the first available component
	for( unsigned int cindex = 0; cindex < MAX_BIGHURTMESHEFFECT_COMPONENTS; ++cindex )
	{
		// If the component doesn't have a parent
		// it's available for use
		if( m_cBigHitMeshEffectComponentInventory[cindex].GetParent() == nullptr )
		{
			m_cBigHitMeshEffectComponentInventory[cindex].Initialize();
			return &m_cBigHitMeshEffectComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateLittleHitMeshEffectComp()
// Purpose: Finds the first available Mesh Effect component in
//			the inventory, initializes it, and returns a pointer
//			to it. Will probably be removed
// Original Author: Kevin Clouden
// Creation Date: 5/31/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateLittleHitMeshEffectComp( /*vec3f pos*/ )
{
	// Find the first available component
	for( unsigned int cindex = 0; cindex < MAX_LITTLEHURTMESHEFFECT_COMPONENTS; ++cindex )
	{
		// If the component doesn't have a parent
		// it's available for use
		if( m_cLittleHitMeshEffectComponentInventory[cindex].GetParent() == nullptr )
		{
			m_cLittleHitMeshEffectComponentInventory[cindex].Initialize();
			return &m_cLittleHitMeshEffectComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateSlimeMonsterStunMeshEffectComp()
// Purpose: Finds the first available Mesh Effect component in
//			the inventory, initializes it, and returns a pointer
//			to it. Will probably be removed
// Original Author: Kevin Clouden
// Creation Date: 5/31/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IComponent* CObjectFactory::CreateSlimeMonsterStunMeshEffectComp(  )
// {
// 	// Find the first available component
// 	for( unsigned int cindex = 0; cindex < MAX_SLIMEMONSTERMESHEFFECT_COMPONENTS; ++cindex )
// 	{
// 		// If the component doesn't have a parent
// 		// it's available for use
// 		if( m_cSlimeMonsterStunMeshEffectComponentInventory[cindex].GetParent() == nullptr )
// 		{
// 			m_cSlimeMonsterStunMeshEffectComponentInventory[cindex].Initialize();
// 			return &m_cSlimeMonsterStunMeshEffectComponentInventory[cindex];
// 		}
// 	}
// 	return nullptr;
// }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateImpactEffectComp()
// Purpose: Finds the first available Mesh Effect component in
//			the inventory, initializes it, and returns a pointer
//			to it. Will probably be deleted
// Original Author: Kevin Clouden
// Creation Date: 5/31/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateImpactMeshEffectComp( vec3f pos )
{
	// Find the first available component
	for( unsigned int cindex = 0; cindex < MAX_IMPACTMESHEFFECT_COMPONENTS; ++cindex )
	{
		// If the component doesn't have a parent
		// it's available for use
		if( m_cImpactMeshEffectComponentInventory[cindex].GetParent() == nullptr )
		{
			m_cImpactMeshEffectComponentInventory[cindex].Initialize();
			//m_cImpactMeshEffectComponentInventory[cindex].GetMatrix().axis_pos = pos;
			m_cImpactMeshEffectComponentInventory[cindex].GetPos() = pos;
			return &m_cImpactMeshEffectComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateHealthComp()
// Purpose: Finds the first available Health component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateHealthComp( void )
{
	// Find the first available component
	for( unsigned int cindex = 0; cindex < MAX_HEALTH_COMPONENTS; ++cindex )
	{
		// If the component doesn't have a parent
		// it's available for use
		if( m_cHealthComponentInventory[cindex].GetParent() == nullptr )
		{
			m_cHealthComponentInventory[cindex].Initialize();
			return &m_cHealthComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateCheckPointComp()
// Purpose: Finds the first available Checkpoint component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Kevin Clouden
// Creation Date: 6/10/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateCheckPointComp( void )
{
	// Find the first available component
	if( m_cCheckPointComponentInventory.GetParent() == nullptr )
	{
		m_cCheckPointComponentInventory.Initialize();
		return &m_cCheckPointComponentInventory;
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreatePlayerColComp()
// Purpose: Finds the first available Player Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreatePlayerColComp( vec3f fPosition )
{		
	// If the component doesn't have a parent
	// it's available for use
	if( nullptr == m_cPlayerCollisionComponent.GetParent() )
	{
		m_cPlayerCollisionComponent.Initialize();
		// Set the position
		m_cPlayerCollisionComponent.GetPos() = fPosition;
		// Set the collision type
		m_cPlayerCollisionComponent.SetCollType( CCMP_PLAYER );
		return &m_cPlayerCollisionComponent;
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateGooColComp()
// Purpose: Finds the first available Goo Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Josh Fields
// Creation Date:	6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateGooColComp( vec3f fPosition, tPhyObject* tPhysicsObject )
{
	// Input Safety Check
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_GOO; ++cindex )
	{
		if( nullptr ==  m_cGooCollisionComponentInventory[cindex].GetParent() )
		{
			m_cGooCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cGooCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cGooCollisionComponentInventory[cindex].SetCollType( CCMP_GOO );
			return &m_cGooCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateWaterPoolComp()
// Purpose: Finds the first available Goo Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Josh Fields
// Creation Date:	6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateWaterPoolColComp( vec3f fPosition )
{
	// Input Safety Check
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_GOO; ++cindex )
	{
		if( nullptr ==  m_cPoolCollisionComponentInventory[cindex].GetParent() )
		{
			m_cPoolCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cPoolCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cPoolCollisionComponentInventory[cindex].SetCollType( CCMP_POOL );
			return &m_cPoolCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateGooColComp()
// Purpose: Finds the first available Goo Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Josh Fields
// Creation Date:	6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateSlimeColComp( vec3f fPosition, tPhyObject* tPhysicsObject )
{
	// Input Safety Check
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_GOO; ++cindex )
	{
		if( nullptr ==  m_cSlimeCollisionComponentInventory[cindex].GetParent() )
		{
			m_cSlimeCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cSlimeCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cSlimeCollisionComponentInventory[cindex].SetCollType( CCMP_GOO );
			return &m_cSlimeCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateGooAIComp()
// Purpose: Finds the first available Goo Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Josh Fields
// Creation Date:	6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateGooAIComp (  )
{
	// Input Safety Check
	for( unsigned int cindex = 0; cindex < MAX_AI_COMPONENTS; ++cindex )
	{
		if( nullptr ==  m_cGooAIComponentInventory[cindex].GetParent() )
		{
			m_cGooAIComponentInventory[cindex].Initialize();
			// Set the AI type
			m_cGooAIComponentInventory[cindex].SetAIType ( AI_LIL_GOO );
			return &m_cGooAIComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateSlimeAIComp()
// Purpose: Finds the first available Goo Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Josh Fields
// Creation Date:	6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateSlimeAIComp (  )
{
	// Input Safety Check
	for( unsigned int cindex = 0; cindex < MAX_AI_COMPONENTS; ++cindex )
	{
		if( nullptr ==  m_cSlimeAIComponentInventory[cindex].GetParent() )
		{
			m_cSlimeAIComponentInventory[cindex].Initialize();
			// Set the AI type
			m_cSlimeAIComponentInventory[cindex].SetAIType ( AI_SLIME_MONSTER );
			return &m_cSlimeAIComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateCrateColComp()
// Purpose: Finds the first available Crate Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateCrateColComp( vec3f fPosition, tPhyObject* tPhysicsObject )
{
	// Input Safety Check
	if( tPhysicsObject )
	{
		for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_CRATE; ++cindex )
		{
			if( nullptr ==  m_cCrateCollisionComponentInventory[cindex].GetParent() )
			{
				m_cCrateCollisionComponentInventory[cindex].Initialize();
				// Set the position
				m_cCrateCollisionComponentInventory[cindex].GetPos() = fPosition;
				// Set the collision type
				m_cCrateCollisionComponentInventory[cindex].SetCollType( CCMP_CRATE );
				// Give the component a physics object
				m_cCrateCollisionComponentInventory[cindex].SetPhysicsBox( tPhysicsObject );
				return &m_cCrateCollisionComponentInventory[cindex];
			}
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateCorkColComp()
// Purpose: Finds the first available Cork Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateCorkColComp( vec3f fPosition, tPhyObject* tPhysicsObject )
{
	// Input Safety Check
	if( /*fPosition &&*/ tPhysicsObject )
	{
		for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_CORK; ++cindex )
		{
			if( nullptr ==  m_cCorkCollisionComponentInventory[cindex].GetParent() )
			{
				m_cCorkCollisionComponentInventory[cindex].Initialize();
				// Set the position
				m_cCorkCollisionComponentInventory[cindex].GetPos() = fPosition;
				// Set the collision type
				m_cCorkCollisionComponentInventory[cindex].SetCollType( CCMP_CORK );
				// Give the component a physics object
				m_cCorkCollisionComponentInventory[cindex].SetPhysicsBox( tPhysicsObject );
				return &m_cCorkCollisionComponentInventory[cindex];
			}
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateGooPitColComp()
// Purpose: Finds the first available GooPit Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateGooPitColComp( vec3f fPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_GOOPIT; ++cindex )
	{
		if( nullptr ==  m_cGooPitCollisionComponentInventory[cindex].GetParent() )
		{
			m_cGooPitCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cGooPitCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cGooPitCollisionComponentInventory[cindex].SetCollType( CCMP_GOOPIT );
			return &m_cGooPitCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateBreakableWallColComp()
// Purpose: Finds the first available BreakableWall Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateBreakableWallColComp( vec3f fPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_BREAKWALL; ++cindex )
	{
		if( nullptr ==  m_cBreakableWallCollisionComponentInventory[cindex].GetParent() )
		{
			m_cBreakableWallCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cBreakableWallCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cBreakableWallCollisionComponentInventory[cindex].SetCollType( CCMP_BREAKABLE );
			return &m_cBreakableWallCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateSteamPipeColComp()
// Purpose: Finds the first available BreakableWall Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: JM
// Last Modification Date: 5/31/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateSteamPipeColComp( vec3f fPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_STEAMPIPE; ++cindex )
	{
		if( nullptr ==  m_cSteamPipeCollisionComponentInventory[cindex].GetParent() )
		{
			m_cSteamPipeCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cSteamPipeCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cSteamPipeCollisionComponentInventory[cindex].SetCollType( CCMP_STEAMPIPE );
			// TODO
			// REMOVE THIS
			// The level editor will create the AABB
			m_cSteamPipeCollisionComponentInventory[cindex].BuildSteamAABB( 500, 0 );
			return &m_cSteamPipeCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateIntakePipeColComp()
// Purpose: Finds the first available Intake Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateIntakePipeColComp( vec3f fPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_STEAMPIPE; ++cindex )
	{
		if( nullptr ==  m_cIntakePipeCollisionComponentInventory[cindex].GetParent() )
		{
			m_cIntakePipeCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cIntakePipeCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cIntakePipeCollisionComponentInventory[cindex].SetCollType( CCMP_STEAMPIPE );
			return &m_cIntakePipeCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateSpawnPipeColComp()
// Purpose: Finds the first available BreakableWall Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateSpawnPipeColComp( vec3f fPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_SPAWNPIPE; ++cindex )
	{
		if( nullptr ==  m_cSpawnPipeCollisionComponentInventory[cindex].GetParent() )
		{
			m_cSpawnPipeCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cSpawnPipeCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cSpawnPipeCollisionComponentInventory[cindex].SetCollType( CCMP_STEAMPIPE );
			return &m_cSpawnPipeCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateBridgeColComp()
// Purpose: Finds the first available Bridge Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateBridgeColComp( vec3f fPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_BRIDGE; ++cindex )
	{
		if( nullptr ==  m_cBridgeCollisionComponentInventory[cindex].GetParent() )
		{
			m_cBridgeCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cBridgeCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cBridgeCollisionComponentInventory[cindex].SetCollType( CCMP_BRIDGE );
			return &m_cBridgeCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateDoorColComp()
// Purpose: Finds the first available Bridge Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateDoorColComp( vec3f fPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_DOOR; ++cindex )
	{
		if( nullptr ==  m_cDoorCollisionComponentInventory[cindex].GetParent() )
		{
			m_cDoorCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cDoorCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cDoorCollisionComponentInventory[cindex].SetCollType( CCMP_DOOR );
			return &m_cDoorCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateOutletColComp()
// Purpose: Finds the first available Outlet Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateOutletColComp( vec3f fPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_OUTLET; ++cindex )
	{
		if( nullptr ==  m_cOutletCollisionComponentInventory[cindex].GetParent() )
		{
			m_cOutletCollisionComponentInventory[cindex].Initialize();
			// Set the position
			m_cOutletCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cOutletCollisionComponentInventory[cindex].SetCollType( CCMP_OUTLET );
			return &m_cOutletCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreatePlugColComp()
// Purpose: Finds the first available Plug Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreatePlugColComp( vec3f fPosition, tPhyObject* tPhysicsObject )
{
	// Input Safety Check
	if( tPhysicsObject )
	{
		for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_PLUG; ++cindex )
		{
			if( nullptr ==  m_cPlugCollisionComponentInventory[cindex].GetParent() )
			{
				m_cPlugCollisionComponentInventory[cindex].Initialize();
				// Set the position
				m_cPlugCollisionComponentInventory[cindex].GetPos() = fPosition;
				// Set the collision type
				m_cPlugCollisionComponentInventory[cindex].SetCollType( CCMP_PLUG );
				// Give the component a physics object
				m_cPlugCollisionComponentInventory[cindex].SetPhysicsBox( tPhysicsObject );
				return &m_cPlugCollisionComponentInventory[cindex];
			}
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreatePlayerArmColComp()
// Purpose: Finds the first available Player Arm Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Andy Madruga
// Creation Date: 5/30/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreatePlayerArmColComp( vec3f fPosition )
{
	// If the component doesn't have a parent
	// it's available for use
	if( NULL == m_cPlayerArmCollisionComponent.GetParent() )
	{
		m_cPlayerArmCollisionComponent.Initialize();
		// Set the position
		m_cPlayerArmCollisionComponent.GetPos() = fPosition;
		// Set the collision type
		m_cPlayerArmCollisionComponent.SetCollType( CCMP_PLAYERARM );
		return &m_cPlayerArmCollisionComponent;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateCrusherCollisionComp()
// Purpose: Finds the first available Crusher Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Andy Madruga
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateCrusherCollisionComp( vec3f fPosition )
{
	// If the component doesn't have a parent
	// it's available for use
	for( unsigned int cIndex = 0; cIndex < MAX_COLLISION_COMPONENTS_CRUSHER; cIndex++)
	{
		if( NULL == m_cCrusherCollisionInventory[cIndex].GetParent() )
		{
			m_cCrusherCollisionInventory[cIndex].Initialize();
			// Set Position
			m_cCrusherCollisionInventory[cIndex].GetPos() = fPosition;
			// Set the Collision type
			m_cCrusherCollisionInventory[cIndex].SetCollType( CCMP_CRUSHER );
			return &m_cCrusherCollisionInventory[cIndex];
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreatePlayerSoundComp():		Gets the first available component and returns that as
//								the sound component.
// Creation Date:				5/31/12
// Mod Name:					JM
// Mod Date:					5/31/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateSoundComp(void)
{
	for( unsigned int cindex = 0; cindex < MAX_SOUND_COMPONENTS; ++cindex )
	{
		if( nullptr ==  m_cSoundComponentInventory[cindex].GetParent() )
		{
			m_cSoundComponentInventory[cindex].Initialize();
			return &m_cSoundComponentInventory[cindex];
		}
	}
	return nullptr;
}