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
#include "../../AI/AIStatesInclude.h"
#include "../../AnimationLib/State.h"
#include "../../RenderEngine/TextureManager.h"
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
			AddObjectComponents( &m_cObjectInventory[objindex], d3dPosMat );

			// Make sure it's rendered
			m_cObjectInventory[objindex].SetRendered(true);
			// Make sure it's alive
			m_cObjectInventory[objindex].SetAlive(true);
			// Initialize all components
			m_cObjectInventory[objindex].InitComponents();
			// Set up data members
			InitObjectComponents( &m_cObjectInventory[objindex], d3dPosMat, _partner );

			return &m_cObjectInventory[objindex];
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateAnimatedBGObject
// Purpose: Adds the animated background object to the ObjectManager and adds a reference to that object
// Original Author: Rueben Massey
// Creation Date: 7/21/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
IBaseObject* CObjectFactory::CreateAnimatedBGObject ( eOBJECT_TYPE eType, D3DXMATRIX _ObjectMatrix, const char* _szRenderNode, State* _AnimationState )
{
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
			IComponent* compToAdd;
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( _szRenderNode ) );

			// add the component to the object's
			// component list
			m_cObjectInventory[objindex].AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateAnimationComp();
			// add the component to the object's
			// component list
			m_cObjectInventory[objindex].AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			// Set Position

			// Most likely not used for these objects but
			// just to be safe.
			m_cObjectInventory[objindex].SetAlive(true);
			// Initialize all components
			m_cObjectInventory[objindex].InitComponents();

			// Set the initial state of the animation to Idle
			((CComponent_Animation*)compToAdd)->GetAnimStateMachine()->ChangeState( _AnimationState );

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
void CObjectFactory::AddObjectComponents( IBaseObject* pObject, D3DXMATRIX d3dPosMat )
{
	// Get the position vector from the matrix
	vec3f vPosition = vec3f( d3dPosMat._41, d3dPosMat._42, d3dPosMat._43 );
	pObject->SetLocalMat ((matrix4f*)&d3dPosMat);

	// adds specific components based on the object type
	// each component needs to be added individually for each case
	switch( pObject->GetType() )
	{
	case OBJ_PLAYER:
		{
			IComponent* compToAdd = CreatePlayerColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if !_ANIMATIONS
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "PudgeShape" ) );
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume( "PudgeShape" );
#else
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "Pudge" ) );
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume( "Pudge" );
#endif
			pObject->SetCollidableObject(volume);

			((CComponent_Render*)compToAdd)->GetRenderNode()->m_pOwner->AddFlags( RE_PLAYER );
			((CComponent_Render*)compToAdd)->GetRenderNode()->m_pOwner->renderData.m_uiTextureHandles[5] = TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/T_PlayerDirty_D.png" );
			((CComponent_Render*)compToAdd)->GetRenderNode()->m_pOwner->renderData.m_uiTextureHandles[6] = ((CComponent_Render*)compToAdd)->GetRenderNode()->m_pOwner->renderData.m_uiTextureHandles[1];
			((CComponent_Render*)compToAdd)->GetRenderNode()->m_pOwner->renderData.m_uiTextureHandles[7] = ((CComponent_Render*)compToAdd)->GetRenderNode()->m_pOwner->renderData.m_uiTextureHandles[2];
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

			compToAdd = CreateEmitterComp( FOOT_STEPS_FX );
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
#endif
			break;
		}
	case OBJ_PLAYER_ARM:
		{
			IComponent* compToAdd = CreatePlayerArmColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// Set up Component for Flattening of hierarchy.
			IBaseObject* player = CObjectManager::GetInstance()->GetPlayer();
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if !_ANIMATIONS
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "PudgeArm" ) );
#else
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "VacuumArm" ) );
#endif
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateAnimationComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
			break;
		}
	case OBJ_GOO:
		{
#if !_ANIMATIONS
			CCollisionVolume* volume = new Box (*(Sphere*)MeshManager::GetInstance()->GetCollisionVolume( "LilGoo" ));			
#else
			CCollisionVolume* volume = new Box (*(Sphere*)MeshManager::GetInstance()->GetCollisionVolume( "LilGoo" ));
#endif
			pObject->SetCollidableObject (volume);
			delete volume;

			tPhyObject* pAddedPhyObj = CObjectManager::GetInstance()->m_PhysicsEnvironment.AddPhyObject( vPosition, 1.0f );

			IComponent* compToAdd = CreateGooColComp( vPosition, pAddedPhyObj );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateGooAIComp(pObject);
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
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "LilGoo" ));
#else
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "LilGoo" ) );			
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
			//delete volume;

			compToAdd = CreateItemComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif

			break;
		}
	case OBJ_MONSTER:
		{
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume( "SlimeMonster" );

			pObject->SetCollidableObject(volume);

			IComponent* compToAdd = CreateSlimeColComp( vPosition, NULL );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateSlimeAIComp(pObject);
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

			compToAdd = CreateFlasherComp(2.0f, false);
			// add the component to the object's component list
			pObject->AddComponent( compToAdd );
			// add the component to the component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS 
			// Create an Animation component to animate the Slime Monster.
			compToAdd = CreateAnimationComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif
			break;
		}
	case OBJ_HULK:
		{
			break;
		}
	case OBJ_SLUDGE:
		{
#if !_ANIMATIONS
			IComponent* compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "PudgeShape" ) );
#else
			IComponent* compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "Sludge" ) );
#endif
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
#endif
			break;
		}
	case OBJ_BOSS:
		{
#if !_ANIMATIONS
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume( "PudgeShape" );
#else
			CCollisionVolume* volume = MeshManager::GetInstance()->GetCollisionVolume( "SludgesShip" );
#endif
			pObject->SetCollidableObject(volume);

			IComponent* compToAdd = CreateBossColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateBossAIComp( pObject );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );


#if !_ANIMATIONS
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "PudgeShape" ) );
#else
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "SludgesShip" ) );
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
			pObject->AddComponent(compToAdd);
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent(compToAdd);

#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif
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
	case OBJ_CRUSHERBASE:
		{
			// Created By: Andy Madruga
			CCollisionAABB volume;
			// Create a collision volume to be added to the Object.
			MeshManager::GetInstance()->LoadHazard( "CrusherBase", volume, d3dPosMat );
			volume.SetWorldPoint( vec3f(d3dPosMat._41, d3dPosMat._42, d3dPosMat._43) );
			pObject->SetCollidableObject( &volume );

			// Create a Render Component for the Crusher.
			IComponent* compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "CrusherBase" ) );
			// Add the Component to the Objects List.
			pObject->AddComponent( compToAdd );
			// Add the Component to the Component Managers.
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
#endif

			break;
		}
	case OBJ_DOOR:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "Door1", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);

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
#endif
			break;
		}
	case OBJ_PLUG:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionVolume* volume = new Box (*(AABB*)MeshManager::GetInstance()->GetCollisionVolume( "Plug" ));
			pObject->SetCollidableObject(volume);
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
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "GooPit", volume, d3dPosMat );
			pObject->SetCollidableObject(&volume);

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
	case OBJ_WALL_BREAKABLE:
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
			CCollisionAABB volume;
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
			CCollisionAABB volume;
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

			// Create a Render Component to be added
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "SpawnPipe" ) );
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

			break;
		}
	case OBJ_CORK:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionVolume* volume = new Box (*(AABB*)MeshManager::GetInstance()->GetCollisionVolume( "Cork" ));
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
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "Cork" ) );
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
	case OBJ_LAND_MESHEFFECT:
		{
			IComponent* compToAdd = CreateLandMeshEffectComp();
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

#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif
			break;
		}
	case OBJ_CHECKPOINT:
		{
			IComponent* compToAdd = CreateCheckPointComp( vPosition );
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

			break;
		}
	case OBJ_GOO_PARTICLE_EFFECT:
		{
#if _PARTICLES
			// Create a Emitter Comp using the GOO Explode as the default particle effect.
			IComponent* compToAdd = CreateEmitterComp( GOO_EXPLODE_FX );
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
			IComponent* compToAdd = CreateEmitterComp( CRUSHERSPARKS_FX );
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
			CComponent_Render* renderComp = (CComponent_Render*)pObject->GetComponent( ECOMP_RENDER );

			if( renderComp )
			{
				renderComp->GetRenderNode()->AddFlags( RE_FORWARD );
				renderComp->GetRenderNode()->Render = RenderEngine::DrawSmoothSkinnedMesh;
			}
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif
			break;
		}
	case OBJ_BOXBUTTON:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "Button1", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);

			IComponent* compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "Button" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateButtonColComp( vPosition );
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

#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif
		}
		break;
	case OBJ_DOORBUTTON:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "Button1", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);
			
			IComponent* compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "Button" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateButtonColComp( vPosition );
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
#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif
			break;
		}
	case OBJ_PIPE_BOX:
		{
			// Create a Collision Volume to be Added to the Object
			CCollisionAABB volume;
			MeshManager::GetInstance()->LoadHazard( "BoxSpawnPipe", volume, d3dPosMat );
			pObject->SetCollidableObject (&volume);

			IComponent* compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( "BoxSpawnPipe" ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateBoxPipeColComp( vPosition );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the
			// component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			break;
		}
	case OBJ_OBJECT_SELECTION:
		{
			IComponent* compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "SelectionMesh" ) );
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif
			break;
		}
	case OBJ_SPAWNPIPE_DRIP:
		{
			IComponent* compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNodeCopy( "SpawnPipeDrip" ) );
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

#if _ANIMATIONS
			compToAdd = CreateAnimationComp();
			pObject->AddComponent( compToAdd );
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );
#endif

			break;
		}
	case OBJ_WALLCHUNK:
		{
			std::string szMeshName;

			// Decide which mesh to use.
			switch( rand() % 6 )
			{
			case 0:
				szMeshName = "WallChunkShape1";
				break;
			case 1:
				szMeshName = "WallChunkShape2";
				break;
			case 2:
				szMeshName = "WallChunkShape3";
				break;
			case 3:
				szMeshName = "WallChunkShape4";
				break;
			case 4:
				szMeshName = "WallChunkShape5";
				break;
			case 5:
				szMeshName = "WallChunkShape6";
				break;
			}

			// Create a Collision Volume to be Added to the Object
			CCollisionAABB AABBvolume;
			MeshManager::GetInstance()->LoadHazard( szMeshName.c_str(), AABBvolume, d3dPosMat );
			CCollisionVolume* volume = new Box((AABB)AABBvolume);
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

			// Create a Render Component to be added
			compToAdd = CreateRenderComp( MeshManager::GetInstance()->GetRenderNode( szMeshName.c_str() ) );
			// add the component to the object's
			// component list
			pObject->AddComponent( compToAdd );
			// add the component to the component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			compToAdd = CreateFlasherComp(3.0f, true);

			pObject->AddComponent( compToAdd );
			// add the component to the component manager
			CObjectManager::GetInstance()->m_pComponentManager.AddComponent( compToAdd );

			break;
		}
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: InitObjectComponents()
// Purpose: Associates objects with components and passes them off to the Object Manager
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CObjectFactory::InitObjectComponents( IBaseObject* pObject, D3DXMATRIX d3dPosMat, IBaseObject* _partner )
{
	// Get the position vector from the matrix
	vec3f vPosition = vec3f( d3dPosMat._41, d3dPosMat._42, d3dPosMat._43 );
	pObject->SetLocalMat ((matrix4f*)&d3dPosMat);
	// adds specific components based on the object type
	// each component needs to be added individually for each case
	switch( pObject->GetType() )
	{
	case OBJ_PLAYER:
		{
#if !_ANIMATIONS
#else
			// Set the initial state of the animation to Idle
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION))->GetAnimStateMachine()->ChangeState( PudgeIdleState::GetState() );
#endif
			break;
		}
	case OBJ_PLAYER_ARM:
		{
			IBaseObject* player = CObjectManager::GetInstance()->GetPlayer();
			CComponent_PlayerArmCollision* pPlayerComp = (CComponent_PlayerArmCollision*)pObject->GetComponent( ECOMP_COLLISION );

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

			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( VacuumArmIdleState::GetState() );
#endif
			break;
		}
	case OBJ_GOO:
		{

#if _ANIMATIONS
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( LilGooWalkState::GetState() );
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->Update( 0.0f );
#endif
			break;
		}
	case OBJ_MONSTER:
		{
#if _ANIMATIONS 
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SlimeMonsterIdleState::GetState() );
#endif
			break;
		}
	case OBJ_HULK:
		{
			break;
		}
	case OBJ_SLUDGE:
		{
#if _ANIMATIONS
			// Set the initial state of the animation to Idle
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgeIdleState::GetState() );
#endif
			break;
		}
	case OBJ_BOSS:
		{
#if _ANIMATIONS
			// Set the initial state of the animation to Idle
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SludgesShipIdleState::GetState() );
#endif
			break;
		}
	case OBJ_CRUSHER:
		{
			break;
		}
	case OBJ_CRUSHERBASE:
		{
			break;
		}
	case OBJ_BRIDGE:
		{
#if _ANIMATIONS 
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( BridgeIdleState::GetState() );
#endif
			break;
		}
	case OBJ_DOOR:
		{
#if _ANIMATIONS
			// Set the Animation State
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( DoorIdleState::GetState() );
#endif
			break;
		}
	case OBJ_PLUG:
		{
			break;
		}
	case OBJ_OUTLET:
		{
			break;
		}
	case OBJ_PIT:
		{
			break;
		}
	case OBJ_WALL_BREAKABLE:
		{
			break;
		}
	case OBJ_PIPE_STEAM:
		{
			break;
		}
	case OBJ_PIPE_INTAKE:
		{
			break;
		}
	case OBJ_PIPE_SPAWN:
		{
			break;
		}
	case OBJ_CRATE:
		{
			pObject->SetLocalMat( (matrix4f*)&d3dPosMat );

			break;
		}
	case OBJ_CORK:
		{
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
			break;
		}
	case OBJ_LAND_MESHEFFECT:
		{
#if _ANIMATIONS
			// Set the initial state of the animation to Idle
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( LandIdleEffectState::GetState() );
#endif
			break;
		}
	case OBJ_CHECKPOINT:
		{
			((CComponent_Checkpoint*)pObject->GetComponent( ECOMP_CHECKPOINT ))->BuildAABB ();

			break;
		}
	case OBJ_GOO_PARTICLE_EFFECT:
		{
			break;
		}
	case OBJ_CRUSHER_PARTICLE_EFFECT:
		{
			break;
		}
	case OBJ_PLAYER_DEATH_EFFECT:
		{
#if _ANIMATIONS
			// Set the initial state of the animation to Idle
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( DeathCloudPudgeDeathState::GetState() );
#endif
			break;
		}
	case OBJ_BOXBUTTON:
		{
#if _ANIMATIONS

			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( ButtonIdleState::GetState() );
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->Update( 0.0f );
#endif
		}
		break;
	case OBJ_DOORBUTTON:
		{			
#if _ANIMATIONS
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( ButtonIdleState::GetState() );
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->Update( 0.0f );
#endif
			break;
		}
	case OBJ_PIPE_BOX:
		{
			break;
		}
	case OBJ_OBJECT_SELECTION:
		{
#if _ANIMATIONS
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SelectionMeshSpawnState::GetState() );
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->Update( 0.0f );
#endif
			break;
		}
	case OBJ_SPAWNPIPE_DRIP:
		{
#if _ANIMATIONS
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->GetAnimStateMachine()->ChangeState( SpawnPipeDripState::GetState() );
			((CComponent_Animation*)pObject->GetComponent( ECOMP_ANIMATION ))->Update( 0.0f );
#endif

			break;
		}
	case OBJ_WALLCHUNK:
		{
			pObject->SetLocalMat( (matrix4f*)&d3dPosMat );
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
			//pComp->Shutdown();
			CObjectManager::GetInstance()->m_pComponentManager.RemoveComponent( pComp );
		}
	}
	// release the object back to the inventory
	pObject->Release();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

#pragma region COMPONENT CREATION METHODS
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
			m_cEmitterComponentInventory[cindex].SetType(nType);
			return &m_cEmitterComponentInventory[cindex];
		}
	}
	return nullptr;
}

#pragma region EFFECT COMPONENT CREATION METHODS

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
IComponent* CObjectFactory::CreateLandMeshEffectComp( void )
{
	// Find the first available component
	if( m_cLandMeshEffectComponentInventory.GetParent() == nullptr )
	{
		return &m_cLandMeshEffectComponentInventory;
	}
	return nullptr;
}
#pragma endregion

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
IComponent* CObjectFactory::CreateCheckPointComp( vec3f vPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_CHECKPOINT_COMPONENTS; ++cindex )
	{
		// Find the first available component
		if( m_cCheckPointComponentInventory[cindex].GetParent() == nullptr )
		{
			m_cCheckPointComponentInventory[cindex].SetPosition ( vPosition );
			return &m_cCheckPointComponentInventory[cindex];
		}
	}
	return nullptr;
}

IComponent* CObjectFactory::CreateFlasherComp( float time, bool _active )
{
	// Find the first available component
	for( unsigned int cindex = 0; cindex < MAX_FLASHER_COMPONENTS; ++cindex )
	{
		// If the component doesn't have a parent
		// it's available for use
		if( m_cFlasherComponentInventory[cindex].GetParent() == nullptr )
		{
			m_cFlasherComponentInventory[cindex].SetTimeLeft( time );
			m_cFlasherComponentInventory[cindex].SetActive( _active );
			return &m_cFlasherComponentInventory[cindex];
		}
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
		// Set the position
		m_cPlayerCollisionComponent.GetPos() = fPosition;
		// Set the collision type
		m_cPlayerCollisionComponent.SetCollType( CCMP_PLAYER );
		return &m_cPlayerCollisionComponent;
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateBossColComp()
// Purpose: Finds the first available Player Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Rueben Massey
// Creation Date: 5/25/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateBossColComp( vec3f fPosition )
{		
	// If the component doesn't have a parent
	// it's available for use
	if( nullptr == m_cBossCollisionComponent.GetParent() )
	{
		// Set the position
		m_cBossCollisionComponent.GetPos() = fPosition;
		// Set the collision type
		m_cBossCollisionComponent.SetCollType( CCMP_BOSS );
		return &m_cBossCollisionComponent;
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
			// Set the position
			m_cGooCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cGooCollisionComponentInventory[cindex].SetCollType( CCMP_GOO );
			// Give the component a physics object
			m_cGooCollisionComponentInventory[cindex].SetPhysicsBox( tPhysicsObject );
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
IComponent* CObjectFactory::CreateGooAIComp ( IBaseObject* _pObject )
{
	// Input Safety Check
	for( unsigned int cindex = 0; cindex < MAX_AI_COMPONENTS; ++cindex )
	{
		if( nullptr ==  m_cGooAIComponentInventory[cindex].GetParent() )
		{
			// Set the AI type
			//m_cGooAIComponentInventory[cindex].SetAIType( AI_LIL_GOO );

			 m_cGooAIComponentInventory[cindex].AddAI(AI_MOVE, new CLilGooMoveAI(_pObject));
			 m_cGooAIComponentInventory[cindex].AddAI(AI_IDLE, new CLilGooIdleAI(_pObject));
			 m_cGooAIComponentInventory[cindex].AddAI(AI_DEAD, new CLilGooDeadAI(_pObject));

			 //m_cGooAIComponentInventory[cindex].InitLoadedAI();
			 m_cGooAIComponentInventory[cindex].SwitchAI(AI_IDLE);

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
// Last Modification By: Josh Morgan
// Last Modification Date: 8/15/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateSlimeAIComp(IBaseObject* _pObject)
{
	// Input Safety Check
	for( unsigned int cindex = 0; cindex < MAX_AI_COMPONENTS; ++cindex )
	{
		if( nullptr ==  m_cSlimeAIComponentInventory[cindex].GetParent() )
		{
			// Set the AI type
			m_cSlimeAIComponentInventory[cindex].AddAI(AI_ATTACK, new CSlimeMonsterAttackAI(_pObject));
			m_cSlimeAIComponentInventory[cindex].AddAI(AI_IDLE, new CSlimeMonsterIdleAI(_pObject));
			m_cSlimeAIComponentInventory[cindex].AddAI(AI_DEAD, new CSlimeMonsterDeadAI(_pObject));
			m_cSlimeAIComponentInventory[cindex].AddAI(AI_HIDE, new CSlimeMonsterHideAI(_pObject));
			
			m_cSlimeAIComponentInventory[cindex].SwitchAI(AI_HIDE);
			
			return &m_cSlimeAIComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateBossAIComp()
// Purpose: Finds the first available Goo Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Josh Fields
// Creation Date:	6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateBossAIComp ( IBaseObject* _pObject  )
{
	// Input Safety Check
	//for( unsigned int cindex = 0; cindex < MAX_AI_COMPONENTS; ++cindex )
	{
		if( nullptr ==  m_cBossAIComponent.GetParent() )
		{
			m_cBossAIComponent.SetParent( _pObject );
			//m_cBossAIComponent.Initialize();
			// Set the AI type
			//m_cGooAIComponentInventory[cindex].SetAIType ( AI_LIL_GOO );
			
			m_cBossAIComponent.AddAI (AI_IDLE, new BossIdleAI(_pObject));
			m_cBossAIComponent.AddAI (AI_ATTACK, new BossAttackAI(_pObject));
			m_cBossAIComponent.AddAI (AI_MOVE, new BossMoveAI(_pObject));

			//m_cBossAIComponent.InitLoadedAI();
			m_cBossAIComponent.SwitchAI (AI_IDLE);
			
			return &m_cBossAIComponent;
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
			// Set the position
			m_cSteamPipeCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cSteamPipeCollisionComponentInventory[cindex].SetCollType( CCMP_STEAMPIPE );
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
// Purpose: Finds the first available Spawn Pipe Collision component in
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
// Function Name: CreateBoxPipeColComp()
// Purpose: Finds the first available Box Pipe Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Josh Fields
// Creation Date: 6/27/2012
// Last Modification By: Josh Morgan
// Last Modification Date: 7/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateBoxPipeColComp( vec3f fPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_BOXPIPE; ++cindex )
	{
		if( nullptr ==  m_cBoxPipeCollisionComponentInventory[cindex].GetParent() )
		{
			// Set the position
			m_cBoxPipeCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cBoxPipeCollisionComponentInventory[cindex].SetCollType( CCMP_BOXPIPE );
			return &m_cBoxPipeCollisionComponentInventory[cindex];
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CreateButtonColComp()
// Purpose: Finds the first available Button Collision component in
//			the inventory, initializes it, and returns a pointer
//			to it.
// Original Author: Josh Morgan
// Creation Date: 7/8/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IComponent* CObjectFactory::CreateButtonColComp( vec3f fPosition )
{
	for( unsigned int cindex = 0; cindex < MAX_COLLISION_COMPONENTS_BUTTONS; ++cindex )
	{
		if( nullptr ==  m_cButtonCollisionComponentInventory[cindex].GetParent() )
		{
			// Set the position
			m_cButtonCollisionComponentInventory[cindex].GetPos() = fPosition;
			// Set the collision type
			m_cButtonCollisionComponentInventory[cindex].SetCollType( CCMP_BUTTON );
			return &m_cButtonCollisionComponentInventory[cindex];
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
IComponent* CObjectFactory::CreateSoundComp( void )
{
	for( unsigned int cindex = 0; cindex < MAX_SOUND_COMPONENTS; ++cindex )
	{
		if( nullptr ==  m_cSoundComponentInventory[cindex].GetParent() )
		{
			return &m_cSoundComponentInventory[cindex];
		}
	}
	return nullptr;
}
#pragma endregion
#pragma endregion