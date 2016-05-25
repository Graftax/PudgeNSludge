#include "GamePlayState.h"
#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "CPauseState.h"
#include "CEndGameState.h"
#include "CWinGameState.h"
#include "CLoadingState.h"
#include "../../RenderEngine/TextureManager.h"

CGamePlayState* CGamePlayState::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CGamePlayState
// Purpose: Default Constructor.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CGamePlayState::CGamePlayState()
{
	m_pGame = nullptr;
	m_pInput = nullptr;
	m_pAudio = nullptr;
	m_bEndGame = false;
	m_bWinGame = false;
	particleMan = NULL;
	m_bOnTop = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ~CGamePlayState
// Purpose: Default Destructor.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CGamePlayState::~CGamePlayState()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetInstance
// Purpose: Returns the instance of the Main Menu.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
CGamePlayState* CGamePlayState::GetInstance ()
{
	if (!m_pInstance)
	{
		m_pInstance = new CGamePlayState;
	}
	//LOG(LOG_INFO, "CGamePlayState::GetInstance()", "CGamePlayState's instance creation succeeded");
	return m_pInstance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: DeleteInstance
// Purpose: Deletes the instance of the Main Menu.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayState::DeleteInstance ()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Init
// Purpose: Initializes the state.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	JM
// Last Modification Date:	6/1/12
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayState::Init ()
{
	/*matrix4f posmat;
	posmat.make_identity();
	posmat.axis_pos = vec3f( -100.0f, 10.0f, 200.0f );*/

	//
	m_cTimer.Start();
	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_cTimer.GetElapsedTime());
	m_bWinGameFade = false;
	m_fWindGameFadeFactor = 0.0f;
	m_pWinGameTexture = nullptr;
	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_cTimer.GetElapsedTime() );
	m_pWinGameTexture = TextureManager::GetInstance()->GetTexture( TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_Win.png" ) );

	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pAudio = m_pGame->GetAudioManager();
	m_pAudio->PlayMusic(BGM_GAMEPLAY_LEVEL1);
	CObjectManager::GetInstance()->Init();
	m_pCamera = m_pGame->GetCamera();
	m_pCamera->Initialize();
	particleMan = CParticleManager::GetInstance();//new CParticleManager();
	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_cTimer.GetElapsedTime() );
	m_cHUD.Init();

	m_pInput->OffSetMouseForStart();

	float fov = D3DX_PI/3.0f;
	float _aspect = SCREEN_WIDTH/(float)(SCREEN_HEIGHT);
	m_pCamera->BuildPerspective( fov, _aspect, 0.1f, 10000.0f);
	m_pCamera->SetViewPosition(0.0f, 0.0f, -400.0f);
	RenderEngine::SetCamera( m_pCamera );

	// Set the Camera and build the frustum.
	//RenderEngine::SetCamera( m_pCamera );

	MeshManager::GetInstance()->LoadCollisionEnvironment("./Resources/CollisionData/Sector1_BoundingVolumes.InfiniteHPCollision");
	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_cTimer.GetElapsedTime() );
	//MeshManager::GetInstance()->LoadCollisionEnvironment("./Resources/CollisionData/Sector2_BoundingVolumes.InfiniteHPCollision");

	particleMan->LoadParticles("./Resources/Effects/PullEffects.bin");
	particleMan->LoadParticles("./Resources/Effects/pushEffect.bin");
	particleMan->LoadParticles("./Resources/Effects/SteamPipeVfx.bin");
	particleMan->LoadParticles("./Resources/Effects/runDust.bin");
	particleMan->LoadParticles("./Resources/Effects/JumpSteamStart.bin");
	particleMan->LoadParticles("./Resources/Effects/SmallDamageVfx.bin");
	particleMan->LoadParticles("./Resources/Effects/LargeDamageVfx.bin");
    particleMan->LoadParticles("./Resources/Effects/lilGooExplode.bin" );
    particleMan->LoadParticles( "./Resources/Effects/SlimeThrownTrail.bin" );
	particleMan->LoadParticles( "./Resources/Effects/CrusherSparks.bin" );

	particleMan->CreateVertexBuffer(RenderEngine::m_pD3Dev);
	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_cTimer.GetElapsedTime() );
	D3DXMATRIX _ObjectMatrix, rot;

	D3DXMatrixIdentity( &_ObjectMatrix );
	D3DXMatrixTranslation( &_ObjectMatrix, -800.0f, -50.0f, -300.0f );//Sector1
	//D3DXMatrixTranslation( &_ObjectMatrix, -100.0f, -50.0f, -300.0f );//Sector2
	CObjectManager::GetInstance()->CreateObject(OBJ_PLAYER, _ObjectMatrix );

 	D3DXMatrixIdentity( &_ObjectMatrix );
 	D3DXMatrixTranslation( &_ObjectMatrix, -500.0f, 200.0f, -300.0f );

	//CObjectManager::GetInstance()->CreateObject(OBJ_PULL_MESHEFFECT, _ObjectMatrix );
	//CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_cTimer.GetElapsedTime() );
	
	//D3DXMatrixIdentity( &_ObjectMatrix );
	//D3DXMatrixTranslation( &_ObjectMatrix, -600.0f, 200.0f, -300.0f );
	//CObjectManager::GetInstance()->CreateObject(OBJ_PUSH_MESHEFFECT, _ObjectMatrix );

	D3DXMatrixIdentity( &_ObjectMatrix );
 	CObjectManager::GetInstance()->CreateObject(OBJ_LAND_MESHEFFECT, _ObjectMatrix );

	D3DXMatrixIdentity( &_ObjectMatrix );

	CObjectManager::GetInstance()->CreateObject(OBJ_JUMP_MESHEFFECT, _ObjectMatrix );
	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_cTimer.GetElapsedTime() );

//	D3DXMatrixIdentity( &_ObjectMatrix );
//	CObjectManager::GetInstance()->CreateObject(OBJ_BIGHIT_MESHEFFECT, _ObjectMatrix );

// 	D3DXMatrixIdentity( &_ObjectMatrix );
// 	CObjectManager::GetInstance()->CreateObject(OBJ_LITTLEHIT_MESHEFFECT, _ObjectMatrix );

	D3DXMatrixIdentity( &_ObjectMatrix );

	CObjectManager::GetInstance()->CreateObject(OBJ_PLAYER_ARM, _ObjectMatrix );
	CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_cTimer.GetElapsedTime() );
	matrix4f CrusherMatrix;
	CrusherMatrix.make_identity();
	CrusherMatrix.translate_pre( -130.0f, 700.0f, -300.0f);
	CrusherMatrix.rotate_x_pre( D3DXToRadian(-90) );

	CObjectManager::GetInstance()->CreateObject( OBJ_CRUSHER, CrusherMatrix.ma );

	D3DXMatrixIdentity( &_ObjectMatrix );
	D3DXMatrixTranslation( &_ObjectMatrix, 3400.0f, 300.0f, -300.0f );
	CObjectManager::GetInstance ()->CreateObject (OBJ_MONSTER, _ObjectMatrix);
	// TODO
	// Get this loaded in by the level editor
	D3DXMatrixIdentity( &rot );
	D3DXMatrixRotationY( &rot, D3DXToRadian(90) );
	D3DXMATRIX rotZ;
	D3DXMatrixRotationZ( &rotZ, D3DXToRadian(90) );
	D3DXMatrixMultiply( &rot, &rotZ, &rot );
	D3DXMatrixIdentity( &_ObjectMatrix );
	D3DXMatrixTranslation( &_ObjectMatrix, 2975.0f, 244.0f, -407.0f );
	D3DXMatrixMultiply( &_ObjectMatrix, &rot, &_ObjectMatrix );
	CObjectManager::GetInstance()->CreateObject( OBJ_DOOR, _ObjectMatrix );

CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_cTimer.GetElapsedTime() );
	
	// Had to call it twice to get rid of the cursor 
	ShowCursor( FALSE );

	MeshManager::GetInstance()->LoadEnvironmentTraps("./Resources/LevelData/TrapData/Sector1_Trap.InfiniteHPHazards", "./Resources/LevelData/TrapData/Sector1_Trap.InfiniteHPHazardData");
	MeshManager::GetInstance()->LoadEnvironmentMoveableObjects("./Resources/LevelData/ObjectData/Sector1_MoveableObject.InfiniteHPMoveableObjects");
while( CLoadingState::GetInstance()->GetStatus() )
	{
		CLoadingState::GetInstance()->Input();
		CLoadingState::GetInstance()->ThreadUpdate( MAX_LOADING_TIME - (float)m_cTimer.GetElapsedTime() );
	}
	CLoadingState::GetInstance()->Shutdown();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ShutDown
// Purpose: Shuts down the state
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayState::Shutdown ()
{
	m_pAudio->StopMusic(BGM_GAMEPLAY_LEVEL1);
	particleMan->DeleteInstance();
	CObjectManager::GetInstance()->ClearAll();
	CObjectManager::GetInstance()->Shutdown();
	CObjectManager::DeleteInstance();
	DeleteInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Input
// Purpose: Gets input to update the state.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	JM
// Last Modification Date:	6/1/12
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGamePlayState::Input ()
{
	if( CGame::GetInstance()->GetLoading() )
		return true;

	// How do I get input?
	if(!m_pInput)
	{
		return false;
	}

	if(m_pInput->KeyPressed(DIK_ESCAPE))
	{
		m_pAudio->StopMusic(BGM_GAMEPLAY_LEVEL1);
		m_pAudio->PauseAll();
		m_pAudio->PlayEffect(SFX_MENU_PAUSE);
		m_pGame->PushState(CPauseState::GetInstance());
		m_bOnTop = false;
	}

	// SPAWNING KEYS DISABLED
	//if (m_pInput->KeyPressed (DIK_X))
	//{
	//	//m_bWinGameFade = true;
	//	D3DXMATRIX _ObjectMatrix;
	//	D3DXMatrixIdentity( &_ObjectMatrix );
	//	D3DXMatrixTranslation( &_ObjectMatrix, -200.0f, 400.0f, -300.0f );
	//	CObjectManager::GetInstance ()->CreateObject (OBJ_GOO, _ObjectMatrix);
	//}

	//if (m_pInput->KeyPressed (DIK_Z))
	//{
	//	D3DXMATRIX _ObjectMatrix;
	//	D3DXMatrixIdentity( &_ObjectMatrix );
	//	D3DXMatrixTranslation( &_ObjectMatrix, -500.0f, 400.0f, -300.0f );
	//	CObjectManager::GetInstance ()->CreateObject (OBJ_MONSTER, _ObjectMatrix);
	//}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update
// Purpose: Updates the state.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayState::Update (float fElapsedTime)
{
	if( m_fWindGameFadeFactor > 1.0f )
		m_bWinGame = true;
	if(m_bOnTop == false)
	{
		//if I wasn't on top I'm now on top and playing music
		m_bOnTop = true;
		m_pAudio->PlayMusic(BGM_GAMEPLAY_LEVEL1);
	}

	m_cHUD.Update( fElapsedTime );
	D3DXMATRIX i;
	D3DXMatrixIdentity( &i );
	RenderEngine::BuildFrustum( D3DX_PI/3.0f, 0.1f, 2000.0f, (SCREEN_WIDTH/(float)(SCREEN_HEIGHT)), /*(*(matrix4f*)&i)*/(*(matrix4f*)&m_pCamera->GetWorldMatrix()) );
	// what system do I need for swapping the textures
	// for every option
	if( !m_bEndGame && !m_bWinGame )
	{
		CObjectManager::GetInstance()->Update(fElapsedTime);

		m_pGame->MoveCamera(fElapsedTime);

		if(m_pGame->GetDontMoveCamera() == false)
		{
			IBaseObject* pPlayer = CObjectManager::GetInstance()->GetPlayer();

			if( pPlayer )
			{
				m_pCamera->MoveCamWithMouse(pPlayer->GetWorldPos(), m_pGame->GetHWND());
			}
		}

		m_pGame->GetRenderEngine()->SetModelViewProjection( m_pCamera->GetViewMatrix() * m_pCamera->GetProjectionMatrix() );
		m_pGame->GetRenderEngine()->SetCamera( m_pCamera );
		
		if(m_pAudio && m_pCamera)
		{
			m_pAudio->SetListenerPosition(vec3f(	m_pCamera->GetViewPosition().x, 
													m_pCamera->GetViewPosition().y, 
													0.0f));
		}
		if( m_bWinGameFade )
			m_fWindGameFadeFactor += fElapsedTime * 0.40f;
	}
	else if( m_bEndGame )
	{
		m_pAudio->StopMusic(BGM_GAMEPLAY_LEVEL1);
		//I'm no longer on top
		m_bOnTop = false;
		CGame::GetInstance()->ChangeState( CEndGameState::GetInstance() );
	}
	else
	{
		//I'm no longer on top
		m_bOnTop = false;
		CGame::GetInstance()->ChangeState( CWinGameState::GetInstance() );
	}

	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Render
// Purpose: Renders the state;
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CGamePlayState::Render ()
{
	if( CGame::GetInstance()->GetLoading() )
		return;
	// How do I Render my stuff?
	CObjectManager::GetInstance()->Render();
	m_cHUD.Render();

	if( m_bWinGameFade )
	{
		RenderEngine::RasterTexture( m_pWinGameTexture, 0.5f - (m_fWindGameFadeFactor / 2.0f), 0.5f - (m_fWindGameFadeFactor / 2.0f), 0.5f + (m_fWindGameFadeFactor / 2.0f), 0.5f + (m_fWindGameFadeFactor / 2.0f), m_fWindGameFadeFactor );		
	}
}