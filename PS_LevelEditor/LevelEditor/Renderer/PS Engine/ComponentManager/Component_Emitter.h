#ifndef CCOMPONENT_EMITTER_H_
#define CCOMPONENT_EMITTER_H_

#include "IComponent.h"
#include "../PS/CGame.h"
#include "../EffectsLib/Effects.h"
#include "../EffectsLib/Emitter.h"
#include "../EffectsLib/ParticleManager.h"

enum ParticleEffects { PULL_FX = 0, PUSH_FX, STEAM_FX, FOOT_STEPS_FX, JUMP_FX, SMALLDMG_FX, LARGEDMG_FX, GOO_EXPLODE_FX, GOO_TRAIL_FX, CRUSHERSPARKS_FX, LAND_FX, 
	                   BOXHELD_FX, COCKHELD_FX, GOOHELD_FX, 
	                   PLUGHELD_FX, CRUSHER_FX, IMPACT_FX,  SLIMEMONSTER_IMPACT_FX, BREAKABLE_FX,  
					   MAX_FX };

class CObjectManager;
// class CParticleManager;
// class CEmitter;
// class CEffects;
class CGamePlayState;
class CGame;
class CCamera;

__declspec(align(32))
class CComponent_Emitter : public IComponent
{
	CEmitter* m_pEmitter;
	CParticleManager* m_pParticleManager;
	CGamePlayState* m_pCgamePlay;
	CGame* m_pCgame;
	CCamera* m_pCamera;
	CEffects* m_pEffect;
	int m_nType;
	IBaseObject* m_pPlayer;
	vec3f m_Direction;

	vec3f m_fPullStart;

	float EffectEndTimer;
	bool  ActivateEffectEndTimer;

	bool m_bConnectedPull;
	bool m_bConnectedPush;

public:
	vec3f GetDirection() const { return m_Direction; }
void SetDirection(vec3f val) { m_Direction = val; }

	CComponent_Emitter(void);
	~CComponent_Emitter(void);
	
	bool Initialize();
	void Update(float dt);
	void Render();
	void Shutdown();
	
	CParticleManager* GetParticleManager()  { return m_pParticleManager; }
	void SetParticleManager(CParticleManager* val) { m_pParticleManager = val; }

// 	vector<CEffects*> GetEffects() const { return m_vEffects; }
// 	void SetEffects(vector<CEffects*> val) { m_vEffects = val; }

	CEffects* GetEffects()  { return m_pEffect; }
	void SetEffects(CEffects* val) { m_pEffect = val; }

	int GetType()  { return m_nType; }
	void SetType(int val) { m_nType = val; }
	void SetPullStart( vec3f _pos ){ m_fPullStart = _pos; }

	void ActivateEffectTimer() { ActivateEffectEndTimer = true; }
};

#endif