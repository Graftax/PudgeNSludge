#include "GamePlayState.h"
#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "CPauseState.h"
#include "CEndGameState.h"
#include "CWinGameState.h"
#include "CLoadingState.h"
#include "../../RenderEngine/TextureManager.h"
#include "../../LevelManager/CLevelManager.h"

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
	m_bOnTop = false;
	m_nPrevLevel = 0;

	HasLevelBeenReset = false;
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
	//
	m_cTimer.Start();
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime());
	m_bWinGameFade = false;
	m_fWindGameFadeFactor = 0.0f;
	m_pWinGameTexture = TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_Win.png" );;
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime() );
	//m_pWinGameTexture = /*TextureManager::GetInstance()->GetTexture(*/ TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_Win.png" );// );

	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pAudio = m_pGame->GetAudioManager();
	//m_pAudio->PlayMusic(BGM_GAMEPLAY_LEVEL1);
	m_pCamera = m_pGame->GetCamera();
	m_pCamera->Initialize();
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime() );
	m_cHUD.Init();

	m_pInput->OffSetMouseForStart();

	float fov = D3DX_PI/3.0f;
	float _aspect = CGame::GetInstance()->GetScreenWidth()/(float)(CGame::GetInstance()->GetScreenHeight());
	m_pCamera->BuildPerspective( fov, _aspect, 0.1f, 10000.0f);
	m_pCamera->SetViewPosition(0.0f, 0.0f, -400.0f);
	RenderEngine::SetCamera( m_pCamera );

	// Set the Camera and build the frustum.
	//RenderEngine::SetCamera( m_pCamera );

	// this loads up the first sector
	LevelManager* _lvlMng = LevelManager::GetInstance();

	// Load all the render nodes for all the levels.
	_lvlMng->LoadAllLevelsToRender();
	// Then load all the objects for level 1 and its next level.
	_lvlMng->LoadLevel();
	_lvlMng->SetLoadNextLevel( true );
	_lvlMng->LoadNextLevel();
	// Set the current levels time to 1.0f so it starts with lights on.
	_lvlMng->GetCurrentLevel()->GetLightManager().SetTimeToRender( 0.5f );

	m_nPrevLevel = 0;

	/*D3DXMatrixIdentity( &_ObjectMatrix );
 	CObjectManager::GetInstance()->CreateObject(OBJ_LAND_MESHEFFECT, _ObjectMatrix );*/
	CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime() );

	
	// Had to call it twice to get rid of the cursor 
	ShowCursor( FALSE );

	while( !CLoadingState::GetInstance()->GetStatus() )
	{
		CLoadingState::GetInstance()->Input();
		CLoadingState::GetInstance()->Update( MAX_LOADING_TIME - (float)CLoadingState::GetInstance()->GetTimer().GetElapsedTime() );
	}
	CLoadingState::GetInstance()->Shutdown();

	m_pInput->ResetIgnoreTimer();
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
	//TextureManager::GetInstance()->UnloadTexture( m_pWinGameTexture );

	//m_pAudio->StopMusic(AK::EVENTS::STOP_BGM_GAMEPLAY_LEVEL1);
	LevelManager::GetInstance()->Shutdown();
	CObjectManager::GetInstance()->ClearAll();
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
		if(m_pAudio)
		{
			m_pAudio->StopEffect(AK::EVENTS::STOP_SFX_AMBIENCE);

			//m_pAudio->StopMusic(AK::EVENTS::STOP_BGM_GAMEPLAY_LEVEL1);
			m_pAudio->PauseAll();
			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_PAUSE);
			m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
			m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_CHECKPOINT);
			m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
			m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
			m_pGame->PushState(CPauseState::GetInstance());
		}
		m_bOnTop = false;
	}

	//** Le Haxxors **//
	//if( m_pInput->KeyPressed( DIK_LSHIFT ) )
	//{
	//	IBaseObject* player = CObjectManager::GetInstance()->GetPlayer();
	//
	//	if( m_pCamera )
	//		player->GetFrame().GetLocalMat().axis_pos = m_pCamera->GetWorldCursor();
	//	//player->GetFrame().GetLocalMat().axis_pos.x += 500.0f;
	//	player->SetLocalPos( &player->GetFrame().GetLocalMat().axis_pos );
	//}
	//if( m_pInput->KeyPressed( DIK_V ) )
	//{
	//	IBaseObject* player = CObjectManager::GetInstance()->GetPlayer();
	//	player->GetFrame().GetLocalMat().axis_pos.x -= 500.0f;
	//	player->SetLocalPos( &player->GetFrame().GetLocalMat().axis_pos );
	//}
	//
	//if( m_pInput->KeyPressed( DIK_Z ) )
	//{
	//	/*IBaseObject* player = CObjectManager::GetInstance()->GetPlayer();
	//	player->GetFrame().GetLocalMat().axis_pos.x = 17000.0f;
	//	player->GetFrame().GetLocalMat().axis_pos.y = 600.0f;
	//	player->SetLocalPos( &player->GetFrame().GetLocalMat().axis_pos );*/
	//	LevelManager* pLvlMng = LevelManager::GetInstance();
	//
	//	// Unload first level and load up the next level.
	//	pLvlMng->SetUnloadFirstLevel( true );
	//	pLvlMng->UnloadFirstActiveLevel();
	//	pLvlMng->SetLoadNextLevel( true );
	//	pLvlMng->LoadNextLevel();
	//
	//	IBaseObject* player = CObjectManager::GetInstance()->GetPlayer();
	//	player->GetFrame().GetLocalMat().axis_pos = pLvlMng->GetCurrentLevel()->GetPlayerSpawnPos();
	//	player->GetFrame().Update();
	//}
	//** End le haxxors **//

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
	m_pInput->IgnoreInputForSetAmountOfTime( fElapsedTime, 0.2f );
	LevelManager* LvlMng = LevelManager::GetInstance();

	if( m_fWindGameFadeFactor > 1.0f )
		m_bWinGame = true;
	if(m_bOnTop == false)
	{
		//if I wasn't on top I'm now on top and playing music
		m_bOnTop = true;

		if(m_pAudio)
		{
			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_AMBIENCE);

			switch(LvlMng->GetCurrentLevelIndex())
			{
			case 0:
				{
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL1);
				}
				break;
			case 1:
				{
					m_pAudio->UnMuteMusic(AK::EVENTS::UNMUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL1);
				}
				break;
			case 2:
				{
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL2);
				}
				break;
			case 3:
				{
					m_pAudio->UnMuteMusic(AK::EVENTS::UNMUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL2);
				}
				break;
			case 4:
				{
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVELBOSS);
				}
				break;
			default:
				{
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL1);
				}
				break;
			}
		}
	}

	//if I'm in the game and the level changes, change the music
	if(m_nPrevLevel != LvlMng->GetCurrentLevelIndex())
	{
		if(m_pAudio)
		{
			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_AMBIENCE);

			switch( LvlMng->GetCurrentLevelIndex() )
			{
			case 0:
				{
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL1);
				}
				break;
			case 1:
				{
					m_pAudio->UnMuteMusic(AK::EVENTS::UNMUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL1);
				}
				break;
			case 2:
				{
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL2);
				}
				break;
			case 3:
				{
					m_pAudio->UnMuteMusic(AK::EVENTS::UNMUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL2);
				}
				break;
			case 4:
				{
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVELBOSS);
				}
				break;
			default:
				{
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_CHECKPOINT);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
					m_pAudio->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK);
					m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL1);
				}
				break;
			}
		}

		m_nPrevLevel = LevelManager::GetInstance()->GetCurrentLevelIndex();
	}
		

	m_cHUD.Update( fElapsedTime );
	D3DXMATRIX i;
	D3DXMatrixIdentity( &i );
	RenderEngine::BuildFrustum( D3DX_PI/3.0f, 0.1f, 3000.0f, (CGame::GetInstance()->GetScreenWidth()/(float)(CGame::GetInstance()->GetScreenHeight())), (*(matrix4f*)&m_pCamera->GetWorldMatrix()) );
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
		//m_pAudio->StopMusic(AK::EVENTS::STOP_BGM_GAMEPLAY_LEVEL1);
		////I'm no longer on top
		//m_bOnTop = false;
		//CGame::GetInstance()->ChangeState( CEndGameState::GetInstance() );
		IBaseObject* pPlayer = CObjectManager::GetInstance()->GetPlayer();

		if( !HasLevelBeenReset )
		{
			LevelManager::GetInstance()->ResetCurrentLevel();
			m_pCamera->SetInterpolateCameraPosition();
			m_pCamera->SetInterpolateFuturePosition( pPlayer->GetFrame().GetWorldMat().axis_pos );

			HasLevelBeenReset = true;
		}

		if( HasLevelBeenReset )
		{
			if( m_pCamera->InterpolateCameraOverTime(fElapsedTime, pPlayer->GetFrame().GetWorldMat().axis_pos ) )
			{
				HasLevelBeenReset = false;
				m_bEndGame = false;
			}
		}

		m_pGame->GetRenderEngine()->SetModelViewProjection( m_pCamera->GetViewMatrix() * m_pCamera->GetProjectionMatrix() );
		m_pGame->GetRenderEngine()->SetCamera( m_pCamera );


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
		RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_pWinGameTexture), 0.5f - (m_fWindGameFadeFactor / 2.0f), 0.5f - (m_fWindGameFadeFactor / 2.0f), 0.5f + (m_fWindGameFadeFactor / 2.0f), 0.5f + (m_fWindGameFadeFactor / 2.0f), m_fWindGameFadeFactor );		
	}
}