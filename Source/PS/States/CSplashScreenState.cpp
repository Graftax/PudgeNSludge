#include "CSplashScreenState.h"
#include "../../RenderEngine/TextureManager.h"
#include "../../InputLib/InputManager.h"

CSplashScreenState* CSplashScreenState::m_pInstance = nullptr;

unsigned __stdcall  SplashStateThreadProc(LPVOID lpParameter)
{
	bool running = ((CSplashScreenState*)lpParameter)->GetRunning();
	CRITICAL_SECTION critSec = ((CSplashScreenState*)lpParameter)->GetCriticalSection();
	while(!((CSplashScreenState*)lpParameter)->GetSpinLock() ) {}
	while(running)
	{
		((CSplashScreenState*)lpParameter)->ThreadUpdate();		
		ECS( critSec );
		((CSplashScreenState*)lpParameter)->ThreadRender();
		running = ((CSplashScreenState*)lpParameter)->GetRunning();
		LCS( critSec );
		//Give time to load and the other thread to enter the critical section
		Sleep(15);
	}

	((CSplashScreenState*)lpParameter)->SetStatus( true );
	LCS( critSec );
	return true;
}

CSplashScreenState* CSplashScreenState::GetInstance(void)
{
	//we are a singleton, so have we been made yet?
	if(!m_pInstance)
	{
		//if not create a new instance of us
		m_pInstance = new CSplashScreenState();

		m_pInstance->Init();
	}
	//return the current instance of the class
	return m_pInstance;
}

void CSplashScreenState::DeleteInstance(void)
{
	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void CSplashScreenState::Init(void)
{
	m_pInput = CGame::GetInstance()->GetInputManager();
	m_pAudio = CGame::GetInstance()->GetAudioManager();
	InitializeCriticalSection( &criticalSection );
	D3DXCreateTextureFromFileA( CGame::GetInstance()->renderEngine.GetDevice(), "./Resources/Textures/PS_FE_GPGAMES.png", &m_lpSplashTextures[0] );
	D3DXCreateTextureFromFileA( CGame::GetInstance()->renderEngine.GetDevice(), "./Resources/Textures/PS_FE_IHPLOGO.png", &m_lpSplashTextures[2] );
	D3DXCreateTextureFromFileA( CGame::GetInstance()->renderEngine.GetDevice(), "./Resources/Textures/StudioLogo.png", &m_lpSplashTextures[1] );
	D3DXCreateTextureFromFileA( CGame::GetInstance()->renderEngine.GetDevice(), "./Resources/Textures/PS_FE_DirectXLogo.png", &m_lpSplashTextures[3] );
	D3DXCreateTextureFromFileA( CGame::GetInstance()->renderEngine.GetDevice(), "./Resources/Textures/PS_FE_Wwise.png", &m_lpSplashTextures[4] );
	m_unCurrentScreen = 0u;	
	running = true;
	_fadeFactor = 0.0f;
	finished = false;
	spinLock = false;	
	fadeUp = true;
	
	m_cTimer.Start();
	m_cFadeTimer.Start();
	threadHandle = (HANDLE)_beginthreadex(0, 0, &SplashStateThreadProc, (void*)this, 0, &threadID);
	spinLock = true;
}

void CSplashScreenState::Shutdown(void)
{
	CloseHandle( threadHandle );
	DeleteCriticalSection( &criticalSection );

	for( unsigned int i = 0u; i < SPLASH_SCREENS; ++i )
		m_lpSplashTextures[i]->Release();
	//DeleteInstance();
}

void CSplashScreenState::Update( float fTime )
{
	if( finished )
	{	
		//
		CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );
		CGame::GetInstance()->SetLoading( false );
		//RenderEngine::ResetDevice();
		//CloseHandle( threadHandle );
	}
}

void CSplashScreenState::ThreadUpdate( void )
{
	if( fadeUp )
	{
		_fadeFactor = (float)(m_cFadeTimer.GetElapsedTime() / FADETIME);
		if( _fadeFactor >= 1.0f )
		{
			_fadeFactor = 1.0f;
			fadeUp = false;
			wait = true;
			m_cFadeTimer.Reset();
		}
	}
	else if( wait )
	{
		_fadeFactor = 1.0f;
		if( m_cFadeTimer.GetElapsedTime() > WAITTIME )
		{
			wait = false;
			m_cFadeTimer.Reset();
		}
	}
	else
	{
		_fadeFactor = 1.0f - ((float)m_cFadeTimer.GetElapsedTime() / FADETIME);
	}
	if( (float)m_cTimer.GetElapsedTime() > ((FADETIME * 2.0f) + WAITTIME) )
	{
		m_unCurrentScreen++;
		if(m_unCurrentScreen == 2 && m_pAudio)
		{
			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_SPLASHSCREEN_INFINITEHP);
		}
		if(m_unCurrentScreen == 4 && m_pAudio)
		{
			m_pAudio->PlayEffect(AK::EVENTS::PLAY_SFX_WWISESPLASH);
		}

		if( m_unCurrentScreen >= SPLASH_SCREENS )
		{
			ECS(criticalSection);
			running = false;
			LCS(criticalSection);
			//CGame::GetInstance()->PushState( CMainMenuState::GetInstance() );
			m_unCurrentScreen = SPLASH_SCREENS - 1;
		}

		_fadeFactor = 0.0f;
		fadeUp = true;
		m_cTimer.Reset();m_cFadeTimer.Reset();
	}	
}

bool CSplashScreenState::Input(void)
{
	if( !m_bFirstPlay )
	{
		if( m_pInput->CheckKeys() )
		{
			ECS(criticalSection);
			running = false;
			LCS(criticalSection);
		}
	}
	return true;
}

void CSplashScreenState::ThreadRender(void)
{
	//ECS(criticalSection);
	CGame::GetInstance()->renderEngine.Clear();
	CGame::GetInstance()->renderEngine.BeginScene();
	CGame::GetInstance()->renderEngine.RasterTexture( m_lpSplashTextures[m_unCurrentScreen], 0.0f, 0.0f, 1.0f, 1.0f, _fadeFactor );
	CGame::GetInstance()->renderEngine.EndScene();
	CGame::GetInstance()->renderEngine.Present();
	//LCS(criticalSection);
}
