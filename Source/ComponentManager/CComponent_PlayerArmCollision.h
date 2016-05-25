#ifndef CCOMPONENT_PLAYERARMCOLLISION_H
#define	CCOMPONENT_PLAYERARMCOLLISION_H

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_PlayerArmCollision.h
// Author:              Andy Madruga
// Date:                5/16/12
// Purpose:             Defines the collisions and physics for the player.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_Collision.h"
#include "CComponent_Animation.h"

class CGame;
class CCamera;

__declspec(align(32))
class CComponent_PlayerArmCollision : public CComponent_Collision
{

private:
	CGame*		m_pGame;
	CCamera*	m_pCamera;

	IBaseObject* m_ObjectsParent;
	CComponent_Animation* pAnimComp;
	int m_nFrameIndex;
public:

	CComponent_PlayerArmCollision();
	~CComponent_PlayerArmCollision(void);

	bool Initialize();

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	
	/////////////////////////////
	//  Mutators
	/////////////////////////////
	void SetObjectsParent(IBaseObject* obj );
	void SetFrameIndex(const char* jointName){ m_nFrameIndex = pAnimComp->GetBindPose()->GetJoint(jointName)->nJointIndex;}
	void SetAnimationComponent( ){ pAnimComp = (CComponent_Animation*)m_ObjectsParent->GetComponent( ECOMP_ANIMATION); }
	void TranslateObjectByJoint();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Update():	Updates the component, flattening the hierarchy.
	//
	// Returns:		Void
	//
	// Mod. Name:   Andy Madruga
	// Mod. Date:   5/31/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );

	void Shutdown(void);

};

#endif