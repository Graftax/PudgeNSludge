#include "CLoadingState.h"
#include "GamePlayState.h"
#include "../../RenderEngine/RenderEngine.h"
#include "../../RenderEngine/TextureManager.h"

CLoadingState* CLoadingState::m_pInstance = nullptr;

CLoadingState::CLoadingState(void) 
{
	m_pLoadingTexture = 0;
	m_BlockTextureHandle = 0;
	m_pInput = nullptr;
	finished = false;
}

CLoadingState::~CLoadingState(void) 
{

}

CLoadingState* CLoadingState::GetInstance(void) 
{
	if( m_pInstance )
		return m_pInstance;
	else
	{
		m_pInstance = new CLoadingState();
		m_pInstance->Init();
		return m_pInstance;
	}
	return nullptr;
}

void CLoadingState::Init(void) 
{
	m_pLoadingTexture = 0;
	m_BlockTextureHandle = 0;
	m_pInput = CGame::GetInstance()->GetInputManager();

	CGame::GetInstance()->SetLoading( true );

	m_pLoadingTexture = TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_FE_Loading.png" );
	m_BlockTextureHandle = TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/T_Health_D.png" );

	m_fCurrentProgress = 0.0f;
	finished = false;

	m_cTimer.Start();
}

void CLoadingState::Update(float _progress) 
{
	m_fCurrentProgress = _progress;
	m_fCurrentProgress /= MAX_LOADING_TIME;
	if( _progress < 0.0f )// MAX_LOADING_TIME )
	{
		finished = true;
	}
	Render();
}

void CLoadingState::Render(void)
{
	CGame::GetInstance()->renderEngine.Clear();
	CGame::GetInstance()->renderEngine.BeginScene();
	CGame::GetInstance()->renderEngine.RasterTexture( TextureManager::GetInstance()->GetTexture(m_BlockTextureHandle), 0.0f, 0.0f, 1.0f - m_fCurrentProgress, 1.0f );
	CGame::GetInstance()->renderEngine.RasterTexture( TextureManager::GetInstance()->GetTexture(m_pLoadingTexture), 0.0f, 0.0f, 1.0f, 1.0f );	
	CGame::GetInstance()->renderEngine.EndScene();
	CGame::GetInstance()->renderEngine.Present();
}

bool CLoadingState::Input(void) 
{
	return true;
}

void CLoadingState::Shutdown(void) 
{
	TextureManager* pTM = TextureManager::GetInstance();
	pTM->UnloadTexture(m_pLoadingTexture);
	pTM->UnloadTexture(m_BlockTextureHandle);

	CGame::GetInstance()->SetLoading(false);
	m_pInput->OffSetMouseForStart();
	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
