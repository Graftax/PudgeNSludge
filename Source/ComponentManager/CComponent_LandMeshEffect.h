#ifndef CCOMPONENT_LANDMESHEFFECT_H_
#define CCOMPONENT_LANDMESHEFFECT_H_

#include "IComponent.h"
#include "../PS/CGame.h"

class IBaseObject;
class CObjectManager;
class CComponent_Animation;
class CComponent_Emitter;
class CComponent_Input;
class CComponent_PlayerCollision;
class CComponent_Health;
class CParticleManager;
class CEffects;
class CEmitter;
class CCamera;

__declspec(align(32))
class CComponent_LandMeshEffect : public IComponent
{
private:
	CComponent_Animation* m_pAnimation;
	CComponent_Emitter* m_pEmitterComponent;
	CComponent_PlayerCollision* m_pPlayerCol;
	CComponent_Health* m_pPlayerHealth;
	CInputManager* m_pInput;
	CGame* m_pGame;
	CCamera* m_pCamera;
	IBaseObject* m_pPlayer;

	CParticleManager* m_pParticleMan;
	CEmitter* m_pEmitter;

	bool m_bEffectHasPlayed;
	bool m_bCollidingGround;
	bool isCollidingLast;
public:
	CComponent_LandMeshEffect(void);
	~CComponent_LandMeshEffect(void);

	bool Initialize();
	void Update(float dt);
	void Render();
	void Shutdown();
};


#endif