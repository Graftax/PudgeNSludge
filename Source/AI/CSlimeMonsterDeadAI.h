////////////////////////////////////////////////////////////////////
// CLilGooDeadAI.h
// Author: Josh Morgan
// Date: 8/9/12
// Purpose: This is to handle the dead state of the Slime Monster.
//			Plays my dead effect, and adds me to be removed.
////////////////////////////////////////////////////////////////////
#ifndef CSLIMEMONSTERDEADAI_H_
#define CSLIMEMONSTERDEADAI_H_

#include "IBaseAI.h"
#include "../Utilities/CollisionLib/CollisionShapes.h"
#include "../ComponentManager/CComponent_Sound.h"
#include "../ComponentManager/CComponent_Item.h"
#include "../ComponentManager/CComponent_Animation.h"
#include "../ComponentManager/CComponent_Flasher.h"

class CSlimeMonsterDeadAI : public IBaseAI
{
private:
	float m_fElapsedTime;
	CComponent_Item* m_pItemCmp;
	CComponent_Animation* m_pAnimCmp;
	CComponent_Flasher* m_pFlasherCmp;
	bool m_bPrevDead;

public:
	CSlimeMonsterDeadAI(IBaseObject* _pParent);
	~CSlimeMonsterDeadAI(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Init():		This function is a version of the virtual Init in the IBaseAI. The base ai Init() should be called in this.
	//
	// Returns:		void
	//
	// Mod. Name:	Josh Morgan
	// Mod. Date:	8/15/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Init(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():	This function updates the actions that are expected to be observed in this state.
	//
	// Returns:		void
	//
	// Mod. Name:	Josh Morgan
	// Mod. Date:	8/15/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update(float _fDT);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Shutdown():	This will clean up any memory that has been created, and set all the pointers back to null.
	//
	// Returns:		void
	//
	// Mod. Name:	Josh Morgan
	// Mod. Date:	8/15/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown(void);
};

#endif //CSLIMEMONSTERDEADAI_H_