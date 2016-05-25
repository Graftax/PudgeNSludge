#ifndef CCOMPONENT_WATER_POOL_COLLISION_H
#define CCOMPONENT_WATER_POOL_COLLISION_H

#include "CComponent_Collision.h"

class CComponent_Sound;

class CComponent_WaterPoolCollision : public CComponent_Collision
{
private:
	float m_fHealAmount;
	CComponent_Sound* m_pSoundCmp;
	float m_fSoundTimer;

	void StaticCollision ( IBaseObject* Collider, IBaseObject* Collide );
	void DynamicCollision ( IBaseObject* Collider, IBaseObject* Collide );
	void PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction );

public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CComponent_SlimeMonsterCollision():	Default constructor
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CComponent_WaterPoolCollision();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CComponent_SlimeMonsterCollision():	Default Destructor
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~CComponent_WaterPoolCollision(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Initialize():	intis the little goo collision informtion
	//
	// Returns:		bool
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Initialize();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	float GetHealAmount () { return m_fHealAmount; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetHealAmount ( float _fHealAmount ) { m_fHealAmount = _fHealAmount; }

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
	// Shutdown():	shutdowns the little goo collision information
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown(void);

};
#endif