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
#include "CComponent_Health.h"
#include "..//ObjectLib/ObjectManager/CObjectManager.h"
#include "..//PS/States/GamePlayState.h"
#include "..//RenderEngine/RenderEngine.h"
#include "../AnimationLib/AnimationIncludes.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Initialize()
// Purpose: Initializes data members
// Original Author: Rueben Massey
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_Health::Initialize( void )
{
	m_unHealth = MAX_HEALTH;

	m_fDamageTimer = 0.0f;
	m_bInGooPit = false;
	m_bHealing = false;
	m_bPlayerHasTakenLightDamage = false;
	m_bPlayerHasTakenHeavyDamage = false;
	pAnimComponent = NULL;
	m_pSoundComponent = nullptr;
	m_fGooPitDamageVal = 5.0f;
	m_fDeathTimerTransition = 0.0f;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update()
// Purpose: No purpose yet...
// Original Author: Rueben Massey
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Health::Update( float fDT )
{
	// Get the input component of this components parent object
	CComponent_Input* inputComp = (CComponent_Input*)(CObjectManager::GetInstance()->GetPlayer()->GetComponent( ECOMP_INPUT ));
	
	if( !pAnimComponent )
		pAnimComponent = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );


	if( !m_pParent->GetAlive() )
	{
		if( pAnimComponent )
 		{
			if(pAnimComponent->GetInterpolator().IsAnimationDone())
			{
				m_fDeathTimerTransition += fDT;

				if( m_fDeathTimerTransition >= 1.5f )
					KillObject();
			}
		}
		else
			KillObject();
	}

	m_fDamageTimer += fDT;

	if(!m_pSoundComponent && m_pParent)
	{
		m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));
	}

	/*char buffer[100];
	sprintf_s( buffer,  "DAMAGE TIMER : %f", fDT );
	LOG( LOG_DEBUG, "CComponent_GooPitCollision::Update()", buffer );*/
	if( m_bInGooPit )
	{
		if(m_fDamageTimer > 1.0f)
		{
			m_fDamageTimer = 0.0f;
			m_pSoundComponent->PlaySound(DLG_PLAYER_HURT);
		}

		// Damage the player
			AddSludge( m_fGooPitDamageVal * fDT );

		// Component Safety Check
		if( inputComp )
		{
			// Slow the player speed
			inputComp->SetSpeedMod( GOO_SPEED_MODIFIER );
		}
	}
	else
	{
		// Component Safety Check
		if( inputComp )
		{
			// Set speed back to normal
			inputComp->SetSpeedMod( PLAYER_SPEED_MODIFIER );
		}
	}

	if (m_bHealing)
	{
		// TODO:
		// change this to use the heal amount from the water pools
		RemoveSludge (10.0f * fDT);
		m_bHealing = false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: AddSludge()
// Purpose: Add sludge to the player
// Original Author: Rueben Massey
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Health::AddSludge( float fSludgeAmount )
{
	if( !m_pParent->GetAlive() )
		return;

	// decrease health
	m_unHealth -= fSludgeAmount;

	//char buffer[100];
	//sprintf_s( buffer, "HEALTH = %d", m_unHealth );

	//LOG( LOG_DEBUG, "CComponent_Health::AddSludge()", buffer );

	//If a little goo attacks the player play the small damage animation
	if(fSludgeAmount == LIGHT_DAMAGE)
	{
		CObjectManager::GetInstance()->CreateObject( OBJ_LITTLEHIT_MESHEFFECT, m_pParent->GetPositionMatrix().ma);

		if(pAnimComponent)
		{
			if(pAnimComponent->GetAnimStateMachine()->GetCurrentState() != PudgeSmallDamageState::GetState())
			{
				m_bPlayerHasTakenLightDamage = true;
				m_bPlayerHasTakenHeavyDamage = false;
				pAnimComponent->GetAnimStateMachine()->ChangeState(PudgeSmallDamageState::GetState());
			}
		}
	}

	//If a junk hunk attacks the player play the big damage animation
	if(fSludgeAmount == HEAVY_DAMAGE)
	{
		CObjectManager::GetInstance()->CreateObject( OBJ_BIGHIT_MESHEFFECT, m_pParent->GetPositionMatrix().ma);

		if(pAnimComponent)
		{
			if(pAnimComponent->GetAnimStateMachine()->GetCurrentState() != PudgeBigDamageState::GetState())
			{
				m_bPlayerHasTakenLightDamage = false;
				m_bPlayerHasTakenHeavyDamage = true;
				pAnimComponent->GetAnimStateMachine()->ChangeState(PudgeBigDamageState::GetState());
			}
		}
	}

	// if the health is less then 0
	if( m_unHealth <= 0.0f )
	{
		// set the parents alive to false
		m_pParent->SetAlive (false);
		// set the health to 0
		m_unHealth = 0.0f;
		// play some sound
		m_pSoundComponent->PlaySound(DLG_PLAYER_DEATH);
		//KillObject();
 		if( pAnimComponent )
 		{
			// Create the Death Mesh Effect.
			CObjectManager::GetInstance()->CreateObject( OBJ_PLAYER_DEATH_EFFECT, m_pParent->GetPositionMatrix().ma);
			// Transition to the Death By Goo Animation if not already dead.
			pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeDeathByGooState::GetState() );
			((CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION ))->CleanRemoveVelocity();
		}
		else
		{
 			KillObject();
 		}
	}

	// update the HUD
	CGamePlayState::GetInstance()->GetHUD()->SetSludgeAmount(m_unHealth);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: TakeDamage()
// Purpose: Decrement enemy health
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Health::TakeDamage( float fDamageAmount )
{
	m_unHealth -= fDamageAmount;
	if( m_unHealth <= 0 )
	{
		// set the parents alive to false
		m_pParent->SetAlive (false);

		m_unHealth = 0.0f;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: RemoveSludge()
// Purpose: Remove sludge from the player
// Original Author: Rueben Massey
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Health::RemoveSludge( float fAmountHealed )
{
	m_unHealth += fAmountHealed;
	//cap health at 100
	if( m_unHealth > 100 )
	{
		m_unHealth = 100;
	}
	// update the HUD
	CGamePlayState::GetInstance()->GetHUD()->SetSludgeAmount(m_unHealth);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Shutdown()
// Purpose: Reset all data
// Original Author: Rueben Massey
// Creation Date: 5/8/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Health::Shutdown()
{
	m_pParent = nullptr;
	m_bHealing = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: KillObject()
// Purpose: Remove the parent of this component from the game
// Original Author: Rueben Massey
// Creation Date: 5/14/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Health::KillObject()
{
	//Play death animation

	// if this is the player
	if( m_pParent->GetType() == OBJ_PLAYER )
	{
		// Reset player to Checkpoint
		CGamePlayState::GetInstance()->EndGame();
		RenderEngine::SetObjPulled(false);
	}
	else
	{
		// kill the object
		CObjectManager::GetInstance()->AddObjectToRemove( m_pParent );
	}

}