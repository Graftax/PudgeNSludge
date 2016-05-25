#ifndef CCOMPONENT_PUSHMESHEFFECT_H_
#define CCOMPONENT_PUSHMESHEFFECT_H_

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
class CComponent_PushMeshEffect : public IComponent
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
	float fCounter;
	bool HasUpdatedPosition;
public:
	CComponent_PushMeshEffect(void);
	~CComponent_PushMeshEffect(void);

	bool Initialize();
	void Update(float dt);
	void Render();
	void Shutdown();

	void SetJointMatrix( D3DXMATRIX mat );
};

#endif