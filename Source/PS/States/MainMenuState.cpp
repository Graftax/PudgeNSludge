/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: MainMenuState
// Purpose: This is the Main Menu State
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MainMenuState.h"
#include "CCreditsState.h"
#include "..//..//RenderEngine/RenderEngine.h"
#include "..//..//RenderEngine/TextureManager.h"
#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "GamePlayState.h"
#include "COptionsState.h"
#include "../../InputLib/InputManager.h"
#include "../../AudioLib/AudioManager.h"
#include "..//..//Utilities/Logger/Logger.h"
#include "../../LevelManager/CLevelManager.h"

enum SCREEN_RES
{
	RES_1024 = 1024,
	RES_1280 = 1280,
	RES_1920 = 1920,
};

CMainMenuState* CMainMenuState::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CMainMenuState
// Purpose: Default Constructor.
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CMainMenuState::CMainMenuState()
{
	m_pGame = NULL;
	m_pInput = NULL;
	m_pAudio = NULL;
	m_nHighlighted = 0;
	m_bOnTop = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ~CMainMenuState
// Purpose: Default Destructor.
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CMainMenuState::~CMainMenuState()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetInstance
// Purpose: Returns the instance of the Main Menu.
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
CMainMenuState* CMainMenuState::GetInstance ()
{
	if (!m_pInstance)
	{
		m_pInstance = new CMainMenuState;
	}
	LOG(LOG_INFO, "CMainMenuState::GetInstance()", "CMainMenuState's instance creation succeeded");
	return m_pInstance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: DeleteInstance
// Purpose: Deletes the instance of the Main Menu.
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainMenuState::DeleteInstance ()
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
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainMenuState::Init ()
{
	// AddVerts()
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pInput->ResetIgnoreTimer();
	m_pAudio = m_pGame->GetAudioManager();
	soundTimer = 0.0f;
	//m_pAudio->PlayMusic(MUSIC_TITLE);

	m_nStartTextureHandle = /*TextureManager::GetInstance()->GetTexture( */TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_MainMenu_Start.png" );// );
	m_nOptionsTextureHandle = /*TextureManager::GetInstance()->GetTexture( */TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_MainMenu_Options.png" );// );
	m_nCreditsTextureHandle = /*TextureManager::GetInstance()->GetTexture( */TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_MainMenu_Credits.png" );// );
	m_nExitTextureHandle = /*TextureManager::GetInstance()->GetTexture( */TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_MainMenu_Exit.png" );// );
	m_nReticle = /*TextureManager::GetInstance()->GetTexture( */TextureManager::GetInstance()->LoadTexture("./Resources/Textures/T_Reticle_D.png");//);
	m_nBackgroundID = TextureManager::GetInstance()->LoadTexture("./Resources/Menus/MenuBackground.png");

	ShowCursor( true );

	if(m_pInput)
	{
		m_pInput->OffSetMouseForStart();
	}

	m_msMain.Initialize( "mainQuit.html" );
	m_msMain.StackPage( "main.html" );

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Shutdown
// Purpose: Shuts down the state.
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainMenuState::Shutdown ()
{
	// Unload the Textures loaded up to save on memory.
	TextureManager::GetInstance()->UnloadTexture( m_nStartTextureHandle );
	TextureManager::GetInstance()->UnloadTexture( m_nOptionsTextureHandle );
	TextureManager::GetInstance()->UnloadTexture( m_nCreditsTextureHandle );
	TextureManager::GetInstance()->UnloadTexture( m_nExitTextureHandle );
	TextureManager::GetInstance()->UnloadTexture( m_nReticle );

	CGame::GetInstance()->SaveOptions();

	if(m_pGame)
	{
		m_pGame = NULL;
	}

	if(m_pInput)
	{
		m_pInput = NULL;
	}

	m_msMain.Shutdown();

	DeleteInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Input
// Purpose: Gets input to update the state.
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMainMenuState::Input ()
{
	if(!m_pInput)
	{
		return false;
	}

	//if(m_pInput->KeyPressed(DIK_ESCAPE))
	{
		//return false;
	}

	//Keyboard Input
	//if(m_pInput->KeyPressed(DIK_UP) || m_pInput->KeyPressed(DIK_W) )
	//{
	//	m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);

	//	////move cursor up
	//	//--m_nHighlighted;

	//	//if(m_nHighlighted < 0)
	//	//{
	//	//	m_nHighlighted = NUM_ITEMS - 1;
	//	//}
	//	m_msMain.Move( CMSD_UP );
	//}

	//if(m_pInput->KeyPressed(DIK_DOWN) || m_pInput->KeyPressed(DIK_S))
	//{
	//	m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);

	//	////move cusor down
	//	//++m_nHighlighted;

	//	//if(m_nHighlighted >= NUM_ITEMS)
	//	//{
	//	//	m_nHighlighted = STARTGAME;
	//	//}
	//	m_msMain.Move( CMSD_DOWN );
	//}

	//if(m_pInput->KeyPressed(DIK_LEFT) || m_pInput->KeyPressed(DIK_A))
	//{
	//	m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
	//	m_msMain.Move( CMSD_LEFT );
	//}
	//if(m_pInput->KeyPressed(DIK_RIGHT) || m_pInput->KeyPressed(DIK_D))
	//{
	//	m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
	//	m_msMain.Move( CMSD_RIGHT );
	//}

	// Rueben's debug stuff
	/*if( m_pInput->CheckBufferedLeftMouseButtonEx() )
	{
		char buffer[64];
		sprintf_s(buffer, "x-%f | y-%f ", _mousePos.x, _mousePos.y );
		LOG(LOG_DEBUG, "MainMenu::Input()", buffer);
	}*/

	//if(m_pInput->KeyPressed(DIK_RETURN) || 
	//	m_pInput->KeyPressed(DIK_SPACE) ||
	//	m_pInput->LeftMouseButtonPressed() )
	//{
	//	m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
	//	//enter into the next menu item
	//	switch(m_nHighlighted)
	//	{
	//	case STARTGAME:
	//		{
	//			//change state to gameplay state. stop music and I'm no longer on top.
	//			//m_pAudio->StopMusic(AK::EVENTS::STOP_BGM_MENU);
	//			m_bOnTop = false;
	//			LevelManager::GetInstance()->SetCurrentLevel (0);
	//			CGame::GetInstance()->ChangeState( CGamePlayState::GetInstance() );
	//			break;
	//		}
	//	case OPTIONS:
	//		{
	//			//change state to the options state. stop music and I'm no longer on top.
	//			//m_pAudio->PauseAll();
	//			//m_pAudio->StopMusic(AK::EVENTS::STOP_BGM_MENU);
	//			m_bOnTop = false;

	//			CGame::GetInstance()->PushState(COptionsState::GetInstance());
	//			break;
	//		}
	//	case CREDITS:
	//		{
	//			//change state to the credits state. stop music and I'm no longer on top.
	//			/*UNCOMMENT THIS IN WHEN CREDITSSTATE IS ADDED*/
	//			//m_pAudio->StopMusic(AK::EVENTS::STOP_BGM_MENU);
	//			m_bOnTop = false;
	//			CGame::GetInstance()->PushState(CCreditsState::GetInstance());
	//			break;
	//		}
	//	case EXIT:
	//		{
	//			//exit the game
	//			return false;
	//			break;
	//		}
	//	default:
	//		{
	//			break;
	//		}
	//	}
	//}


	return m_msMain.GetRunning();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update
// Purpose: Updates the state.
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainMenuState::Update (float fElapsedTime)
{
	m_pInput->IgnoreInputForSetAmountOfTime( 1.0f/60.0f, 0.2f );

	soundTimer += fElapsedTime;

	//if I'm updating I'm now on top
	if(m_bOnTop == false)
	{
		//m_pAudio->ResumeAll();
		//LOG(LOG_DEBUG, "CMainMenuState::Update()", "Playing Music");
		m_pAudio->PlayMusic(AK::SWITCHES::MUSIC::SWITCH::PS_MENU);
		m_bOnTop = true;
	}

	//// Cursor movemont
	//POINT ptCursor;
	//GetCursorPos(&ptCursor);
	//ScreenToClient(CGame::GetInstance()->GetHWND(), &ptCursor);
	//
	//POINT ptScreenCursor = ptCursor;

	//if( GetFocus() == CGame::GetInstance()->GetHWND() )
	//{
	//	if( ptCursor.x < 0 )
	//	{
	//		ptScreenCursor.x = 0;
	//		ClientToScreen(CGame::GetInstance()->GetHWND(),&ptScreenCursor);
	//		SetCursorPos( ptScreenCursor.x, ptScreenCursor.y );
	//	}
	//	else if( ptCursor.x > CGame::GetInstance()->GetScreenWidth() )
	//	{
	//		ptScreenCursor.x = CGame::GetInstance()->GetScreenWidth();
	//		ClientToScreen(CGame::GetInstance()->GetHWND(),&ptScreenCursor);
	//		SetCursorPos( ptScreenCursor.x, ptScreenCursor.y );
	//	}
	//	if( ptCursor.y < 0 )
	//	{
	//		ptScreenCursor.y = 0;
	//		ClientToScreen(CGame::GetInstance()->GetHWND(),&ptScreenCursor);
	//		SetCursorPos( ptScreenCursor.x, ptScreenCursor.y );
	//	}
	//	else if( ptCursor.y > CGame::GetInstance()->GetScreenHeight() )
	//	{
	//		ptScreenCursor.y = CGame::GetInstance()->GetScreenHeight();
	//		ClientToScreen(CGame::GetInstance()->GetHWND(),&ptScreenCursor);
	//		SetCursorPos( ptScreenCursor.x, ptScreenCursor.y );
	//	}
	//}

	//GetCursorPos(&ptCursor);
	//ScreenToClient(CGame::GetInstance()->GetHWND(), &ptCursor);
	//_mousePos = D3DXVECTOR4((FLOAT)ptCursor.x, 768.0f - (FLOAT)ptCursor.y, 1.0f, 1.0f );
	//_mousePos /= _mousePos.w + 400;
	
	int OldFX = m_pAudio->GetFXVolume();
	int OldDiag = m_pAudio->GetDLGVolume();

	m_msMain.SetValue( "EffectVol", (float)m_pAudio->GetFXVolume() );
	m_msMain.SetValue( "MusicVol", (float)m_pAudio->GetBGMVolume() );
	m_msMain.SetValue( "DialogueVol", (float)m_pAudio->GetDLGVolume() );
	m_msMain.SetValue( "Gamma", ( RenderEngine::GetGamma() + 0.5f ) * 100.0f );

	if( m_pGame->GetFullScreen() )
	{
		m_msMain.SetValue( "fullscreen", 1.0f );
		m_msMain.SetActive( true, "Gamma" );
	}
	else
	{
		m_msMain.SetValue( "fullscreen", 0.0f );
		m_msMain.SetActive( false, "Gamma" );
	}

	if(m_pInput)
	{
		m_msMain.Update(fElapsedTime, m_pInput->GetMousePosX(), m_pInput->GetMousePosY());
	}

	if( m_pInput->LeftMouseButtonDown() )
		m_msMain.MouseDown();

	if( m_pInput->KeyPressed( DIK_SPACE ) || m_pInput->KeyPressed( DIK_RETURN ) )
	{
		m_msMain.Activate();
	}

	if( m_pInput->KeyPressed( DIK_BACKSPACE ) || m_pInput->KeyPressed( DIK_ESCAPE ) )
	{
		m_msMain.Back();
	}

	if(m_pInput->KeyPressed(DIK_UP) || m_pInput->KeyPressed(DIK_W) )
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
		m_msMain.Move( CMSD_UP );
	}

	if(m_pInput->KeyPressed(DIK_DOWN) || m_pInput->KeyPressed(DIK_S))
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
		m_msMain.Move( CMSD_DOWN );
	}

	if(m_pInput->KeyPressed(DIK_LEFT) || m_pInput->KeyPressed(DIK_A))
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
		m_msMain.Move( CMSD_LEFT );
	}
	if(m_pInput->KeyPressed(DIK_RIGHT) || m_pInput->KeyPressed(DIK_D))
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
		m_msMain.Move( CMSD_RIGHT );
	}

	m_msMain.RecalcValues();

	m_pAudio->SetFXVolume( (int)m_msMain.GetValue( "EffectVol" ) );
	m_pAudio->SetBGMVolume( (int)m_msMain.GetValue( "MusicVol" ) );
	m_pAudio->SetDLGVolume( (int)m_msMain.GetValue( "DialogueVol" ) );
	RenderEngine::SetGamma( ( m_msMain.GetValue( "Gamma" ) / 100.0f ) - 0.5f );

	if( m_msMain.GetValue( "fullscreen" ) > 0 )
	{
		if( !m_pGame->GetFullScreen() )
		{
			m_pGame->SetFullScreen( true );
			RenderEngine::ChangeDisplayParam( m_pGame->GetScreenWidth(), m_pGame->GetScreenHeight(), m_pGame->GetFullScreen(), true );
			m_msMain.SetActive( true, "Gamma" );
		}
	}
	else
	{
		if( m_pGame->GetFullScreen() )
		{
			m_pGame->SetFullScreen( false );
			RenderEngine::ChangeDisplayParam( m_pGame->GetScreenWidth(), m_pGame->GetScreenHeight(), m_pGame->GetFullScreen(), true );
			m_msMain.SetActive( false, "Gamma" );
		}
	}
	
	if( m_msMain.GetValue( "1024" ) > 0 && m_pGame->GetScreenWidth() != 1024 )
	{
		m_pGame->SetScreenWidth( 1024 );
		m_pGame->SetScreenHeight( 768 );
		RenderEngine::ChangeDisplayParam( m_pGame->GetScreenWidth(), m_pGame->GetScreenHeight(), m_pGame->GetFullScreen(), true );
		m_msMain.SetValue( "1280", 0.0f );
		m_msMain.SetValue( "1920", 0.0f );
	}

	if( m_msMain.GetValue( "1280" ) > 0 && m_pGame->GetScreenWidth() != 1280 )
	{
		m_pGame->SetScreenWidth( 1280 );
		m_pGame->SetScreenHeight( 720 );
		RenderEngine::ChangeDisplayParam( m_pGame->GetScreenWidth(), m_pGame->GetScreenHeight(), m_pGame->GetFullScreen(), true );
		m_msMain.SetValue( "1024", 0.0f );
		m_msMain.SetValue( "1920", 0.0f );
	}

	if( m_msMain.GetValue( "1920" ) > 0 && m_pGame->GetScreenWidth() != 1920 )
	{
		m_pGame->SetScreenWidth( 1920 );
		m_pGame->SetScreenHeight( 1080 );
		RenderEngine::ChangeDisplayParam( m_pGame->GetScreenWidth(), m_pGame->GetScreenHeight(), m_pGame->GetFullScreen(), true );
		m_msMain.SetValue( "1280", 0.0f );
		m_msMain.SetValue( "1024", 0.0f );
	}

	if( m_msMain.GetValue( "RESET" ) == 1.0f )
	{
		m_msMain.SetValue( "RESET", 0.0f );
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);

		m_pGame->GetAudioManager()->SetBGMVolume( 100 );
		m_pGame->GetAudioManager()->SetDLGVolume( 100 );
		m_pGame->GetAudioManager()->SetFXVolume( 100 );
		RenderEngine::SetGamma( 0.0f );

#if _DEBUG
		m_pGame->SetFullScreen( false );
		m_msMain.SetActive( false, "Gamma" );
#else
		m_pGame->SetFullScreen( true );
		m_msMain.SetActive( true, "Gamma" );
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

	if(m_pInput)
	{
		_mousePos.x = (float)m_pInput->GetMousePosX() / CGame::GetInstance()->GetScreenWidth();
		_mousePos.y = ((float)m_pInput->GetMousePosY() / CGame::GetInstance()->GetScreenHeight());
	}

	// limit cursor movement
	// to the screen
	//_mousePos *= 0.75f;
	//_mousePos.x += 0.5f;
	//_mousePos.y += 0.5f;

	switch( m_pGame->GetScreenWidth() )
	{
	case RES_1024:
		{
			m_msMain.SetValue( "1024", 1.0f );
			break;
		}
	case RES_1280:
		{
			m_msMain.SetValue( "1280", 1.0f );
			break;
		}
	case RES_1920:
		{
			m_msMain.SetValue( "1920", 1.0f );
			break;
		}
	}
	//// Change the selected item
	//// based on cursor position
	//if( _mousePos.x > MENU_MIN_X && _mousePos.x < MENU_MAX_X )
	//{
	//	if( _mousePos.y > START_MIN_Y && _mousePos.y < START_MAX_Y )
	//	{
	//		m_nHighlighted = STARTGAME;
	//	}
	//	if( _mousePos.y > OPTIONS_MIN_Y && _mousePos.y < OPTIONS_MAX_Y )
	//	{
	//		m_nHighlighted = OPTIONS;
	//	}
	//	if( _mousePos.y > CREDITS_MIN_Y && _mousePos.y < CREDITS_MAX_Y )
	//	{
	//		m_nHighlighted = CREDITS;
	//	}
	//	if( _mousePos.y > EXIT_MIN_Y && _mousePos.y < EXIT_MAX_Y )
	//	{
	//		m_nHighlighted = EXIT;
	//	}
	//}

	if( m_msMain.ShouldPlay() )
	{
		m_msMain.StopPlay();
		//change state to gameplay state. stop music and I'm no longer on top.
		//m_pAudio->StopMusic(AK::EVENTS::STOP_BGM_MENU);
		m_bOnTop = false;
		LevelManager::GetInstance()->SetCurrentLevel (0);
		CGame::GetInstance()->ChangeState( CGamePlayState::GetInstance() );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Render
// Purpose: Render the state.
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainMenuState::Render ()
{
	// Swap texture backgrounds
	// based on the selected item
	//switch( m_nHighlighted )
	//{
	//case STARTGAME:
	//	{
	//		RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_nStartTextureHandle), 0.0f, 0.0f, 1.0f, 1.0f );
	//	}
	//	break;

	//case OPTIONS:
	//	{
	//		RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_nOptionsTextureHandle), 0.0f, 0.0f, 1.0f, 1.0f );
	//	}
	//	break;

	//case CREDITS:
	//	{
	//		RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_nCreditsTextureHandle), 0.0f, 0.0f, 1.0f, 1.0f );
	//	}
	//	break;

	//case EXIT:
	//	{
	//		RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_nExitTextureHandle), 0.0f, 0.0f, 1.0f, 1.0f );
	//	}
	//	break;

	//default:
	//	break;
	//}
	RenderEngine::RasterTexture(TextureManager::GetInstance()->GetTexture(m_nBackgroundID), 0.0f, 0.0f, 1.0f, 1.0f );
	
	m_msMain.Render();

	RenderEngine::RasterTexture(TextureManager::GetInstance()->GetTexture(m_nReticle), _mousePos.x - 0.02f, (-_mousePos.y - 0.02f) + 1.0f, _mousePos.x + 0.02f, (-_mousePos.y + 0.02f) + 1.0f );
}