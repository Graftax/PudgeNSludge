#include "CCreditsState.h"
#include "../../RenderEngine/TextureManager.h"

CCreditsState* CCreditsState::m_pInstance = nullptr;

CCreditsState::CCreditsState(void)
{
	m_pCreditTexture = nullptr;
	m_fCurrentTop = 0.0f;
	m_pAudio = nullptr;
}

CCreditsState::~CCreditsState(void)
{

}

CCreditsState* CCreditsState::GetInstance(void)
{
	if( m_pInstance )
		return m_pInstance;
	else
		m_pInstance = new CCreditsState;

	return m_pInstance;
}
void CCreditsState::Init(void)
{
	m_pCreditTexture = TextureManager::GetInstance()->GetTexture( TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_Credits.png" ) );
	m_fCurrentTop = 0.0f;
	m_pInput = CGame::GetInstance()->GetInputManager();
	m_pAudio = CGame::GetInstance()->GetAudioManager();
	if(m_pAudio)
	{
		m_pAudio->PlayMusic(BGM_CREDITS);
	}
}

bool CCreditsState::Input(void)
{
	if( m_pInput->KeyPressed(DIK_RETURN) || m_pInput->KeyPressed(DIK_ESCAPE) || m_pInput->KeyPressed(DIK_SPACE))
	{
		CGame::GetInstance()->PopState();
	}
	return true;
}

void CCreditsState::Update( float fDT )
{
	m_fCurrentTop += fDT * CREDITS_SCROLL_SPEED;

	if( m_fCurrentTop > 4.1f )
		CGame::GetInstance()->PopState();
}

void CCreditsState::Render(void)
{
	RenderEngine::RasterTexture( m_pCreditTexture, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f );
	RenderEngine::RasterTexture( m_pCreditTexture, 0.0f, m_fCurrentTop - 3.0f, 1.0f, m_fCurrentTop );
}

void CCreditsState::Shutdown(void)
{
	if(m_pAudio)
	{
		m_pAudio->StopMusic(BGM_CREDITS);
	}

	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
	
	TextureManager::GetInstance()->UnloadTexture( m_pCreditTexture );
}
