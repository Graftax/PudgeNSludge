#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "../../RenderEngine/RenderEngine.h"
#include "../../RenderEngine/TextureManager.h"
#include "CPauseState.h"
#include "COptionsState.h"
#include "MainMenuState.h"

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

	TextureManager* pTM = TextureManager::GetInstance();

	m_nCurrentlySelected = P_RESUME;

	m_lpResume			= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_Pause_Resume.png" ) );
	m_lpOptions			= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_Pause_Options.png" ) );
	m_lpExit			= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_Pause_Exit.png" ) );
	m_nReticle			= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/T_Reticle_D.png"));

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

	if(m_pInput->KeyPressed(DIK_ESCAPE))
	{
		m_pAudio->ResumeAll();
		m_pAudio->PlayEffect(SFX_MENU_CANCEL);
		m_pAudio->PlayEffect(SFX_MENU_UNPAUSE);
		m_pGame->PopState();
		return true;
	}
	//Keyboard Input
	if(m_pInput->KeyPressed(DIK_UP) || m_pInput->KeyPressed(DIK_W))
	{
		m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);

		//move cursor up
		--m_nCurrentlySelected;

		if(m_nCurrentlySelected < P_RESUME)
		{
			m_nCurrentlySelected = P_EXIT;
		}
	}

	if(m_pInput->KeyPressed(DIK_DOWN) || m_pInput->KeyPressed(DIK_S))
	{
		m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);

		//move cusor down
		++m_nCurrentlySelected;

		if(m_nCurrentlySelected > P_EXIT)
		{
			m_nCurrentlySelected = P_RESUME;
		}
	}

	// Rueben's debug
	/*if( m_pInput->CheckBufferedLeftMouseButtonEx() )
	{
		char buffer[64];
		sprintf_s(buffer, "x-%f | y-%f ", _mousePos.x, _mousePos.y );
		LOG(LOG_DEBUG, "MainMenu::Input()", buffer);
	}	*/								

	if(m_pInput->KeyPressed(DIK_RETURN) || 
		m_pInput->KeyPressed(DIK_SPACE) ||
		m_pInput->LeftMouseButtonPressed() )
	{
		//enter into the next menu item
		switch(m_nCurrentlySelected)
		{
		case P_RESUME:
			{
				//change state to gameplay state
				m_pAudio->ResumeAll();
				m_pAudio->PlayEffect(SFX_MENU_UNPAUSE);
				m_pGame->PopState();
				return true;
				break;
			}
		case P_OPTIONS:
			{
				//change state to the options state
				m_pAudio->PlayEffect(SFX_MENU_ACCEPT);
				CGame::GetInstance()->PushState(COptionsState::GetInstance());
				break;
			}
		case P_EXIT:
			{
				//change state to the credits state
				/*UNCOMMENT THIS IN WHEN CREDITSSTATE IS ADDED*/
				m_pAudio->PlayEffect(SFX_MENU_ACCEPT);
				CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
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
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CPauseState::Update (float fElapsedTime)
{
	// what system do I need for swapping the textures
	// for every option

POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(CGame::GetInstance()->GetHWND(), &ptCursor);

	_mousePos = D3DXVECTOR4((FLOAT)ptCursor.x, 768.0f - (FLOAT)ptCursor.y, 1.0f, 1.0f );
	_mousePos /= _mousePos.w + 400;

	if(_mousePos.x < 0)
	{
		_mousePos.x = 0;
	}

	if(_mousePos.y < 0)
	{
		_mousePos.y = 0;
	}

	if(_mousePos.x > SCREEN_WIDTH)
	{
		_mousePos.x = SCREEN_WIDTH;
	}

	if(_mousePos.y > SCREEN_HEIGHT)
	{
		_mousePos.y = SCREEN_HEIGHT;
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

	if( _mousePos.x > PMENU_MIN_X && _mousePos.x < PMENU_MAX_X )
	{
		if( _mousePos.y > PRESUME_MIN_Y && _mousePos.y < PRESUME_MAX_Y )
		{
			m_nCurrentlySelected = P_RESUME;
		}
		if( _mousePos.y > POPTIONS_MIN_Y && _mousePos.y < POPTIONS_MAX_Y )
		{
			m_nCurrentlySelected = P_OPTIONS;
		}
		if( _mousePos.y > PEXIT_MIN_Y && _mousePos.y < PEXIT_MAX_Y )
		{
			m_nCurrentlySelected = P_EXIT;
		}
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
	// How do I Render my stuff?
	switch( m_nCurrentlySelected )
	{
	case P_RESUME:
		{
			RenderEngine::RasterTexture( m_lpResume, 0.0f, 0.0f, 1.0f, 1.0f );
			break;
		}
	case P_OPTIONS:
		{
			RenderEngine::RasterTexture( m_lpOptions, 0.0f, 0.0f, 1.0f, 1.0f );
			break;
		}
	case P_EXIT:
		{
			RenderEngine::RasterTexture( m_lpExit, 0.0f, 0.0f, 1.0f, 1.0f );
			break;
		}
	default:
		break;
	}
	RenderEngine::RasterTexture(m_nReticle, _mousePos.x - 0.02f, _mousePos.y - 0.02f, _mousePos.x + 0.02f, _mousePos.y + 0.02f );
}