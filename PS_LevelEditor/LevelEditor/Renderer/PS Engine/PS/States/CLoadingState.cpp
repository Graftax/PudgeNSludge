#include "CLoadingState.h"
#include "GamePlayState.h"
#include "../../RenderEngine/RenderEngine.h"
#include "../../RenderEngine/TextureManager.h"

CLoadingState* CLoadingState::m_pInstance = nullptr;

unsigned __stdcall  LoadingStateThreadProc(LPVOID lpParameter)
{
	bool running = ((CLoadingState*)lpParameter)->GetRunning();
	CRITICAL_SECTION critSec = ((CLoadingState*)lpParameter)->GetCriticalSection();
	while(!((CLoadingState*)lpParameter)->GetSpinLock() ) {}
	while(running)
	{
		ECS( critSec );
		//((CLoadingState*)lpParameter)->ThreadUpdate();		
		((CLoadingState*)lpParameter)->ThreadRender();
		running = ((CLoadingState*)lpParameter)->GetRunning();
		LCS( critSec );
		//Give time to load and the other thread to enter the critical section
		Sleep(15);
	}

	((CLoadingState*)lpParameter)->SetStatus( true );
	LCS( critSec );
	return true;
}

CLoadingState::CLoadingState(void) 
{
	m_pLoadingTexture = nullptr;
	m_pInput = nullptr;
	running = false;
	finished = true;
	spinLock = false;	
	loadingStatus = false;
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
	m_pLoadingTexture = nullptr;
	m_pInput = CGame::GetInstance()->GetInputManager();

	CGame::GetInstance()->SetLoading( true );

	InitializeCriticalSection( &criticalSection );

	D3DXCreateTextureFromFileA( CGame::GetInstance()->renderEngine.GetDevice(), "./Resources/Textures/PS_FE_Loading.png", &m_pLoadingTexture );
	D3DXCreateTextureFromFileA( CGame::GetInstance()->renderEngine.GetDevice(), "./Resources/Textures/T_HealthOutline_D.png", &m_OutlineTextureHandle );
	D3DXCreateTextureFromFileA( CGame::GetInstance()->renderEngine.GetDevice(), "./Resources/Textures/T_Health_D.png", &m_BlockTextureHandle );
	D3DXCreateTextureFromFileA( CGame::GetInstance()->renderEngine.GetDevice(), "./Resources/Textures/PS_FE_LoadingText.png", &m_pPressToContinue );

	m_bWaitingToPress = false;
	m_fCurrentProgress = 0.0f;
	running = true;
	finished = true;
	spinLock = false;	
	loadingStatus = true;

	m_cTimer.Start();
	//threadHandle = (HANDLE)_beginthreadex(0, 0, &LoadingStateThreadProc, (void*)this, 0, &threadID);
	spinLock = true;
}

void CLoadingState::Update(float fDT) 
{
	if( finished )
	{
		CGame::GetInstance()->SetLoading(false);
		CGame::GetInstance()->ChangeState( CGamePlayState::GetInstance() );
		loadingStatus = false;
	}
}

void CLoadingState::ThreadUpdate(float _progress) 
{
	m_fCurrentProgress = _progress;
	m_fCurrentProgress /= MAX_LOADING_TIME;
	if( _progress < 0.0f )// MAX_LOADING_TIME )
	{
		m_bWaitingToPress = true;
	}

	ThreadRender();
}

void CLoadingState::Render(void)
{

}

void CLoadingState::ThreadRender(void) 
{
	CGame::GetInstance()->renderEngine.Clear();
	CGame::GetInstance()->renderEngine.BeginScene();
	CGame::GetInstance()->renderEngine.RasterTexture( m_pLoadingTexture, 0.0f, 0.0f, 1.0f, 1.0f );
	CGame::GetInstance()->renderEngine.RasterTexture( m_BlockTextureHandle, 0.0f,0.01f, 1.0f - m_fCurrentProgress, 0.06f );
	CGame::GetInstance()->renderEngine.RasterTexture( m_OutlineTextureHandle, -0.1f,0.01f, 1.1f, 0.06f );
	if( m_bWaitingToPress )
		CGame::GetInstance()->renderEngine.RasterTexture( m_pPressToContinue, 0.0f, 0.0f, 1.0f, 1.0f );
	CGame::GetInstance()->renderEngine.EndScene();
	CGame::GetInstance()->renderEngine.Present();
}

bool CLoadingState::Input(void) 
{
	
	m_pInput->Update();
	if( m_bWaitingToPress )
		if( m_pInput->KeyPressed(DIK_RETURN) || m_pInput->LeftMouseButtonDown() || m_pInput->RightMouseButtonDown() )
			finished = false;
	m_pInput->ClearInput();
	return true;
}

void CLoadingState::Shutdown(void) 
{
	//CloseHandle( threadHandle );
	DeleteCriticalSection( &criticalSection );
	SAFE_RELEASE( m_pLoadingTexture );
	SAFE_RELEASE( m_OutlineTextureHandle );
	SAFE_RELEASE( m_BlockTextureHandle );
	SAFE_RELEASE( m_pPressToContinue );
	CGame::GetInstance()->SetLoading(false);
	m_pInput->OffSetMouseForStart();
	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
