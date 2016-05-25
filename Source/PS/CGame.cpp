#define _BROKEN_AUDIO
#include "config.h"
#include "CGame.h"
#include "States\IGameState.h"
#include "States\MainMenuState.h"
#include "States\CSplashScreenState.h"
#include "States\CLoadingState.h"
#include "States\GamePlayState.h"
#include "States\CHelpState.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../RenderEngine/TextureManager.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "../RenderEngine/LightSystem/LightManager.h"
#include "../EffectsLib/ParticleManager.h"

#include "../LevelManager/CLevelManager.h"
//#include "../ObjectLib/ObjectManager/CObjectManager.h"

// This is the memory leak debug stuff
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif

struct LoadedOptions
{
	float fGamma;
	bool bFirstPlay;
	bool bIcon;
	LoadedOptions(void)
	{
		fGamma = 1.0f;
		bFirstPlay = false;
	}
};

#define OPTIONSFILE "./Resources/PSUserOptions.PSdat"

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

	m_nScreenWidth = 1024;
	m_nScreenHeight = 768;
	m_pIcon = nullptr;

	m_bAudioInit = m_Audio.Initialize();
	LoadAndSetOptions();
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

	m_bDeviceReset = false;
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Init
// Purpose: Initalizes all dependancies in CGame, and begins the main game loop.
// Original Author: Josh Fields
// Creation Date: 3/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::Init(HWND hWnd, HINSTANCE hInstance)
{

	m_hWnd = hWnd;
	m_hInstance = hInstance;

	LoadedOptions optionsToBeSet;
	
	optionsToBeSet = LoadAndSetOptions();
	//Just to test that the camera doesn't break the game
	//float fov = D3DX_PI/3.0f;
	//cam.BuildPerspective( fov, CGame::GetInstance()->GetScreenWidth()/(float)(CGame::GetInstance()->GetScreenHeight()), 0.1f, 10000.0f);
	//cam.SetViewPosition(0.0f, 0.0f, -400.0f);

	renderEngine.InitRenderEngine( hWnd, hInstance, CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), m_bFullScreen, true, D3DXVECTOR3(0.0f,0.0f,0.0f));
	renderEngine.SetGamma( optionsToBeSet.fGamma );
	
	//m_pntScreenSize.x = 1024;
	//m_pntScreenSize.y = 768;

	CParticleManager::GetInstance()->Initialize( renderEngine.GetDevice() );
	PushState (CSplashScreenState::GetInstance());

	CSplashScreenState::GetInstance()->SetFirstPlay( optionsToBeSet.bFirstPlay );

	CRITICAL_SECTION cs = CSplashScreenState::GetInstance()->GetCriticalSection();
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "JumpSteam", string( "Resources/Particles/JumpSteamVFX.xml" ) );
	CParticleManager::GetInstance()->LoadParticleEffect( "LandingSmoke", string( "Resources/Particles/LandingSmoke.xml" ) );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "RunSmoke", string( "Resources/Particles/RunSmoke.xml" ) );
	CParticleManager::GetInstance()->LoadParticleEffect( "SteamPipe", string( "Resources/Particles/SteamPipe.xml" ) );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "FullHealthSparkles", string( "Resources/Particles/FullHealthSparkles.xml" ) );
	CParticleManager::GetInstance()->LoadParticleEffect( "GooDripping", "Resources/Particles/GooDripping.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "LilGooExplosion", "Resources/Particles/LilGooExplosion.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "LilGooSpittle",  "Resources/Particles/LilGooSpittle.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "BreakableWall",  "Resources/Particles/BreakableWall.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "PushInside", "Resources/Particles/PushInside.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "PushOutside", "Resources/Particles/PushOutside.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "PushSparkles", "Resources/Particles/PushSparkles.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "PullSmokeCloud", "Resources/Particles/PullSmokeCloud.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "PullSmokeSparkles", "Resources/Particles/PullSmokeSparkles.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "Sparks", "Resources/Particles/Sparks.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "ImpactSmoke", "Resources/Particles/ImpactSmoke.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "SteamIntake", "Resources/Particles/SteamIntake.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "HUDTest", "Resources/Particles/HUDTest.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "HUDVFX", "Resources/Particles/HudVFX.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "GooBubble", "Resources/Particles/GooBubble.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "WallSmoke", "Resources/Particles/WallSmoke.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "LilGooWalkSplat", "Resources/Particles/LilGooWalkSplat.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "Showers", "Resources/Particles/Showers.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "BodyBubbles", "Resources/Particles/BodyBubbles.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "StraySuds", "Resources/Particles/StraySuds.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "UFODamage", "Resources/Particles/UFODamage.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "FloorSplash", "Resources/Particles/FloorSplash.xml" );
	LCS(cs);
	ECS(cs);
	CParticleManager::GetInstance()->LoadParticleEffect( "UfoExplosion", "Resources/Particles/UfoExplosion.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "UfoExplosionSparks", "Resources/Particles/UfoExplosionSparks.xml" );
	CParticleManager::GetInstance()->LoadParticleEffect( "UfoExplosionSmoke", "Resources/Particles/UfoExplosionSmoke.xml" );
	LCS(cs);

	m_bInputInit = m_Input.Initialize(hWnd, hInstance);
	m_nSensitivity = 1.0f;
	m_Input.SetSensitivity(m_nSensitivity);
	//m_bAudioInit = m_Audio.Initialize();
	//LoadAndSetOptions();
	//push on the init state

	

	m_Timer.Reset();
	m_Timer.Start();

	m_Dontmovecamera = false;
	m_bLoading = true;
	
	ECS(cs);
	m_nReticle = TextureManager::GetInstance()->LoadTexture("./Resources/Textures/T_Reticle_D.png");
	LCS(cs);

	//LevelManager lev;
	//lev.Init ("");


	LevelManager::GetInstance ()->Init ("Levels");

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
	Animation* PudgeBackPeddle		= new Animation();

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
	PudgeBackPeddle->LoadFromFile(			"Pudge_BackPeddle"		);

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
	AnimationManager::GetInstance()->AddAnimation( PudgeBackPeddle		);

	AnimationManager::GetInstance()->AddAnimation( VacuumArmIdle );
#pragma endregion

#pragma region SLUDGE
	// Sludges Animations.
	Animation* SludgeIdle = new Animation();
	Animation* SludgePullLeverDown = new Animation();
	Animation* SludgeSpinAttack = new Animation();
	Animation* SludgeStartCrusherAttack = new Animation();
	Animation* SludgeFallCrusherAttack = new Animation();
	Animation* SludgeLandCrusherAttack = new Animation();
	Animation* SludgeSteamAttack = new Animation();
	Animation* SludgeFlyAway = new Animation();

	SludgeIdle->LoadFromFile( "Sludge_Idle" );
	SludgePullLeverDown->LoadFromFile( "Sludge_PullLever" );
	SludgeSpinAttack->LoadFromFile( "Sludge_SpinAttack" );
	SludgeStartCrusherAttack->LoadFromFile( "Sludge_StartCrusherAttack" );
	SludgeFallCrusherAttack->LoadFromFile( "Sludge_FallCrusherAttack" );
	SludgeLandCrusherAttack->LoadFromFile( "Sludge_LandCrusherAttack" );
	SludgeSteamAttack->LoadFromFile( "Sludge_SteamAttack" );
	SludgeFlyAway->LoadFromFile( "Sludge_FlyAway" );

	AnimationManager::GetInstance()->AddAnimation( SludgeIdle );
	AnimationManager::GetInstance()->AddAnimation( SludgePullLeverDown );
	AnimationManager::GetInstance()->AddAnimation( SludgeSpinAttack );
	AnimationManager::GetInstance()->AddAnimation( SludgeStartCrusherAttack );
	AnimationManager::GetInstance()->AddAnimation( SludgeFallCrusherAttack );
	AnimationManager::GetInstance()->AddAnimation( SludgeLandCrusherAttack );
	AnimationManager::GetInstance()->AddAnimation( SludgeSteamAttack );
	AnimationManager::GetInstance()->AddAnimation( SludgeFlyAway );

	// Sludges Ship Animations.
	Animation* SludgesShipIdle = new Animation();
	Animation* SludgesShipPullLeverDown = new Animation();
	Animation* SludgesShipSpinAttack	= new Animation();
	Animation* SludgesShipStartCrusherAttack = new Animation();
	Animation* SludgesShipFallCrusherAttack = new Animation();
	Animation* SludgesShipLandCrusherAttack = new Animation();
	Animation* SludgesShipSteamAttack = new Animation();
	Animation* SludgesShipIdleDamage = new Animation();

	SludgesShipIdle->LoadFromFile( "SludgesShip_Idle" );
	SludgesShipPullLeverDown->LoadFromFile( "SludgesShip_PullLeverDown" );
	SludgesShipSpinAttack->LoadFromFile( "SludgesShip_SpinAttack" );
	SludgesShipStartCrusherAttack->LoadFromFile( "SludgesShip_StartCrusherAttack" );
	SludgesShipFallCrusherAttack->LoadFromFile( "SludgesShip_FallCrusherAttack" );
	SludgesShipLandCrusherAttack->LoadFromFile( "SludgesShip_LandCrusherAttack" );
	SludgesShipSteamAttack->LoadFromFile( "SludgesShip_SteamAttack" );
	SludgesShipIdleDamage->LoadFromFile( "SludgesShip_IdleDamaged" );

	AnimationManager::GetInstance()->AddAnimation( SludgesShipIdle );
	AnimationManager::GetInstance()->AddAnimation( SludgesShipPullLeverDown );
	AnimationManager::GetInstance()->AddAnimation( SludgesShipSpinAttack );
	AnimationManager::GetInstance()->AddAnimation( SludgesShipStartCrusherAttack );
	AnimationManager::GetInstance()->AddAnimation( SludgesShipFallCrusherAttack );
	AnimationManager::GetInstance()->AddAnimation( SludgesShipLandCrusherAttack );
	AnimationManager::GetInstance()->AddAnimation( SludgesShipSteamAttack );
	AnimationManager::GetInstance()->AddAnimation( SludgesShipIdleDamage );

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
	Animation* SelectionMeshSpawnEffect = new Animation();
	Animation* SelectionMeshRotationEffect = new Animation();
	Animation* SpawnPipeDrip = new Animation();


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
	SelectionMeshSpawnEffect->LoadFromFile( "SelectionMesh_Spawn" );
	SelectionMeshRotationEffect->LoadFromFile( "SelectionMesh_Rotating" );
	SpawnPipeDrip->LoadFromFile( "SpawnPipeDrip_Drip" );

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
	AnimationManager::GetInstance()->AddAnimation( SelectionMeshSpawnEffect );
	AnimationManager::GetInstance()->AddAnimation( SelectionMeshRotationEffect );
	AnimationManager::GetInstance()->AddAnimation( SpawnPipeDrip );



#pragma endregion

#pragma region SLIMEMONSTER_ANIM
	Animation* SlimeMonsterIdle = new Animation();
	Animation* SlimeMonsterWalk = new Animation();
	Animation* SlimeMonsterThrowAttack = new Animation();
	Animation* SlimeMonsterSneakAttack = new Animation();
	Animation* SlimeMonsterStun = new Animation();
	Animation* SlimeMonsterDeath = new Animation();
	Animation* SlimeMonsterHide = new Animation();

	SlimeMonsterIdle->LoadFromFile( "SlimeMonster_Idle" );
	SlimeMonsterWalk->LoadFromFile( "SlimeMonster_Walk" );
	SlimeMonsterThrowAttack->LoadFromFile( "SlimeMonster_ThrowAttack" );
	SlimeMonsterSneakAttack->LoadFromFile( "SlimeMonster_SneakAttack" );
	SlimeMonsterStun->LoadFromFile( "SlimeMonster_Stun" );
	SlimeMonsterDeath->LoadFromFile( "SlimeMonster_Death" );
	SlimeMonsterHide->LoadFromFile( "SlimeMonster_Hide" );


	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterIdle );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterWalk );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterThrowAttack );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterSneakAttack );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterStun );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterDeath );
	AnimationManager::GetInstance()->AddAnimation( SlimeMonsterHide );
#pragma endregion

#pragma region LILGOO_ANIM
	Animation* LilGooWalk = new Animation();
	Animation* LilGooAttack = new Animation();

	Animation* LilGooFlying = new Animation();

	LilGooWalk->LoadFromFile( "LilGoo_Walk" );
	LilGooAttack->LoadFromFile( "LilGoo_Attack" );

	LilGooFlying->LoadFromFile( "LilGoo_Flying" );

	AnimationManager::GetInstance()->AddAnimation( LilGooWalk );
	AnimationManager::GetInstance()->AddAnimation( LilGooAttack );

	AnimationManager::GetInstance()->AddAnimation( LilGooFlying );
#pragma endregion

#pragma region HAZARD_ANIM
	Animation* Bridge = new Animation();
	Animation* BridgeIdle = new Animation();
	Animation* BridgeDownIdle = new Animation();
	Animation* DoorOpen = new Animation();
	Animation* DoorIdle = new Animation();
	Animation* DoorOpenIdle = new Animation();
	Animation* ButtonIdle = new Animation();
	Animation* ButtonDown = new Animation();

	Bridge->LoadFromFile( "Bridge_HalfRetract" );
	BridgeIdle->LoadFromFile( "Bridge_Idle" );
	BridgeDownIdle->LoadFromFile( "Bridge_HalfRetractIdle" );
	DoorOpen->LoadFromFile( "Door_Open" );
	DoorIdle->LoadFromFile( "Door_Idle" );
	DoorOpenIdle->LoadFromFile( "Door_OpenIdle" );
	ButtonIdle->LoadFromFile( "Button_Idle" );
	ButtonDown->LoadFromFile( "Button_GoingDown" );

	AnimationManager::GetInstance()->AddAnimation( Bridge );
	AnimationManager::GetInstance()->AddAnimation( BridgeIdle );
	AnimationManager::GetInstance()->AddAnimation( BridgeDownIdle );
	AnimationManager::GetInstance()->AddAnimation( DoorOpen );
	AnimationManager::GetInstance()->AddAnimation( DoorIdle );
	AnimationManager::GetInstance()->AddAnimation( DoorOpenIdle );
	AnimationManager::GetInstance()->AddAnimation( ButtonIdle );
	AnimationManager::GetInstance()->AddAnimation( ButtonDown );

#pragma endregion

#pragma region BACKGROUND_ANIM

	Animation* BoxesRun		= new Animation();
	Animation* BeltRun		= new Animation();
	Animation* ClawRun		= new Animation();
	Animation* FanRun		= new Animation();
	Animation* CrateClawRun	= new Animation();
	Animation* BossRun		= new Animation();

	BoxesRun->LoadFromFile( "Boxes_Run" );	
	BeltRun->LoadFromFile( "ConveyorBelt_Run" );	
	ClawRun->LoadFromFile( "Claw_Run" );	
	FanRun->LoadFromFile( "Blades_Run" );
	CrateClawRun->LoadFromFile( "CrateClaw_Movement" );
	BossRun->LoadFromFile( "BossMachine_Run" );

	AnimationManager::GetInstance()->AddAnimation( BoxesRun );
	AnimationManager::GetInstance()->AddAnimation( BeltRun );
	AnimationManager::GetInstance()->AddAnimation( ClawRun );
	AnimationManager::GetInstance()->AddAnimation( FanRun );
	AnimationManager::GetInstance()->AddAnimation( CrateClawRun );
	AnimationManager::GetInstance()->AddAnimation( BossRun );

#pragma endregion

#endif

	m_Timer.Reset();
	m_Timer.Start();
	ShowCursor( true );

	// Icon
	m_pIcon = CIcon::GetInstance();
	m_pIcon->SetIsOn( optionsToBeSet.bIcon );

	CObjectManager::GetInstance()->Init();
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
	CObjectManager::GetInstance()->Shutdown();
	CObjectManager::DeleteInstance();

	SaveOptions();

	while( !m_StateStack.empty() )
	{
		m_StateStack.back()->Shutdown();
		m_StateStack.pop_back();
	}

	m_Input.Shutdown();
	m_Audio.Shutdown();
	LevelManager::GetInstance()->Shutdown();
	CSplashScreenState::DeleteInstance();

	CParticleManager::GetInstance()->Shutdown();
	CParticleManager::GetInstance()->DeleteInstance();

	CObjectManager::DeleteInstance();
//	MultiThreadRender::GetInstance()->Shutdown();
	renderEngine.ShutdownRenderEngine();
	CGamePlayState::DeleteInstance();
	CMainMenuState::DeleteInstance();
	AnimationManager::DeleteInstance();
	LevelManager::GetInstance()->DeleteInstance();

	// Icon
	m_pIcon->DeleteInstance();
	m_pIcon = nullptr;
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
	m_pIcon->Update( fDeltaTime );
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

	CParticleManager::GetInstance()->Update( fDeltaTime );

	fDeltaTime = 1.0f/60.0f;
	if (m_StateStack.size())
	{
		m_StateStack.back()->Update(fDeltaTime);
	}
	
	LevelManager::GetInstance()->Update( fDeltaTime );
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
	if( m_bDeviceReset )
		return;

	if( CSplashScreenState::GetInstance()->GetRunning() || !CSplashScreenState::GetInstance()->GetStatus() )
		return;

	if( m_bLoading )
		return;

	if( renderEngine.GetDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET )
	{
		renderEngine.ResetDevice();
	}

	//static bool blah = true;
	//if( blah )
	//{
	//	renderEngine.ChangeDisplayParam(CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), !m_bFullScreen, true);
	//	blah = false;
	//	return;
	//}

	renderEngine.Clear();
	renderEngine.BeginScene();

	//CObjectManager::GetInstance()->Render();

	//put render engine stuff here
	renderEngine.Render();

	CParticleManager::GetInstance()->Render( renderEngine.GetCamera() );

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
//#if _DEBUG
		if((m_Input.KeyDown(DIK_LALT) || m_Input.KeyDown(DIK_RALT)) && 
			m_Input.KeyPressed(DIK_RETURN))
		{
			if(!m_bLoading)
			{
				SetFullScreen(!m_bFullScreen);
				renderEngine.ChangeDisplayParam(CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), GetFullScreen(), true);
				//m_bFullScreen = !m_bFullScreen;
				
				//ShowCursor(false);
				m_bDeviceReset = true;
			}
			return true;
		}
//#endif
	}

	if( m_Input.KeyPressed( DIK_F11 ) && !m_bLoading )
	{
		if( GetScreenWidth() == 1680 )
		{
			SetScreenWidth( 1024 );
			SetScreenHeight( 768 );
		}
		else
		{
			SetScreenWidth( 1680 );
			SetScreenHeight( 1050 );
		}
		RenderEngine::ChangeDisplayParam( GetScreenWidth(), GetScreenHeight(), GetFullScreen(), true );
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

	if( m_Input.KeyPressed( DIK_F1 ) )
	{
		if( m_StateStack.back() != CHelpState::GetInstance() )
			PushState( CHelpState::GetInstance() );
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
 	float PLAYER_MOVEMENT = 1500.0f;
 	float PLAYER_Rot = 25.0f;
 	if(m_Input.GetDIKCode() == DIK_LEFT)
 	{
 		cam.RotateLocalY(-D3DXToRadian(7.0f) * fDT * PLAYER_Rot);
 	}
 	if(m_Input.GetDIKCode() == DIK_RIGHT)
 	{
 		cam.RotateLocalY(D3DXToRadian(7.0f) * fDT * PLAYER_Rot);
 	}
 	if(m_Input.GetDIKCode() == DIK_UP)
 	{
 		cam.TranslateLocalZ(PLAYER_MOVEMENT * fDT);
 	}
 	if(m_Input.GetDIKCode() == DIK_DOWN)
 	{
 		cam.TranslateLocalZ(-PLAYER_MOVEMENT * fDT);
 	}
 	if(m_Input.GetDIKCode() == DIK_R)
 	{
 		cam.TranslateLocalY(PLAYER_MOVEMENT * fDT);
 	}
 	if(m_Input.GetDIKCode() == DIK_F)
 	{
 		cam.TranslateLocalY(-PLAYER_MOVEMENT * fDT);
 	}
 	if(m_Input.KeyPressed(DIK_1))
 		m_Dontmovecamera = !m_Dontmovecamera;

	//if(m_Input.KeyPressed(DIK_NUMPAD9))
		//SwapLevels();
}

void CGame::SwapLevels(char* szLevelName)
{
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//	MeshManager::GetInstance()->ShutdownMeshManager();
//	CObjectManager::GetInstance()->ClearAll();
//
//	RenderEngine::ClearData();
//
//	MeshManager::GetInstance()->InitMeshManager();
//	CObjectManager::GetInstance()->Init();
//
//	RenderEngine::BuildScreenMesh();
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//	m_nReticle = TextureManager::GetInstance()->LoadTexture("./Resources/Textures/T_Reticle_D.png");
//
//	LightManager::GetInstance()->ShutdownLightManager();
//
//	LightManager::GetInstance()->InitLightManager();
//
//	D3DXMATRIX i;
//	D3DXMatrixIdentity( &i );
//
//	LightManager::GetInstance()->LoadLights( "./Resources/LevelData/TestLights.bslights" );
//
//#if !_ANIMATIONS
//	// Player
//	MeshManager::GetInstance()->LoadMesh("PudgeShape", i);
//	MeshManager::GetInstance()->LoadMesh("PudgeArm", i);
//	MeshManager::GetInstance()->LoadMesh("SlimeMonster1", i);
//	MeshManager::GetInstance()->LoadMesh("LilGoo1", i);
//#else
//	// Player
//	MeshManager::GetInstance()->LoadAnimatedMesh("Pudge", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("VacuumArm", i);
//
//	// Enemies
//	MeshManager::GetInstance()->LoadAnimatedMesh("SlimeMonster", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("LilGoo", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("PullMesh", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("PushMesh", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("ImpactMesh", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("BigHurtMesh", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("LittleHurtMesh", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("JumpSteamStartMesh", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("JumpSteamFloorMesh", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("DeathCloud", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("SelectionMesh", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("SpawnPipeDrip", i);
//
//
//#endif
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//
//	MeshManager::GetInstance()->LoadMesh("Crate", i);
//	MeshManager::GetInstance()->LoadMesh("Goopit", i );
//	MeshManager::GetInstance()->LoadMesh("Cork", i );
//	MeshManager::GetInstance()->LoadMesh("SteamPipe", i );
//	MeshManager::GetInstance()->LoadMesh("BreakableWall", i );
//	MeshManager::GetInstance()->LoadMesh("Intake", i );
//	MeshManager::GetInstance()->LoadMesh("Plug", i );
//	MeshManager::GetInstance()->LoadMesh("SpawnPipe", i );
//	MeshManager::GetInstance()->LoadMesh("Crusher", i );
//	MeshManager::GetInstance()->LoadMesh("CrusherBase", i );
//
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//
//#if _ANIMATIONS
//	MeshManager::GetInstance()->LoadAnimatedMesh("Bridge", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh( "Door", i );
//#else
//	MeshManager::GetInstance()->LoadMesh("Bridge1", i);
//	MeshManager::GetInstance()->LoadMesh( "Door1", i );
//#endif
//
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//
//	MeshManager::GetInstance()->LoadMesh("Outlet", i);
//	MeshManager::GetInstance()->LoadMesh("WaterPool", i);
//	MeshManager::GetInstance()->LoadAnimatedMesh("Button", i);
//	MeshManager::GetInstance()->LoadMesh("Button1", i);
//	MeshManager::GetInstance()->LoadMesh("BoxSpawnPipe", i);
//
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//
//	std::string ScenePath = "./Resources/Models/";
//	ScenePath += szLevelName;
//	ScenePath += ".InfiniteHPScene";
//
//	MeshManager::GetInstance()->LoadEnvironment(ScenePath.c_str());
//
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//
//	std::string BoundingPath = "./Resources/CollisionData/";
//	BoundingPath += szLevelName;
//	BoundingPath += "_BoundingVolumes.InfiniteHPCollision";
//
//	MeshManager::GetInstance()->LoadCollisionEnvironment(BoundingPath.c_str());
//
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//
//	D3DXMATRIX _ObjectMatrix;
//
//	//D3DXMatrixIdentity( &_ObjectMatrix );
//	//D3DXMatrixTranslation( &_ObjectMatrix, -500.0f, 200.0f, 0.0f );
//	//CObjectManager::GetInstance()->CreateObject(OBJ_PULL_MESHEFFECT, _ObjectMatrix );
//
//	//D3DXMatrixIdentity( &_ObjectMatrix );
// // 	D3DXMatrixTranslation( &_ObjectMatrix, -600.0f, 200.0f, 0.0f );
//	//CObjectManager::GetInstance()->CreateObject(OBJ_PUSH_MESHEFFECT, _ObjectMatrix );
//
//	//D3DXMatrixIdentity( &_ObjectMatrix );
// //	CObjectManager::GetInstance()->CreateObject(OBJ_LAND_MESHEFFECT, _ObjectMatrix );
//
//	//D3DXMatrixIdentity( &_ObjectMatrix );
//	//CObjectManager::GetInstance()->CreateObject(OBJ_JUMP_MESHEFFECT, _ObjectMatrix );
//
//	//D3DXMatrixIdentity( &_ObjectMatrix );
//	//CObjectManager::GetInstance()->CreateObject(OBJ_BIGHIT_MESHEFFECT, _ObjectMatrix );
//
//	//D3DXMatrixIdentity( &_ObjectMatrix );
//	//CObjectManager::GetInstance()->CreateObject(OBJ_LITTLEHIT_MESHEFFECT, _ObjectMatrix );
//
//	std::string TrapPath = "./Resources/LevelData/TrapData/";
//	TrapPath += szLevelName;
//	TrapPath += "_Trap.InfiniteHPHazards";
//
//	std::string TrapDataPath = "./Resources/LevelData/TrapData/";
//	TrapDataPath += szLevelName;
//	TrapDataPath += "_Trap.InfiniteHPHazardData";
//
//	std::string ObjPath = "./Resources/LevelData/ObjectData/";
//	ObjPath += szLevelName;
//	ObjPath += "_MoveableObject.InfiniteHPMoveableObjects";
//
//	std::string ObjDataPath = "./Resources/LevelData/ObjectData/";
//	ObjDataPath += szLevelName;
//	ObjDataPath += "_MoveableObject.InfiniteHPMoveableObjectsData";
//
//	MeshManager::GetInstance()->LoadEnvironmentTraps(TrapPath.c_str(), TrapDataPath.c_str());
//
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//
//	MeshManager::GetInstance()->LoadEnvironmentMoveableObjects(ObjPath.c_str(), ObjDataPath.c_str());
//
//	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime());
//
//	while( CLoadingState::GetInstance()->GetStatus() )
//	{
//		CLoadingState::GetInstance()->Input();
//		CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_Timer.GetElapsedTime() );
//	}
//	CLoadingState::GetInstance()->Shutdown();
}

LoadedOptions CGame::LoadAndSetOptions(void)
{
	LoadedOptions options;
	char saveFolder[MAX_PATH] = {0};
	if( !SHGetSpecialFolderPath( GetDesktopWindow(), saveFolder, CSIDL_LOCAL_APPDATA, false ) )
		return options;
	strcat_s( saveFolder, "\\Double Stuffed Studios" );//\\Infinite HP\\Pudge and Sludge" );
	int direct = _mkdir(saveFolder);
	if( direct != 0 )
	{
		int err;
		_get_errno(&err);
		if( err != EEXIST )
			return options;
	}
	strcat_s( saveFolder, "\\InfiniteHP" );
	direct = _mkdir(saveFolder);
	if( direct != 0 )
{
		int err;
		_get_errno(&err);
		if( err != EEXIST )
			return options;
	}
	strcat_s( saveFolder, "\\Pudge and Sludge" );
	direct = _mkdir(saveFolder);
	if( direct != 0 )
	{
		int err;
		_get_errno(&err);
		if( err != EEXIST )
			return options;
	}
	strcat_s( saveFolder, "\\PSUserOptions.PSdat" );

	fstream fileIO(saveFolder, ios_base::binary | ios_base::in );
	if( fileIO.is_open() )
	{
		int reader = 0;
		fileIO.read( (char*)&reader, sizeof(int) );
		m_Audio.SetBGMVolume( reader );

		fileIO.read( (char*)&reader, sizeof(int) );
		m_Audio.SetFXVolume( reader );

		fileIO.read( (char*)&reader, sizeof(int) );
		m_Audio.SetDLGVolume( reader );

		fileIO.read( (char*)&options.fGamma, sizeof(float) );

		fileIO.read( (char*)&m_bFullScreen, sizeof(bool) );

		fileIO.read( (char*)&m_nScreenWidth, sizeof(short) );

		fileIO.read( (char*)&m_nScreenHeight, sizeof(short) );

		//Rueben
		//Icon
		fileIO.read( (char*)&options.bIcon, sizeof(bool) );
	}
	else
	{
		options.bFirstPlay = true;
	}

	fileIO.close();

	return options;
}

void CGame::SaveOptions(void)
{
	char saveFolder[MAX_PATH] = {0};
	if( !SHGetSpecialFolderPath( GetDesktopWindow(), saveFolder, CSIDL_LOCAL_APPDATA, false ) )
		return;
	strcat_s( saveFolder, "\\Double Stuffed Studios" );//\\Infinite HP\\Pudge and Sludge" );
	int direct = _mkdir(saveFolder);
	if( direct != 0 )
	{
		int err;
		_get_errno(&err);
		if( err != EEXIST )
			return;
	}
	strcat_s( saveFolder, "\\InfiniteHP" );
	direct = _mkdir(saveFolder);
	if( direct != 0 )
	{
		int err;
		_get_errno(&err);
		if( err != EEXIST )
			return;
	}
	strcat_s( saveFolder, "\\Pudge and Sludge" );
	direct = _mkdir(saveFolder);
	if( direct != 0 )
	{
		int err;
		_get_errno(&err);
		if( err != EEXIST )
			return;
	}
	strcat_s( saveFolder, "\\PSUserOptions.PSdat" );

	fstream fileIO(saveFolder, ios_base::binary | ios_base::out );

	if( fileIO.is_open() )
	{
		int toWrite = m_Audio.GetBGMVolume();
		fileIO.write( (char*)&toWrite, sizeof(int) );

		toWrite = m_Audio.GetFXVolume();
		fileIO.write( (char*)&toWrite, sizeof(int) );

		toWrite = m_Audio.GetDLGVolume();
		fileIO.write( (char*)&toWrite, sizeof(int) );

		float fGamma = RenderEngine::GetGamma();
		fileIO.write( (char*)&fGamma, sizeof(float) );

		fileIO.write( (char*)&m_bFullScreen, sizeof(bool) );

		fileIO.write( (char*)&m_nScreenWidth, sizeof(short) );

		fileIO.write( (char*)&m_nScreenHeight, sizeof(short) );

		// Rueben
		// Icon
		bool isOn = m_pIcon->GetIsOn();
		fileIO.write( (char*)&isOn, sizeof(bool) );
	}

	fileIO.close();
}
