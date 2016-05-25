#ifndef CSPLASHSCREENSTATE_H_
#define CSPLASHSCREENSTATE_H_


//#include "../../RenderEngine/MultiThreadRender.h"
#include "IGameState.h"
#include "MainMenuState.h"

#define SPLASH_SCREENS 3

#define FADETIME 1.5f
#define WAITTIME 1.0f


class CInputManager;
class CAudioManager;

__declspec(align(32))
class CSplashScreenState : public IGameState
{
private:
	static CSplashScreenState* m_pInstance;
	CTimer m_cTimer;
	CTimer m_cFadeTimer;
	LPDIRECT3DTEXTURE9 m_lpSplashTextures[SPLASH_SCREENS];
	unsigned int m_unCurrentScreen;
	float _fadeFactor;
	bool fadeUp;
	bool wait;
	CInputManager* m_pInput;

	CSplashScreenState(void){}
	~CSplashScreenState(void){}
	CSplashScreenState( const CSplashScreenState& that );
	CSplashScreenState& operator=( const CSplashScreenState& that );

	CRITICAL_SECTION criticalSection;
	bool running;
	volatile bool finished;
	volatile bool spinLock;
	unsigned int threadID;
	HANDLE threadHandle;

public:
	static CSplashScreenState* GetInstance(void);
	static void DeleteInstance(void);
	void Init(void);
	void Shutdown(void);
	void Update( float fTime );
	void ThreadUpdate( void );
	bool Input(void);
	void Render(void){}
	void ThreadRender(void);
	
	bool GetRunning(void) { return running; }
	bool GetSpinLock(void) { return spinLock; }
	bool GetStatus(void) { return finished; }
	void SetStatus(bool _status) { finished = _status; }
	CRITICAL_SECTION GetCriticalSection(void) { return criticalSection; }

};















#endif