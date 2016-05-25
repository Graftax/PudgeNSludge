////////////////////////////////////////////
// CSlimeMonsterIdleAI.h
// Author: Josh Morgan
// Date: 8/15/12
// Purpose: This is to handle the attack state of the
//			little goo.
////////////////////////////////////////////
#ifndef CSLIMEMONSTERATTACKAI_H_
#define CSLIMEMONSTERATTACKAI_H_

#include "IBaseAI.h"
#include "../Utilities/CollisionLib/CollisionShapes.h"
#include "../ComponentManager/CComponent_Sound.h"
#include "../ComponentManager/CComponent_Item.h"
#include "../ComponentManager/CComponent_Animation.h"

class CSlimeMonsterAttackAI : public IBaseAI
{
private:
	float m_fElapsedTime;
	vec3f m_tPlayerPos;
	bool m_bChucked;
	std::vector <CSceneObject*> m_vCurNearObjects;
	CComponent_Animation* m_pAnimComponent;
	CComponent_Item* m_pItemCmp;

public:
	CSlimeMonsterAttackAI(IBaseObject* _pParent);
	~CSlimeMonsterAttackAI(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Init():	This function is a version of the virtual Init in the IBaseAI. The base ai Init() should be called in this.
	//
	// Returns:		void
	//
	// Mod. Name: Josh Morgan
	// Mod. Date:8/15/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Init(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():	This function updates the actions that are expected to be observed in this state.
	//
	// Returns:		void
	//
	// Mod. Name: Josh Morgan
	// Mod. Date:8/15/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update(float _fDT);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Chuck():		This function spawns a volatile lil goo and sends it towards the player.
	//
	// Returns:		void
	//
	// Mod. Name:	Josh Morgan
	// Mod. Date:	8/15/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Chuck(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Shutdown(): This will clean up any memory that has been created, and set all the pointers back to null.
	//
	// Returns:		void
	//
	// Mod. Name: Josh Morgan
	// Mod. Date:8/15/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown(void);
};

#endif //CSLIMEMONSTERATTACKAI_H_