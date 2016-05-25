#ifndef CCOMPONENT_LITTLEHURTMESHEFFECT_H_
#define CCOMPONENT_LITTLEHURTMESHEFFECT_H_

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
class CComponent_LittleHurtMeshEffect : public IComponent
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

	IComponent* _pTargetHealthCMP;

	float duration;
public:
	CComponent_LittleHurtMeshEffect(void);
	~CComponent_LittleHurtMeshEffect(void);

	bool Initialize();
	void Update(float dt);
	void Render();
	void Shutdown();
};


#endif