#include "CSlimeMonsterDeadAI.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../ObjectLib/Object Factory/CObjectFactory.h"
#include "../PS/States/GamePlayState.h"
#include "../AnimationLib/AnimationIncludes.h"

CSlimeMonsterDeadAI::CSlimeMonsterDeadAI(IBaseObject* _pParent) : IBaseAI(_pParent)
{
	m_fElapsedTime = 0.0f;
	
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
	m_pAnimCmp = nullptr;
	m_pFlasherCmp = nullptr;

	m_bPrevDead = false;
}

CSlimeMonsterDeadAI::~CSlimeMonsterDeadAI(void)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init():		This function is a version of the virtual Init in the IBaseAI. The base ai Init() should be called in this.
//
// Returns:		void
//
// Mod. Name:	Josh Morgan
// Mod. Date:	8/15/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterDeadAI::Init(void)
{
	IBaseAI::Init ();

	m_fElapsedTime = 0.0f;

	m_pSoundCmp = (CComponent_Sound*)m_pParentObject->GetComponent(ECOMP_SOUND);
	m_pItemCmp = (CComponent_Item*)m_pParentObject->GetComponent(ECOMP_ITEM);
	m_pAnimCmp = (CComponent_Animation*)m_pParentObject->GetComponent(ECOMP_ANIMATION);
	m_pFlasherCmp = (CComponent_Flasher*)m_pParentObject->GetComponent(ECOMP_FLASHER);

	m_bPrevDead = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	This function updates the actions that are expected to be observed in this state.
//
// Returns:		void
//
// Mod. Name:	Josh Morgan
// Mod. Date:	8/15/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterDeadAI::Update(float _fDT)
{
	m_fElapsedTime = _fDT;

	if(!m_bPrevDead)
	{
		//stop us in our tracks
		m_pParentCollcmp->CleanRemoveVelocity();

		if(m_pSoundCmp)
		{
			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_DEATH);
			m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_DEATH);
		}

		//play the death explosion particle effect
		CParticleManager::GetInstance()->CreateEffectOneShot( "LilGooExplosion", m_pParentObject->GetWorldPos() );
		CParticleManager::GetInstance()->CreateEffectOneShot( "LilGooSpittle", m_pParentObject->GetWorldPos() );

		if(m_pAnimCmp)
		{
			m_pAnimCmp->GetAnimStateMachine()->ChangeState(SlimeMonsterDeathState::GetState());
		}

		m_bPrevDead = true;
	}

	//////////////////////////////////////////////////////////
	/********************************************************/
	/******** Don't forget to tell our children that ********/
	/********         their parent has DIED          ********/
	/********************************************************/
	//////////////////////////////////////////////////////////

	//add the object to be destroyed
	if(m_pAnimCmp && m_pAnimCmp->GetInterpolator().IsAnimationDone())
	{
		m_pFlasherCmp->SetActive(true);
		CGame::GetInstance()->GetAudioManager()->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown():	This will clean up any memory that has been created, and set all the pointers back to null.
//
// Returns:		void
//
// Mod. Name:	Josh Morgan
// Mod. Date:	8/15/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSlimeMonsterDeadAI::Shutdown(void)
{
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
	IBaseAI::Shutdown();
}