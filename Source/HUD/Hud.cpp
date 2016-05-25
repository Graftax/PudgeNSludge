/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CHud
//
// Purpose: To display the Sludge meter to the screen
//
// Original Author: Rueben Massey
//
// Creation Date: 5/14/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Hud.h"
#include "..//RenderEngine/RenderEngine.h"
#include "..//RenderEngine/TextureManager.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../ComponentManager/CComponent_Input.h"
#include "../ObjectLib/Objects/IBaseObject.h"
#include "../LevelManager/CLevelManager.h"

// health bottom .6757
// health top	.0625
// health length .6132f
#define HEALTH_TOP		0.063f
#define HEALTH_BOTTOM	0.6757f
#define HEALTH_DIFF		0.6137f
#define BOSS_HEALTH_DIFF 1.0f

CHud::CHud() 
{
	m_fSludgeLevel = 0;
	m_fBossLife = 0;
	m_currentCycloneTexture = 0;
	m_currentPulledObject = MAX_PULLED_OBJECTS;
	m_pTextureMngr = nullptr;

	m_nBackgroundHUD = 0;

	m_nPudgeHUD[0] =	 0;
	m_nPudgeHUD[1] =	 0;
	m_nPudgeHUD[2] =	 0;
	m_nPudgeHUD[3] =	 0;
	m_nPudgeHUD[4] =	 0;
	m_nPudgeHUD[5] =	 0;
	m_nPudgeHUD[6] =	 0;
	m_nPudgeHUD[7] =	 0;

	m_nObjectHUD[0][0] = 0;
	m_nObjectHUD[0][1] = 0;
	m_nObjectHUD[0][2] = 0;
	m_nObjectHUD[0][3] = 0;
	m_nObjectHUD[0][4] = 0;
	m_nObjectHUD[0][5] = 0;
	m_nObjectHUD[0][6] = 0;
	m_nObjectHUD[0][7] = 0;

	m_nObjectHUD[1][0] = 0;
	m_nObjectHUD[1][1] = 0;
	m_nObjectHUD[1][2] = 0;
	m_nObjectHUD[1][3] = 0;
	m_nObjectHUD[1][4] = 0;
	m_nObjectHUD[1][5] = 0;
	m_nObjectHUD[1][6] = 0;
	m_nObjectHUD[1][7] = 0;
	m_nObjectHUD[2][0] = 0;
	m_nObjectHUD[2][1] = 0;
	m_nObjectHUD[2][2] = 0;
	m_nObjectHUD[2][3] = 0;
	m_nObjectHUD[2][4] = 0;
	m_nObjectHUD[2][5] = 0;
	m_nObjectHUD[2][6] = 0;
	m_nObjectHUD[2][7] = 0;

	m_nObjectHUD[3][0] = 0;
	m_nObjectHUD[3][1] = 0;
	m_nObjectHUD[3][2] = 0;
	m_nObjectHUD[3][3] = 0;
	m_nObjectHUD[3][4] = 0;
	m_nObjectHUD[3][5] = 0;
	m_nObjectHUD[3][6] = 0;
	m_nObjectHUD[3][7] = 0;

	m_nPanelHUD[0] =	 0;
	m_nPanelHUD[1] =	 0;
	m_nPanelHUD[2] =	 0;
	m_nPanelHUD[3] =	 0;
	m_nPanelHUD[4] =	 0;

	m_pPlayer = nullptr;
	m_pPlayerInput = nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Init()
// Purpose: Initializes data members
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHud::Init ()
{
	m_pTextureMngr = TextureManager::GetInstance();

	// New HUD stuff
	m_nBackgroundHUD = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/Background.png" );

	m_nPudgeHUD[0] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/1.png" );
	m_nPudgeHUD[1] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/2.png" );
	m_nPudgeHUD[2] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/3.png" );
	m_nPudgeHUD[3] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/4.png" );
	m_nPudgeHUD[4] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/5.png" );
	m_nPudgeHUD[5] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/6.png" );
	m_nPudgeHUD[6] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/7.png" );
	m_nPudgeHUD[7] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/8.png" );

	m_nObjectHUD[0][0] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/cork1.png" );
	m_nObjectHUD[0][1] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/cork2.png" );
	m_nObjectHUD[0][2] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/cork3.png" );
	m_nObjectHUD[0][3] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/cork4.png" );
	m_nObjectHUD[0][4] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/cork5.png" );
	m_nObjectHUD[0][5] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/cork6.png" );
	m_nObjectHUD[0][6] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/cork7.png" );
	m_nObjectHUD[0][7] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/cork8.png" );

	m_nObjectHUD[1][0] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/crate1.png" );
	m_nObjectHUD[1][1] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/crate2.png" );
	m_nObjectHUD[1][2] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/crate3.png" );
	m_nObjectHUD[1][3] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/crate4.png" );
	m_nObjectHUD[1][4] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/crate5.png" );
	m_nObjectHUD[1][5] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/crate6.png" );
	m_nObjectHUD[1][6] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/crate7.png" );
	m_nObjectHUD[1][7] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/crate8.png" );

	m_nObjectHUD[2][0] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/goo1.png" );
	m_nObjectHUD[2][1] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/goo2.png" );
	m_nObjectHUD[2][2] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/goo3.png" );
	m_nObjectHUD[2][3] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/goo4.png" );
	m_nObjectHUD[2][4] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/goo5.png" );
	m_nObjectHUD[2][5] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/goo6.png" );
	m_nObjectHUD[2][6] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/goo7.png" );
	m_nObjectHUD[2][7] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/goo8.png" );
	/*
	m_nObjectHUD[3][0] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/plug1.png" );
	m_nObjectHUD[3][1] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/plug2.png" );
	m_nObjectHUD[3][2] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/plug3.png" );
	m_nObjectHUD[3][3] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/plug4.png" );
	m_nObjectHUD[3][4] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/plug5.png" );
	m_nObjectHUD[3][5] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/plug6.png" );
	m_nObjectHUD[3][6] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/plug7.png" );
	m_nObjectHUD[3][7] = m_pTextureMngr->LoadTexture( "./Resources/HUDImages/plug8.png" );*/

	m_nPanelHUD[0] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/screen1.png" );
	m_nPanelHUD[1] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/screen2.png" );
	m_nPanelHUD[2] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/screen3.png" );
	m_nPanelHUD[3] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/screen4.png" );
	m_nPanelHUD[4] =		m_pTextureMngr->LoadTexture( "./Resources/HUDImages/screen5.png" );

	m_cBitmapFont.Initialize( 42.0f, 39.0f, 10.0f, 512.0f, 64.0f, '0' );

	m_HUDBossHealthTextureHandle = m_pTextureMngr->LoadTexture( "./Resources/Textures/bosshudBar.png" );
	m_HUDBossOutlineTextureHandle = m_pTextureMngr->LoadTexture( "./Resources/Textures/bosshud.png" );

	m_SludgeParticles = CParticleManager::GetInstance()->CreateEffect( "HUDVFX", vec3f( -CGame::GetInstance()->GetScreenWidth() * 0.65f, CGame::GetInstance()->GetScreenHeight() * 0.59f, 0.0f ) );

	m_SludgeParticles->m_nEmitterType = 2;
	m_SludgeParticles->m_fHalfExtents = vec3f( CGame::GetInstance()->GetScreenWidth() * 0.085f, 1.0f, 0.0f );
	m_SludgeParticles->m_bScreenSpace = true;
	m_SludgeParticles->m_nParticleRate = 2000;
	m_SludgeParticles->m_fLifespan = 0.0f;
	m_SludgeParticles->m_fGravity = vec3f( 0.0f, 2.0f, 0.0f );

	m_fRotationTimer = 0.0f;
	m_nRotationIndex = 0;
	m_fPanelTimer = 0.0f;
	m_nPanelIndex = 0;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Shutdown()
// Purpose: Cleans up memory
// Original Author: Rueben Massey
// Creation Date: 8/2/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHud::Shutdown()
{
	// unload particles
	CParticleManager::GetInstance()->DestroyEffect( m_SludgeParticles );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update()
// Purpose: update the meter over time
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHud::Update( float fElapsedTime )
{	
	if( !m_pPlayer )	
	{
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();
	}
	if(  !m_pPlayerInput && m_pPlayer )
	{
		m_pPlayerInput = (CComponent_Input*)m_pPlayer->GetComponent( ECOMP_INPUT );
	}

	switch( m_pPlayerInput->GetPulledObjectType() )
	{
	case OBJ_CORK:
		{
			m_currentPulledObject = 0;
			break;
		}
	case OBJ_CRATE:
		{
			m_currentPulledObject = 1;
			break;
		}
	case OBJ_GOO:
		{
			m_currentPulledObject = 2;
			break;
		}
	case OBJ_PLUG:
		{
			m_currentPulledObject = 3;
			break;
		}
	default:
		{
			m_currentPulledObject = MAX_PULLED_OBJECTS;
			break;
		}
	}

	m_fRotationTimer += fElapsedTime;
	if( m_fRotationTimer > 0.15f )
	{
		m_fRotationTimer = 0.0f;
		++m_nRotationIndex;
		if( m_nRotationIndex > 7 )
		{
			m_nRotationIndex = 0;
		}

	}

	if( m_pPlayerInput->GetPulling() || m_currentPulledObject < MAX_PULLED_OBJECTS - 1 )
	{
		m_fPanelTimer += fElapsedTime;
		if( m_fPanelTimer > 0.05f )
		{
			m_fPanelTimer = 0.0f;
			++m_nPanelIndex;
			if( m_nPanelIndex > NUM_PANEL_TEXTURES - 1 )
			{
				m_nPanelIndex = NUM_PANEL_TEXTURES - 1;
			}
		}
	}
	else
	{
		m_fPanelTimer += fElapsedTime;
		if( m_fPanelTimer > 0.05f )
		{
			m_fPanelTimer = 0.0f;
			--m_nPanelIndex;
			if( m_nPanelIndex < -1 )
			{
				m_nPanelIndex = -1;
			}
		}
	}

	// adjust Lifespan based on health
	if( 768 == CGame::GetInstance()->GetScreenHeight() )
	{
		m_SludgeParticles->m_fLifespan = m_fSludgeLevel * 0.018f;
	}
	else if( 720 == CGame::GetInstance()->GetScreenHeight() )
	{
		m_SludgeParticles->m_fLifespan = m_fSludgeLevel * 0.0175f;
	}
	else
	{
		m_SludgeParticles->m_fLifespan = m_fSludgeLevel * 0.0185f;
	}


	m_SludgeParticles->m_fPosition = vec3f( -CGame::GetInstance()->GetScreenWidth() * 0.65f, CGame::GetInstance()->GetScreenHeight() * 0.59f, 0.0f );

	m_SludgeParticles->Emit( fElapsedTime );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Render()
// Purpose: Render the sludge meter to the screen
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHud::Render()
{
	if( !m_pPlayer || !m_pPlayerInput )
	{
		return;
	}
	// Render the Panel
	if( m_nPanelIndex > -1 )
	{
		RenderEngine::RasterTexture( m_pTextureMngr->GetTexture(m_nPanelHUD[m_nPanelIndex]), 0.15f, 0.775f, 0.35f, 0.975f );
	}
	if( m_nPanelIndex == NUM_PANEL_TEXTURES - 1 && m_currentPulledObject < MAX_PULLED_OBJECTS - 1 )
	{
		RenderEngine::RasterTexture( m_pTextureMngr->GetTexture(m_nObjectHUD[m_currentPulledObject][m_nRotationIndex]), 0.15f, 0.775f, 0.35f, 0.975f );
	}

	RenderEngine::RasterTexture(  m_pTextureMngr->GetTexture( m_nBackgroundHUD), 0.105f,0.705f, 0.25f, 0.95f );

	// set health to a ratio of total health
	// to HUD bar length
	float health = ( HEALTH_DIFF / 100.0f) * m_fSludgeLevel;

	// Health goo particles
	CParticleManager::GetInstance()->RenderEmitter( m_SludgeParticles );

	// HUD Background
	RenderEngine::RasterTexture( m_pTextureMngr->GetTexture(m_nPudgeHUD[m_nRotationIndex]), 0.1f,0.7f, 0.3f, 1.025f );

	char buffer[4];
	int nsludge = (int)m_fSludgeLevel / 10;
	if( nsludge > 9 )
	{
		nsludge = 0;
	}
	sprintf_s( buffer, "%d%d%d", (int)m_fSludgeLevel / 100, nsludge, (int)m_fSludgeLevel % 10 );
	m_cBitmapFont.Print(buffer, 0.1975f, 0.69f );

	// Render Boss Health if we are in the boss level
	if (LevelManager::GetInstance()->GetCurrentLevelIndex() >= LevelManager::GetInstance()->GetNumLevels()-1)
	{
		float BossHealth = ( BOSS_HEALTH_DIFF / 100.0f) * m_fBossLife;
		RenderEngine::RasterSourceRectTexture( m_pTextureMngr->GetTexture(m_HUDBossHealthTextureHandle),0.267f, 0.09f, 0.727f , 0.13f, 0.0f, BossHealth, 0.0f, 1.0f );	
		RenderEngine::RasterTexture( m_pTextureMngr->GetTexture(m_HUDBossOutlineTextureHandle), 0.24f, 0.015f, 0.77f, 0.23f );
	}
}