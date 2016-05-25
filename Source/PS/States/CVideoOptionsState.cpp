#include "CVideoOptionsState.h"
#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "../../RenderEngine/TextureManager.h"
#include "../../RenderEngine/RenderEngine.h"

#define ADJUST_AMOUNT 1
#define PLAY_TIME_CAP 1.0f

CVideoOptionsState* CVideoOptionsState::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: COptionsState
// Purpose: Default Constructor.
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CVideoOptionsState::CVideoOptionsState()
{
	m_pGame = nullptr;
	m_pInput = nullptr;
	m_pAudio = nullptr;

	m_nCurrentlySelected = 0;
	m_lpCurrentSelection = 0;

	m_bFullScreen = false;
	m_bSound = false;
	m_bPrevDown = false;
	m_bLocked = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ~COptionsState
// Purpose: Default Destructor.
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CVideoOptionsState::~CVideoOptionsState()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetInstance
// Purpose: Returns the instance of the Main Menu.
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
CVideoOptionsState* CVideoOptionsState::GetInstance ()
{
	if (!m_pInstance)
	{
		m_pInstance = new CVideoOptionsState;
	}
	LOG(LOG_INFO, "CVideoOptionsState::GetInstance()", "CVideoOptionsState's instance creation succeeded");
	return m_pInstance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: DeleteInstance
// Purpose: Deletes the instance of the Main Menu.
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CVideoOptionsState::DeleteInstance ()
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
// Creation Date: 5/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CVideoOptionsState::Init(void)
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pAudio = m_pGame->GetAudioManager();

	TextureManager* pTM = TextureManager::GetInstance();
	m_lpOptionsVideoGamma	= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_VideoMenu_1.png" );// );
	m_lpOptionsVideoFS_On	= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_VideoMenu_2.png" );// );
	m_lpOptionsVideoFS_Off	= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_VideoMenu_3.png" );// );
	m_lpOptionsVideoExit	= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_VideoMenu_4.png" );// );
	m_lpOptionsCheck		= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_VideoMenu_Check.png" );// );
	m_lpOptionsSlider		= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_Slider.png" );// );
	m_nReticle				= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/T_Reticle_D.png");//);
	m_lpFullScreenBlockOut	= /*pTM->GetTexture(*/ pTM->LoadTexture("./Resources/Textures/PS_FE_SliderFade.png");//);

	// Initial Slider Positions
	m_fGammaSliderPosVec		= vec2f( VSLIDER_MIN + VSLIDER_BAR_WIDTH * (RenderEngine::GetGamma() + 0.5f), VSLIDER_GAMMA_POSY );

	m_bFullScreen = m_pGame->GetFullScreen();
	if( m_bFullScreen )
	{
		m_fCheckMarkPos				= vec2f( VCHECK_ON_MIN_X, VCHECK_ON_MIN_Y );
	}
	else
	{
		m_fCheckMarkPos				= vec2f( VCHECK_OFF_MIN_X, VCHECK_OFF_MIN_Y );
	}

	// position Brightness Slider
	// Based on a ratio of brightness to slider width
	// position Gamma Slider
	// Based on a ratio of gamma to slider width

	m_nCurrentlySelected	= VIDEO_GAMMA;

	m_bPrevDown = false;
	m_bLocked = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Shutdown
// Purpose: Shuts down the state.
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CVideoOptionsState::Shutdown()
{
	TextureManager* pTM = TextureManager::GetInstance();

	// Unload all the textures
	pTM->UnloadTexture( m_lpOptionsVideoGamma );
	pTM->UnloadTexture( m_lpOptionsVideoExit );
	pTM->UnloadTexture( m_lpOptionsVideoFS_On );
	pTM->UnloadTexture( m_lpOptionsVideoFS_Off );
	pTM->UnloadTexture( m_lpOptionsCheck);
	pTM->UnloadTexture( m_lpOptionsSlider );
	pTM->UnloadTexture( m_nReticle );
	pTM->UnloadTexture( m_lpFullScreenBlockOut );
	DeleteInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update
// Purpose: Updates the state.
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CVideoOptionsState::Update(float fTime)
{
	//// Get the cursor in screen space
	//POINT ptCursor;
	//GetCursorPos(&ptCursor);
	//ScreenToClient(CGame::GetInstance()->GetHWND(), &ptCursor);
	//
	//// convert it to a value between 0 and 1
	//_mousePos = D3DXVECTOR4((FLOAT)ptCursor.x, 768.0f - (FLOAT)ptCursor.y, 1.0f, 1.0f );
	//_mousePos /= _mousePos.w + 400;

	if(m_pInput)
	{
		_mousePos = D3DXVECTOR2((float)m_pInput->GetMousePosX(), (float)m_pInput->GetMousePosY());
	}

	_mousePos.x = _mousePos.x / CGame::GetInstance()->GetScreenWidth();
	_mousePos.y = _mousePos.y / CGame::GetInstance()->GetScreenHeight();

	// Change Selection
	// Based on the cursor position
	if( _mousePos.x > VMENU_MIN_X && _mousePos.x < VMENU_MAX_X && !m_pInput->LeftMouseButtonDown() )
	{
		if( _mousePos.y > VGAMMA_MIN_Y && _mousePos.y < VGAMMA_MAX_Y )
		{
			m_nCurrentlySelected = VIDEO_GAMMA;
			m_bFullScreen = m_pGame->GetFullScreen();
		}
		if( _mousePos.y > VEXIT_MIN_Y && _mousePos.y < VEXIT_MAX_Y )
		{
			m_nCurrentlySelected = VIDEO_EXIT;
			m_bFullScreen = m_pGame->GetFullScreen();
		}
		if( _mousePos.y > VFULLSCREEN_MIN_Y && _mousePos.y < VFULLSCREEN_MAX_Y )
		{
			m_nCurrentlySelected = VIDEO_FS;
			if( _mousePos.x > VFULLSCREEN_ON_MIN_X && _mousePos.x < VFULLSCREEN_ON_MAX_X )
			{
				m_bFullScreen = true;
			}
			if( _mousePos.x > VFULLSCREEN_OFF_MIN_X && _mousePos.x < VFULLSCREEN_OFF_MAX_X )
			{
				m_bFullScreen = false;
			}
		}
	}

	// texture swapping
	switch( m_nCurrentlySelected)
	{
	case VIDEO_GAMMA:
		{
			m_lpCurrentSelection = m_lpOptionsVideoGamma;

			break;
		}
	case VIDEO_FS:
		{
			if( m_bFullScreen )
			{
				m_lpCurrentSelection = m_lpOptionsVideoFS_On;
			}
			else
			{
				m_lpCurrentSelection = m_lpOptionsVideoFS_Off;
			}
			break;
		}
	case VIDEO_EXIT:
		{
			m_lpCurrentSelection = m_lpOptionsVideoExit;
			m_bFullScreen = m_pGame->GetFullScreen();
			break;
		}
	default:
		break;
	}

	if( m_bFullScreen )
	{
		m_fCheckMarkPos	= vec2f( VCHECK_ON_MIN_X, VCHECK_ON_MIN_Y );
	}
	else
	{
		m_fCheckMarkPos	= vec2f( VCHECK_OFF_MIN_X, VCHECK_OFF_MIN_Y );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Input
// Purpose: Gets input to update the state.
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CVideoOptionsState::Input()
{
	// Safety Check
	if(!m_pInput)
	{
		return false;
	}

	if(m_pInput->KeyPressed(DIK_ESCAPE))
	{
		// Change to Options menu
		m_pGame->PopState();
		return true;
	}

	// KeyBoard Input
	if( m_pInput->KeyPressed( DIK_UP ) || m_pInput->KeyPressed(DIK_W) )
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);

		--m_nCurrentlySelected;

		/*if(m_pGame->GetFullScreen())
		{
		m_bFullScreen = true;
		}
		else
		{
		m_bFullScreen = false;
		}*/

		if( m_nCurrentlySelected < VIDEO_GAMMA)
			m_nCurrentlySelected = VIDEO_EXIT;
	}
	if( m_pInput->KeyPressed( DIK_DOWN ) || m_pInput->KeyPressed(DIK_S) )
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);

		++m_nCurrentlySelected;

		/*if(m_pGame->GetFullScreen())
		{
		m_bFullScreen = true;
		}
		else
		{
		m_bFullScreen = false;
		}*/

		if( m_nCurrentlySelected > VIDEO_EXIT)
			m_nCurrentlySelected = VIDEO_GAMMA;
	}

	if( m_pInput->KeyPressed( DIK_RIGHT ) || m_pInput->KeyPressed(DIK_D) )
	{
		if( VIDEO_GAMMA == m_nCurrentlySelected && m_pGame->GetFullScreen() )
		{
			// set gamma
			RenderEngine::SetGamma(RenderEngine::GetGamma() + (ADJUST_AMOUNT * 0.01f));

			// set the new slider position based on the gamma change
			m_fGammaSliderPosVec.x = VSLIDER_MIN + VSLIDER_BAR_WIDTH * ((RenderEngine::GetGamma() + 0.5f));

			// play sound
			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
		}

		if( VIDEO_FS == m_nCurrentlySelected )
		{
			m_bFullScreen = !m_bFullScreen;
			RenderEngine::ChangeDisplayParam( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), !m_bFullScreen, true );
		}
	}
	if( m_pInput->KeyPressed( DIK_LEFT ) || m_pInput->KeyPressed(DIK_A) )
	{
		if( VIDEO_GAMMA == m_nCurrentlySelected && m_pGame->GetFullScreen() )
		{
			// set gamma
			RenderEngine::SetGamma(RenderEngine::GetGamma() - (ADJUST_AMOUNT * 0.01f));

			// set the new slider position based on the gamma change
			m_fGammaSliderPosVec.x = VSLIDER_MIN + VSLIDER_BAR_WIDTH * ((RenderEngine::GetGamma() + 0.5f));

			// play sound
			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
		}
		if( VIDEO_FS == m_nCurrentlySelected )
		{
			m_bFullScreen = !m_bFullScreen;
			RenderEngine::ChangeDisplayParam( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), !m_bFullScreen, true );
		}
	}

	//// Rueben's debug
	//if( m_pInput->LeftMouseButtonPressed() )
	//{
	//char buffer[64];
	//sprintf_s(buffer, "x-%f | y-%f ", _mousePos.x, _mousePos.y );
	//LOG(LOG_DEBUG, "CVideoOptionsState::Input()", buffer);
	//}

	if(m_pInput->KeyPressed(DIK_RETURN) || 
		m_pInput->KeyPressed(DIK_SPACE) ||
		m_pInput->LeftMouseButtonPressed() )
	{
		if( m_nCurrentlySelected == VIDEO_EXIT )
		{
			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_CANCEL);
			//m_pAudio->ResumeAll();
			m_pGame->PopState();
			return true;
		}

		if( m_nCurrentlySelected == VIDEO_FS )
		{
			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
			m_pGame->SetFullScreen( m_bFullScreen );

			RenderEngine::ChangeDisplayParam( CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), !m_bFullScreen, true );
		}
	}

	if( m_pInput->LeftMouseButtonDown() )
	{
		// if the cursor is on the Gamma Slider
		/*if( _mousePos.x > m_fGammaSliderPosVec.x && _mousePos.x < m_fGammaSliderPosVec.x + VSLIDER_WIDTH
		&& _mousePos.y > m_fGammaSliderPosVec.y && _mousePos.y < m_fGammaSliderPosVec.y + VSLIDER_HEIGHT )*/
		if( m_nCurrentlySelected == VIDEO_GAMMA && m_pGame->GetFullScreen() )
		{
			//Check to see if I clicked inside the slider bar area
			if( _mousePos.x > VSLIDER_MIN && _mousePos.x < VSLIDER_MAX + VSLIDER_WIDTH
				&& _mousePos.y > m_fGammaSliderPosVec.y && _mousePos.y < m_fGammaSliderPosVec.y + (VSLIDER_HEIGHT) && !m_bPrevDown)
			{
				m_bLocked = true;
			}

			if(m_bLocked)
			{
				// set x of the slider pos to the x of the cursor pos
				m_fGammaSliderPosVec.x = _mousePos.x - (VSLIDER_DOWN_WIDTH * 0.5f);
				if( m_fGammaSliderPosVec.x > VSLIDER_MAX )
				{
					m_fGammaSliderPosVec.x = VSLIDER_MAX;
				}
				if( m_fGammaSliderPosVec.x < VSLIDER_MIN )
				{
					m_fGammaSliderPosVec.x = VSLIDER_MIN;
				}

				// play sound
				if(!m_bSound)
				{
					m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);
					m_bSound = true;
				}

				// set gamma
				RenderEngine::SetGamma(((m_fGammaSliderPosVec.x - VSLIDER_MIN) / VSLIDER_BAR_WIDTH) - 0.5f);
			}
		}
		
		if(!m_bPrevDown)
		{
			m_bPrevDown = true;
		}
	}
	else
	{
		m_bSound = false;
		if(m_bPrevDown)
		{
			m_bPrevDown = false;
		}

		if(m_bLocked)
		{
			m_fGammaSliderPosVec.x = m_fGammaSliderPosVec.x + (VSLIDER_DOWN_WIDTH * 0.5f) - (VSLIDER_WIDTH * 0.5f);
			m_bLocked = false;
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Render
// Purpose: Render the state.
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CVideoOptionsState::Render()
{
	TextureManager* pTM = TextureManager::GetInstance();
	RenderEngine::RasterTexture( pTM->GetTexture(m_lpCurrentSelection), 0.0f, 0.0f, 1.0f, 1.0f );
	// Render Brightness Slider
	//RenderEngine::RasterTexture( m_lpOptionsSlider, m_fBrightnessSliderPosVec.x, m_fBrightnessSliderPosVec.y, m_fBrightnessSliderPosVec.x + VSLIDER_WIDTH, m_fBrightnessSliderPosVec.y + VSLIDER_HEIGHT );

	// render block out if full screen
	if( m_pGame->GetFullScreen() )
	{
		// Render Gamma Slider
		if(m_pInput->LeftMouseButtonDown() && m_nCurrentlySelected == VIDEO_GAMMA && m_bLocked)
		{
			RenderEngine::RasterTexture( pTM->GetTexture(m_lpOptionsSlider), m_fGammaSliderPosVec.x, m_fGammaSliderPosVec.y, m_fGammaSliderPosVec.x + VSLIDER_DOWN_WIDTH, m_fGammaSliderPosVec.y + VSLIDER_DOWN_HEIGHT );
		}
		else
		{
			RenderEngine::RasterTexture( pTM->GetTexture(m_lpOptionsSlider), m_fGammaSliderPosVec.x, m_fGammaSliderPosVec.y, m_fGammaSliderPosVec.x + VSLIDER_WIDTH, m_fGammaSliderPosVec.y + VSLIDER_HEIGHT );
		}
	}
	else
	{
		RenderEngine::RasterTexture( pTM->GetTexture(m_lpFullScreenBlockOut), m_fGammaSliderPosVec.x, m_fGammaSliderPosVec.y, m_fGammaSliderPosVec.x + VSLIDER_WIDTH, m_fGammaSliderPosVec.y + VSLIDER_HEIGHT );
	}

	// Render The Check Mark
	RenderEngine::RasterTexture( pTM->GetTexture(m_lpOptionsCheck), m_fCheckMarkPos.x, m_fCheckMarkPos.y, m_fCheckMarkPos.x + VCHECKMARK_WIDTH, m_fCheckMarkPos.y + VCHECKMARK_HEIGHT );

	//Render the cursor reticle at the mouse position
	RenderEngine::RasterTexture(pTM->GetTexture(m_nReticle), _mousePos.x - 0.02f, (-_mousePos.y - 0.02f) + 1.0f, _mousePos.x + 0.02f, (-_mousePos.y + 0.02f) + 1.0f );
}	
