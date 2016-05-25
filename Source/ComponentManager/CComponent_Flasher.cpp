#include "CComponent_Flasher.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"

CComponent_Flasher::CComponent_Flasher() : IComponent(ECOMP_FLASHER)
{
	m_fTimeLeft = 0.0f;
	m_fFlashTimer = 0.0f;
	m_bActive = false;
}

bool CComponent_Flasher::Initialize( void )
{
	m_fFlashTimer = 0.0f;
	return true;
}

void CComponent_Flasher::Update( float fDT )
{
	if(!m_bActive)
	{
		return;
	}

	m_fTimeLeft -= fDT;

	if( m_fTimeLeft < 0.0f )
	{
		CObjectManager::GetInstance()->AddObjectToRemove( GetParent() );
	}

	if( m_fTimeLeft < 1.0f )
	{
		m_fFlashTimer += fDT;
	}

	if( m_fFlashTimer > 1.0f/16.0f )
	{
		GetParent()->SetRendered( !GetParent()->GetRendered() );
		m_fFlashTimer = 0.0f;
	}

	if( !m_pParent->GetRendered() )
	{
		CComponent_Render* cmpRender = (CComponent_Render*)m_pParent->GetComponent( ECOMP_RENDER );
		cmpRender->GetRenderNode()->GetRenderData().m_vTransform.clear();
	}
}

void CComponent_Flasher::Shutdown()
{
	m_fTimeLeft = 0.0f;
	m_fFlashTimer = 0.0f;
	m_bActive = false;
}

void CComponent_Flasher::SetTimeLeft( float _time )
{
	m_fTimeLeft = _time;
}