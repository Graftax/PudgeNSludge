#ifndef CCOMPONENT_GOOPITCOLLISION_H
#define	CCOMPONENT_GOOPITCOLLISION_H

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Josh Fields
// Date:                5/16/12
// Purpose:             Defines the collisions and physics for the player.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_Collision.h"
#include "../EffectsLib/ParticleManager.h"

class CComponent_Sound;
class CComponent_Health;
class CComponent_Input;

__declspec(align(32))
class CComponent_GooPitCollision : public CComponent_Collision
{

private:
	CComponent_Sound* m_pSoundComponent;
	float m_fElapsedTime;
	bool m_bSoundSplash;
	bool m_bHurtPlayer;
	float m_fSoundTimer;
	float m_fDamageDealt;
	float m_fDamageAmount;

	CComponent_Health* m_pPlayerHealth;	
	CComponent_Input* m_pPlayerInput;	

	void StaticCollision ( IBaseObject* Collider, IBaseObject* Collide );
	void DynamicCollision ( IBaseObject* Collider, IBaseObject* Collide );
	void PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction );

	CPM_ParEmitter* m_pGooPitBubbleEffect;

public:

	CComponent_GooPitCollision();
	~CComponent_GooPitCollision(void);

	bool Initialize();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	
	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetHurtPlayer (bool _bHurt) { m_bHurtPlayer = _bHurt; }
	void SetDamageDealt ( float _fDamage ) { m_fDamageDealt = _fDamage; }
	void SetDamageAmount ( float _fDamage ) { m_fDamageAmount = _fDamage; }


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():	Updates the collison component, incrementing its physics and detecting collision with other objects.
	//
	// Returns:		Void
	//
	// Mod. Name:   Josh Fields
	// Mod. Date:   5/16/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );

	void Shutdown(void);

};

#endif