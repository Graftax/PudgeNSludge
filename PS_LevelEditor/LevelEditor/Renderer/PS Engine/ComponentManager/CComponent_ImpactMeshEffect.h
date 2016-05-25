#ifndef CCOMPONENT_IMPACTMESHEFFECT_H_
#define CCOMPONENT_IMPACTMESHEFFECT_H_

#include "IComponent.h"
#include "../PS/CGame.h"

class IBaseObject;
class CObjectManager;
class CComponent_Animation;
class CComponent_Emitter;
class CComponent_PlayerCollision;
class CComponent_CrateCollision;
class CComponent_Input;
class CInputManager;
class CParticleManager;
class CEffects;
class CEmitter;
class CCamera;

__declspec(align(32))
class CComponent_ImpactMeshEffect : public IComponent
{
private:
	CComponent_Animation* m_pAnimation;
	CComponent_Emitter* m_pEmitterComponent;
	CComponent_CrateCollision* m_pCrateComponent;
	CInputManager* m_pInput;
	CGame* m_pGame;
	CCamera* m_pCamera;
	//IBaseObject* m_pPlayer;

	CParticleManager* m_pParticleMan;
	CEffects* m_pEffects;
	CEmitter* m_pEmitter;

	matrix4f _worldMat;
	vec3f m_fPos;

public:
	CComponent_ImpactMeshEffect(void);
	~CComponent_ImpactMeshEffect(void);

	bool Initialize();
	void Update(float dt);
	void Render();
	void Shutdown();

	void SetMatrix(matrix4f x){_worldMat = x;}
	matrix4f GetMatrix(){return _worldMat;}

	vec3f& GetPos() { return m_fPos; }
	void SetPos(vec3f x) { m_fPos = x; }
};


#endif