#ifndef CCOMPONENT_JUMPMESHEFFECT_H_
#define CCOMPONENT_JUMPMESHEFFECT_H_

#include "IComponent.h"
#include "../PS/CGame.h"

class IBaseObject;
class CObjectManager;
class CComponent_Animation;
class CComponent_Emitter;
class CComponent_Input;
class CParticleManager;
class CEffects;
class CEmitter;
class CCamera;


__declspec(align(32))
class CComponent_JumpMeshEffect : public IComponent
{
private:
	CComponent_Animation* m_pAnimation;
	CComponent_Emitter* m_pEmitterComponent;
	CInputManager* m_pInput;
	CGame* m_pGame;
	CCamera* m_pCamera;
	IBaseObject* m_pPlayer;

	CParticleManager* m_pParticleMan;
	CEffects* m_pEffects;
	CEmitter* m_pEmitter;

	bool m_bEffectHasPlayed;
	bool m_bDoubleJump;
	int m_nCounter;
	bool m_bSingleJump;
public:
	CComponent_JumpMeshEffect(void);
	~CComponent_JumpMeshEffect(void);

	bool Initialize();
	void Update(float dt);
	void Render();
	void Shutdown();
};


#endif