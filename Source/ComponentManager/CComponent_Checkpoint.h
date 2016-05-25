/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CComponent_Checkpoint
//
// Purpose: Saves players progress, heals the player
//
// Original Author: Kevin Clouden
//
// Creation Date: 6/10/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CCOMPONENT_CHECKPOINT_H_
#define CCOMPONENT_CHECKPOINT_H_


// Includes
#include "IComponent.h"

class CComponent_Animation;
class CInputManager;
class LevelManager;
class CParticleManager;
class CPM_ParEmitter;
class CComponent_Sound;

__declspec(align(32))
class CComponent_Checkpoint : public IComponent
{
private:
	struct tCheckpoint
	{
		vec3f m_nPosition;
		int m_nHealth;
		int m_nLevelID;
	};

	CComponent_Sound* m_pSoundCmp;
	std::string m_szNextLevel;
	vec3f m_vPosition;
	CComponent_Animation* m_pAnimComponent;
	CInputManager* m_pInput;
	IBaseObject* m_pPlayer;
	IComponent* m_pCmp;

	//Currently hardcoded
	AABB m_Sector1CheckPoint;
	AABB m_Sector1HealingPoint;

	bool m_bSector1;
	bool m_bPlayed;
	bool m_bHasPrevLevelBeenUnloaded;

	bool m_bPrevCollided;	// Determine whether you were previously colliding with the checkpoint.
	bool m_bColliding;		// Determine whether you are currently colliding with the checkpoint.

	LevelManager* m_pLvlMng;		// Pointer to the Level Manager.
	CParticleManager* m_pPartMng;	// Pointer to the Particle Manager.

	CPM_ParEmitter*	m_pShowerEffect1;	// Pointer to the Particle Shower Emitter.
	CPM_ParEmitter*	m_pShowerEffect2;	// Pointer to the Particle Shower Emitter.
	CPM_ParEmitter*	m_pShowerEffect3;	// Pointer to the Particle Shower Emitter.
	CPM_ParEmitter* m_pFloorSplashEffect;

public:

	CComponent_Checkpoint();

	~CComponent_Checkpoint();

	/////////////////////////////////////////
	//	Accessors
	/////////////////////////////////////////
	//tCheckpoint GetCurrentLevel();

	/////////////////////////////////////////
	//	Mutators
	/////////////////////////////////////////
	void SetNextLevel (char* szLevelName) 
	{ 
		m_szNextLevel = szLevelName; 
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Initialize()
	// Purpose: Initializes data members
	// Original Author: Kevin Clouden
	// Creation Date: 6/10/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Initialize( void );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Shutdown()
	// Purpose: Shuts down the Check point
	// Original Author: Josh Fields
	// Creation Date: 6/27/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown ( );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Update()
	// Purpose: No purpose yet...
	// Original Author: Kevin Clouden
	// Creation Date: 6/10/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: SaveProgress()
	// Purpose: Saves player progress
	// Original Author: Kevin Clouden
	// Creation Date: 6/10/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool SaveProgress(  );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: LoadProgress()
	// Purpose: Loads player progress
	// Original Author: Kevin Clouden
	// Creation Date: 6/10/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadProgress(  );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: LoadNextLevel()
	// Purpose: Loads next lvl
	// Original Author: Kevin Clouden
	// Creation Date: 6/10/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool LoadNextLevel( char* szFilename );

	void SetPosition ( vec3f _vPosition ) { m_vPosition = _vPosition; }

	void BuildAABB ();
};
#endif  //CCOMPONENT_CHECKPOINT_H_