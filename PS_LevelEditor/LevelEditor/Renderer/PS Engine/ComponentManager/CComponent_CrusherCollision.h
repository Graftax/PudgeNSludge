#ifndef CCOMPONENT_CRUSHERCOLLISION_H
#define	CCOMPONENT_CRUSHERCOLLISION_H

//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_CrusherCollision.h
// Author:              Andy Madruga
// Date:                6/4/12
// Purpose:             Defines the collisions and physics for the Crusher.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_Collision.h"

class CComponent_Sound;

#define _CRUSHER_UP		1	// True means it is Up.
#define _CRUSHER_DOWN	0	// False means it is Down.

class CComponent_CrusherCollision : public CComponent_Collision
{
private:
	AABB	m_bCrusherVolume;		// The volume for the flat surface of the crusher.
	bool	m_bIsCrusherUp;			// Determine whether the state of the Crusher is Up or Down.	
	bool	m_bIsCrusherCrushing;	// This Flag will determine whether the crusher is moving.
	bool	m_bCollided;			// this tells us that we collided with something that is stoping us

	int		m_nLength;				// The length is the number of Game Units the Crusher will move by.
	int		m_nDirection;			// The Direction the Crusher moves and its bounding box is generated from.
	float	m_fLengthInGameUnits;	// The length is the distance in Game units of how far the Crusher should move.
	float	m_fCrusherDuration;		// The Time it takes for the Crusher to Drop and Rise.
	float	m_fCrusherFrequency;	// The timer counting up to the next time the Crusher will drop.
	float	m_fDurationTimer;		// The current duration timer counting up.
	float	m_fFrequencyTimer;		// The current frequency timer counting up.
	// Possible Direction not needed just move on Forward Z.

	float	m_fCrusherSpeed;		// The Speed at which the Crusher falls and rises.
	float	m_fCrusherDropDuration;	// The duration at which the Crusher will drop.
	
	CComponent_Sound* m_pSoundCmp;
	float	m_fClickTimer;

	bool	m_bCrusherSparksCreated;// Bool determining whether the crusher sparks effect was created or not.
	float	m_fEffectToFrequencyTimerMax;// Timer used to create the effect.

	vec3f	CrusherSparkPositions[2];	// The positions of the Spark effects.

private:
	void StaticCollision ( IBaseObject* Collider, IBaseObject* Collide );
	void DynamicCollision ( IBaseObject* Collider, IBaseObject* Collide );
	void PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction );

	

public:
	CComponent_CrusherCollision(void);
	~CComponent_CrusherCollision(void);

	bool Initialize(void);
	void Update( float fDT );
	void Shutdown(void);

	///////////////////////////////////////////////////////
	////////			***Accessors***			///////////
	///////////////////////////////////////////////////////
	AABB GetCrusherCollisionVolume() { return m_bCrusherVolume; }


	///////////////////////////////////////////////////////
	////////			***Mutators***			///////////
	///////////////////////////////////////////////////////
	void SetCrusherFrequency( float Frequency ) { m_fCrusherFrequency = Frequency; }
	void SetCrusherDuration ( float Duration  ) { m_fCrusherDuration  = Duration;  }
	void SetCrusherLength	( int	Length	  ) { m_nLength			  = Length;	   }
	void SetCrusherDirection( int	Direction ) { m_nDirection		  = Direction; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: BuildCrusherAABB()
	// Purpose: Create an AABB for the Crusher
	// Original Author: Andy Madruga
	// Creation Date: 6/12/2012
	// Last Modification By: 
	// Last Modification Date: 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void BuildCrusherAABB(void);

};


#endif