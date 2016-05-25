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
#include "../LevelManager/CLevelManager.h"

#define SOUND_TIME 0.33f

CComponent_Health::CComponent_Health() : IComponent( ECOMP_HEALTH ), m_unHealth( 0 ) 
{
	m_unHealth = 0;

	m_fDamageTimer = 0.0f;
	m_fHealTimer = 0.0f;
	m_fSoundTimer = 0.0f;
	m_bInGooPit = false;
	m_bHealing = false;
	m_bPlayerHasTakenLightDamage = false;
	m_bPlayerHasTakenHeavyDamage = false;
	pAnimComponent = nullptr;
	m_pSoundComponent = nullptr;
	m_fGooPitDamageVal = 0.0f;
	m_fDeathTimerTransition = 0.0f;
	m_bPrevDead = false;
}

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
	m_fHealTimer = 0.0f;
	m_fSoundTimer = 0.0f;
	m_bInGooPit = false;
	m_bHealing = false;
	m_bPlayerHasTakenLightDamage = false;
	m_bPlayerHasTakenHeavyDamage = false;
	pAnimComponent = NULL;
	m_pSoundComponent = nullptr;
	m_fGooPitDamageVal = 5.0f;
	m_fDeathTimerTransition = 0.0f;

	pAnimComponent = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );

	m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));

	m_pBodyBubbleEffect = CParticleManager::GetInstance()->CreateEffect( "BodyBubbles", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_pBodyBubbleEffect->m_bLocalSpace = true;
	m_pStraySudsEffect = CParticleManager::GetInstance()->CreateEffect( "StraySuds", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_bPrevDead = false;

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
	m_pBodyBubbleEffect->m_fPosition = GetParent()->GetWorldPos() + vec3f( 0.0f, 40.0f, -20.0f );
	m_pStraySudsEffect->m_fPosition = GetParent()->GetWorldPos() + vec3f( 0.0f, 40.0f, -20.0f );

	if( !m_pParent->GetAlive() )
	{
		if(m_pParent->GetType() == OBJ_PLAYER && m_pSoundComponent && !m_bPrevDead)
		{
			m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_DLG_PLAYER_DEATH);
		}

		if( pAnimComponent )
		{
			if(pAnimComponent->GetInterpolator().IsAnimationDone())
			{
				m_fDeathTimerTransition += fDT;

				if( m_fDeathTimerTransition >= 1.5f )
				{
					m_fDeathTimerTransition = 0.0f;
					if (m_pParent->GetType () != OBJ_BOSS)
					{
						KillObject();
					}
				}
			}
		}
		else
		{
			if (m_pParent->GetType () != OBJ_BOSS)
			{
				KillObject();
			}
		}

		m_bPrevDead = true;
	}

	m_fDamageTimer += fDT;
	m_fHealTimer += fDT;
	m_fSoundTimer += fDT;

	// If this is the player's Health Component
	if( OBJ_PLAYER == m_pParent->GetType() )
	{
		// update the HUD
		CGamePlayState::GetInstance()->GetHUD()->SetSludgeAmount(m_unHealth);
	}

	if (OBJ_BOSS == m_pParent->GetType () )
	{
		CGamePlayState::GetInstance()->GetHUD()->SetBossHealth(m_unHealth);
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

	//If a little goo attacks the player play the small damage animation
	if(fSludgeAmount <= LIGHT_DAMAGE)
	{

		if(pAnimComponent)
		{
			if(pAnimComponent->GetAnimStateMachine()->GetCurrentState() != PudgeSmallDamageState::GetState())
			{
				m_bPlayerHasTakenLightDamage = true;
				m_bPlayerHasTakenHeavyDamage = false;
				pAnimComponent->GetAnimStateMachine()->ChangeState(PudgeSmallDamageState::GetState());
			}
		}

		if(m_pSoundComponent && m_fSoundTimer > SOUND_TIME && m_pParent->GetType() == OBJ_PLAYER)
		{
			m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_DLG_PLAYER_HURT_SMALL);
			m_fSoundTimer = 0.0f;
		}
	}

	//If a junk hunk attacks the player play the big damage animation
	if(fSludgeAmount > LIGHT_DAMAGE)
	{
		if(pAnimComponent)
		{
			if(pAnimComponent->GetAnimStateMachine()->GetCurrentState() != PudgeBigDamageState::GetState())
			{
				m_bPlayerHasTakenLightDamage = false;
				m_bPlayerHasTakenHeavyDamage = true;
				pAnimComponent->GetAnimStateMachine()->ChangeState(PudgeBigDamageState::GetState());

			}
		}

		if(m_pSoundComponent && m_fSoundTimer > SOUND_TIME && m_pParent->GetType() == OBJ_PLAYER)
		{
			m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_DLG_PLAYER_HURT_BIG);
		}
	}

	// if the health is less then 0
	if( m_unHealth <= 0.0f )
	{
		// set the parents alive to false
		m_pParent->SetAlive (false);
		// set the health to 0
		m_unHealth = 0.0f;
		//if(m_pSoundComponent)
		//{
		//	// play some sound
		//	m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_DLG_PLAYER_DEATH);
		//}
		//KillObject();
		if( pAnimComponent )
		{
			// Create the Death Mesh Effect.
			CObjectManager::GetInstance()->CreateObject( OBJ_PLAYER_DEATH_EFFECT, m_pParent->GetPositionMatrix().ma, LevelManager::GetInstance()->GetCurrentLevelIndex() );
			// Transition to the Death By Goo Animation if not already dead.
			pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeDeathByGooState::GetState() );
			((CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION ))->CleanRemoveVelocity();
		}
		else
		{
			KillObject();
		}
	}
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

		if( m_pParent->GetType() == OBJ_PLAYER )
			CObjectManager::GetInstance()->CreateObject( OBJ_PLAYER_DEATH_EFFECT, m_pParent->GetPositionMatrix().ma, LevelManager::GetInstance()->GetCurrentLevelIndex() );
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

	m_pBodyBubbleEffect->Emit( 1.0f/60.0f );
	m_pStraySudsEffect->Emit( 1.0f/60.0f );

	//cap health at 100
	if( m_unHealth > 100 )
	{
		m_unHealth = 100;
	}
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
	CParticleManager::GetInstance()->DestroyEffect( m_pBodyBubbleEffect );
	CParticleManager::GetInstance()->DestroyEffect( m_pStraySudsEffect );

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: ResetPlayer()
// Purpose: Reset the Player to his Spawn Position.
// Original Author: Andy Madruga
// Creation Date: 7/21/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Health::ResetPlayer()
{
	if( m_pParent->GetType() == OBJ_PLAYER )
	{
		m_pParent->GetFrame().GetLocalMat().axis_pos = LevelManager::GetInstance()->GetCurrentLevel()->GetPlayerSpawnPos();
		m_pParent->GetFrame().Update();
		m_pParent->GetFrame().GetWorldMat();

		m_pParent->SetAlive( true );
		m_unHealth = 100.0f;
	}
}