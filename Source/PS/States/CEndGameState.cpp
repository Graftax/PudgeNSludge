#include "CEndGameState.h"
#include "MainMenuState.h"
#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "../../RenderEngine/RenderEngine.h"
#include "../../RenderEngine/TextureManager.h"
#include "GamePlayState.h"


CEndGameState* CEndGameState::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CEndGameState
// Purpose: Default Constructor.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CEndGameState::CEndGameState()
{
	m_nSelectedItem = 0;
	m_pAudio = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ~CEndGameState
// Purpose: Default Destructor.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CEndGameState::~CEndGameState()
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
CEndGameState* CEndGameState::GetInstance ()
{
	if (!m_pInstance)
	{
		m_pInstance = new CEndGameState;
	}
	LOG(LOG_INFO, "CEndGameState::GetInstance()", "CEndGameState's instance creation succeeded");
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
void CEndGameState::DeleteInstance ()
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
void CEndGameState::Init ()
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pAudio = m_pGame->GetAudioManager();
	m_nSelectedItem = 0;
	m_nEndTextureHandle = /*TextureManager::GetInstance()->GetTexture(*/ TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_Death_Exit.png" );// );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ShutDown
// Purpose: Shuts down the state
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CEndGameState::Shutdown ()
{
	TextureManager::GetInstance()->UnloadTexture( m_nEndTextureHandle );

	m_nSelectedItem = 0;
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
bool CEndGameState::Input ()
{
	// How do I get input?
	if(!m_pInput)
	{
		return false;
	}

	//Keyboard Input
	if(m_pInput->KeyPressed(DIK_UP) || m_pInput->KeyPressed(DIK_W) )
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);

		//move cursor up
		--m_nSelectedItem;

		if(m_nSelectedItem < 0)
		{
			m_nSelectedItem = EX_NUM_ITEMS - 1;
		}
	}

	if(m_pInput->KeyPressed(DIK_DOWN) || m_pInput->KeyPressed(DIK_S))
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT);

		//move cusor down
		++m_nSelectedItem;

		if(m_nSelectedItem >= NUM_ITEMS)
		{
			m_nSelectedItem = EX_LASTCHECK;
		}
	}

	if(m_pInput->KeyPressed(DIK_RETURN) || 
		m_pInput->KeyPressed(DIK_SPACE) ||
		m_pInput->LeftMouseButtonPressed() )
	{
		m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_MENU_ACCEPT);
		//enter into the next menu item
		switch(m_nSelectedItem)
		{
		case EX_LASTCHECK:
			{
				//change state to gameplay state. stop music and I'm no longer on top.
				//m_pAudio->StopMusic(BGM_TITLE);

				CGame::GetInstance()->ChangeState( CGamePlayState::GetInstance() );
				break;
			}
		case EX_EXIT:
			{
				//exit the game
				CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );
				break;
			}
		default:
			{
				break;
			}
		}
	}	
	return true;

	// How do I get input?
	/*if(!m_pInput)
	{
		return false;
	}

	if(m_pInput->KeyPressed(DIK_RETURN) || 
		m_pInput->KeyPressed(DIK_SPACE))
	{
		CGame::GetInstance()->ChangeState ( CMainMenuState::GetInstance() ); 
	}

	return true;*/
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update
// Purpose: Updates the state.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CEndGameState::Update (float fElapsedTime)
{
	// what system do I need for swapping the textures
	// for every option
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Render
// Purpose: Renders the state;
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CEndGameState::Render ()
{
	// How do I Render my stuff?'
	RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture(m_nEndTextureHandle), 0.0f, 0.0f, 1.0f, 1.0f );
}