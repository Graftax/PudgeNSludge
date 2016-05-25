#include "COptionsState.h"
#include "CVideoOptionsState.h"
#include "CAudioOptionsStates.h"
#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "../../RenderEngine/TextureManager.h"
#include "../../RenderEngine/RenderEngine.h"


COptionsState* COptionsState::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: COptionsState
// Purpose: Default Constructor.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
COptionsState::COptionsState()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ~COptionsState
// Purpose: Default Destructor.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
COptionsState::~COptionsState()
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
COptionsState* COptionsState::GetInstance ()
{
	if (!m_pInstance)
	{
		m_pInstance = new COptionsState;
	}
	LOG(LOG_INFO, "COptionsState::GetInstance()", "COptionsState's instance creation succeeded");
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
void COptionsState::DeleteInstance ()
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
void COptionsState::Init ()
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pAudio = m_pGame->GetAudioManager();

	TextureManager* pTM = TextureManager::GetInstance();

	m_lpOptionsVideo		= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_OptionMenu_Video.png" ) );
	m_lpOptionsAudio        = pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_OptionMenu_Audio.png" ) );
	m_lpOptionsExit			= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_OptionMenu_Exit.png" ) );
	m_nReticle				= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/T_Reticle_D.png"));

	m_nCurrentlySelected	= OPT_VIDEO;
}						  

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ShutDown
// Purpose: Shuts down the state
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void COptionsState::Shutdown ()
{
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsVideo );
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsAudio );
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsExit );
	TextureManager::GetInstance()->UnloadTexture( m_nReticle );
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
bool COptionsState::Input ()
{
	if(!m_pInput)
	{
		return false;
	}

	if(m_pInput->KeyPressed(DIK_ESCAPE))
	{
		m_pAudio->PlayEffect(SFX_MENU_CANCEL);
		//m_pAudio->ResumeAll();
		m_pGame->PopState();
		return true;
	}

	if( m_pInput->KeyPressed( DIK_UP ) || m_pInput->KeyPressed(DIK_W) )
	{
		m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);

		--m_nCurrentlySelected;

				if( m_nCurrentlySelected < OPT_VIDEO )
					m_nCurrentlySelected = OPT_EXIT;
	}
	if( m_pInput->KeyPressed( DIK_DOWN ) || m_pInput->KeyPressed(DIK_S) )
	{
		m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);

		++m_nCurrentlySelected;

				if( m_nCurrentlySelected > OPT_EXIT )
					m_nCurrentlySelected = OPT_VIDEO;
	}
	// Rueben's debug
	if( m_pInput->LeftMouseButtonPressed() )
	{
		char buffer[64];
		sprintf_s(buffer, "x-%f | y-%f ", _mousePos.x, _mousePos.y );
		LOG(LOG_DEBUG, "COptionsState::Input()", buffer);
	}									

	if(m_pInput->KeyPressed(DIK_RETURN) || 
		m_pInput->KeyPressed(DIK_SPACE) ||
		m_pInput->LeftMouseButtonPressed() )
	{
		
				if( m_nCurrentlySelected == OPT_EXIT )
				{
					m_pAudio->PlayEffect(SFX_MENU_CANCEL);
					//m_pAudio->ResumeAll();
					m_pGame->PopState();
					return true;
				}
				if( m_nCurrentlySelected == OPT_VIDEO )
				{
					m_pAudio->PlayEffect(SFX_MENU_ACCEPT);
					//TODO
					// Change to video options
					m_pGame->PushState( CVideoOptionsState::GetInstance() );
				}
				if( m_nCurrentlySelected == OPT_AUDIO )
				{
					m_pAudio->PlayEffect(SFX_MENU_ACCEPT);
					//TODO
					// Change to Audio Options
					m_pGame->PushState( CAudioOptionsState::GetInstance() );
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
void COptionsState::Update (float fElapsedTime)
{
	// what system do I need for swapping the textures
	// for every option
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(CGame::GetInstance()->GetHWND(), &ptCursor);

	_mousePos = D3DXVECTOR4((FLOAT)ptCursor.x, 768.0f - (FLOAT)ptCursor.y, 1.0f, 1.0f );
	_mousePos /= _mousePos.w + 400;
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

	if( _mousePos.x > OMENU_MIN_X && _mousePos.x < OMENU_MAX_X )
	{
		if( _mousePos.y > OVIDEO_MIN_Y && _mousePos.y < OVIDEO_MAX_Y )
		{
			m_nCurrentlySelected = OPT_VIDEO;
		}
		if( _mousePos.y > OAUDIO_MIN_Y && _mousePos.y < OAUDIO_MAX_Y )
		{
			m_nCurrentlySelected = OPT_AUDIO;
		}
		if( _mousePos.y > OEXIT_MIN_Y && _mousePos.y < OEXIT_MAX_Y )
		{
			m_nCurrentlySelected = OPT_EXIT;
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
void COptionsState::Render ()
{
	// How do I Render my stuff?
	switch( m_nCurrentlySelected )
	{
	case OPT_VIDEO:
		{
			RenderEngine::RasterTexture( m_lpOptionsVideo, 0.0f, 0.0f, 1.0f, 1.0f );
			break;
		}
	case OPT_AUDIO:
		{
			RenderEngine::RasterTexture( m_lpOptionsAudio, 0.0f, 0.0f, 1.0f, 1.0f );
			break;
		}
	case OPT_EXIT:
		{
			RenderEngine::RasterTexture( m_lpOptionsExit, 0.0f, 0.0f, 1.0f, 1.0f );
			break;
		}
	default:
		break;
	}

	//Render the cursor reticle at the mouse position
	RenderEngine::RasterTexture(m_nReticle, _mousePos.x - 0.02f, _mousePos.y - 0.02f, _mousePos.x + 0.02f, _mousePos.y + 0.02f );
}