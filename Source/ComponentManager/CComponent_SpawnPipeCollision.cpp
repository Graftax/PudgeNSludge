#include "CComponent_SpawnPipeCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../Utilities/KdTree/KdTree.h"
#include "..//PS/States/CWinGameState.h"
#include "../PS/CGame.h"
#include "../AudioLib/AudioManager.h"
#include "CComponent_CorkCollision.h"
#include "Component_Emitter.h"
#include "../EffectsLib/ParticleManager.h"


CComponent_SpawnPipeCollision::CComponent_SpawnPipeCollision(void)
{
	m_fFrequency = 0.0f;
	m_fFreqTimer = 0.0f;
	m_nPipeType = EPIPE_SPAWN;
	this->CleanRemoveVelocity();

	EffectCreated = false;
}

CComponent_SpawnPipeCollision::~CComponent_SpawnPipeCollision(void)
{
}

bool CComponent_SpawnPipeCollision::Initialize(void)
{
	CComponent_BasePipe::Initialize();
	m_fFrequency = 0.0f;
	m_fFreqTimer = 0.0f;
	m_nPipeType = EPIPE_SPAWN;
	this->CleanRemoveVelocity();

	EffectCreated = false;

	return true;
}

void CComponent_SpawnPipeCollision::Update( float fDT )
{
	CComponent_BasePipe::Update(fDT);


	if (m_nActiveSpawns < (int)(m_unMaxSpawns))
	{
		m_fFreqTimer += fDT;
	}
	else
	{
		m_fFreqTimer = 0.0f;
	}

	vec3f NewCenter = m_pParent->GetWorldMat()->axis_pos;
	m_abTarget.SetWorldPoint (NewCenter);
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (NewCenter);

	if( m_fFreqTimer > (m_fFrequency - 0.2f) )
	{
		if(m_nActiveSpawns < (int)m_unMaxSpawns && !EffectCreated)
		{
			EffectCreated = true;
			// Create Spawn Pipe Drip Effect.
			CObjectManager::GetInstance ()->CreateObject (OBJ_SPAWNPIPE_DRIP, m_pParent->GetPositionMatrix().ma, m_pParent->GetLevelID());
		}
	}

	if(m_fFreqTimer > m_fFrequency)
	{
		m_fFreqTimer = 0.0f;
		EffectCreated = false;

		if(m_nActiveSpawns < (int)m_unMaxSpawns)
		{
 			D3DXMATRIX pos;
			D3DXMatrixIdentity( &pos );
			D3DXMatrixTranslation( &pos, m_pParent->GetWorldMat()->axis_pos.x, m_pParent->GetWorldMat()->axis_pos.y, m_pParent->GetWorldMat()->axis_pos.z );
			IBaseObject* Goo = CObjectManager::GetInstance ()->CreateObject (OBJ_GOO, pos, m_pParent->GetLevelID());
			if( Goo )
			{
				AddChild(Goo); //This will add the goo to the list of children and increment the number of active children
				CComponent_Item* GooItem = ((CComponent_Item*)Goo->GetComponent(ECOMP_ITEM));
				if (GooItem)
				{
					GooItem->SetSpawnIgnoreTime( 2.0f );// set the ingore timer for the little goo;
				}
				CComponent_LittleGooCollision* GooCol = ((CComponent_LittleGooCollision*)Goo->GetComponent(ECOMP_COLLISION));
				if (GooCol)
				{
					GooCol->SetSpawnObject( m_pParent );
					GooCol->CleanRemoveVelocity();
				}

				//spawn facing left
				matrix4f _localMat = (*Goo->GetLocalMat());
				matrix4f rotationMatrix;
				rotationMatrix.make_rotation_y( D3DXToRadian(90) );
				rotationMatrix.axis_pos = _localMat.axis_pos;
				_localMat = rotationMatrix;

				Goo->SetLocalMat(&_localMat);
			}
			if(m_pSoundCmp)
			{
				m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_LILGOO_DISPENSOR);
			}
		}
	}
}

void CComponent_SpawnPipeCollision::Render(void)
{

}

void CComponent_SpawnPipeCollision::Shutdown(void)
{
	CComponent_BasePipe::Shutdown();

	m_fFrequency = 0.0f;
	m_fFreqTimer = 0.0f;
	m_nPipeType = EPIPE_SPAWN;
	this->CleanRemoveVelocity();

	EffectCreated = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: BuildCorkableAABB()
// Purpose: Create an AABB for the cork to collide with
// Original Author: Rueben Massey
// Creation Date: 6/5/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_SpawnPipeCollision::BuildCorkableAABB( int nDirection )
{
	switch( nDirection)
	{
	case 0:
		{
			m_abTarget.SetMaxPoint (vec2f ( SPAWNCORKABLE_AABB_WIDTH, SPAWNCORKABLE_AABB_OFFSET + SPAWNCORKABLE_AABB_HEIGHT ));
			m_abTarget.SetMinPoint (vec2f ( -SPAWNCORKABLE_AABB_WIDTH, SPAWNCORKABLE_AABB_OFFSET ));
			break;
		}
	case 1:
		{
			m_abTarget.SetMaxPoint (vec2f ( SPAWNCORKABLE_AABB_WIDTH, -SPAWNCORKABLE_AABB_OFFSET ));
			m_abTarget.SetMinPoint (vec2f ( -SPAWNCORKABLE_AABB_WIDTH, -SPAWNCORKABLE_AABB_OFFSET - SPAWNCORKABLE_AABB_HEIGHT  ));
			break;
		}
	case 2:
		{
			m_abTarget.SetMaxPoint (vec2f ( -SPAWNCORKABLE_AABB_OFFSET, SPAWNCORKABLE_AABB_WIDTH ));
			m_abTarget.SetMinPoint (vec2f ( -SPAWNCORKABLE_AABB_OFFSET - SPAWNCORKABLE_AABB_HEIGHT, -SPAWNCORKABLE_AABB_WIDTH ));
			break;
		}
	case 3:
		{
			m_abTarget.SetMaxPoint (vec2f ( SPAWNCORKABLE_AABB_OFFSET + SPAWNCORKABLE_AABB_HEIGHT, SPAWNCORKABLE_AABB_WIDTH ));
			m_abTarget.SetMinPoint (vec2f ( SPAWNCORKABLE_AABB_OFFSET, -SPAWNCORKABLE_AABB_WIDTH ));
			break;
		}

	};
}