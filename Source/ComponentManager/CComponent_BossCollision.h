#ifndef _BOSSCOLLISION_H
#define _BOSSCOLLISION_H

#include "CComponent_Collision.h"
#include "../AI/IBaseAI.h"
#include "../AI/CBossIdleAI.h"
#include "../AI/CBossAttackAI.h"
#include "../EffectsLib/ParticleManager.h"

class CComponent_Sound;
class CComponent_Animation;
class CComponent_Health;

class CComponent_BossCollision : public CComponent_Collision
{
private:
	CComponent_Sound* m_pSoundComponent;
	CComponent_Animation* m_pAnimationComponent;
	CComponent_Health* m_pHealthComponent;
	AABB m_PipeAABB;
	AABB m_SludgeAABB;
	AABB* m_UFOAABB;
	float m_fElapsedTime;

	//BossIdleAI m_IdleAI;
	//BossAttackAI m_AttackAI;

	//IBaseAI* m_pAI;
	int OrignalShipTexture;
	int OrignalSludgeTexture;
	RenderNode* m_pSludgeNode;
	RenderNode* m_pShipNode;

	int RedTexture;
	float m_fFlashTimer;
	bool m_bFlashShip;
	bool m_bFlashSludge;

	CPM_ParEmitter* m_pSmokeEffect;
	CPM_ParEmitter* m_pExplosionFireEffect;
	CPM_ParEmitter* m_pExplosionSparksEffect;
	CPM_ParEmitter* m_pExplosionSmokeEffect;

	float m_fSparksTimer;
	float m_fSparksTimerCap;

	void CheckDynamicCollision ( IBaseObject* _pCollide);
	void CheckStaticCollision ( IBaseObject* _pCollide);
	void PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction );

public:

	CComponent_BossCollision();
	~CComponent_BossCollision(void);

	bool Initialize();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	AABB GetUFOAABB () { return *m_UFOAABB; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():	Updates the collison component, incrementing its physics and detecting collision with other objects.
	//
	// Returns:		Void
	//
	// Mod. Name:   Josh Fields
	// Mod. Date:   5/16/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Shutdown():	deletes all memory allocated and uninits all member variables
	//
	// Returns:		Void
	//
	// Mod. Name:   Josh Fields
	// Mod. Date:   5/16/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown(void);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Function Name: CheckDynamicCollisions()
	//// Purpose: Check collisions with all dynamic objects
	//// Original Author: Rueben Massey
	//// Creation Date: 6/11/2012
	//// Last Modification By: 
	//// Last Modification Date: 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//void CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Function Name: CheckStaticCollisions()
	//// Purpose: Check collisions with all static objects
	//// Original Author: Rueben Massey
	//// Creation Date: 6/11/2012
	//// Last Modification By: 
	//// Last Modification Date: 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//void CheckStaticCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT );

	//void SetAI (IBaseAI* _pAI) { m_pAI = _pAI; }
	//IBaseAI* GetIdleAI () { return &m_IdleAI; }
	//IBaseAI* GetAttackAI () { return &m_AttackAI; }
};
#endif