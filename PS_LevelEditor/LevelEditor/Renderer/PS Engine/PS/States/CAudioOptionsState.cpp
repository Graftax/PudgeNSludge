#include "CAudioOptionsStates.h"
#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "../../RenderEngine/TextureManager.h"
#include "../../RenderEngine/RenderEngine.h"

#define ADJUST_AMOUNT 5
#define PLAY_TIME_CAP 1.0f

CAudioOptionsState* CAudioOptionsState::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: COptionsState
// Purpose: Default Constructor.
// Original Author: Rueben Massey
// Creation Date: 5/30/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CAudioOptionsState::CAudioOptionsState()
{
	m_pGame = nullptr;
	m_pInput = nullptr;
	m_pAudio = nullptr;

	m_nCurrentlySelected = 0;
	m_lpCurrentSelection = nullptr;

	m_lpOptionsAudioSound = nullptr;
	m_lpOptionsAudioMusic = nullptr;
	m_lpOptionsAudioDiag = nullptr;
	m_lpOptionsAudioExit = nullptr;
	m_lpOptionsSlider = nullptr;

	m_fPlayTimer = 0.0f;

	m_nReticle = nullptr;

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
CAudioOptionsState::~CAudioOptionsState()
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
CAudioOptionsState* CAudioOptionsState::GetInstance ()
{
	if (!m_pInstance)
	{
		m_pInstance = new CAudioOptionsState;
	}
	LOG(LOG_INFO, "CAudioOptionsState::GetInstance()", "CAudioOptionsState's instance creation succeeded");
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
void CAudioOptionsState::DeleteInstance ()
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
void CAudioOptionsState::Init(void)
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pAudio = m_pGame->GetAudioManager();

	TextureManager* pTM = TextureManager::GetInstance();
	m_lpOptionsAudioSound	= pTM->GetTexture(pTM->LoadTexture("./Resources/Textures/PS_FE_AudioMenu_1.png"));
	m_lpOptionsAudioMusic	= pTM->GetTexture(pTM->LoadTexture("./Resources/Textures/PS_FE_AudioMenu_2.png"));
	m_lpOptionsAudioDiag	= pTM->GetTexture(pTM->LoadTexture("./Resources/Textures/PS_FE_AudioMenu_3.png"));
	m_lpOptionsAudioExit	= pTM->GetTexture(pTM->LoadTexture("./Resources/Textures/PS_FE_AudioMenu_4.png"));
	m_lpOptionsSlider		= pTM->GetTexture(pTM->LoadTexture("./Resources/Textures/PS_FE_Slider.png"));
	m_nReticle				= pTM->GetTexture(pTM->LoadTexture("./Resources/Textures/T_Reticle_D.png"));

	// Initial Slider Positions. Set to the position relative to the sounds they represent.
	m_fSFXSliderPosVec = vec2f( ASLIDER_MIN + ASLIDER_BAR_WIDTH * ((float)(m_pAudio->GetFXVolume()) * 0.01f), ASLIDER_SFX_POSY );
	m_fMusicSliderPosVec = vec2f( ASLIDER_MIN + ASLIDER_BAR_WIDTH * ((float)(m_pAudio->GetBGMVolume()) * 0.01f), ASLIDER_MUSIC_POSY );
	m_fDialogueSliderPosVec = vec2f( ASLIDER_MIN + ASLIDER_BAR_WIDTH * ((float)(m_pAudio->GetDLGVolume()) * 0.01f), ASLIDER_DIALOGUE_POSY );

	// position SFX Slider
	// Based on a ratio of volume to slider width
	//m_fSFXSliderPosVec.x = ASLIDER_MIN + (ASLIDER_BAR_WIDTH * 0.01f) * m_pAudio->GetFXVolume();
	// position Music Slider
	// Based on a ratio of volume to slider width
	// position Dialogue Slider
	// Based on a ratio of volume to slider width

	m_nCurrentlySelected = AUDIO_SFX;

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
void CAudioOptionsState::Shutdown()
{
	// Unload all the textures
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsAudioSound );
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsAudioExit );
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsAudioMusic );
	TextureManager::GetInstance()->UnloadTexture( m_lpOptionsAudioDiag );
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
void CAudioOptionsState::Update(float fTime)
{
	m_fPlayTimer += fTime;

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
	if(!m_pInput->LeftMouseButtonDown())
	{
		if( _mousePos.x > AMENU_MIN_X && _mousePos.x < AMENU_MAX_X )
		{
			if( _mousePos.y > ASFX_MIN_Y && _mousePos.y < ASFX_MAX_Y )
			{
				m_nCurrentlySelected = AUDIO_SFX;
				m_pAudio->StopMusic(BGM_TITLE);
			}
			if( _mousePos.y > AMUSIC_MIN_Y && _mousePos.y < AMUSIC_MAX_Y )
			{
				if(m_nCurrentlySelected != AUDIO_MUSIC)
				{
					m_pAudio->PlayMusic(BGM_TITLE);
				}

				m_nCurrentlySelected = AUDIO_MUSIC;
			}
			if( _mousePos.y > ADIALOGUE_MIN_Y && _mousePos.y < ADIALOGUE_MAX_Y )
			{
				m_nCurrentlySelected = AUDIO_DIALOGUE;
				m_pAudio->StopMusic(BGM_TITLE);
			}
			if( _mousePos.y > AEXIT_MIN_Y && _mousePos.y < AEXIT_MAX_Y )
			{
				m_nCurrentlySelected = AUDIO_EXIT;
				m_pAudio->StopMusic(BGM_TITLE);
			}
		}
	}

	// texture swapping
	switch( m_nCurrentlySelected)
	{
	case AUDIO_SFX:
		{
			m_lpCurrentSelection = m_lpOptionsAudioSound;
			break;
		}
	case AUDIO_MUSIC:
		{
			m_lpCurrentSelection = m_lpOptionsAudioMusic;
			break;
		}
	case AUDIO_DIALOGUE:
		{
			m_lpCurrentSelection = m_lpOptionsAudioDiag;
			break;
		}
	case AUDIO_EXIT:
		{
			m_lpCurrentSelection = m_lpOptionsAudioExit;
			break;
		}
	default:
		break;
	}

	if(m_pInput->LeftMouseButtonDown() && m_nCurrentlySelected != AUDIO_EXIT)
	{
		// keep the mouse at the position of the slider
		//_mousePos.x = m_fGammaSliderPosVec.x + (VSLIDER_WIDTH * 0.5f);
		D3DXVECTOR4 mousePosCopy;

		if(_mousePos.x < AMENU_MIN_X + (ASLIDER_WIDTH))
		{
			_mousePos.x = AMENU_MIN_X + (ASLIDER_WIDTH);
		}
		if(_mousePos.x > AMENU_MAX_X - (ASLIDER_WIDTH))
		{
			_mousePos.x = AMENU_MAX_X - (ASLIDER_WIDTH);
		}

		if(m_nCurrentlySelected == AUDIO_SFX)
		{
			_mousePos.y = m_fSFXSliderPosVec.y + (ASLIDER_HEIGHT * 0.5f);
		}
		if(m_nCurrentlySelected == AUDIO_MUSIC)
		{
			_mousePos.y = m_fMusicSliderPosVec.y + (ASLIDER_HEIGHT * 0.5f);
		}
		if(m_nCurrentlySelected == AUDIO_DIALOGUE)
		{
			_mousePos.y = m_fDialogueSliderPosVec.y + (ASLIDER_HEIGHT * 0.5f);
		}
		
		mousePosCopy = _mousePos;
		mousePosCopy *= mousePosCopy.w + 400;
		mousePosCopy = D3DXVECTOR4(mousePosCopy.x, 768.0f - mousePosCopy.y, 1.0f, 1.0f );
		POINT ptCursor;
		ptCursor.x = (LONG)mousePosCopy.x;
		ptCursor.y = (LONG)mousePosCopy.y;
		
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
bool CAudioOptionsState::Input()
{
	// Safety Check
	if(!m_pInput)
	{
		return false;
	}

	if(m_pInput->KeyPressed(DIK_ESCAPE))
	{
		// Change to Options menu
		m_pAudio->StopMusic(BGM_TITLE);
		m_pGame->PopState();
		return true;
	}

	// KeyBoard Input
	if( m_pInput->KeyPressed( DIK_UP ) || m_pInput->KeyPressed(DIK_W) )
	{
		m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);

		--m_nCurrentlySelected;

		//if we're on music, play the music, if not stop the music
		if(m_nCurrentlySelected == AUDIO_MUSIC)
		{
			m_pAudio->PlayMusic(BGM_TITLE);
		}
		else
		{
			m_pAudio->StopMusic(BGM_TITLE);
		}

		if( m_nCurrentlySelected < AUDIO_SFX)
			m_nCurrentlySelected = AUDIO_EXIT;
	}
	if( m_pInput->KeyPressed( DIK_DOWN ) || m_pInput->KeyPressed(DIK_S) )
	{
		m_pAudio->PlayEffect(SFX_MENU_HIGHLIGHT);

		++m_nCurrentlySelected;

		//if we're on music, play the music, if not stop the music
		if(m_nCurrentlySelected == AUDIO_MUSIC)
		{
			m_pAudio->PlayMusic(BGM_TITLE);
		}
		else
		{
			m_pAudio->StopMusic(BGM_TITLE);
		}

		if( m_nCurrentlySelected > AUDIO_EXIT)
			m_nCurrentlySelected = AUDIO_SFX;
	}

	if( m_pInput->KeyPressed( DIK_RIGHT ) || m_pInput->KeyPressed(DIK_D) )
	{
		// Move Sliders
		// based on current selection
		if( AUDIO_SFX == m_nCurrentlySelected )
		{
			//adjust the volume, and then the slider based on the new volume
			m_pAudio->SetFXVolume(m_pAudio->GetFXVolume() + ADJUST_AMOUNT);
			m_fSFXSliderPosVec.x = ASLIDER_MIN + ASLIDER_BAR_WIDTH * ((float)(m_pAudio->GetFXVolume()) * 0.01f);
			if(m_fPlayTimer > PLAY_TIME_CAP)
			{
				m_pAudio->PlayEffect(SFX_CRATE_CRASH);
				m_fPlayTimer = 0.0f;
			}
		}
		if( AUDIO_MUSIC == m_nCurrentlySelected )
		{
			//adjust the volume, and then the slider based on the new volume
			m_pAudio->SetBGMVolume(m_pAudio->GetBGMVolume() + ADJUST_AMOUNT);
			m_fMusicSliderPosVec.x  = ASLIDER_MIN + ASLIDER_BAR_WIDTH * ((float)(m_pAudio->GetBGMVolume()) * 0.01f);
		}
		if( AUDIO_DIALOGUE == m_nCurrentlySelected )
		{
			//adjust the volume, and then the slider based on the new volume
			m_pAudio->SetDLGVolume(m_pAudio->GetDLGVolume() + ADJUST_AMOUNT);
			m_fDialogueSliderPosVec.x  = ASLIDER_MIN + ASLIDER_BAR_WIDTH * ((float)(m_pAudio->GetDLGVolume()) * 0.01f);
			if(m_fPlayTimer > PLAY_TIME_CAP)
			{
				m_pAudio->PlayDialogue(DLG_PLAYER_HURT);
				m_fPlayTimer = 0.0f;
			}
		}
	}
	if( m_pInput->KeyPressed( DIK_LEFT ) || m_pInput->KeyPressed(DIK_A) )
	{
		// Move Sliders
		// based on current selection
		if( AUDIO_SFX == m_nCurrentlySelected )
		{
			//adjust the volume, and then the slider based on the new volume
			m_pAudio->SetFXVolume(m_pAudio->GetFXVolume() - ADJUST_AMOUNT);
			m_fSFXSliderPosVec.x = ASLIDER_MIN + ASLIDER_BAR_WIDTH * ((float)(m_pAudio->GetFXVolume()) * 0.01f);
			if(m_fPlayTimer > PLAY_TIME_CAP)
			{
				m_pAudio->PlayEffect(SFX_CRATE_CRASH);
				m_fPlayTimer = 0.0f;
			}
		}
		if( AUDIO_MUSIC == m_nCurrentlySelected )
		{
			//adjust the volume, and then the slider based on the new volume
			m_pAudio->SetBGMVolume(m_pAudio->GetBGMVolume() - ADJUST_AMOUNT);
			m_fMusicSliderPosVec.x  = ASLIDER_MIN + ASLIDER_BAR_WIDTH * ((float)(m_pAudio->GetBGMVolume()) * 0.01f);
		}
		if( AUDIO_DIALOGUE == m_nCurrentlySelected )
		{
			//adjust the volume, and then the slider based on the new volume
			m_pAudio->SetDLGVolume(m_pAudio->GetDLGVolume() - ADJUST_AMOUNT);
			m_fDialogueSliderPosVec.x  = ASLIDER_MIN + ASLIDER_BAR_WIDTH * ((float)(m_pAudio->GetDLGVolume()) * 0.01f);
			if(m_fPlayTimer > PLAY_TIME_CAP)
			{
				m_pAudio->PlayDialogue(DLG_PLAYER_HURT);
				m_fPlayTimer = 0.0f;
			}
		}
	}

	// Rueben's debug
	/*if( m_pInput->LeftMouseButtonPressed() )
	{
	char buffer[64];
	sprintf_s(buffer, "x-%f | y-%f ", _mousePos.x, _mousePos.y );
	LOG(LOG_DEBUG, "CAudioOptionsState::Input()", buffer);
	}*/

	if(m_pInput->KeyPressed(DIK_RETURN) || 
		m_pInput->KeyPressed(DIK_SPACE) ||
		m_pInput->LeftMouseButtonPressed() )
	{
		if( m_nCurrentlySelected == AUDIO_EXIT )
		{
			m_pAudio->PlayEffect(SFX_MENU_CANCEL);
			//m_pAudio->ResumeAll();
			m_pGame->PopState();
			return true;
		}
	}

	if( m_pInput->LeftMouseButtonDown() )
	{
		// if the cursor is on the Brightness Slider
		/*if( _mousePos.x > m_fSFXSliderPosVec.x && _mousePos.x < m_fSFXSliderPosVec.x + ASLIDER_WIDTH
		&& _mousePos.y > m_fSFXSliderPosVec.y && _mousePos.y < m_fSFXSliderPosVec.y + ASLIDER_HEIGHT )*/
		if( m_nCurrentlySelected == AUDIO_SFX )
		{
			// set x of the slider pos to the x of the cursor pos
			m_fSFXSliderPosVec.x = _mousePos.x - (ASLIDER_DOWN_WIDTH * 0.5f);
			if( m_fSFXSliderPosVec.x > ASLIDER_MAX )
			{
				m_fSFXSliderPosVec.x = ASLIDER_MAX;
			}
			if( m_fSFXSliderPosVec.x < ASLIDER_MIN )
			{
				m_fSFXSliderPosVec.x = ASLIDER_MIN;
			}
		
			// Set Volume based on the location of the slider
			m_pAudio->SetFXVolume((int)(((m_fSFXSliderPosVec.x - ASLIDER_MIN) / ASLIDER_BAR_WIDTH) * 100));

			// Player Sound
			if(m_fPlayTimer > PLAY_TIME_CAP)
			{
				m_pAudio->PlayEffect(SFX_CRATE_CRASH);
				m_fPlayTimer = 0.0f;
			}

			m_bPrevDown = true;
		}

		// if the cursor is on the Gamma Slider
		/*if( _mousePos.x > m_fMusicSliderPosVec.x && _mousePos.x < m_fMusicSliderPosVec.x + ASLIDER_WIDTH
			&& _mousePos.y > m_fMusicSliderPosVec.y && _mousePos.y < m_fMusicSliderPosVec.y + ASLIDER_HEIGHT )*/
		if( m_nCurrentlySelected == AUDIO_MUSIC )
		{
			// set x of the slider pos to the x of the cursor pos
			m_fMusicSliderPosVec.x = _mousePos.x - (ASLIDER_DOWN_WIDTH * 0.5f);
			if( m_fMusicSliderPosVec.x > ASLIDER_MAX )
			{
				m_fMusicSliderPosVec.x = ASLIDER_MAX;
			}
			if( m_fMusicSliderPosVec.x < ASLIDER_MIN )
			{
				m_fMusicSliderPosVec.x = ASLIDER_MIN;
			}

			// Set Volume based on the location of the slider
			m_pAudio->SetBGMVolume((int)(((m_fMusicSliderPosVec.x - ASLIDER_MIN) / ASLIDER_BAR_WIDTH) * 100));

			m_bPrevDown = true;
		}

		// if the cursor is on the Gamma Slider
		/*if( _mousePos.x > m_fDialogueSliderPosVec.x && _mousePos.x < m_fDialogueSliderPosVec.x + ASLIDER_WIDTH
			&& _mousePos.y > m_fDialogueSliderPosVec.y && _mousePos.y < m_fDialogueSliderPosVec.y + ASLIDER_HEIGHT )*/
		if( m_nCurrentlySelected == AUDIO_DIALOGUE )
		{
			// set x of the slider pos to the x of the cursor pos
			m_fDialogueSliderPosVec.x = _mousePos.x - (ASLIDER_DOWN_WIDTH * 0.5f);
			if( m_fDialogueSliderPosVec.x > ASLIDER_MAX )
			{
				m_fDialogueSliderPosVec.x = ASLIDER_MAX;
			}
			if( m_fDialogueSliderPosVec.x < ASLIDER_MIN )
			{
				m_fDialogueSliderPosVec.x = ASLIDER_MIN;
			}

			// Set Volume based on the location of the slider
			m_pAudio->SetDLGVolume((int)(((m_fDialogueSliderPosVec.x - ASLIDER_MIN) / ASLIDER_BAR_WIDTH) * 100));

			// Player Sound
			if(m_fPlayTimer > PLAY_TIME_CAP)
			{
				m_pAudio->PlayDialogue(DLG_PLAYER_HURT);
				m_fPlayTimer = 0.0f;
			}

			m_bPrevDown = true;
		}
	}
	else
	{
		if(m_bPrevDown)
		{
			if( m_nCurrentlySelected == AUDIO_SFX )
			{
				m_fSFXSliderPosVec.x = m_fSFXSliderPosVec.x + (ASLIDER_DOWN_WIDTH * 0.5f) - (ASLIDER_WIDTH * 0.5f);
			}

			if( m_nCurrentlySelected == AUDIO_MUSIC )
			{
				m_fMusicSliderPosVec.x = m_fMusicSliderPosVec.x + (ASLIDER_DOWN_WIDTH * 0.5f) - (ASLIDER_WIDTH * 0.5f);
			}

			if( m_nCurrentlySelected == AUDIO_DIALOGUE )
			{
				m_fDialogueSliderPosVec.x = m_fDialogueSliderPosVec.x + (ASLIDER_DOWN_WIDTH * 0.5f) - (ASLIDER_WIDTH * 0.5f);
			}
			
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
void CAudioOptionsState::Render()
{
	// Render Background
	RenderEngine::RasterTexture( m_lpCurrentSelection, 0.0f, 0.0f, 1.0f, 1.0f );
	// Render SFX Slider
	if(m_pInput->LeftMouseButtonDown() && m_nCurrentlySelected == AUDIO_SFX)
	{
		RenderEngine::RasterTexture( m_lpOptionsSlider, m_fSFXSliderPosVec.x, m_fSFXSliderPosVec.y, m_fSFXSliderPosVec.x + ASLIDER_DOWN_WIDTH, m_fSFXSliderPosVec.y + ASLIDER_DOWN_HEIGHT );
	}
	else
	{
		RenderEngine::RasterTexture( m_lpOptionsSlider, m_fSFXSliderPosVec.x, m_fSFXSliderPosVec.y, m_fSFXSliderPosVec.x + ASLIDER_WIDTH, m_fSFXSliderPosVec.y + ASLIDER_HEIGHT );
	}
	// Render Music Slider
	if(m_pInput->LeftMouseButtonDown() && m_nCurrentlySelected == AUDIO_MUSIC)
	{
		RenderEngine::RasterTexture( m_lpOptionsSlider, m_fMusicSliderPosVec.x, m_fMusicSliderPosVec.y, m_fMusicSliderPosVec.x + ASLIDER_DOWN_WIDTH, m_fMusicSliderPosVec.y + ASLIDER_DOWN_HEIGHT );
	}
	else
	{
		RenderEngine::RasterTexture( m_lpOptionsSlider, m_fMusicSliderPosVec.x, m_fMusicSliderPosVec.y, m_fMusicSliderPosVec.x + ASLIDER_WIDTH, m_fMusicSliderPosVec.y + ASLIDER_HEIGHT );
	}
	// Render Dialogue Slider
	if(m_pInput->LeftMouseButtonDown() && m_nCurrentlySelected == AUDIO_DIALOGUE)
	{
		RenderEngine::RasterTexture( m_lpOptionsSlider, m_fDialogueSliderPosVec.x, m_fDialogueSliderPosVec.y, m_fDialogueSliderPosVec.x + ASLIDER_DOWN_WIDTH, m_fDialogueSliderPosVec.y + ASLIDER_DOWN_HEIGHT );
	}
	else
	{
		RenderEngine::RasterTexture( m_lpOptionsSlider, m_fDialogueSliderPosVec.x, m_fDialogueSliderPosVec.y, m_fDialogueSliderPosVec.x + ASLIDER_WIDTH, m_fDialogueSliderPosVec.y + ASLIDER_HEIGHT );
	}
	//Render the cursor reticle at the mouse position
	RenderEngine::RasterTexture(m_nReticle, _mousePos.x - 0.02f, _mousePos.y - 0.02f, _mousePos.x + 0.02f, _mousePos.y + 0.02f );
}