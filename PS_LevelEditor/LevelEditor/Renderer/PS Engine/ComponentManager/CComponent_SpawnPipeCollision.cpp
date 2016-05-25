#include "CComponent_SpawnPipeCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../Utilities/KdTree/KdTree.h"
#include "..//PS/States/CWinGameState.h"
#include "../PS/CGame.h"
#include "../AudioLib/AudioManager.h"
#include "CComponent_CorkCollision.h"
#include "Component_Emitter.h"
#include "../EffectsLib/ParticleManager.h"
#include "../EffectsLib/Emitter.h"
#include "../EffectsLib/Effects.h"


CComponent_SpawnPipeCollision::CComponent_SpawnPipeCollision(void)
{
	//m_abTarget = nullptr;
	//m_abTarget = new AABB();

	//((AABB*)&m_abTarget)->SetMaxPoint (vec2f (60.0f, 0.0f));
	//((AABB*)&m_abTarget)->SetMinPoint (vec2f (-60.0f, -40.0f));

	m_bCorked = false;
	m_bCurrentlyActive = false;
	m_unMaxEnemies = 5u;
	m_nActiveEnemies = 0u;
	m_bCorkable = false;
	m_fFrequency = 0.0f;
	m_fFreqTimer = 0.0f;
	this->CleanRemoveVelocity();
}

CComponent_SpawnPipeCollision::~CComponent_SpawnPipeCollision(void)
{
	//if( m_abTarget )
		//delete m_abTarget;
}

bool CComponent_SpawnPipeCollision::Initialize(void)
{
	m_bCorked = false;
	m_bCurrentlyActive = false;
	m_unMaxEnemies = 5u;
	m_nActiveEnemies = 0u;
	m_bCorkable = false;

	CComponent_Collision::Initialize();
	m_bCurrentlyActive = true;
	m_fFrequency = 0.0f;
	m_fFreqTimer = 0.0f;
	this->CleanRemoveVelocity();

	return true;
}

void CComponent_SpawnPipeCollision::Update( float fDT )
{
	if( !m_bCurrentlyActive || m_bCorked )
	{
		if( !m_bCurrentlyActive && !m_bCorked )
		{
			m_bCurrentlyActive = true;
		}
		else
			LOG(LOG_INFO, "Spawn Error", "Spawn" );
		return;
	}
	if (m_pParent->GetCollidableObject()->GetVolumeType () != VMT_AABB)
	{
		return;
	}

	if (!m_bCorked && m_nActiveEnemies < (int)m_unMaxEnemies)
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

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_GOO) | (1<<OBJ_MONSTER);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects (m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects);

	vec2f Direction;
	vec2f CP;
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = ((IBaseObject*)NearestObjects[objindex]);

		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(_pObject)->GetComponent (ECOMP_COLLISION);
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();
		// Safety Check

		if( curVolume && curCollision )
		{
			if( curCollision->GetParent() == GetParent() )
				continue;

			if( BT_DYNAMIC == curVolume->GetBodyType() )
			{
				// collision checks
				// based on volume type
				// if its a little goo and the little goo ingore timer is > 0.0 dont check collision
				if ( _pObject->GetType () == OBJ_GOO)
				{
					if ( ((CComponent_LittleGooCollision*)_pObject->GetComponent (ECOMP_COLLISION))->GetSpawnIngore ( ) > 0.0f)
					{
						continue;
					}

				}
				CheckDynamicCollisions( curCollision, curVolume, fDT );

			}
		}
	}

	if( m_fFreqTimer > m_fFrequency )
	{
		m_fFreqTimer = 0.0f;

		if (m_nActiveEnemies < (int)m_unMaxEnemies )
		{
			m_nActiveEnemies++;

			D3DXMATRIX pos;
			D3DXMatrixIdentity( &pos );
			D3DXMatrixTranslation( &pos, GetPos().x, GetPos().y, GetPos().z );
			IBaseObject* Goo = CObjectManager::GetInstance ()->CreateObject (OBJ_GOO, pos);
			((CComponent_LittleGooCollision*)Goo->GetComponent (ECOMP_COLLISION))->SetSpawnIngore ( 2.0f );// set the ingore timer for the little goo;
			((CComponent_LittleGooCollision*)Goo->GetComponent (ECOMP_COLLISION))->SetSpawnObject ( m_pParent );

		}
	}

}

void CComponent_SpawnPipeCollision::Render(void)
{

}

void CComponent_SpawnPipeCollision::Shutdown(void)
{
	//if( m_cvPipe )
	//	delete m_cvPipe;
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Rueben Massey
// Creation Date: 6/11/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_SpawnPipeCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
{
	vec2f CP;
	vec2f Direction;
	switch( pCurVolume->GetVolumeType() )
	{
	case VMT_AABB:
		{
			if( ((AABB*)pCurVolume)->AABBToAABB( *((AABB*)m_pParent->GetCollidableObject()), Direction ) )
			{
				pCurCollision->CleanOffset( Direction );
				pCurCollision->CleanForce( Direction * 0.5f );
				// update the AABB
				((AABB*)pCurCollision->GetParent()->GetCollidableObject())->SetWorldPoint( pCurCollision->GetPos2DClean() );
				// set the player to grounded
				// if he is on top of us
				if( OBJ_PLAYER == pCurCollision->GetParent()->GetType() )
				{
					if( dot_product( Direction, vec2f( 0.0f, 1.0f ) ) > 0.5f )
					{
						((CComponent_PlayerCollision*)pCurCollision)->SetCollidingGroundAlone( true );
					}
				}
			}
			break;
		}
	case VMT_BOX:
		{
			if( pCurCollision->GetParent()->GetType () == OBJ_CORK && m_bCorkable && !m_bCorked )
			{
				// We have collided
				// Do stuff to the object
				if( ((Box*)pCurVolume)->BoxToAABB( m_abTarget, CP, Direction ) )
				{
					m_bCorked = true;
					m_bCurrentlyActive = false;
					((CComponent_CorkCollision*)pCurCollision)->SetCorked(true);
					pCurCollision->GetParent()->SetLocalPos( &vec3f( 
						( m_abTarget.GetMax2DTransformed().x + m_abTarget.GetMin2DTransformed().x ) / 2.0f, 
						( m_abTarget.GetMax2DTransformed().y + m_abTarget.GetMin2DTransformed().y ) / 2.0f, 
						-500.0f ) );
				}
			}

			if( ((Box*)pCurVolume)->BoxToAABB( *((AABB*)m_pParent->GetCollidableObject()), CP, Direction ) )
			{
				pCurCollision->CleanOffset( Direction );
				pCurCollision->CleanForce( Direction * 0.5f );
			}

			break;
		}
	case VMT_SPHERE:
		{
			break;
		}
	}
}
