#define _BROKEN_AUDIO
#include "../PS/config.h"
#include "CLevelManager.h"
#include "../Utilities/MathLib/vec3.h"
#include "../PS/States/CLoadingState.h"
#include "../PS/States/GamePlayState.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../RenderEngine/MeshManager.h"
#include "../RenderEngine/LightSystem/LightManager.h"
#include "../RenderEngine/TextureManager.h"
#include "../PS/States/CSplashScreenState.h"
#include "../AnimationLib/AnimationManager.h"

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

LevelManager* LevelManager::m_pInstance = NULL;

LevelManager::LevelManager ()
{
	m_vLevels.clear();
	m_nCurLevelIndex = 0;
	
	m_bLoadNext = false;
	m_bUnloadFirstLevel = false;

	for( unsigned int i = 0; i < MAX_ACTIVE_LEVELS; i++)
	{
		m_pActiveLevels[i] = NULL;
	}
}

LevelManager::~LevelManager ()
{

}

LevelManager* LevelManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new LevelManager();
	}
	return m_pInstance;
}

void LevelManager::DeleteInstance ()
{
	delete m_pInstance;
	m_pInstance = NULL;
}

void LevelManager::Init (char* _szLevelDataFileName)
{
	m_nCurLevelIndex = 0;
	m_nCurLevelToLoadIndex = 0;

	std::string LevelPath = "./Resources/LevelData/";
	LevelPath += _szLevelDataFileName;
	LevelPath += ".InfiniteHPLevelData";

	fstream LevelData;
	LevelData.open( LevelPath, ios_base::in | ios_base::binary );

	if (!LevelData.is_open())
	{
		LevelData.close();
		return;
	}

	int NumLevels = 0;

	LevelData.read ((char*)&NumLevels, sizeof(NumLevels));

	for (int LevelIndex = 0; LevelIndex < NumLevels; ++LevelIndex)
	{
		CLevel lvl;

		// Set the Level ID to the index.
		lvl.SetLevelID( LevelIndex );

		int NameLength = 0;
		LevelData.read( (char*)&NameLength, sizeof(NameLength) );

		char* LevelName = NULL;

		if (NameLength > 0)
		{
			LevelName = new char[NameLength + 1];
			LevelData.read( LevelName, sizeof(LevelName[0]) * NameLength );
			LevelName[NameLength] = '\0';
			lvl.SetName ( LevelName );
			delete LevelName;
		}

		D3DXMATRIX _worldMatrix;
		D3DXMatrixIdentity (&_worldMatrix);

		double CheckPosX, CheckPosY, CheckPosZ;
		LevelData.read( (char*)&CheckPosX, sizeof(CheckPosX) );
		LevelData.read( (char*)&CheckPosY, sizeof(CheckPosY) );
		LevelData.read( (char*)&CheckPosZ, sizeof(CheckPosZ) );

		lvl.SetCheckPointPos ( vec3f ((float)CheckPosX, (float)CheckPosY, (float)CheckPosZ) );

		int NextNameLength = 0;
		LevelData.read( (char*)&NextNameLength, sizeof(NextNameLength) );

		char* NextLevel = NULL;

		if (NameLength > 0)
		{
			NextLevel = new char[NextNameLength + 1];
			LevelData.read( NextLevel, sizeof(NextLevel[0]) * NextNameLength );
			NextLevel[NextNameLength] = '\0';
			lvl.SetNextLevel ( NextLevel );
			delete NextLevel;
		}

		float MaxX, MaxY, MaxZ;
		LevelData.read( (char*)&MaxX, sizeof(MaxX) );
		LevelData.read( (char*)&MaxY, sizeof(MaxY) );
		LevelData.read( (char*)&MaxZ, sizeof(MaxZ) );

		float MinX, MinY, MinZ;
		LevelData.read( (char*)&MinX, sizeof(MinX) );
		LevelData.read( (char*)&MinY, sizeof(MinY) );
		LevelData.read( (char*)&MinZ, sizeof(MinZ) );

		lvl.SetKdTreeMax ( vec3f (MaxX, MaxY, MaxZ) );
		lvl.SetKdTreeMin ( vec3f (MinX, MinY, MinZ) );

		m_vLevels.push_back (lvl);
	}

	LevelData.close();
}


void LevelManager::Shutdown ()
{
	m_bUnloadFirstLevel = true;
	UnloadFirstActiveLevel();

	m_bUnloadFirstLevel = true;
	UnloadFirstActiveLevel();

	m_bUnloadFirstLevel = true;
	UnloadFirstActiveLevel();
}

void LevelManager::LoadLevel ()
{
	//m_nCurLevelToLoadIndex = 4;
	if (m_nCurLevelToLoadIndex >= (int)m_vLevels.size())
	{
		return;
	}

	CLevel* _lvlNext = &m_vLevels[m_nCurLevelToLoadIndex];

	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	// Load up the Collision for this level.
	std::string BoundingPath = "./Resources/CollisionData/";
	BoundingPath += _lvlNext->GetName();
	BoundingPath += "_BoundingVolumes.InfiniteHPCollision";

	MeshManager::GetInstance()->LoadCollisionEnvironment(BoundingPath.c_str(), _lvlNext->GetLevelID() );


	std::string TrapPath = "./Resources/LevelData/TrapData/";
	TrapPath += _lvlNext->GetName();
	TrapPath += "_Trap.InfiniteHPHazards";

	std::string TrapDataPath = "./Resources/LevelData/TrapData/";
	TrapDataPath += _lvlNext->GetName();
	TrapDataPath += "_Trap.InfiniteHPHazardData";

	std::string ObjPath = "./Resources/LevelData/ObjectData/";
	ObjPath += _lvlNext->GetName();
	ObjPath += "_MoveableObject.InfiniteHPMoveableObjects";

	std::string ObjDataPath = "./Resources/LevelData/ObjectData/";
	ObjDataPath += _lvlNext->GetName();
	ObjDataPath += "_MoveableObject.InfiniteHPMoveableObjectsData";

	/*std::string LightPath = "./Resources/LevelData/";
	LightPath += _lvlNext->GetName();
	LightPath += ".InfiniteHPLightsData";
	_lvlNext->GetLightManager().LoadLights( LightPath.c_str() );*/

	MeshManager::GetInstance()->LoadEnvironmentTraps(TrapPath.c_str(), TrapDataPath.c_str(), _lvlNext->GetLevelID());

	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	MeshManager::GetInstance()->LoadEnvironmentMoveableObjects(ObjPath.c_str(), ObjDataPath.c_str(), _lvlNext->GetLevelID());

	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	// Load up a checkpoint if its the level has one.
	// If it is at the origin then the checkpoint is ignored.

	D3DXMATRIX _worldMatrix;
	D3DXMatrixIdentity (&_worldMatrix);

	if( _lvlNext->GetCheckPointPos().x != 0.0f || 
		_lvlNext->GetCheckPointPos().y != 0.0f ||
		_lvlNext->GetCheckPointPos().z != 0.0f )
	{

		_worldMatrix._41 = (float)_lvlNext->GetCheckPointPos().x;
		_worldMatrix._42 = (float)_lvlNext->GetCheckPointPos().y;
		_worldMatrix._43 = (float)_lvlNext->GetCheckPointPos().z;

		IBaseObject* CheckPointObject = CObjectManager::GetInstance()->CreateObject( OBJ_CHECKPOINT, _worldMatrix, _lvlNext->GetLevelID() );
	}

	if (m_nCurLevelToLoadIndex == m_vLevels.size ()-1)
	{
		// spawn a boss
		D3DXMatrixIdentity (&_worldMatrix);

		IBaseObject* Sludge = CObjectManager::GetInstance()->CreateObject( OBJ_SLUDGE, _worldMatrix, _lvlNext->GetLevelID() );

		D3DXMatrixIdentity (&_worldMatrix);

		_worldMatrix._41 = 33500.0f;
		_worldMatrix._42 = 2000.0f;
		_worldMatrix._43 = 0.0f;

		IBaseObject* Boss = CObjectManager::GetInstance()->CreateObject( OBJ_BOSS, _worldMatrix, _lvlNext->GetLevelID() );

		Boss->GetFrame().AddChild( (Node*)&Sludge->GetFrame() );
	}

	for( unsigned int i = 0; i < MAX_ACTIVE_LEVELS; i++)
	{
		// Set the first available level pointer to this level.
		if( NULL == m_pActiveLevels[i] )
		{
			m_pActiveLevels[i] = _lvlNext;
			break;
		}
	}

	if( m_pActiveLevels[0] )
		m_nCurLevelIndex = m_pActiveLevels[0]->GetLevelID();

	m_nCurLevelToLoadIndex++;
}

void LevelManager::LoadNextLevel ()
{
	if (!m_bLoadNext)
	{
		return;
	}

	m_bLoadNext = false;

	if ( m_nCurLevelToLoadIndex >= (int)m_vLevels.size () )
	{
		//CGamePlayState::GetInstance()->WinGameFade();
		return;
	}
	// make sure we are ready to switch levels
	// if there is no next level to load go to win game state

	CLevel* _lvlNext = &m_vLevels[m_nCurLevelToLoadIndex];

	// Load up the Collision for this level.
	std::string BoundingPath = "./Resources/CollisionData/";
	BoundingPath += _lvlNext->GetName();
	BoundingPath += "_BoundingVolumes.InfiniteHPCollision";

	MeshManager::GetInstance()->LoadCollisionEnvironment(BoundingPath.c_str(), _lvlNext->GetLevelID() );

	// Load up all the objects for this level.
	std::string TrapPath = "./Resources/LevelData/TrapData/";
	TrapPath += _lvlNext->GetName();
	TrapPath += "_Trap.InfiniteHPHazards";

	std::string TrapDataPath = "./Resources/LevelData/TrapData/";
	TrapDataPath += _lvlNext->GetName();
	TrapDataPath += "_Trap.InfiniteHPHazardData";

	std::string ObjPath = "./Resources/LevelData/ObjectData/";
	ObjPath += _lvlNext->GetName();
	ObjPath += "_MoveableObject.InfiniteHPMoveableObjects";

	std::string ObjDataPath = "./Resources/LevelData/ObjectData/";
	ObjDataPath += _lvlNext->GetName();
	ObjDataPath += "_MoveableObject.InfiniteHPMoveableObjectsData";

	/*std::string LightPath = "./Resources/LevelData/";
	LightPath += _lvlNext->GetName();
	LightPath += ".InfiniteHPLightsData";
	_lvlNext->GetLightManager().LoadLights( LightPath.c_str() );*/

	MeshManager::GetInstance()->LoadEnvironmentTraps(TrapPath.c_str(), TrapDataPath.c_str(), _lvlNext->GetLevelID());

	MeshManager::GetInstance()->LoadEnvironmentMoveableObjects(ObjPath.c_str(), ObjDataPath.c_str(), _lvlNext->GetLevelID());

	// Load up a checkpoint if there is one.
	// If it is at the origin then it is not considered a checkpoint.
	D3DXMATRIX _worldMatrix;
	D3DXMatrixIdentity (&_worldMatrix);

	if( _lvlNext->GetCheckPointPos().x != 0.0f || 
		_lvlNext->GetCheckPointPos().y != 0.0f ||
		_lvlNext->GetCheckPointPos().z != 0.0f )
	{

		_worldMatrix._41 = (float)_lvlNext->GetCheckPointPos().x;
		_worldMatrix._42 = (float)_lvlNext->GetCheckPointPos().y;
		_worldMatrix._43 = (float)_lvlNext->GetCheckPointPos().z;

		IBaseObject* CheckPointObject = CObjectManager::GetInstance()->CreateObject( OBJ_CHECKPOINT, _worldMatrix, _lvlNext->GetLevelID() );
	}


	if (m_nCurLevelToLoadIndex == m_vLevels.size ()-1)
	{
		// spawn a boss
		D3DXMatrixIdentity (&_worldMatrix);

		IBaseObject* Sludge = CObjectManager::GetInstance()->CreateObject( OBJ_SLUDGE, _worldMatrix, _lvlNext->GetLevelID() );

		D3DXMatrixIdentity (&_worldMatrix);

		_worldMatrix._41 = 33500.0f;
		_worldMatrix._42 = 2000.0f;
		_worldMatrix._43 = 0.0f;

		IBaseObject* Boss = CObjectManager::GetInstance()->CreateObject( OBJ_BOSS, _worldMatrix, _lvlNext->GetLevelID() );
		
		Boss->GetFrame().AddChild( (Node*)&Sludge->GetFrame() );
	}

	for( unsigned int i = 0; i < MAX_ACTIVE_LEVELS; i++)
	{
		// Set the first available level pointer to this level.
		if( NULL == m_pActiveLevels[i] )
		{
			m_pActiveLevels[i] = _lvlNext;
			break;
		}
	}

	if( m_pActiveLevels[0] )
		m_nCurLevelIndex = m_pActiveLevels[0]->GetLevelID();

	m_nCurLevelToLoadIndex++;
}

void LevelManager::ResetCurrentLevel(void)
{
	// Player Pointer
	IBaseObject* player = CObjectManager::GetInstance()->GetPlayer();
	// Reset the Player.
	((CComponent_Health*)player->GetComponent( ECOMP_HEALTH ))->ResetPlayer();

	//if( m_pActiveLevels[0]->GetLevelID() == m_vLevels.size() )
	//{
	//	// If we reach this point then this is the boss so lets unload the boss.
	//	CObjectManager::GetInstance()->ResetBoss();
	//}

	// Reset the Hazards and Moveable Objects To the First Active Level.
	int LevelIndex = 0;
	if( m_pActiveLevels[0] )
	{
		LevelIndex = m_pActiveLevels[0]->GetLevelID();
		CObjectManager::GetInstance()->ClearObjectByLevel( m_pActiveLevels[0]->GetLevelID() );
		m_pActiveLevels[0] = NULL;
	}

	m_nCurLevelToLoadIndex = LevelIndex;

	//// Load All Traps
	//std::string TrapPath = "./Resources/LevelData/TrapData/";
	//TrapPath += m_vLevels[LevelIndex].GetName();
	//TrapPath += "_Trap.InfiniteHPHazards";
	//std::string TrapDataPath = "./Resources/LevelData/TrapData/";
	//TrapDataPath += m_vLevels[LevelIndex].GetName();
	//TrapDataPath += "_Trap.InfiniteHPHazardData";
	//MeshManager::GetInstance()->LoadEnvironmentTraps(TrapPath.c_str(), TrapDataPath.c_str(), LevelIndex);

	//// Load all Moveable Object.
	//std::string ObjPath = "./Resources/LevelData/ObjectData/";
	//ObjPath += m_vLevels[LevelIndex].GetName();
	//ObjPath += "_MoveableObject.InfiniteHPMoveableObjects";
	//std::string ObjDataPath = "./Resources/LevelData/ObjectData/";
	//ObjDataPath += m_vLevels[LevelIndex].GetName();
	//ObjDataPath += "_MoveableObject.InfiniteHPMoveableObjectsData";
	//MeshManager::GetInstance()->LoadEnvironmentMoveableObjects(ObjPath.c_str(), ObjDataPath.c_str(), LevelIndex);

	//// Load Up Collision Environment
	//std::string BoundingPath = "./Resources/CollisionData/";
	//BoundingPath += m_vLevels[LevelIndex].GetName();
	//BoundingPath += "_BoundingVolumes.InfiniteHPCollision";

	//MeshManager::GetInstance()->LoadCollisionEnvironment(BoundingPath.c_str(), LevelIndex );

	m_bLoadNext = true;
	LoadNextLevel();

	// Reset the Object the Player has.
	((CComponent_Input*)player->GetComponent( ECOMP_INPUT ))->ResetPulledObject();

	// Reset the Players Spawn Position.
	player->GetFrame().GetLocalMat().axis_pos = m_vLevels[LevelIndex].GetPlayerSpawnPos();
	player->GetFrame().Update();

	if( m_pActiveLevels[0] )
		m_nCurLevelIndex = m_pActiveLevels[0]->GetLevelID();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: UnloadFirstActiveLevel
// Purpose: Clears the objects belonging to the first active level, and removes it from the active level array.
// Original Author: Andy Madruga
// Creation Date: 8/2/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LevelManager::UnloadFirstActiveLevel(void)
{
	if( !m_bUnloadFirstLevel )
		return;

	m_bUnloadFirstLevel = false;

	if( m_pActiveLevels[0] )
	{
		int FirstLevelIndex = m_pActiveLevels[0]->GetLevelID();
		CObjectManager::GetInstance()->ClearObjectByLevel( FirstLevelIndex );

		// Now that the objects were removed lets remove the level from the active levels and move the pointers up in the array.
		// Since there are only 3 pointers this is the simplest solution as of right now.
		m_pActiveLevels[0] = m_pActiveLevels[1];
		m_pActiveLevels[1] = m_pActiveLevels[2];
		m_pActiveLevels[2] = NULL;
	}

	if( m_pActiveLevels[0] )
	{
		m_nCurLevelIndex = m_pActiveLevels[0]->GetLevelID();
		m_nCurLevelToLoadIndex = m_nCurLevelIndex + 1;

	}
	else
	{
		m_nCurLevelIndex = 0;
		m_nCurLevelToLoadIndex = m_nCurLevelIndex;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: LoadAllLevelsToRender
// Purpose: This will Load up all the render nodes for all 5 levels so everything is rendered at once.
// Original Author: Andy Madruga
// Creation Date: 8/2/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LevelManager::LoadAllLevelsToRender(void)
{
	m_nCurLevelIndex = 0;
	m_nCurLevelToLoadIndex = 0;

	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	CObjectManager* ObjMng = CObjectManager::GetInstance();
	// Clear Out the Object Manager because this is the first level loaded up.
	ObjMng->ClearAll();
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());
	// Re-Init the Object Manager.
	ObjMng->Init();
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	// Clear Render Engine Data.
	RenderEngine::ClearData();
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	// Build the Screen Mesh we will render to.
	RenderEngine::BuildScreenMesh();
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	// Load up and set the reticle texture which the player will use as the cursor.
	CGame::GetInstance()->SetReticleTexture (TextureManager::GetInstance()->LoadTexture("./Resources/Textures/T_Reticle_D.png"));

	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	// Load up all the meshes that will be used to render.
		D3DXMATRIX i;
	D3DXMatrixIdentity( &i );

#if !_ANIMATIONS
	// Player
	MeshManager::GetInstance()->LoadMesh("PudgeShape", i);
	MeshManager::GetInstance()->LoadMesh("PudgeArm", i);
	MeshManager::GetInstance()->LoadMesh("SlimeMonster1", i);
	MeshManager::GetInstance()->LoadMesh("LilGoo1", i);

	// Sludge 
	MeshManager::GetInstance()->LoadMesh("SludgeShape", i);
	MeshManager::GetInstance()->LoadMesh ("SludgesShip", i);
#else
	// Player
	MeshManager::GetInstance()->LoadAnimatedMesh("Pudge", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("VacuumArm", i);
	LOG( LOG_ERROR, "LevelManager::LoadAllLevelsToRender", "Done Loading the Player Models");
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());
	
	// Sludge
	MeshManager::GetInstance()->LoadAnimatedMesh( "Sludge", i );
	MeshManager::GetInstance()->LoadAnimatedMesh("SludgesShip", i);
	LOG( LOG_ERROR, "LevelManager::LoadAllLevelsToRender", "Done Loading the Sludge Models");
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	// Enemies
	MeshManager::GetInstance()->LoadAnimatedMesh("SlimeMonster", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("LilGoo", i);
	LOG( LOG_ERROR, "LevelManager::LoadAllLevelsToRender", "Done Loading the Enemy Models");
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	// Mesh Effects
	MeshManager::GetInstance()->LoadAnimatedMesh("BigHurtMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("LittleHurtMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("JumpSteamStartMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("JumpSteamFloorMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("DeathCloud", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("SelectionMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("SpawnPipeDrip", i);
	LOG( LOG_ERROR, "LevelManager::LoadAllLevelsToRender", "Done Loading the Mesh Effect Models");
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());


#endif
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	MeshManager::GetInstance()->LoadMesh("Crate", i);
	MeshManager::GetInstance()->LoadMesh("Goopit", i );
	MeshManager::GetInstance()->LoadMesh("Cork", i );
	MeshManager::GetInstance()->LoadMesh("SteamPipe", i );
	MeshManager::GetInstance()->LoadMesh("BreakableWall", i );
	MeshManager::GetInstance()->LoadMesh("WallChunkShape1", i );
	MeshManager::GetInstance()->LoadMesh("WallChunkShape2", i );
	MeshManager::GetInstance()->LoadMesh("WallChunkShape3", i );
	MeshManager::GetInstance()->LoadMesh("WallChunkShape4", i );
	MeshManager::GetInstance()->LoadMesh("WallChunkShape5", i );
	MeshManager::GetInstance()->LoadMesh("WallChunkShape6", i );
	MeshManager::GetInstance()->LoadMesh("Intake", i );
	MeshManager::GetInstance()->LoadMesh("Plug", i );
	MeshManager::GetInstance()->LoadMesh("SpawnPipe", i );
	MeshManager::GetInstance()->LoadMesh("Crusher", i );
	MeshManager::GetInstance()->LoadMesh("CrusherBase", i );

	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

#if _ANIMATIONS
	MeshManager::GetInstance()->LoadAnimatedMesh("Bridge", i);
	MeshManager::GetInstance()->LoadAnimatedMesh( "Door", i );
	MeshManager::GetInstance()->LoadAnimatedMesh( "Claw", i );
	MeshManager::GetInstance()->LoadAnimatedMesh( "ConveyorBelt", i );
	MeshManager::GetInstance()->LoadAnimatedMesh( "Blades", i );
	MeshManager::GetInstance()->LoadAnimatedMesh( "Boxes", i );
	MeshManager::GetInstance()->LoadAnimatedMesh( "CrateClaw", i);
	MeshManager::GetInstance()->LoadAnimatedMesh( "BossMachine", i);
#else
	MeshManager::GetInstance()->LoadMesh("Bridge1", i);
	MeshManager::GetInstance()->LoadMesh( "Door1", i );
#endif

	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	MeshManager::GetInstance()->LoadMesh("Outlet", i);
	MeshManager::GetInstance()->LoadMesh("WaterPool", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("Button", i);
	MeshManager::GetInstance()->LoadMesh("Button1", i);
	MeshManager::GetInstance()->LoadMesh("BoxSpawnPipe", i);

	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

	// Loop through each level and load up its Scene file.
	for( unsigned int i = 0; i < m_vLevels.size(); i++)
	{
		CLevel* _lvlNext = &m_vLevels[i];

		// Shutdown the Light Manager.
		_lvlNext->GetLightManager().ShutdownLightManager();
		// Re init the Light Manager.
		_lvlNext->GetLightManager().InitLightManager();

		// Load up the Scene from the Scene Path
		std::string ScenePath = "./Resources/Models/";
		ScenePath += _lvlNext->GetName();
		ScenePath += ".InfiniteHPScene";

		MeshManager::GetInstance()->LoadEnvironment(ScenePath.c_str());

		CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());

		std::string LightPath = "./Resources/LevelData/";
		LightPath += _lvlNext->GetName();
		LightPath += ".InfiniteHPLightsData";
		_lvlNext->GetLightManager().LoadLights( LightPath.c_str() );

		CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());
	}

	// We have now loaded up all levels to render so we leave it to the manager to load up
	// the objects when needed.
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update
// Purpose: This will update the level manager which will load the next level and unload levels. As
//			well as updating the light managers in each level.
// Original Author: Andy Madruga
// Creation Date: 8/7/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void LevelManager::Update( float _fDT )
{
	// Only unloads the first active level if the bool was set to true.
	UnloadFirstActiveLevel();
	// Loads the next level if the bool was set to true.
	LoadNextLevel();

	int NumOfLevels = m_vLevels.size();
	for( int i = 0; i < NumOfLevels; i++)
		m_vLevels[i].GetLightManager().Update( _fDT );
}