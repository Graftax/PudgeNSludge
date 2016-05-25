//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CComponent_LittleGooCollision.h
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds the information for little goo collision
//////////////////////////////////////////////////////////////////////////////////////
#ifndef CCOMPONENT_LITTLEGOOCOLLISION_H
#define CCOMPONENT_LITTLEGOOCOLLISION_H

#include "CComponent_Collision.h"

class CComponent_Animation;
class CComponent_Sound;
class CComponent_AI;
class CComponent_Item;

class CComponent_LittleGooCollision : public CComponent_Collision
{
private:
	IBaseObject* m_pSpawnObject;
	bool m_bPulled;
	float m_fPlayerIgnoreTimer;			// this gets set when the player lets go of the lil goo to avoid early pops

	CComponent_AI* m_aiGoo;
	CComponent_Item* m_pItemCmp;
	bool m_bVolitile;					// If the goo will die on contact or not.
	bool m_bAlive;						// Set at the beginning of update to the parents alive bool.
	bool m_bPrevAlive;					// set so we don't have to constantly get component.

	CComponent_Animation* m_pAnimComp;
	CComponent_Sound* m_pSoundCmp;
	vec3f m_tPrevPos;

	bool m_bPreviouslyCollided;
	bool m_bCurrentlyCollided;

	void StaticCollision ( IBaseObject* Collider, IBaseObject* Collide );
	void DynamicCollision ( IBaseObject* Collider, IBaseObject* Collide );
	void PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction );
	
public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CComponent_LittleGooCollision():	Default constructor
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CComponent_LittleGooCollision();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CComponent_LittleGooCollision():	Default Destructor
	//
	// Returns:		Void
	//
	// Mod. Name: JF
	// Mod. Date: 6/2/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~CComponent_LittleGooCollision(void);

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
	bool& GetVolitile() { return m_bVolitile; }
	IBaseObject* GetSpawnObject () { return m_pSpawnObject; }
	bool GetPulled () { return m_bPulled; }
	bool GetCurCollided(void) { return m_bCurrentlyCollided; }

	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetSpawnObject ( IBaseObject* _pSpawnObject ) { m_pSpawnObject = _pSpawnObject; }
	void SetPulled ( bool _bPulled ) { m_bPulled = _bPulled; }
	void SetCurCollided(bool _bCol) { m_bCurrentlyCollided = _bCol; }
	void SetPlayerIgnoreTimer(float _ignoreTime) { m_fPlayerIgnoreTimer = _ignoreTime; }


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