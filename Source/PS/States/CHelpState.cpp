#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "../../RenderEngine/RenderEngine.h"
#include "../../RenderEngine/TextureManager.h"
#include "CHelpState.h"
#include "COptionsState.h"
#include "MainMenuState.h"

CHelpState* CHelpState::m_pInstance = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CPauseState
// Purpose: Default Constructor.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:	
// Last Modification Date:	
/////////////////////////////////////////////////////////////////////////////////////////////////////
CHelpState::CHelpState()
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
CHelpState::~CHelpState()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetInstance
// Purpose: Returns the instance of the Help Screen.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
CHelpState* CHelpState::GetInstance ()
{
	if (!m_pInstance)
	{
		m_pInstance = new CHelpState;
	}
	LOG(LOG_INFO, "CHelpState::GetInstance()", "CHelpState's instance creation succeeded");
	return m_pInstance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: DeleteInstance
// Purpose: Deletes the instance of the Help Screen.
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CHelpState::DeleteInstance ()
{
	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = nullptr;
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
void CHelpState::Init ()
{
	m_pGame = CGame::GetInstance();
	m_pInput = CGame::GetInstance()->GetInputManager();

	m_nHelpScreen = TextureManager::GetInstance()->LoadTexture( "./Resources/Menus/PS_FE_HelpScreen.png" );

	m_InputDelay = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ShutDown
// Purpose: Shuts down the state
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CHelpState::Shutdown ()
{
	TextureManager::GetInstance()->UnloadTexture( m_nHelpScreen );

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
bool CHelpState::Input ()
{
	if( m_InputDelay )
	{
		m_InputDelay = false;
		return true;
	}
	if( m_pInput )
	{
		if( m_pInput->KeyPressed( DIK_F1 ) || m_pInput->KeyPressed( DIK_ESCAPE ) || m_pInput->KeyPressed( DIK_RETURN ) || m_pInput->KeyPressed( DIK_SPACE ) || m_pInput->LeftMouseButtonPressed() )
		{
			m_pGame->PopState();
		}
	}
	else
		m_pInput = CGame::GetInstance()->GetInputManager();

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
void CHelpState::Update (float fElapsedTime)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Render
// Purpose: Renders the state;
// Original Author: Ethan Pendergraft
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CHelpState::Render ()
{
	RenderEngine::RasterTexture( TextureManager::GetInstance()->GetTexture( m_nHelpScreen ), 0.1f, 0.1f, 0.9f, 0.9f );
}