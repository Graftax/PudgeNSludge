#define _BROKEN_AUDIO
#include "config.h"
#include "CGame.h"
#include "States\IGameState.h"
#include "States\MainMenuState.h"
#include "States\CSplashScreenState.h"
#include "States\CLoadingState.h"
#include "States\GamePlayState.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../RenderEngine/TextureManager.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "../RenderEngine/LightSystem/LightManager.h"

//#include "../ObjectLib/ObjectManager/CObjectManager.h"

// This is the memory leak debug stuff
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif

CGame* CGame::mainGame = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Game
// Purpose: Default constructor for the game.
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CGame::CGame()
{
	//m_pntScreenSize.x = 0;
	//m_pntScreenSize.y = 0;
	//CObjectManager::GetInstance ();
	m_bInputInit = false;
	m_bAudioInit = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ~Game
// Purpose: Default destructor for the game.
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CGame::~CGame()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Main
// Purpose: Maintains the game loop
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGame::Main()
{
	if(!Input())
	{
		return false;
	}

	//Need to process Events here;

	// Get Elapsed Time;
	float fTimeElapsed = (float)m_Timer.GetElapsedTime();
	m_Timer.Reset();

	//static float TimeStep = 0.0f;

	//TimeStep += fTimeElapsed;

	//if ( TimeStep >= 0.125f )
	{
		//fTimeElapsed = 1.0f/60.0f;

	Update(fTimeElapsed);
		//TimeStep = 0.0f;
	}

	Render();

	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Init
// Purpose: Initalizes all dependancies in CGame, and begins the main game loop.
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	Zach Bolthouse
// Last Modification Date:	2/27/2012
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::Init(HWND hWnd, HINSTANCE hInstance)
{

	m_hWnd = hWnd;
	m_hInstance = hInstance;

#if _DEBUG
	m_bFullScreen = false;
#else
	m_bFullScreen = true;
#endif

	//Just to test that the camera doesn't break the game
	//float fov = D3DX_PI/3.0f;
	//cam.BuildPerspective( fov, SCREEN_WIDTH/(float)(SCREEN_HEIGHT), 0.1f, 10000.0f);
	//cam.SetViewPosition(0.0f, 0.0f, -400.0f);

	renderEngine.InitRenderEngine( hWnd, hInstance, SCREEN_WIDTH, SCREEN_HEIGHT, !m_bFullScreen, true, D3DXVECTOR3(0.0f,0.0f,0.0f));
	renderEngine.SetGamma(0.0f);
	//m_pntScreenSize.x = 1024;
	//m_pntScreenSize.y = 768;

	//CObjectManager::GetInstance()->Init();
	
	m_bInputInit = m_Input.Initialize(hWnd, hInstance);
	m_nSensitivity = 1.0f;
	m_Input.SetSensitivity(m_nSensitivity);
	m_bAudioInit = m_Audio.Initialize();
	
	//push on the init state

	PushState (CSplashScreenState::GetInstance());

	m_Timer.Reset();
	m_Timer.Start();

	m_Dontmovecamera = false;
	m_bLoading = false;

	m_nReticle = TextureManager::GetInstance()->LoadTexture("./Resources/Textures/T_Reticle_D.png");


	D3DXMATRIX i;
	D3DXMatrixIdentity( &i );
	CRITICAL_SECTION cs = CSplashScreenState::GetInstance()->GetCriticalSection();
	ECS(cs);
	LightManager::GetInstance()->LoadLights( "./Resources/LevelData/TestLights.bslights" );
	LCS(cs);
#if !_ANIMATIONS
	// Player
	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("PudgeShape", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("PudgeArm", i);
	LCS( cs );
	// Enemies
	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("SlimeMonster1", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("LilGoo1", i);
	LCS( cs );
#else
	// Player
	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("Pudge", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("VacuumArm", i);
	LCS( cs );
	// Enemies
	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("SlimeMonster", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("LilGoo", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("PullMesh", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("PushMesh", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("ImpactMesh", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("BigHurtMesh", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("LittleHurtMesh", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("JumpSteamStartMesh", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("JumpSteamFloorMesh", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadAnimatedMesh("DeathCloud", i);
	LCS( cs );

#endif
	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("Crate", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("Goopit", i );
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("CorkShape", i );
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("SteamPipe", i );
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("BreakableWall", i );
	LCS( cs );
	//MeshManager::GetInstance()->LoadMesh("BreakableWallH", i );
	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("Intake", i );
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("Plug", i );
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("SpawnPipe", i );
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("Crusher", i );
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("CrusherBase", i );
	LCS( cs );

	ECS( cs );
#if _ANIMATIONS
	MeshManager::GetInstance()->LoadAnimatedMesh("Bridge", i);
	MeshManager::GetInstance()->LoadAnimatedMesh( "Door", i );
#else
	MeshManager::GetInstance()->LoadMesh("Bridge1", i);
	MeshManager::GetInstance()->LoadAnimatedMesh( "Door1", i );
#endif
	LCS( cs );


	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("Outlet", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadMesh("WaterPool", i);
	LCS( cs );

	ECS( cs );
	MeshManager::GetInstance()->LoadEnvironment("./Resources/Models/Sector1.InfiniteHPScene");
	//MeshManager::GetInstance()->LoadEnvironment("./Resources/Models/Sector2.InfiniteHPScene");
	LCS( cs );


#if _ANIMATIONS
#pragma region PUDGE_ANIM
	Animation* PudgeIdle = new Animation();
	Animation* PudgeRun = new Animation();
	Animation* PudgeJump = new Animation();
	Animation* PudgeFalling = new Animation();
	Animation* PudgeWallJump = new Animation();
	Animation* PudgePush = new Animation();
	Animation* PudgePull = new Animation();
	Animation* PudgeWallSlide = new Animation();
	Animation* PudgeBigHurt = new Animation();
	Animation* PudgeLittleHurt = new Animation();
	Animation* PudgeDeathByGoo = new Animation();
	Animation* PudgeDeathByCrusher = new Animation();

	Animation* VacuumArmIdle = new Animation();

	PudgeIdle->LoadFromFile( "Pudge_Idle" );
	PudgeRun->LoadFromFile( "Pudge_Run" );
	PudgeJump->LoadFromFile( "Pudge_Jump" );
	PudgeFalling->LoadFromFile( "Pudge_Falling" );
	PudgeWallJump->LoadFromFile( "Pudge_WallJump" );
	PudgePull->LoadFromFile( "Pudge_Pull" );
	PudgePush->LoadFromFile( "Pudge_Push" );
	PudgeWallSlide->LoadFromFile( "Pudge_WallSlide" );
	PudgeBigHurt->LoadFromFile( "Pudge_HighDamage" );
	PudgeLittleHurt->LoadFromFile( "Pudge_SmallHurt" );
	PudgeDeathByGoo->LoadFromFile( "Pudge_DeathByGoo" );
	PudgeDeathByCrusher->LoadFromFile( "Pudge_DeathByCrusher" );

	VacuumArmIdle->LoadFromFile( "VacuumArm_Idle" );

	AnimationManager::GetInstance()->AddAnimation( PudgeIdle );
	AnimationManager::GetInstance()->AddAnimation( PudgeRun );
	AnimationManager::GetInstance()->AddAnimation( PudgeJump );
	AnimationManager::GetInstance()->AddAnimation( PudgeFalling );
	AnimationManager::GetInstance()->AddAnimation( PudgeWallJump );
	AnimationManager::GetInstance()->AddAnimation( PudgePull );
	AnimationManager::GetInstance()->AddAnimation( PudgePush );
	AnimationManager::GetInstance()->AddAnimation( PudgeWallSlide );
	AnimationManager::GetInstance()->AddAnimation( PudgeBigHurt );
	AnimationManager::GetInstance()->AddAnimation( PudgeLittleHurt );
	AnimationManager::GetInstance()->AddAnimation( PudgeDeathByGoo );
	AnimationManager::GetInstance()->AddAnimation( PudgeDeathByCrusher );

	AnimationManager::GetInstance()->AddAnimation( VacuumArmIdle );
#pragma endregion

#pragma region MESH_EFFECTS
	Animation* PullEffect = new Animation();
	Animation* PushEffect = new Animation();
	Animation* ImpactEffect = new Animation();
	Animation* PullEffectIdle = new Animation();
	Animation* PushEffectIdle = new Animation();
	Animation* PullEffectPullForever = new Animation();
	Animation* BigHurtEffect = new Animation();
	Animation* LittleHurtEffect = new Animation();
	Animation* JumpStartEffect = new Animation();
	Animation* JumpLandEffect = new Animation();
	Animation* JumpStartIdleEffect = new Animation();
	Animation* JumpLandIdleEffect = new Animation();
	Animation* BigHurtIdleEffect = new Animation();
	Animation* LittleHurtIdleEffect = new Animation();
	Animation* DeathCloudPudgeDeathStateEffect = new Animation();

	PullEffect->LoadFromFile( "PullMesh_Pull" );
	PushEffect->LoadFromFile( "PushMesh_Push" );
	ImpactEffect->LoadFromFile( "ImpactMesh_Impact" );
	PullEffectIdle->LoadFromFile( "PullMesh_Idle" );
	PushEffectIdle->LoadFromFile( "PushMesh_Idle" );
	PullEffectPullForever->LoadFromFile( "PullMesh_PullForever" );
	BigHurtEffect->LoadFromFile( "BigHurtMesh_Hurt" );
	LittleHurtEffect->LoadFromFile( "LittleHurtMesh_Hurt" );
	JumpStartEffect->LoadFromFile( "JumpSteamStartMesh_JumpStart" );
	JumpLandEffect->LoadFromFile( "JumpSteamFloorMesh_JumpLand" );
	JumpStartIdleEffect->LoadFromFile( "JumpSteamStartMesh_JumpIdle" );
	JumpLandIdleEffect->LoadFromFile( "JumpSteamFloorMesh_JumpIdle" );
	BigHurtIdleEffect->LoadFromFile( "BigHurtMesh_Idle" );
	LittleHurtIdleEffect->LoadFromFile( "LittleHurtMesh_Idle" );
	DeathCloudPudgeDeathStateEffect->LoadFromFile( "DeathCloud_PudgeDead" );

	AnimationManager::GetInstance()->AddAnimation( PullEffect );
	AnimationManager::GetInstance()->AddAnimation( PushEffect );
	AnimationManager::GetInstance()->AddAnimation( ImpactEffect );
	AnimationManager::GetInstance()->AddAnimation( PullEffectIdle );
	AnimationManager::GetInstance()->AddAnimation( PushEffectIdle );
	AnimationManager::GetInstance()->AddAnimation( PullEffectPullForever );
	AnimationManager::GetInstance()->AddAnimation( BigHurtEffect );
	AnimationManager::GetInstance()->AddAnimation( LittleHurtEffect );
	AnimationManager::GetInstance()->AddAnimation( JumpStartEffect );
	AnimationManager::GetInstance()->AddAnimation( JumpLandEffect );
	AnimationManager::GetInstance()->AddAnimation( JumpStartIdleEffect );
	AnimationManager::GetInstance()->AddAnimation( JumpLandIdleEffect );
	AnimationManager::GetInstance()->AddAnimation( BigHurtIdleEffect );
	AnimationManager::GetInstance()->AddAnimation( LittleHurtIdleEffect );
	AnimationManager::GetInstance()->AddAnimation( DeathCloudPudgeDeathStateEffect );

#pragma endregion

#pragma region SLIMEMONSTER_ANIM
	Animation* SlimeMonsterIdle = new Animation();
	Animation* SlimeMonsterWalk = new Animation();
	Animation* SlimeMonsterThrowAttack = new Animation();
	Animation* SlimeMonsterSneakAttack = new Animation();
	Animation* SlimeMonsterStun = new Animation();
	Animation* SlimeMonsterDeath = new Animation();

	SlimeMonsterIdle->LoadFromFile( "SlimeMonster_Idle" );
	SlimeMonsterWalk->LoadFromFile( "SlimeMonster_Walk" );
	SlimeMonsterThrowAttack->LoadFromFile( "SlimeMonster_ThrowAttack" );
	SlimeMonsterSneakAttack->LoadFromFile( "SlimeMonster_SneakAttack" );
	SlimeMonsterStun->LoadFromFile( "SlimeMonster_Stun" );
	SlimeMonsterDeath->LoadFromFile( "SlimeMonster_Death" );


	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterIdle );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterWalk );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterThrowAttack );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterSneakAttack );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterStun );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterDeath );
#pragma endregion

#pragma region LILGOO_ANIM
	Animation* LilGooWalk = new Animation();
	Animation* LilGooAttack = new Animation();
	Animation* LilGooDeath = new Animation();
	Animation* LilGooFlying = new Animation();

	LilGooWalk->LoadFromFile( "LilGoo_Walk" );
	LilGooAttack->LoadFromFile( "LilGoo_Attack" );
	LilGooDeath->LoadFromFile( "LilGoo_Death" );
	LilGooFlying->LoadFromFile( "LilGoo_Flying" );

	AnimationManager::GetInstance()->AddAnimation( LilGooWalk );
	AnimationManager::GetInstance()->AddAnimation( LilGooAttack );
	AnimationManager::GetInstance()->AddAnimation( LilGooDeath );
	AnimationManager::GetInstance()->AddAnimation( LilGooFlying );
#pragma endregion

#pragma region HAZARD_ANIM
	Animation* Bridge = new Animation();
	Animation* BridgeIdle = new Animation();
	Animation* BridgeDownIdle = new Animation();
	Animation* DoorOpen = new Animation();
	Animation* DoorIdle = new Animation();
	Animation* DoorOpenIdle = new Animation();

	Bridge->LoadFromFile( "Bridge_HalfRetract" );
	BridgeIdle->LoadFromFile( "Bridge_Idle" );
	BridgeDownIdle->LoadFromFile( "Bridge_HalfRetractIdle" );
	DoorOpen->LoadFromFile( "Door_Open" );
	DoorIdle->LoadFromFile( "Door_Idle" );
	DoorOpenIdle->LoadFromFile( "Door_OpenIdle" );

	AnimationManager::GetInstance()->AddAnimation( Bridge );
	AnimationManager::GetInstance()->AddAnimation( BridgeIdle );
	AnimationManager::GetInstance()->AddAnimation( BridgeDownIdle );
	AnimationManager::GetInstance()->AddAnimation( DoorOpen );
	AnimationManager::GetInstance()->AddAnimation( DoorIdle );
	AnimationManager::GetInstance()->AddAnimation( DoorOpenIdle );
#pragma endregion

#endif

	m_Timer.Reset();
	m_Timer.Start();
	ShowCursor( true );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetInstance
// Purpose: Default Returns a pointer to the current CGame
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CGame* CGame::GetInstance()
{
	if (!mainGame)
	{
		mainGame = new CGame();
	}
	return mainGame;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetInstance
// Purpose: Default Returns a pointer to the current CGame
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::DeleteInstance()
{
	if(mainGame)
	{
		delete mainGame;
		mainGame = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Shutdown
// Purpose: Shuts down all modules in the game
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::Shutdown()
{
	while( !m_StateStack.empty() )
	{
		m_StateStack.back()->Shutdown();
		m_StateStack.pop_back();
	}

	m_Input.Shutdown();
	m_Audio.Shutdown();

	CSplashScreenState::DeleteInstance();

	CObjectManager::DeleteInstance();
//	MultiThreadRender::GetInstance()->Shutdown();
	renderEngine.ShutdownRenderEngine();
	CGamePlayState::DeleteInstance();
	CMainMenuState::DeleteInstance();
	AnimationManager::DeleteInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update
// Purpose: To Update all modules in the game through the state machiene
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::Update( float fDeltaTime )
{

	//if( GetAsyncKeyState( 'C' ) & 1 )
	//{
	//	CObjectManager::GetInstance()->CreateObject( OBJ_TEST_ONE, "" );
	//}

	//if( GetAsyncKeyState( 'V' ) & 1 )
	//{
	//	CObjectManager::GetInstance()->ClearAll();
	//}

	m_Input.Update();
	m_Audio.Update();

	//CObjectManager::GetInstance ()->Update ( fDeltaTime );

	static float fOneSecond = 0.0f;
	static unsigned int unFrames = 0;

	if( fOneSecond > 1.0f )
	{
		RenderEngine::UpdateFPS( unFrames );
		unFrames = 0;
		fOneSecond = 0.0f;
	}

	fOneSecond += fDeltaTime;
	unFrames++;


	//if( fDeltaTime > 1.0f/10.0f )
	//{
	//	fDeltaTime =  1.0f/10.0f;
	//}

	//fDeltaTime = 0.025f;

	//fDeltaTime /= 4.0f;
	//MoveCamera(fDeltaTime);
	
	//if(m_Dontmovecamera == false)
	//{
	//	IBaseObject* pPlayer = CObjectManager::GetInstance()->GetPlayer();
	//	
	//	if( pPlayer )
	//	{
	//		cam.MoveCamWithMouse(pPlayer->GetPos(), m_hWnd);
	//	}
	//}

	//renderEngine.SetModelViewProjection( cam.GetViewMatrix() * cam.GetProjectionMatrix() );
	//renderEngine.SetCamera( &cam );

	fDeltaTime = 1.0f/60.0f;
	if (m_StateStack.size())
	{
		m_StateStack.back()->Update(fDeltaTime);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Render	
// Purpose: Calls the current GameState's Render
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::Render()
{

	if( CSplashScreenState::GetInstance()->GetRunning() || !CSplashScreenState::GetInstance()->GetStatus() )
		return;

	if( m_bLoading )
		return;

	renderEngine.Clear();
	renderEngine.BeginScene();

	//CObjectManager::GetInstance()->Render();

	//put render engine stuff here
	renderEngine.Render();

	vector<IGameState*>::iterator iter = m_StateStack.begin();
	for(; iter != m_StateStack.end(); iter++)
	{
		(*iter)->Render();
	}

	if( GetState() == CGamePlayState::GetInstance() && !CGamePlayState::GetInstance()->GetWinGameFace() )
	{
		D3DXVECTOR4 _mousePos = D3DXVECTOR4(cam.GetWorldCursor().x, cam.GetWorldCursor().y, cam.GetWorldCursor().z, 1.0f );
		D3DXVec4Transform( &_mousePos, &_mousePos, &(cam.GetViewMatrix() * cam.GetProjectionMatrix()) );
		_mousePos /= _mousePos.w;
		_mousePos *= 0.5f;
		_mousePos.x += 0.5f;
		_mousePos.y += 0.5f;
		renderEngine.RasterTexture( TextureManager::GetInstance()->m_umTextureMap[m_nReticle].m_lpTexture, _mousePos.x - 0.02f, _mousePos.y - 0.02f, _mousePos.x + 0.02f, _mousePos.y + 0.02f );
	}
	renderEngine.EndScene();
	renderEngine.Present();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Input
// Purpose: Retrieves any user input, and passes it to the current GameState
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGame::Input()
{
	if(m_bInputInit)
	{
#if _DEBUG
		if((m_Input.KeyDown(DIK_LALT) || m_Input.KeyDown(DIK_RALT)) && 
			m_Input.KeyPressed(DIK_RETURN))
		{
			if(GetState() == CMainMenuState::GetInstance())
			{
				renderEngine.ChangeDisplayParam(SCREEN_WIDTH, SCREEN_HEIGHT, m_bFullScreen, false);
				//m_bFullScreen = !m_bFullScreen;
				SetFullScreen(!m_bFullScreen);
				//ShowCursor(false);
			}
			return true;
		}
#endif
	}

	if(m_Input.KeyPressed(DIK_ADD))
	{
		m_nSensitivity = m_nSensitivity + 0.5f;
		if(m_nSensitivity >= 2.0f)
			m_nSensitivity = 2.0f;
		m_Input.SetSensitivity(m_nSensitivity);
	}
	if(m_Input.KeyPressed(DIK_SUBTRACT))
	{
		m_nSensitivity = m_nSensitivity - 0.5f;
		if(m_nSensitivity <= 0.0f)
			m_nSensitivity = 0.0f;
		m_Input.SetSensitivity(m_nSensitivity);
	}
	
	//Logger::ClearConsole();
	//LOG(LOG_DEBUG, "CGame::Input()", "Sensitivity: %.2f", m_nSensitivity);


	if (m_StateStack.size())
	{
		return m_StateStack.back()->Input();
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: PushState
// Purpose: Pushes a new state on to the stacked states
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::PushState(IGameState* pState)
{
	if (!pState)
	{
		return;
	}
	m_StateStack.push_back(pState);

		if( pState != CSplashScreenState::GetInstance() )
	pState->Init();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: PopState
// Purpose: Pops off the top state on the stacked states
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::PopState()
{
	if( !m_StateStack.empty() )
	{
		m_StateStack.back()->Shutdown();
		m_StateStack.pop_back();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ChangeState
// Purpose: Pops off all states on the stacked states, and pushes on the state passed in
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::ChangeState(IGameState* pState)
{
	if (!pState)
	{
		return;
	}

	while( !m_StateStack.empty() )
	{
		m_StateStack.back()->Shutdown();
		m_StateStack.pop_back();
	}

	m_StateStack.push_back(pState);
	pState->Init();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetState
// Purpose: Returns a pointer to the state that is on top of the stacked states
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
IGameState* CGame::GetState()
{
	return m_StateStack.back();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetScreenSize
// Purpose: Gets the screen size
// Original Author: Scott Drayer
// Creation Date: 3/6/2012
// Last Modification By:	Scott Drayer
// Last Modification Date:	3/6/2012
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::SetFullScreen( bool FullScreen )
{
	m_bFullScreen = FullScreen;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: SetScreenSize
// Purpose: Sets the screen size
// Original Author: Scott Drayer
// Creation Date: 3/6/2012
// Last Modification By:	Scott Drayer
// Last Modification Date:	3/6/2012
/////////////////////////////////////////////////////////////////////////////////////////////////////
//void CGame::SetScreenSize( tPoint pntScreenSize )
//{
//	m_pntScreenSize = pntScreenSize;
//}

void CGame::MoveCamera(float fDT)
{
// 	float PLAYER_MOVEMENT = 1500.0f;
// 	float PLAYER_Rot = 25.0f;
// 	if(m_Input.GetDIKCode() == DIK_LEFT)
// 	{
// 		cam.RotateLocalY(-D3DXToRadian(7.0f) * fDT * PLAYER_Rot);
// 	}
// 	if(m_Input.GetDIKCode() == DIK_RIGHT)
// 	{
// 		cam.RotateLocalY(D3DXToRadian(7.0f) * fDT * PLAYER_Rot);
// 	}
// 	if(m_Input.GetDIKCode() == DIK_UP)
// 	{
// 		cam.TranslateLocalZ(PLAYER_MOVEMENT * fDT);
// 	}
// 	if(m_Input.GetDIKCode() == DIK_DOWN)
// 	{
// 		cam.TranslateLocalZ(-PLAYER_MOVEMENT * fDT);
// 	}
// 	if(m_Input.GetDIKCode() == DIK_R)
// 	{
// 		cam.TranslateLocalY(PLAYER_MOVEMENT * fDT);
// 	}
// 	if(m_Input.GetDIKCode() == DIK_F)
// 	{
// 		cam.TranslateLocalY(-PLAYER_MOVEMENT * fDT);
// 	}
// 	if(m_Input.KeyPressed(DIK_1))
// 		m_Dontmovecamera = !m_Dontmovecamera;

	//if(m_Input.KeyPressed(DIK_NUMPAD9))
		//SwapLevels();
}

void CGame::SwapLevels(void)
{
	//TextureManager::GetInstance()->ShutdownTextureManager();
	MeshManager::GetInstance()->ShutdownMeshManager();
	CObjectManager::GetInstance()->ClearAll();

	RenderEngine::ClearData();

	MeshManager::GetInstance()->InitMeshManager();
	//TextureManager::GetInstance()->InitTextureManager();
	CObjectManager::GetInstance()->Init();

	RenderEngine::BuildScreenMesh();

	m_nReticle = TextureManager::GetInstance()->LoadTexture("./Resources/Textures/T_Reticle_D.png");


	D3DXMATRIX i;
	D3DXMatrixIdentity( &i );
#if !_ANIMATIONS
	// Player
	MeshManager::GetInstance()->LoadMesh("PudgeShape", i);
	MeshManager::GetInstance()->LoadMesh("PudgeArm", i);
	// Enemies
	MeshManager::GetInstance()->LoadMesh("SlimeMonster1", i);
	MeshManager::GetInstance()->LoadMesh("LilGoo1", i);
#else
	// Player
	MeshManager::GetInstance()->LoadAnimatedMesh("Pudge", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("VacuumArm", i);
	// Enemies
	MeshManager::GetInstance()->LoadAnimatedMesh("SlimeMonster", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("LilGoo", i);

	// Mesh Effects
	MeshManager::GetInstance()->LoadAnimatedMesh("PullMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("PushMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("ImpactMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("BigHurtMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("LittleHurtMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("JumpSteamStartMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("JumpSteamFloorMesh", i);
	MeshManager::GetInstance()->LoadAnimatedMesh("DeathCloud", i);


#endif

	MeshManager::GetInstance()->LoadMesh("Crate", i);
	MeshManager::GetInstance()->LoadMesh("Goopit", i );
	MeshManager::GetInstance()->LoadMesh("CorkShape", i );
	MeshManager::GetInstance()->LoadMesh("SteamPipe", i );
	MeshManager::GetInstance()->LoadMesh("BreakableWall", i );
	//MeshManager::GetInstance()->LoadMesh("BreakableWallH", i );
	MeshManager::GetInstance()->LoadMesh("Intake", i );
	MeshManager::GetInstance()->LoadMesh("Plug", i );
	MeshManager::GetInstance()->LoadMesh("SpawnPipe", i );
	MeshManager::GetInstance()->LoadMesh("Crusher", i );
	MeshManager::GetInstance()->LoadMesh("CrusherBase", i );
	MeshManager::GetInstance()->LoadMesh("Bridge", i);
	MeshManager::GetInstance()->LoadMesh("Outlet", i);
	MeshManager::GetInstance()->LoadMesh("WaterPool", i);

	MeshManager::GetInstance()->LoadEnvironment("./Resources/Models/Sector2.InfiniteHPScene");

	MeshManager::GetInstance()->LoadCollisionEnvironment("./Resources/CollisionData/Sector2_BoundingVolumes.InfiniteHPCollision");

	D3DXMATRIX _ObjectMatrix;

	D3DXMatrixIdentity( &_ObjectMatrix );
	D3DXMatrixTranslation( &_ObjectMatrix, -500.0f, 200.0f, -300.0f );
	CObjectManager::GetInstance()->CreateObject(OBJ_PLAYER, _ObjectMatrix );

	D3DXMatrixIdentity( &_ObjectMatrix );
	D3DXMatrixTranslation( &_ObjectMatrix, -500.0f, 200.0f, -300.0f );
	CObjectManager::GetInstance()->CreateObject(OBJ_PULL_MESHEFFECT, _ObjectMatrix );

	D3DXMatrixIdentity( &_ObjectMatrix );
  	D3DXMatrixTranslation( &_ObjectMatrix, -600.0f, 200.0f, -300.0f );
	CObjectManager::GetInstance()->CreateObject(OBJ_PUSH_MESHEFFECT, _ObjectMatrix );

	D3DXMatrixIdentity( &_ObjectMatrix );
 	CObjectManager::GetInstance()->CreateObject(OBJ_LAND_MESHEFFECT, _ObjectMatrix );

	D3DXMatrixIdentity( &_ObjectMatrix );
	CObjectManager::GetInstance()->CreateObject(OBJ_JUMP_MESHEFFECT, _ObjectMatrix );

	D3DXMatrixIdentity( &_ObjectMatrix );
	CObjectManager::GetInstance()->CreateObject(OBJ_BIGHIT_MESHEFFECT, _ObjectMatrix );

	D3DXMatrixIdentity( &_ObjectMatrix );
	CObjectManager::GetInstance()->CreateObject(OBJ_LITTLEHIT_MESHEFFECT, _ObjectMatrix );

	D3DXMatrixIdentity( &_ObjectMatrix );
	CObjectManager::GetInstance()->CreateObject(OBJ_PLAYER_ARM, _ObjectMatrix );

	MeshManager::GetInstance()->LoadEnvironmentTraps("./Resources/LevelData/TrapData/Sector2_Trap.InfiniteHPHazards", "./Resources/LevelData/TrapData/Sector2_Trap.InfiniteHPHazardData");
	MeshManager::GetInstance()->LoadEnvironmentMoveableObjects("./Resources/LevelData/ObjectData/Sector2_MoveableObject.InfiniteHPMoveableObjects");

}