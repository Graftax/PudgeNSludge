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

	m_lpOptionsReset		=					   pTM->LoadTexture("./Resources/Textures/PS_FE_OptionMenu_Reset.png" );
	m_lpOptionsVideo		= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_OptionMenu_Video.png" );// );
	m_lpOptionsAudio        = /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_OptionMenu_Audio.png" );// );
	m_lpOptionsExit			= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_OptionMenu_Exit.png" );// );
	m_nReticle				= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/T_Reticle_D.png");//);

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
	CGame::GetInstance()->SaveOptions();
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
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_CANCEL);
		//m_pAudio->ResumeAll();
		m_pGame->PopState();
		return true;
	}

	if( m_pInput->KeyPressed( DIK_UP ) || m_pInput->KeyPressed(DIK_W) )
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);

		--m_nCurrentlySelected;

				if( m_nCurrentlySelected < OPT_RESET )
					m_nCurrentlySelected = OPT_EXIT;
	}
	if( m_pInput->KeyPressed( DIK_DOWN ) || m_pInput->KeyPressed(DIK_S) )
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);

		++m_nCurrentlySelected;

				if( m_nCurrentlySelected > OPT_EXIT )
					m_nCurrentlySelected = OPT_RESET;
	}
	// Rueben's debug
	//if( m_pInput->LeftMouseButtonPressed() )
	//{
	//	char buffer[64];
	//	sprintf_s(buffer, "x-%f | y-%f ", _mousePos.x, _mousePos.y );
	//	LOG(LOG_DEBUG, "COptionsState::Input()", buffer);
	//}									

	if(m_pInput->KeyPressed(DIK_RETURN) || 
		m_pInput->KeyPressed(DIK_SPACE) ||
		m_pInput->LeftMouseButtonPressed() )
	{
		
				if( m_nCurrentlySelected == OPT_EXIT )
				{
					m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_CANCEL);
					//m_pAudio->ResumeAll();
					m_pGame->PopState();
					return true;
				}
				if( m_nCurrentlySelected == OPT_VIDEO )
				{
					m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
					//TODO
					// Change to video options
					m_pGame->PushState( CVideoOptionsState::GetInstance() );
				}
				if( m_nCurrentlySelected == OPT_AUDIO )
				{
					m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
					//TODO
					// Change to Audio Options
					m_pGame->PushState( CAudioOptionsState::GetInstance() );
				}
				if( m_nCurrentlySelected == OPT_RESET )
				{
					m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
					//TODO
					// Change to Audio Options
					m_pGame->GetAudioManager()->SetBGMVolume( 100 );
					m_pGame->GetAudioManager()->SetDLGVolume( 100 );
					m_pGame->GetAudioManager()->SetFXVolume( 100 );
					RenderEngine::SetGamma( 0.0f );
#if _DEBUG
					m_pGame->SetFullScreen( false );
#else
					m_pGame->SetFullScreen( true );
#endif
					RenderEngine::ChangeDisplayParam( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), !m_pGame->GetFullScreen(), true );
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
	if(m_pInput)
	{
		_mousePos = D3DXVECTOR2((float)m_pInput->GetMousePosX(), (float)m_pInput->GetMousePosY());
	}
	_mousePos.x = _mousePos.x / CGame::GetInstance()->GetScreenWidth();
	_mousePos.y = _mousePos.y / CGame::GetInstance()->GetScreenHeight();

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
		if( _mousePos.y > ORESET_MIN_Y && _mousePos.y < ORESET_MAX_Y )
		{
			m_nCurrentlySelected = OPT_RESET;
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
			RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_lpOptionsVideo), 0.0f, 0.0f, 1.0f, 1.0f );
			break;
		}
	case OPT_AUDIO:
		{
			RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_lpOptionsAudio), 0.0f, 0.0f, 1.0f, 1.0f );
			break;
		}
	case OPT_RESET:
		{
			RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_lpOptionsReset), 0.0f, 0.0f, 1.0f, 1.0f );
			break;
		}
	case OPT_EXIT:
		{
			RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_lpOptionsExit), 0.0f, 0.0f, 1.0f, 1.0f );
			break;
		}
	default:
		break;
	}

	//Render the cursor reticle at the mouse position
	RenderEngine::RasterTexture(TextureManager::GetInstance()->GetTexture(m_nReticle), _mousePos.x - 0.02f, (-_mousePos.y - 0.02f) + 1.0f, _mousePos.x + 0.02f, (-_mousePos.y + 0.02f) + 1.0f );
}