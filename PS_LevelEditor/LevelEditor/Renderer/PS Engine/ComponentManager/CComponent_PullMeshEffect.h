#ifndef CCOMPONENT_PULLMESHEFFECT_H_
#define CCOMPONENT_PULLMESHEFFECT_H_

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
class CComponent_PullMeshEffect : public IComponent
{
private:
	CComponent_Animation* m_pAnimation;
	CComponent_Emitter* m_pEmitterComponent;
	CInputManager* m_pInput;
	CGame* m_pGame;
	CCamera* m_pCamera;
	IBaseObject* m_pPlayer;
	IBaseObject* m_pPlayerArm;
	matrix4f m_mGunJointMatrix;

	CParticleManager* m_pParticleMan;
	CEffects* m_pEffects;
	CEmitter* m_pEmitter;

	bool m_bEffectHasPlayed;

public:
	CComponent_PullMeshEffect(void);
	~CComponent_PullMeshEffect(void);

	bool Initialize();
	void Update(float dt);
	void Render();
	void Shutdown();
};


#endif