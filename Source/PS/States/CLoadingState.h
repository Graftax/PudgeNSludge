#ifndef CLOADINGSTATE_H_
#define CLOADINGSTATE_H_

#include "IGameState.h"
#include "../CGame.h"

#define MAX_LOADING_TIME 5.0f

class CLoadingState : public IGameState
{
private:
	static CLoadingState* m_pInstance;

	int m_pLoadingTexture;
	int m_BlockTextureHandle;

	float m_fCurrentProgress;
	bool finished;

	CInputManager* m_pInput;
	CTimer m_cTimer;

public:
	CLoadingState(void);
	~CLoadingState(void);

	static CLoadingState* GetInstance(void);

	void Init(void);
	void Update(float _progress);
	void Render(void);
	bool Input(void);
	void Shutdown(void);

	bool GetStatus(void) { return finished; }
	CTimer GetTimer(void) { return m_cTimer; }

};












#endif //CLOADINGSTATE_H_