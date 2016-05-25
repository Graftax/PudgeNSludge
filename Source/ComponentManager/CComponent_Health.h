/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CComponent_Health
//
// Purpose: Manage all player and enemy health
//			Kills the object associated with it when necessary
//
// Original Author: Rueben Massey
//
// Creation Date: 5/12/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CCOMPONENT_HEALTH_H_
#define CCOMPONENT_HEALTH_H_

// Defines
#define MAX_HEALTH 100

#define SLUDGE_DAMAGE 10.0f
#define SLUDGE_DAMAGE_TIME 0.01f
#define GOO_SPEED_MODIFIER 0.60f
#define PLAYER_SPEED_MODIFIER 1.0f
#define LIGHT_DAMAGE 10.0f
#define HEAVY_DAMAGE 15.0f

// Includes
#include "IComponent.h"
#include "../EffectsLib/ParticleManager.h"

class CComponent_Sound;
class CComponent_Animation;

__declspec(align(32))
class CComponent_Health : public IComponent
{
private:
	float m_unHealth;
	float m_fDamageTimer;
	float m_fHealTimer;
	float m_fSoundTimer;
	bool m_bInGooPit;
	bool m_bHealing;
	bool m_bPlayerHasTakenLightDamage;
	bool m_bPlayerHasTakenHeavyDamage;
	CComponent_Sound* m_pSoundComponent;
	CComponent_Animation*	pAnimComponent;
	bool m_bPrevDead;

	float		m_fDeathTimerTransition;		// A Timer that counts down after the Animation is done. 

	// Attrib
	float m_fGooPitDamageVal;

	CPM_ParEmitter* m_pBodyBubbleEffect;
	CPM_ParEmitter* m_pStraySudsEffect; 

public:

	CComponent_Health();

	~CComponent_Health() { }

	/////////////////////////////////////////
	//	Accessors
	/////////////////////////////////////////
	float GetHealth( void ) { return m_unHealth; }
	bool GetInGooPit() const { return m_bInGooPit; }
	bool GetHealing() const { return m_bHealing; }
	float GetGooPitDamageVal() const { return m_fGooPitDamageVal; }
	bool GetPlayerHasTakenLightDamage() const { return m_bPlayerHasTakenLightDamage; }
	bool GetPlayerHasTakenHeavyDamage() const { return m_bPlayerHasTakenHeavyDamage; }


	/////////////////////////////////////////
	//	Mutators
	/////////////////////////////////////////
	void SetInGooPit(bool val) { m_bInGooPit = val; }
	void SetHealing ( bool _bHealing ) { m_bHealing = _bHealing; }
	void SetGooPitDamageVal(float val) { m_fGooPitDamageVal = val; }
	void SetPlayerHasTakenLightDamage(bool val) { m_bPlayerHasTakenLightDamage = val; }
	void SetPlayerHasTakenHeavyDamage(bool val) { m_bPlayerHasTakenHeavyDamage = val; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Initialize()
	// Purpose: Initializes data members
	// Original Author: Rueben Massey
	// Creation Date: 5/12/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Initialize( void );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Update()
	// Purpose: No purpose yet...
	// Original Author: Rueben Massey
	// Creation Date: 5/12/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update( float fDT );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: AddSludge()
	// Purpose: Add sludge to the player
	// Original Author: Rueben Massey
	// Creation Date: 5/12/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AddSludge( float fSludgeAmount );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: TakeDamage()
	// Purpose: Decrement enemy health
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TakeDamage( float fDamageAmount );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: RemoveSludge()
	// Purpose: Remove sludge from the player
	// Original Author: Rueben Massey
	// Creation Date: 5/12/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void RemoveSludge( float fAmountHealed );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: Shutdown()
	// Purpose: Reset all data
	// Original Author: Rueben Massey
	// Creation Date: 5/8/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Shutdown();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: KillObject()
	// Purpose: Remove the parent of this component from the game
	// Original Author: Rueben Massey
	// Creation Date: 5/14/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void KillObject();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function Name: ResetPlayer()
	// Purpose: Reset the Player to his Spawn Position.
	// Original Author: Andy Madruga
	// Creation Date: 7/21/2012
	// Last Modification By:
	// Last Modification Date:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ResetPlayer();

};
#endif  //CCOMPONENT_HEALTH_H_