#include "CLilGooDeadAI.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"

CLilGooDeadAI::CLilGooDeadAI(IBaseObject* _pParent) : IBaseAI(_pParent)
{
	m_fElapsedTime = 0.0f;
	
	m_pAnimComponent = nullptr;
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
}

CLilGooDeadAI::~CLilGooDeadAI(void)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init():	This function is a version of the virtual Init in the IBaseAI. The base ai Init() should be called in this.
//
// Returns:		bool
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLilGooDeadAI::Init(void)
{
	IBaseAI::Init ();

	m_fElapsedTime = 0.0f;

#if _ANIMATIONS
	m_pAnimComponent = (CComponent_Animation*)m_pParentObject->GetComponent( ECOMP_ANIMATION );
#endif

	m_pSoundCmp = (CComponent_Sound*)m_pParentObject->GetComponent(ECOMP_SOUND);
	m_pItemCmp = (CComponent_Item*)m_pParentObject->GetComponent(ECOMP_ITEM);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	This function updates the actions that are expected to be observed in this state.
//
// Returns:		bool
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLilGooDeadAI::Update(float _fDT)
{
	m_fElapsedTime = _fDT;

	//stop us in our tracks
	m_pParentCollcmp->CleanRemoveVelocity();

	if(m_pSoundCmp)
	{
		m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_DEATH);
		m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_DLG_LILGOO_DEATH);
	}

	CGame::GetInstance()->GetAudioManager()->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);

	//play the death explosion particle effect
	CParticleManager::GetInstance()->CreateEffectOneShot( "LilGooExplosion", m_pParentObject->GetWorldPos() );
	CParticleManager::GetInstance()->CreateEffectOneShot( "LilGooSpittle", m_pParentObject->GetWorldPos() );

	if(m_pItemCmp->GetTarget())
	{
		//make sure the player and my item component know I'm no longer heading toward the player
		((CComponent_Input*)(CObjectManager::GetInstance()->GetPlayer()->GetComponent( ECOMP_INPUT )))->SetObjectPulled( nullptr );
		m_pItemCmp->SetTarget(nullptr);
	}

	//add the object to be destroyed
	CObjectManager::GetInstance()->AddObjectToRemove(m_pParentObject);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown(): This will clean up any memory that has been created, and set all the pointers back to null.
//
// Returns:		bool
//
// Mod. Name: Josh Morgan
// Mod. Date:8/9/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLilGooDeadAI::Shutdown(void)
{
	m_pAnimComponent = nullptr;
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
	IBaseAI::Shutdown();
}