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
	m_pAudio = m_pGame->GetAudioManager();
	
	//m_pAudio->PlayMusic(MUSIC_TITLE);

	m_nStartTextureHandle = TextureManager::GetInstance()->GetTexture( TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_MainMenu_Start.png" ) );
	m_nOptionsTextureHandle = TextureManager::GetInstance()->GetTexture( TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_MainMenu_Options.png" ) );
	m_nCreditsTextureHandle = TextureManager::GetInstance()->GetTexture( TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_MainMenu_Credits.png" ) );
	m_nExitTextureHandle = TextureManager::GetInstance()->GetTexture( TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_MainMenu_Exit.png" ) );
	m_nReticle = TextureManager::GetInstance()->GetTexture( TextureManager::GetInstance()->LoadTexture("./Resources/Textures/T_Reticle_D.png"));
	
	ShowCursor( true );

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
	if(m_pGame)
	{
		m_pGame = NULL;
	}

	if(m_pInput)
	{
		m_pInput = NULL;
	}

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

	if(m_pInput->KeyPressed(DIK_ESCAPE))
	{
		return false;
	}

	//Keyboard Input
	if(m_pInput->KeyPressed(DIK_UP) || m_pInput->KeyPressed(DIK_W) )
	{
		m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);

		//move cursor up
		--m_nHighlighted;

		if(m_nHighlighted < 0)
		{
			m_nHighlighted = NUM_ITEMS - 1;
		}
	}

	if(m_pInput->KeyPressed(DIK_DOWN) || m_pInput->KeyPressed(DIK_S))
	{
		m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);

		//move cusor down
		++m_nHighlighted;

		if(m_nHighlighted >= NUM_ITEMS)
		{
			m_nHighlighted = STARTGAME;
		}
	}

	// Rueben's debug stuff
	/*if( m_pInput->CheckBufferedLeftMouseButtonEx() )
	{
		char buffer[64];
		sprintf_s(buffer, "x-%f | y-%f ", _mousePos.x, _mousePos.y );
		LOG(LOG_DEBUG, "MainMenu::Input()", buffer);
	}*/

	if(m_pInput->KeyPressed(DIK_RETURN) || 
		m_pInput->KeyPressed(DIK_SPACE) ||
		m_pInput->LeftMouseButtonPressed() )
	{
		m_pAudio->PlayEffect(SFX_MENU_ACCEPT);
		//enter into the next menu item
		switch(m_nHighlighted)
		{
		case STARTGAME:
			{
				//change state to gameplay state. stop music and I'm no longer on top.
				m_pAudio->StopMusic(BGM_TITLE);
				m_bOnTop = false;
				CGame::GetInstance()->ChangeState( CGamePlayState::GetInstance() );
				break;
			}
		case OPTIONS:
			{
				//change state to the options state. stop music and I'm no longer on top.
				//m_pAudio->PauseAll();
				m_pAudio->StopMusic(BGM_TITLE);
				m_bOnTop = false;

				CGame::GetInstance()->PushState(COptionsState::GetInstance());
				break;
			}
		case CREDITS:
			{
				//change state to the credits state. stop music and I'm no longer on top.
				/*UNCOMMENT THIS IN WHEN CREDITSSTATE IS ADDED*/
				m_pAudio->StopMusic(BGM_TITLE);
				m_bOnTop = false;
				CGame::GetInstance()->PushState(CCreditsState::GetInstance());
				break;
			}
		case EXIT:
			{
				//exit the game
				return false;
				break;
			}
		default:
			{
				break;
			}
		}
	}	
	return true;
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
	//if I'm updating I'm now on top
	if(m_bOnTop == false)
	{
		//m_pAudio->ResumeAll();
		//LOG(LOG_DEBUG, "CMainMenuState::Update()", "Playing Music");
		m_pAudio->PlayMusic(BGM_TITLE);
		m_bOnTop = true;
	}

	// Cursor movemont
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(CGame::GetInstance()->GetHWND(), &ptCursor);

	_mousePos = D3DXVECTOR4((FLOAT)ptCursor.x, 768.0f - (FLOAT)ptCursor.y, 1.0f, 1.0f );
	_mousePos /= _mousePos.w + 400;

	// limit cursor movement
	// to the screen
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

	// Change the selected item
	// based on cursor position
	if( _mousePos.x > MENU_MIN_X && _mousePos.x < MENU_MAX_X )
	{
		if( _mousePos.y > START_MIN_Y && _mousePos.y < START_MAX_Y )
		{
			m_nHighlighted = STARTGAME;
		}
		if( _mousePos.y > OPTIONS_MIN_Y && _mousePos.y < OPTIONS_MAX_Y )
		{
			m_nHighlighted = OPTIONS;
		}
		if( _mousePos.y > CREDITS_MIN_Y && _mousePos.y < CREDITS_MAX_Y )
		{
			m_nHighlighted = CREDITS;
		}
		if( _mousePos.y > EXIT_MIN_Y && _mousePos.y < EXIT_MAX_Y )
		{
			m_nHighlighted = EXIT;
		}
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
	switch( m_nHighlighted )
	{
	case STARTGAME:
		{
			RenderEngine::RasterTexture( m_nStartTextureHandle, 0.0f, 0.0f, 1.0f, 1.0f );
		}
		break;

	case OPTIONS:
		{
			RenderEngine::RasterTexture( m_nOptionsTextureHandle, 0.0f, 0.0f, 1.0f, 1.0f );
		}
		break;

	case CREDITS:
		{
			RenderEngine::RasterTexture( m_nCreditsTextureHandle, 0.0f, 0.0f, 1.0f, 1.0f );
		}
		break;

	case EXIT:
		{
			RenderEngine::RasterTexture( m_nExitTextureHandle, 0.0f, 0.0f, 1.0f, 1.0f );
		}
		break;

	default:
		break;
	}

	RenderEngine::RasterTexture(m_nReticle, _mousePos.x - 0.02f, _mousePos.y - 0.02f, _mousePos.x + 0.02f, _mousePos.y + 0.02f );
}