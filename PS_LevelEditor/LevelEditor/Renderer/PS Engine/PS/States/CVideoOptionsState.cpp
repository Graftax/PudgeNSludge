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
	m_lpCurrentSelection = nullptr;

	m_bFullScreen = false;
	m_bSound = false;
	m_bPrevDown = false;
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
	//m_lpOptionsVideoBright	= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_VIdeoMenu_1.png" ) );
	m_lpOptionsVideoGamma	= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_VideoMenu_1.png" ) );
	m_lpOptionsVideoFS_On	= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_VideoMenu_2.png" ) );
	m_lpOptionsVideoFS_Off	= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_VideoMenu_3.png" ) );
	m_lpOptionsVideoExit	= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_VideoMenu_4.png" ) );
	m_lpOptionsCheck		= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_VideoMenu_Check.png" ) );
	m_lpOptionsSlider		= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/PS_FE_Slider.png" ) );
	m_nReticle				= pTM->GetTexture( pTM->LoadTexture("./Resources/Textures/T_Reticle_D.png"));

	// Initial Slider Positions
	//m_fBrightnessSliderPosVec	= vec2f( VSLIDER_MIN, VSLIDER_BRIGHTNESS_POSY );
	m_fGammaSliderPosVec		= vec2f( VSLIDER_MIN + VSLIDER_BAR_WIDTH * (RenderEngine::GetGamma() + 0.5f), VSLIDER_GAMMA_POSY );
	if(m_pGame->GetFullScreen())
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
	m_bFullScreen = m_pGame->GetFullScreen();

	m_bPrevDown = false;
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
	// Unload all the textures
	//TextureManager::GetInstance()->UnloadTexture( m_lpOptionsVideoBright );
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsVideoGamma );
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsVideoExit );
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsVideoFS_On );
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsVideoFS_Off );
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsCheck);
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsSlider );
	TextureManager::GetInstance()->UnloadTexture( m_nReticle );
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
	// Get the cursor in screen space
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(CGame::GetInstance()->GetHWND(), &ptCursor);

	// convert it to a value between 0 and 1
	_mousePos = D3DXVECTOR4((FLOAT)ptCursor.x, 768.0f - (FLOAT)ptCursor.y, 1.0f, 1.0f );
	_mousePos /= _mousePos.w + 400;

	if(_mousePos.x <= 0.0f)
		_mousePos.x = 0.0f;
	if(_mousePos.x >= 1.0f)
		_mousePos.x = 1.0f;

	if(_mousePos.y <= 0.0f)
		_mousePos.y = 0.0f;
	if(_mousePos.y >= 1.0f)
		_mousePos.y = 1.0f;

	// Change Selection
	// Based on the cursor position
	if( _mousePos.x > VMENU_MIN_X && _mousePos.x < VMENU_MAX_X && !m_pInput->LeftMouseButtonDown() )
	{
		//if( _mousePos.y > VBRIGHT_MIN_Y && _mousePos.y < VBRIGHT_MAX_Y )
		//{
		//	m_nCurrentlySelected = VIDEO_BRIGHTNESS;
		//}
		if( _mousePos.y > VGAMMA_MIN_Y && _mousePos.y < VGAMMA_MAX_Y )
		{
			m_nCurrentlySelected = VIDEO_GAMMA;
		}
		if( _mousePos.y > VEXIT_MIN_Y && _mousePos.y < VEXIT_MAX_Y )
		{
			m_nCurrentlySelected = VIDEO_EXIT;
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
	/*if( _mousePos.y > VFULLSCREEN_MIN_Y && _mousePos.y < VFULLSCREEN_MAX_Y )
	{
		if( _mousePos.x > VFULLSCREEN_ON_MIN_X && _mousePos.x < VFULLSCREEN_ON_MAX_X )
		{
			m_nCurrentlySelected = VIDEO_FS_ON;
		}
		if( _mousePos.x > VFULLSCREEN_OFF_MIN_X && _mousePos.x < VFULLSCREEN_OFF_MAX_X )
		{
			m_nCurrentlySelected = VIDEO_FS_OFF;
		}
	}*/

	// texture swapping
	switch( m_nCurrentlySelected)
	{
	//case VIDEO_BRIGHTNESS:
	//	{
	//		m_lpCurrentSelection = m_lpOptionsVideoBright;
	//		m_bFullScreen = m_pGame->GetFullScreen();
	//		break;
	//	}
	case VIDEO_GAMMA:
		{
			m_lpCurrentSelection = m_lpOptionsVideoGamma;
			
			break;
		}
	case VIDEO_FS:
		{
			//m_bFullScreen = m_pGame->GetFullScreen();
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

	if(m_pInput->LeftMouseButtonDown() && m_nCurrentlySelected == VIDEO_GAMMA)
	{
		// keep the mouse at the position of the slider
		//_mousePos.x = m_fGammaSliderPosVec.x + (VSLIDER_WIDTH * 0.5f);
		D3DXVECTOR4 mousePosCopy;

		if(_mousePos.x < VMENU_MIN_X + (VSLIDER_WIDTH))
		{
			_mousePos.x = VMENU_MIN_X + (VSLIDER_WIDTH);
		}
		if(_mousePos.x > VMENU_MAX_X - (VSLIDER_WIDTH))
		{
			_mousePos.x = VMENU_MAX_X - (VSLIDER_WIDTH);
		}

		_mousePos.y = m_fGammaSliderPosVec.y + (VSLIDER_HEIGHT * 0.5f);
		mousePosCopy = _mousePos;
		mousePosCopy *= mousePosCopy.w + 400;
		mousePosCopy = D3DXVECTOR4(mousePosCopy.x, 768.0f - mousePosCopy.y, 1.0f, 1.0f );
		POINT ptCursor;
		ptCursor.x = (LONG)(mousePosCopy.x);
		ptCursor.y = (LONG)(mousePosCopy.y);
		
		ClientToScreen(m_pGame->GetHWND(), &ptCursor);
		
		SetCursorPos(ptCursor.x + 1, ptCursor.y);
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
		m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);

		--m_nCurrentlySelected;

		if(m_pGame->GetFullScreen())
		{
			m_bFullScreen = true;
		}
		else
		{
			m_bFullScreen = false;
		}

		if( m_nCurrentlySelected < VIDEO_GAMMA)
			m_nCurrentlySelected = VIDEO_EXIT;
	}
	if( m_pInput->KeyPressed( DIK_DOWN ) || m_pInput->KeyPressed(DIK_S) )
	{
		m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);

		++m_nCurrentlySelected;

		if(m_pGame->GetFullScreen())
		{
			m_bFullScreen = true;
		}
		else
		{
			m_bFullScreen = false;
		}

		if( m_nCurrentlySelected > VIDEO_EXIT)
			m_nCurrentlySelected = VIDEO_GAMMA;
	}

	if( m_pInput->KeyPressed( DIK_RIGHT ) || m_pInput->KeyPressed(DIK_D) )
	{
		// Move Sliders
		// based on current selection
		//if( VIDEO_BRIGHTNESS == m_nCurrentlySelected )
		//{
		//	m_fBrightnessSliderPosVec.x += VSLIDER_BAR_WIDTH * 0.01f;
		//	// play sound
		//
		//	// set brightness
		//}
		if( VIDEO_GAMMA == m_nCurrentlySelected && m_pGame->GetFullScreen() )
		{
			// set gamma
			RenderEngine::SetGamma(RenderEngine::GetGamma() + (ADJUST_AMOUNT * 0.01f));

			// set the new slider position based on the gamma change
			m_fGammaSliderPosVec.x = VSLIDER_MIN + VSLIDER_BAR_WIDTH * ((RenderEngine::GetGamma() + 0.5f));

			// play sound
			m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);
		}

		if( VIDEO_FS == m_nCurrentlySelected )
		{
			m_bFullScreen = !m_bFullScreen;
		}
	}
	if( m_pInput->KeyPressed( DIK_LEFT ) || m_pInput->KeyPressed(DIK_A) )
	{
		// Move Sliders
		// based on current selection
		//if( VIDEO_BRIGHTNESS == m_nCurrentlySelected )
		//{
		//	m_fBrightnessSliderPosVec.x -= VSLIDER_BAR_WIDTH * 0.01f;
		//	// play sound
		//
		//	// set brightness
		//
		//}
		if( VIDEO_GAMMA == m_nCurrentlySelected && m_pGame->GetFullScreen() )
		{
			// set gamma
			RenderEngine::SetGamma(RenderEngine::GetGamma() - (ADJUST_AMOUNT * 0.01f));

			// set the new slider position based on the gamma change
			m_fGammaSliderPosVec.x = VSLIDER_MIN + VSLIDER_BAR_WIDTH * ((RenderEngine::GetGamma() + 0.5f));

			// play sound
			m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);
		}
		if( VIDEO_FS == m_nCurrentlySelected )
		{
			m_bFullScreen = !m_bFullScreen;
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
			m_pAudio->PlayEffect(SFX_MENU_CANCEL);
			//m_pAudio->ResumeAll();
			m_pGame->PopState();
			return true;
		}

		if( m_nCurrentlySelected == VIDEO_FS )
		{
			m_pAudio->PlayEffect(SFX_MENU_ACCEPT);
			m_pGame->SetFullScreen( m_bFullScreen );
			//RenderEngine::SetGamma(0.0f);
			//m_fGammaSliderPosVec.x = VSLIDER_MIN + VSLIDER_BAR_WIDTH * ((RenderEngine::GetGamma() + 1) * 0.5f);
			if(m_bFullScreen)
			{
				m_fCheckMarkPos	= vec2f( VCHECK_ON_MIN_X, VCHECK_ON_MIN_Y );
			}
			else
			{
				m_fCheckMarkPos	= vec2f( VCHECK_OFF_MIN_X, VCHECK_OFF_MIN_Y );
			}
		}
	}

	if( m_pInput->LeftMouseButtonDown() )
	{
		// if the cursor is on the Brightness Slider
		/*if( _mousePos.x > m_fBrightnessSliderPosVec.x && _mousePos.x < m_fBrightnessSliderPosVec.x + VSLIDER_WIDTH
			&& _mousePos.y > m_fBrightnessSliderPosVec.y && _mousePos.y < m_fBrightnessSliderPosVec.y + VSLIDER_HEIGHT )*/
		//if( m_nCurrentlySelected == VIDEO_BRIGHTNESS )
		//{
		//	// set x of the slider pos to the x of the cursor pos
		//	m_fBrightnessSliderPosVec.x = _mousePos.x - 0.015f;
		//	if( m_fBrightnessSliderPosVec.x > VSLIDER_MAX )
		//	{
		//		m_fBrightnessSliderPosVec.x = VSLIDER_MAX;
		//	}
		//	if( m_fBrightnessSliderPosVec.x < VSLIDER_MIN )
		//	{
		//		m_fBrightnessSliderPosVec.x = VSLIDER_MIN;
		//	}
		//	// play sound
		//
		//	// set brightness
		//
		//}

		// if the cursor is on the Gamma Slider
		/*if( _mousePos.x > m_fGammaSliderPosVec.x && _mousePos.x < m_fGammaSliderPosVec.x + VSLIDER_WIDTH
			&& _mousePos.y > m_fGammaSliderPosVec.y && _mousePos.y < m_fGammaSliderPosVec.y + VSLIDER_HEIGHT )*/
		if( m_nCurrentlySelected == VIDEO_GAMMA && m_pGame->GetFullScreen() )
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
				m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);
				m_bSound = true;
			}

			// set gamma
			RenderEngine::SetGamma(((m_fGammaSliderPosVec.x - VSLIDER_MIN) / VSLIDER_BAR_WIDTH) - 0.5f);

			m_bPrevDown = true;
		}
	}
	else
	{
		m_bSound = false;
		if(m_bPrevDown)
		{
			m_fGammaSliderPosVec.x = m_fGammaSliderPosVec.x + (VSLIDER_DOWN_WIDTH * 0.5f) - (VSLIDER_WIDTH * 0.5f);
			m_bPrevDown = false;
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
	RenderEngine::RasterTexture( m_lpCurrentSelection, 0.0f, 0.0f, 1.0f, 1.0f );
	// Render Brightness Slider
	//RenderEngine::RasterTexture( m_lpOptionsSlider, m_fBrightnessSliderPosVec.x, m_fBrightnessSliderPosVec.y, m_fBrightnessSliderPosVec.x + VSLIDER_WIDTH, m_fBrightnessSliderPosVec.y + VSLIDER_HEIGHT );
	// Render Gamma Slider
	if(m_pInput->LeftMouseButtonDown() && m_nCurrentlySelected == VIDEO_GAMMA)
	{
		RenderEngine::RasterTexture( m_lpOptionsSlider, m_fGammaSliderPosVec.x, m_fGammaSliderPosVec.y, m_fGammaSliderPosVec.x + VSLIDER_DOWN_WIDTH, m_fGammaSliderPosVec.y + VSLIDER_DOWN_HEIGHT );
	}
	else
	{
		RenderEngine::RasterTexture( m_lpOptionsSlider, m_fGammaSliderPosVec.x, m_fGammaSliderPosVec.y, m_fGammaSliderPosVec.x + VSLIDER_WIDTH, m_fGammaSliderPosVec.y + VSLIDER_HEIGHT );
	}
	// Render The Check Mark
	RenderEngine::RasterTexture( m_lpOptionsCheck, m_fCheckMarkPos.x, m_fCheckMarkPos.y, m_fCheckMarkPos.x + VCHECKMARK_WIDTH, m_fCheckMarkPos.y + VCHECKMARK_HEIGHT );

	//Render the cursor reticle at the mouse position
	RenderEngine::RasterTexture(m_nReticle, _mousePos.x - 0.02f, _mousePos.y - 0.02f, _mousePos.x + 0.02f, _mousePos.y + 0.02f );
}