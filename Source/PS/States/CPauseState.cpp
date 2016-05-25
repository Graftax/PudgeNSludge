#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "../../RenderEngine/RenderEngine.h"
#include "../../RenderEngine/TextureManager.h"
#include "CPauseState.h"
#include "COptionsState.h"
#include "MainMenuState.h"

enum SCREEN_RES
{
	RES_1024 = 1024,
	RES_1280 = 1280,
	RES_1920 = 1920,
};

CPauseState* CPauseState::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CPauseState
// Purpose: Default Constructor.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CPauseState::CPauseState()
{
	m_pIcon = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ~CPauseState
// Purpose: Default Destructor.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CPauseState::~CPauseState()
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
CPauseState* CPauseState::GetInstance ()
{
	if (!m_pInstance)
	{
		m_pInstance = new CPauseState;
	}
	LOG(LOG_INFO, "CPauseState::GetInstance()", "CPauseState's instance creation succeeded");
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
void CPauseState::DeleteInstance ()
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
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CPauseState::Init ()
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pAudio = m_pGame->GetAudioManager();
	soundTimer = 0.0f;
	m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_MENU);

	TextureManager* pTM = TextureManager::GetInstance();

	m_nCurrentlySelected = P_RESUME;

	m_lpResume			= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_Pause_Resume.png" );// );
	m_lpOptions			= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_Pause_Options.png" );// );
	m_lpExit			= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_Pause_Exit.png" );// );
	m_nReticle			= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/T_Reticle_D.png");//);

	m_msPause.Initialize( "pause.html" );

	// Icon
	m_pIcon = CIcon::GetInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ShutDown
// Purpose: Shuts down the state
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CPauseState::Shutdown ()
{
	TextureManager* pTM = TextureManager::GetInstance();
	pTM->UnloadTexture(m_lpResume);
	pTM->UnloadTexture(m_lpOptions);
	pTM->UnloadTexture(m_lpExit);
	pTM->UnloadTexture(m_nReticle);

	CGame::GetInstance()->SaveOptions();
	m_msPause.Shutdown();
	m_pIcon->DeleteInstance();
	m_pIcon = nullptr;

	DeleteInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Input
// Purpose: Gets input to update the state.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CPauseState::Input ()
{
	// How do I get input?
	if(!m_pInput)
	{
		return false;
	}

	//if(m_pInput->KeyPressed(DIK_ESCAPE))
	//{
	//	m_pAudio->ResumeAll();
	//	m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_CANCEL);
	//	m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_UNPAUSE);
	//	m_pGame->PopState();
	//	return true;
	//}
	////Keyboard Input
	//if(m_pInput->KeyPressed(DIK_UP) || m_pInput->KeyPressed(DIK_W))
	//{
	//	m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);

	//	//move cursor up
	//	--m_nCurrentlySelected;

	//	if(m_nCurrentlySelected < P_RESUME)
	//	{
	//		m_nCurrentlySelected = P_EXIT;
	//	}
	//}

	//if(m_pInput->KeyPressed(DIK_DOWN) || m_pInput->KeyPressed(DIK_S))
	//{
	//	m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);

	//	//move cusor down
	//	++m_nCurrentlySelected;

	//	if(m_nCurrentlySelected > P_EXIT)
	//	{
	//		m_nCurrentlySelected = P_RESUME;
	//	}
	//}

	// Rueben's debug
	/*if( m_pInput->CheckBufferedLeftMouseButtonEx() )
	{
		char buffer[64];
		sprintf_s(buffer, "x-%f | y-%f ", _mousePos.x, _mousePos.y );
		LOG(LOG_DEBUG, "MainMenu::Input()", buffer);
	}	*/								

	//if(m_pInput->KeyPressed(DIK_RETURN) || 
	//	m_pInput->KeyPressed(DIK_SPACE) ||
	//	m_pInput->LeftMouseButtonPressed() )
	//{
	//	//enter into the next menu item
	//	switch(m_nCurrentlySelected)
	//	{
	//	case P_RESUME:
	//		{
	//			//change state to gameplay state
	//			m_pAudio->ResumeAll();
	//			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_UNPAUSE);
	//			m_pGame->PopState();
	//			return true;
	//			break;
	//		}
	//	case P_OPTIONS:
	//		{
	//			//change state to the options state
	//			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
	//			CGame::GetInstance()->PushState(COptionsState::GetInstance());
	//			break;
	//		}
	//	case P_EXIT:
	//		{
	//			//change state to the credits state
	//			/*UNCOMMENT THIS IN WHEN CREDITSSTATE IS ADDED*/
	//			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
	//			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	//			break;
	//		}
	//	default:
	//		{
	//			break;
	//		}
	//	}
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
void CPauseState::Update (float fElapsedTime)
{
	// what system do I need for swapping the textures
	// for every option

	//POINT ptCursor;
	//GetCursorPos(&ptCursor);
	//ScreenToClient(CGame::GetInstance()->GetHWND(), &ptCursor);
	//
	//_mousePos = D3DXVECTOR4((FLOAT)ptCursor.x, 768.0f - (FLOAT)ptCursor.y, 1.0f, 1.0f );
	//_mousePos /= _mousePos.w + 400;
	if(!m_pInput)
	{
		return;
	}

	soundTimer += fElapsedTime;

	//Keyboard Input

	_mousePos = D3DXVECTOR2((float)m_pInput->GetMousePosX(), (float)-m_pInput->GetMousePosY() + CGame::GetInstance()->GetScreenHeight());

	int OldFX = m_pAudio->GetFXVolume();
	int OldDiag = m_pAudio->GetDLGVolume();

	m_msPause.SetValue( "EffectVol", (float)m_pAudio->GetFXVolume() );
	m_msPause.SetValue( "MusicVol", (float)m_pAudio->GetBGMVolume() );
	m_msPause.SetValue( "DialogueVol", (float)m_pAudio->GetDLGVolume() );
	m_msPause.SetValue( "Gamma", ( RenderEngine::GetGamma() + 0.5f ) * 100.0f );

	if( m_pGame->GetFullScreen() )
	{
		m_msPause.SetValue( "fullscreen", 1.0f );
		m_msPause.SetActive( true, "Gamma" );
	}
	else
	{
		m_msPause.SetValue( "fullscreen", 0.0f );
		m_msPause.SetActive( false, "Gamma" );
	}
	if( m_pIcon->GetIsOn() )
	{
		m_msPause.SetValue( "tutorials", 1.0f );
	}
	else
	{
		m_msPause.SetValue( "tutorials", 0.0f );
	}

	m_msPause.Update( fElapsedTime, (int)(_mousePos.x), (int)(m_pInput->GetMousePosY()) );

	if( m_pInput->LeftMouseButtonDown() )
		m_msPause.MouseDown();

	if( m_pInput->KeyPressed( DIK_SPACE ) || m_pInput->KeyPressed( DIK_RETURN ) )
	{
		m_msPause.Activate();
	}

	if( m_pInput->KeyPressed( DIK_BACKSPACE ) )
	{
		m_msPause.Back();
	}

	if(m_pInput->KeyPressed(DIK_UP) || m_pInput->KeyPressed(DIK_W) )
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
		m_msPause.Move( CMSD_UP );
	}

	if(m_pInput->KeyPressed(DIK_DOWN) || m_pInput->KeyPressed(DIK_S))
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
		m_msPause.Move( CMSD_DOWN );
	}

	if(m_pInput->KeyPressed(DIK_LEFT) || m_pInput->KeyPressed(DIK_A))
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
		m_msPause.Move( CMSD_LEFT );
	}
	if(m_pInput->KeyPressed(DIK_RIGHT) || m_pInput->KeyPressed(DIK_D))
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
		m_msPause.Move( CMSD_RIGHT );
	}

	m_msPause.RecalcValues();

	m_pAudio->SetFXVolume( (int)m_msPause.GetValue( "EffectVol" ) );
	m_pAudio->SetBGMVolume( (int)m_msPause.GetValue( "MusicVol" ) );
	m_pAudio->SetDLGVolume( (int)m_msPause.GetValue( "DialogueVol" ) );
	RenderEngine::SetGamma( ( m_msPause.GetValue( "Gamma" ) / 100.0f ) - 0.5f );

	if( m_msPause.GetValue( "fullscreen" ) > 0 )
	{
		if( !m_pGame->GetFullScreen() )
		{
			m_pGame->SetFullScreen( true );
			RenderEngine::ChangeDisplayParam( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), m_pGame->GetFullScreen(), true );
			m_msPause.SetActive( true, "Gamma" );
		}
	}
	else
	{
		if( m_pGame->GetFullScreen() )
		{
			m_pGame->SetFullScreen( false );
			RenderEngine::ChangeDisplayParam( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), m_pGame->GetFullScreen(), true );
			m_msPause.SetActive( false, "Gamma" );
		}
	}

	if( m_msPause.GetValue( "1024" ) > 0 && m_pGame->GetScreenWidth() != 1024 )
	{
		m_pGame->SetScreenWidth( 1024 );
		m_pGame->SetScreenHeight( 768 );
		RenderEngine::ChangeDisplayParam( m_pGame->GetScreenWidth(), m_pGame->GetScreenHeight(), m_pGame->GetFullScreen(), true );
		m_msPause.SetValue( "1280", 0.0f );
		m_msPause.SetValue( "1920", 0.0f );
	}

	if( m_msPause.GetValue( "1280" ) > 0 && m_pGame->GetScreenWidth() != 1280 )
	{
		m_pGame->SetScreenWidth( 1280 );
		m_pGame->SetScreenHeight( 720 );
		RenderEngine::ChangeDisplayParam( m_pGame->GetScreenWidth(), m_pGame->GetScreenHeight(), m_pGame->GetFullScreen(), true );
		m_msPause.SetValue( "1024", 0.0f );
		m_msPause.SetValue( "1920", 0.0f );
	}

	if( m_msPause.GetValue( "1920" ) > 0 && m_pGame->GetScreenWidth() != 1920 )
	{
		m_pGame->SetScreenWidth( 1920 );
		m_pGame->SetScreenHeight( 1080 );
		RenderEngine::ChangeDisplayParam( m_pGame->GetScreenWidth(), m_pGame->GetScreenHeight(), m_pGame->GetFullScreen(), true );
		m_msPause.SetValue( "1280", 0.0f );
		m_msPause.SetValue( "1024", 0.0f );
	}

	if( m_msPause.GetValue( "tutorials" ) > 0 )
	{
		if( !m_pIcon->GetIsOn() )
		{
			m_pIcon->SetIsOn( true );
		}
	}
	else
	{
		if( m_pIcon->GetIsOn() )
		{
			m_pIcon->SetIsOn( false );
		}
	}

	if( m_msPause.GetValue( "RESET" ) == 1.0f )
	{
		m_msPause.SetValue( "RESET", 0.0f );
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
		//TODO
		// Change to Audio Options
		m_pGame->GetAudioManager()->SetBGMVolume( 100 );
		m_pGame->GetAudioManager()->SetDLGVolume( 100 );
		m_pGame->GetAudioManager()->SetFXVolume( 100 );
		RenderEngine::SetGamma( 0.0f );

#if _DEBUG
		m_pGame->SetFullScreen( false );
		m_msPause.SetActive( false, "Gamma" );
#else
		m_pGame->SetFullScreen( true );
		m_msPause.SetActive( true, "Gamma" );
#endif
		RenderEngine::ChangeDisplayParam( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), !m_pGame->GetFullScreen(), true );
	}

	if( soundTimer > 0.5f )
	{
		if( m_pAudio->GetFXVolume() != OldFX )
		{
			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_CRATE_COLLIDE);
			soundTimer = 0.0f;
		}
		if( m_pAudio->GetDLGVolume() != OldDiag )
		{
			m_pAudio->PlayDialogue(AK::EVENTS::PLAY_DLG_PLAYER_HURT_SMALL);
			soundTimer = 0.0f;
		}
	}

	_mousePos.x = _mousePos.x / CGame::GetInstance()->GetScreenWidth();
	_mousePos.y = _mousePos.y / CGame::GetInstance()->GetScreenHeight();

	if(_mousePos.x < 0)
	{
		_mousePos.x = 0;
	}

	if(_mousePos.y < 0)
	{
		_mousePos.y = 0;
	}

	if(_mousePos.x > (float)CGame::GetInstance()->GetScreenWidth())
	{
		_mousePos.x = (float)CGame::GetInstance()->GetScreenWidth();
	}

	if(_mousePos.y > (float)CGame::GetInstance()->GetScreenHeight())
	{
		_mousePos.y = (float)CGame::GetInstance()->GetScreenHeight();
	}
	//_mousePos *= 0.75f;
	//_mousePos.x += 0.5f;
	//_mousePos.y += 0.5f;

	if(_mousePos.x <= 0.0f)
		_mousePos.x = 0.0f;
	if(_mousePos.x >= 1.0f)
		_mousePos.x = 1.0f;

	if(_mousePos.y <= 0.0f)
		_mousePos.y = 0.0f;
	if(_mousePos.y >= 1.0f)
		_mousePos.y = 1.0f;

	//if( _mousePos.x > PMENU_MIN_X && _mousePos.x < PMENU_MAX_X )
	//{
	//	if( _mousePos.y > PRESUME_MIN_Y && _mousePos.y < PRESUME_MAX_Y )
	//	{
	//		m_nCurrentlySelected = P_RESUME;
	//	}
	//	if( _mousePos.y > POPTIONS_MIN_Y && _mousePos.y < POPTIONS_MAX_Y )
	//	{
	//		m_nCurrentlySelected = P_OPTIONS;
	//	}
	//	if( _mousePos.y > PEXIT_MIN_Y && _mousePos.y < PEXIT_MAX_Y )
	//	{
	//		m_nCurrentlySelected = P_EXIT;
	//	}
	//}
	switch( m_pGame->GetScreenWidth() )
	{
	case RES_1024:
		{
			m_msPause.SetValue( "1024", 1.0f );
			break;
		}
	case RES_1280:
		{
			m_msPause.SetValue( "1280", 1.0f );
			break;
		}
	case RES_1920:
		{
			m_msPause.SetValue( "1920", 1.0f );
			break;
		}
	}



	if( !m_msPause.GetRunning() )
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
		m_pInput->ResetIgnoreTimer();
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return;
	}

	if( m_msPause.ShouldPlay() )
	{
		m_pAudio->ResumeAll();
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_UNPAUSE);
		m_pGame->PopState();
		return;
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
void CPauseState::Render ()
{
	//// How do I Render my stuff?
	//switch( m_nCurrentlySelected )
	//{
	//case P_RESUME:
	//	{
	//		RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_lpResume), 0.0f, 0.0f, 1.0f, 1.0f );
	//		break;
	//	}
	//case P_OPTIONS:
	//	{
	//		RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_lpOptions), 0.0f, 0.0f, 1.0f, 1.0f );
	//		break;
	//	}
	//case P_EXIT:
	//	{
	//		RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_lpExit), 0.0f, 0.0f, 1.0f, 1.0f );
	//		break;
	//	}
	//default:
	//	break;
	//}

	m_msPause.Render();

	RenderEngine::RasterTexture(TextureManager::GetInstance()->GetTexture(m_nReticle), _mousePos.x - 0.02f, _mousePos.y - 0.02f, _mousePos.x + 0.02f, _mousePos.y + 0.02f );
}