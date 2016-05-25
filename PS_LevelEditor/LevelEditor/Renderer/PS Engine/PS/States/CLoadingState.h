#ifndef CLOADINGSTATE_H_
#define CLOADINGSTATE_H_

#include "IGameState.h"
#include "../CGame.h"

#define MAX_LOADING_TIME 3.0f

class CLoadingState : public IGameState
{
private:
	static CLoadingState* m_pInstance;

	LPDIRECT3DTEXTURE9 m_pLoadingTexture;
	LPDIRECT3DTEXTURE9 m_OutlineTextureHandle;
	LPDIRECT3DTEXTURE9 m_BlockTextureHandle;
	LPDIRECT3DTEXTURE9 m_pPressToContinue;

	float m_fCurrentProgress;
	bool m_bWaitingToPress;

	CInputManager* m_pInput;
	CTimer m_cTimer;

	CRITICAL_SECTION criticalSection;
	bool running;
	bool loadingStatus;
	volatile bool finished;
	volatile bool spinLock;
	unsigned int threadID;
	HANDLE threadHandle;

public:
	CLoadingState(void);
	~CLoadingState(void);

	static CLoadingState* GetInstance(void);

	void Init(void);
	void Update(float fDT);
	void ThreadUpdate(float _progress);
	void Render(void);
	void ThreadRender(void);
	bool Input(void);
	void Shutdown(void);

	bool GetRunning(void) { return running; }
	bool GetSpinLock(void) { return spinLock; }
	bool GetStatus(void) { return finished; }
	void SetStatus(bool _status) { m_bWaitingToPress = _status; }
	void SetLoadingStatus(bool _loadStatus) { ECS(criticalSection); loadingStatus = _loadStatus; LCS(criticalSection); }
	CRITICAL_SECTION GetCriticalSection(void) { return criticalSection; }

};












#endif //CLOADINGSTATE_H_