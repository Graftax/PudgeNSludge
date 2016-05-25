#ifndef CCREDITSSTATE_H_
#define CCREDITSSTATE_H_

#include "IGameState.h"
#include "../CGame.h"

#define NUM_CREDIT_TEXTURES 3
#define CREDITS_SCROLL_SPEED 0.15f

class CCreditsState : public IGameState
{
private:
	static CCreditsState* m_pInstance;

	LPDIRECT3DTEXTURE9 m_pCreditTexture;
	float m_fCurrentTop;
	CInputManager* m_pInput;
	CAudioManager* m_pAudio;

public:
	CCreditsState(void);
	~CCreditsState(void);

	static CCreditsState* GetInstance(void);

	void Init(void);
	bool Input(void);
	void Update( float fDT );
	void Render(void);
	void Shutdown(void);

};















#endif //CCREDITSSTATE_H_