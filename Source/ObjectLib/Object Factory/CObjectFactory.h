/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CObjectFactory
//
// Purpose: Allocate memory and manage it for all objects
//			and components
//
// Original Author: Rueben Massey
//
// Creation Date: 5/7/2012
//
// Last Modification By: RCM
// Last Modification Date: 6/4/2012
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef COBJECTFACTORY_H_
#define COBJECTFACTORY_H_

// Includes

// Object
#include "..//Objects/IBaseObject.h"
// RenderComponent
#include "../../ComponentManager/CComponent_Render.h"
// AIComponent
#include "../../ComponentManager/CComponent_AI.h"
#include "../../ComponentManager/CComponent_Lil_Goo_AI.h"
#include "../../ComponentManager/CComponent_Slime_Monster_AI.h"
// InputComponent
#include "..//..//ComponentManager/CComponent_Input.h"
// AnimationComponent
#include "../../ComponentManager/CComponent_Animation.h"
// ItemComponent
#include "../../ComponentManager/CComponent_Item.h"
// SpawnComponent
// EmitterComponent
#include "../../ComponentManager/Component_Emitter.h"
// HealthComponent
#include "..//..//ComponentManager/CComponent_Health.h"
// CollisionComponents
#include "../../ComponentManager/CComponent_Collision.h"
#include "../../ComponentManager/CComponent_PlayerCollision.h"
#include "../../ComponentManager/CComponent_BossCollision.h"
#include "../../ComponentManager/CComponent_LittleGooCollision.h"
#include "../../ComponentManager/CComponent_SlimeMonsterCollision.h"
#include "../../ComponentManager/CComponent_CrateCollision.h"
#include "../../ComponentManager/CComponent_CorkCollision.h"
#include "../../ComponentManager/CComponent_GooPitCollision.h"
#include "../../ComponentManager/CComponent_BreakableWallCollision.h"
#include "../../ComponentManager/CComponent_SteamPipeCollision.h"
#include "../../ComponentManager/CComponent_IntakePipeCollision.h"
#include "../../ComponentManager/CComponent_SpawnPipeCollision.h"
#include "../../ComponentManager/CComponent_PlayerArmCollision.h"
#include "../../ComponentManager/CComponent_CrusherCollision.h"
#include "../../ComponentManager/CComponent_BridgeCollision.h"
#include "../../ComponentManager/CComponent_OutletCollision.h"
#include "../../ComponentManager/CComponent_PlugCollision.h"
#include "../../ComponentManager/CComponent_WaterPoolCollision.h"
#include "../../ComponentManager/CComponent_DoorCollision.h"
#include "../../ComponentManager/CComponent_BoxPipeCollision.h"
#include "../../ComponentManager/CComponent_ButtonCollision.h"
// SoundComponent
#include "../../ComponentManager/CComponent_Sound.h"
// MeshEffectComponent
#include "../../ComponentManager/CComponent_LandMeshEffect.h"
#include "../../ComponentManager/CComponent_Flasher.h"

//CheckPointComponent
#include "../../ComponentManager/CComponent_Checkpoint.h"

class State;

#define MAX_OBJECTS								500
#define MAX_COLLISION_COMPONENTS				50
#define MAX_COLLISION_COMPONENTS_PLAYER			1
#define MAX_COLLISION_COMPONENTS_GOO			100
#define MAX_COLLISION_COMPONENTS_SLIME			100
#define MAX_COLLISION_COMPONENTS_CRATE			100
#define MAX_COLLISION_COMPONENTS_CORK			50
#define MAX_COLLISION_COMPONENTS_GOOPIT			100
#define MAX_COLLISION_COMPONENTS_BREAKWALL		20
#define MAX_COLLISION_COMPONENTS_STEAMPIPE		20
#define MAX_COLLISION_COMPONENTS_INTAKEPIPE		20
#define MAX_COLLISION_COMPONENTS_SPAWNPIPE		20
#define MAX_COLLISION_COMPONENTS_CRUSHER		20
#define MAX_COLLISION_COMPONENTS_BRIDGE			20
#define MAX_COLLISION_COMPONENTS_OUTLET			20
#define MAX_COLLISION_COMPONENTS_PLUG			20
#define MAX_COLLISION_COMPONENTS_POOL			50

#define MAX_COLLISION_COMPONENTS_DOOR			20
#define MAX_COLLISION_COMPONENTS_BUTTONS		50
#define MAX_COLLISION_COMPONENTS_BOXPIPE		20

#define MAX_SOUND_COMPONENTS					400
#define	MAX_AI_COMPONENTS						100
#define MAX_RENDER_COMPONENTS					200
#define MAX_INPUT_COMPONENTS					1
#define MAX_ANIMATION_COMPONENTS				200
#define MAX_ITEM_COMPONENTS						150
#define MAX_EMITTER_COMPONENTS					50
#define MAX_LANDMESHEFFECT_COMPONENTS			1
#define MAX_FLASHER_COMPONENTS					20
#define MAX_HEALTH_COMPONENTS					100
#define VEL_CONSTRAINT							30.0f
#define MAX_CHECKPOINT_COMPONENTS				5

__declspec(align(32))
class CObjectFactory
{
private:

	// Arrays needed:
	// Object Inventory Array
	IBaseObject m_cObjectInventory[MAX_OBJECTS];
	short m_sAvailableObjectIndex;

	// RenderComponent Inventory Array
	CComponent_Render m_cRenderComponentInventory[MAX_RENDER_COMPONENTS];

	// CollisionComponent Inventory Arrays
	CComponent_PlayerCollision			m_cPlayerCollisionComponent;
	CComponent_BossCollision			m_cBossCollisionComponent;

	CComponent_LittleGooCollision		m_cGooCollisionComponentInventory[MAX_COLLISION_COMPONENTS_GOO];
	CComponent_SlimeMonsterCollision	m_cSlimeCollisionComponentInventory[MAX_COLLISION_COMPONENTS_SLIME];
	CComponent_CrateCollision			m_cCrateCollisionComponentInventory[MAX_COLLISION_COMPONENTS_CRATE];
	CComponent_CorkCollision			m_cCorkCollisionComponentInventory[MAX_COLLISION_COMPONENTS_CORK];
	CComponent_GooPitCollision			m_cGooPitCollisionComponentInventory[MAX_COLLISION_COMPONENTS_GOOPIT];
	CComponent_BreakableWallCollision	m_cBreakableWallCollisionComponentInventory[MAX_COLLISION_COMPONENTS_BREAKWALL];
	CComponent_SteamPipeCollision		m_cSteamPipeCollisionComponentInventory[MAX_COLLISION_COMPONENTS_STEAMPIPE];
	CComponent_IntakePipeCollision		m_cIntakePipeCollisionComponentInventory[MAX_COLLISION_COMPONENTS_INTAKEPIPE];
	CComponent_SpawnPipeCollision		m_cSpawnPipeCollisionComponentInventory[MAX_COLLISION_COMPONENTS_SPAWNPIPE];
	CComponent_BridgeCollision			m_cBridgeCollisionComponentInventory[MAX_COLLISION_COMPONENTS_BRIDGE];
	CComponent_OutletCollision			m_cOutletCollisionComponentInventory[MAX_COLLISION_COMPONENTS_OUTLET];
	CComponent_PlugCollision			m_cPlugCollisionComponentInventory[MAX_COLLISION_COMPONENTS_PLUG];
	CComponent_PlayerArmCollision		m_cPlayerArmCollisionComponent;
	CComponent_CrusherCollision			m_cCrusherCollisionInventory[MAX_COLLISION_COMPONENTS_CRUSHER];
	CComponent_WaterPoolCollision		m_cPoolCollisionComponentInventory[MAX_COLLISION_COMPONENTS_POOL];

	CComponent_DoorCollision			m_cDoorCollisionComponentInventory[MAX_COLLISION_COMPONENTS_DOOR];
	CComponent_ButtonCollision			m_cButtonCollisionComponentInventory[MAX_COLLISION_COMPONENTS_BUTTONS];
	CComponent_BoxPipeCollision			m_cBoxPipeCollisionComponentInventory[MAX_COLLISION_COMPONENTS_BOXPIPE];

	// Sound Component inventory array
	CComponent_Sound m_cSoundComponentInventory[MAX_SOUND_COMPONENTS];

	// AIComponent Inventory Array
	CComponent_AI			m_cGooAIComponentInventory[MAX_AI_COMPONENTS];
	CComponent_AI			m_cSlimeAIComponentInventory[MAX_AI_COMPONENTS];
	CComponent_AI			m_cBossAIComponent;

	// InputComponent Inventory Array
	CComponent_Input m_cInputComponentInventory;
#if _ANIMATIONS
	// AnimationComponent Inventory Array
	CComponent_Animation m_cAnimationComponentInventory[MAX_ANIMATION_COMPONENTS];
#endif

	// ItemComponent Inventory Array
	CComponent_Item m_cItemComponentInventory[MAX_ITEM_COMPONENTS];

	// SpawnComponent Inventory Array
	//CComponent_Spawn m_cSpawnComponentInventory[MAX_COMPONENTS]

	// MeshEffectComponent Array
	CComponent_LandMeshEffect m_cLandMeshEffectComponentInventory;

	// EmitterComponent Array
	CComponent_Emitter m_cEmitterComponentInventory[MAX_EMITTER_COMPONENTS];

	// HealthComponent Array
	CComponent_Health m_cHealthComponentInventory[MAX_HEALTH_COMPONENTS];

	//CheckPointComponent Array
	CComponent_Checkpoint m_cCheckPointComponentInventory[MAX_CHECKPOINT_COMPONENTS];
	CComponent_Flasher m_cFlasherComponentInventory[MAX_FLASHER_COMPONENTS];

#pragma region COMPONENT CREATION METHODS
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: AddObjectComponents()
	// Purpose: Associates objects with components and passes them off to the Object Manager
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddObjectComponents( IBaseObject* pObject, D3DXMATRIX d3dPosMat );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: InitObjectComponents()
	// Purpose: Associates objects with components and passes them off to the Object Manager
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void InitObjectComponents( IBaseObject* pObject, D3DXMATRIX d3dPosMat, IBaseObject* _partner = nullptr );

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
	IComponent* CreateRenderComp( RenderNode* pRenderNode );

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
	IComponent* CreateAIComp( void );

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
	IComponent* CreateGooAIComp( IBaseObject* _pObject );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateAIComp()
	// Purpose: Finds the first available AI component in
	//			the inventory, initializes it, and returns a pointer
	//			to it.
	// Original Author: Josh Fields
	// Creation Date: 6/4/2012
	// Last Modification By: Josh Morgan
	// Last Modification Date: 8/15/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateSlimeAIComp(IBaseObject* _pObject);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateBossAIComp()
	// Purpose: Finds the first available AI component in
	//			the inventory, initializes it, and returns a pointer
	//			to it.
	// Original Author: Josh Fields
	// Creation Date: 6/4/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateBossAIComp( IBaseObject* _pObject );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateInputComp()
	// Purpose: Initializes the input component and returns a pointer
	//			to it.
	// Original Author: Rueben Massey
	// Creation Date: 5/25/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateInputComp( void );

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
	IComponent* CreateAnimationComp( void );
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
	IComponent* CreateItemComp( void );

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
	IComponent* CreateSpawnComp( void );

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
	IComponent* CreateEmitterComp( int nType );	

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
	IComponent* CreateLandMeshEffectComp( void );
	
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
	IComponent* CreateHealthComp( void );

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
	IComponent* CreatePlayerColComp( vec3f fPosition );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateBossColComp()
	// Purpose: Finds the first available Boss Collision component in
	//			the inventory, initializes it, and returns a pointer
	//			to it.
	// Original Author: Josh Fields
	// Creation Date: 5/25/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateBossColComp( vec3f fPosition );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateGooColComp()
	// Purpose: Finds the first available Player Collision component in
	//			the inventory, initializes it, and returns a pointer
	//			to it.
	// Original Author: Rueben Massey
	// Creation Date: 5/25/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateGooColComp( vec3f fPosition, tPhyObject* tPhysicsObject );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateGooColComp()
	// Purpose: Finds the first available Player Collision component in
	//			the inventory, initializes it, and returns a pointer
	//			to it.
	// Original Author: Rueben Massey
	// Creation Date: 5/25/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateSlimeColComp( vec3f fPosition, tPhyObject* tPhysicsObject );

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
	IComponent* CreateCrateColComp( vec3f fPosition, tPhyObject* tPhysicsObject );

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
	IComponent* CreateCorkColComp( vec3f fPosition, tPhyObject* tPhysicsObject );

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
	IComponent* CreateGooPitColComp( vec3f fPosition );

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
	IComponent* CreateWaterPoolColComp( vec3f fPosition );

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
	IComponent* CreateBreakableWallColComp( vec3f fPosition );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateSteamPipeColComp()
	// Purpose: Finds the first available SteamPipe Collision component in
	//			the inventory, initializes it, and returns a pointer
	//			to it.
	// Original Author: Rueben Massey
	// Creation Date: 5/25/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateSteamPipeColComp( vec3f fPosition );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateSteamPipeColComp()
	// Purpose: Finds the first available Intake Collision component in
	//			the inventory, initializes it, and returns a pointer
	//			to it.
	// Original Author: Rueben Massey
	// Creation Date: 5/25/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateIntakePipeColComp( vec3f fPosition );

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
	IComponent* CreateSpawnPipeColComp( vec3f fPosition );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateBoxPipeColComp()
	// Purpose: Finds the first available Box Pipe Collision component in
	//			the inventory, initializes it, and returns a pointer
	//			to it.
	// Original Author: Josh Fields
	// Creation Date: 6/27/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateBoxPipeColComp( vec3f fPosition );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Function Name: CreateBoxPipeButtonColComp()
	//// Purpose: Finds the first available Box Pipe Button Collision component in
	////			the inventory, initializes it, and returns a pointer
	////			to it.
	//// Original Author: Josh Fields
	//// Creation Date: 6/27/2012
	//// Last Modification By: 
	//// Last Modification Date: 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//IComponent* CreateBoxPipeButtonColComp( vec3f fPosition );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Function Name: CreateDoorButtonColComp()
	//// Purpose: Finds the first available Door Button Collision component in
	////			the inventory, initializes it, and returns a pointer
	////			to it.
	//// Original Author: Josh Fields
	//// Creation Date: 6/27/2012
	//// Last Modification By: 
	//// Last Modification Date: 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//IComponent* CreateDoorButtonColComp( vec3f fPosition );

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
	IComponent* CreateButtonColComp( vec3f fPosition );

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
	IComponent* CreateBridgeColComp( vec3f fPosition );

	// Function Name: CreateDoorColComp()
	// Purpose: Finds the first available Door Collision component in
	//			the inventory, initializes it, and returns a pointer
	//			to it.
	// Original Author: Rueben Massey
	// Creation Date: 6/15/2012
	// Last Modification By: 
	// Last Modification Date: 
	// TODO
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateDoorColComp( vec3f fPosition );

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
	IComponent* CreateOutletColComp( vec3f fPosition );

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
	IComponent* CreatePlugColComp( vec3f fPosition, tPhyObject* tPhysicsObject );

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
	IComponent* CreatePlayerArmColComp( vec3f fPosition );

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
	IComponent* CreateCrusherCollisionComp( vec3f fPosition );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CreateSoundComp():	Finds the first available BreakableWall Collision component in
	//						the inventory, initializes it, and returns a pointer
	//						to it.
	// Creation Date:		5/31/12
	// Mod Name:			JM
	// Mod Date:			5/31/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IComponent* CreateSoundComp( void );

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
	IComponent* CreateCheckPointComp( vec3f vPosition );

	IComponent* CreateFlasherComp( float time, bool _active );

#pragma endregion

public:

	// Default Constructory
	CObjectFactory();

	// Destructory
	~CObjectFactory();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Init()
	// Purpose: Initializes all object and component memory pools.
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Init( void );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateObject()
	// Purpose: Associates objects with components and passes them off to the Object Manager
	// Original Author: Ethan Pendergraft
	// Creation Date: 5/15/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IBaseObject* CreateObject( eOBJECT_TYPE eType, D3DXMATRIX d3dPosMat, IBaseObject* _partner = nullptr );

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: CreateAnimatedBGObject
	// Purpose: Adds the animated background object to the ObjectManager and adds a reference to that object
	// Original Author: Rueben Massey
	// Creation Date: 7/21/2012
	// Last Modification By:
	// Last Modification Date:
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	IBaseObject* CreateAnimatedBGObject ( eOBJECT_TYPE eType, D3DXMATRIX _ObjectMatrix, const char* _szRenderNode, State* _AnimationState );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: ReleaseObject()
	// Purpose: Add object back to inventory and release components
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ReleaseObject( IBaseObject* pObject );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: ReleaseComponent()
	// Purpose: Not currently applicable. DO NOT USE
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//void ReleaseComponent( IBaseObject* pObject, IComponent* pComponent );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Shutdown()
	// Purpose: FREE THE MEMORY
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown( void );	
};




#endif// COBJECTFACTORY_H_