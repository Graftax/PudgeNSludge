#include "CEndGameState.h"
#include "MainMenuState.h"
#include "../../ObjectLib/ObjectManager/CObjectManager.h"
#include "../../RenderEngine/RenderEngine.h"
#include "../../RenderEngine/TextureManager.h"



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

	m_nEndTextureHandle = TextureManager::GetInstance()->GetTexture( TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_Death_Exit.png" ) );
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

	if(m_pInput->KeyPressed(DIK_RETURN) || 
		m_pInput->KeyPressed(DIK_SPACE))
	{
		CGame::GetInstance()->ChangeState ( CMainMenuState::GetInstance() ); 
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
	RenderEngine::RasterTexture( m_nEndTextureHandle, 0.0f, 0.0f, 1.0f, 1.0f );
}