#include "IBaseAI.h"

IBaseAI::IBaseAI (IBaseObject* _pParent)
{
	if (!_pParent)
	{
		return;
	}

	m_pParentAIcmp = nullptr;
	m_pParentObject = _pParent;
	m_pParentCollcmp = (CComponent_Collision*)_pParent->GetComponent(ECOMP_COLLISION);
	m_pSoundCmp = nullptr;

	m_bActive = false;
	m_bAlive = false;
}

IBaseAI::~IBaseAI ()
{
	Shutdown();
}

void IBaseAI::Init ()
{
	m_pParentCollcmp = (CComponent_Collision*)m_pParentObject->GetComponent(ECOMP_COLLISION);
	m_bActive = true;
	m_bAlive = true;
	m_pSoundCmp = (CComponent_Sound*)m_pParentObject->GetComponent(ECOMP_SOUND);
}

void IBaseAI::Shutdown ()
{
	m_pParentObject = nullptr;
	m_pParentCollcmp = nullptr;
	m_pParentAIcmp = nullptr;
	m_bActive = false;
}

void IBaseAI::Update (float _fElapsedTime)
{

}