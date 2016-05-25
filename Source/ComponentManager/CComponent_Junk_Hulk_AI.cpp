#include "CComponent_Junk_Hulk_AI.h"

CComponent_Junk_Hulk_AI::CComponent_Junk_Hulk_AI ()
{
	m_bStuned = false;
}

CComponent_Junk_Hulk_AI::~CComponent_Junk_Hulk_AI ()
{
	Shutdown ();
}

bool CComponent_Junk_Hulk_AI::Initialize ()
{
	m_aiType = AI_SLIME_MONSTER;
	m_spAggro.SetRadius (300.0f);
	m_spAttack.SetRadius (60.0f);
	m_bStuned = false;

	return true;
}

void CComponent_Junk_Hulk_AI::Shutdown ()
{
	m_bStuned = false;
}

void CComponent_Junk_Hulk_AI::Update ( float _fElapsedTime )
{
	if ( m_bStuned )
	{
		return;
	}

	// get some objects from the KD Tree to find a target and to avoid the crushers

	if (m_pTarget)
	{
		CCollisionVolume* _pTargetVolume = m_pTarget->GetCollidableObject();
		if ( _pTargetVolume && _pTargetVolume->GetVolumeType () == VMT_AABB)
		{
			vec2f Useless;
			bool _bAggro = m_spAggro.SphereToAABB (*((AABB*)_pTargetVolume), Useless);

			if (!_bAggro)
			{
				m_pTarget = NULL;
				return;
			}

			bool _bAttack = m_spAttack.SphereToAABB (*((AABB*)_pTargetVolume), Useless);
			if ( _bAggro && !_bAttack )
			{
				Move (m_pTarget);
			}

			if (_bAttack)
			{
				Attack ();
			}
		}
	}

	Move ();
	//Avoide ( );
}

void CComponent_Junk_Hulk_AI::Attack ()
{

}

void CComponent_Junk_Hulk_AI::Move ()
{

}

void CComponent_Junk_Hulk_AI::Move (IBaseObject* _pTarget)
{

}

void CComponent_Junk_Hulk_AI::Avoide (IBaseObject* _pHazard)
{

}

void CComponent_Junk_Hulk_AI::Damaged ()
{

}
