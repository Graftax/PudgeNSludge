#include "CComponent_BasePipe.h"
#include "../Utilities/KdTree/KdTree.h"
#include "CComponent_PlayerCollision.h"
#include "CComponent_LittleGooCollision.h"
#include "CComponent_Item.h"
#include "CComponent_Sound.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"

CComponent_BasePipe::CComponent_BasePipe(void)
{
	m_nActiveSpawns = 0;
	m_unMaxSpawns = 0;
	m_bActive = false;
	m_fElapsedTime = 0.0f;
	m_nPipeType = EPIPE_DEFAULT;
	m_pSoundCmp = nullptr;
}

CComponent_BasePipe::~CComponent_BasePipe(void)
{

}

bool CComponent_BasePipe::Initialize(void)
{
	CComponent_Collision::Initialize();
	m_nActiveSpawns = 0;
	m_unMaxSpawns = 0;
	m_bActive = false;
	m_fElapsedTime = 0.0f;
	m_nPipeType = EPIPE_DEFAULT;

	m_pSoundCmp = (CComponent_Sound*)m_pParent->GetComponent(ECOMP_SOUND);

	return true;
}

void CComponent_BasePipe::Update(float fDT)
{
	if (m_pParent->GetCollidableObject()->GetVolumeType () != VMT_AABB)
	{
		return;
	}

	m_fElapsedTime = fDT;

	vec3f NewCenter = m_pParent->GetWorldMat()->axis_pos;
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (NewCenter);

	// create a return vector to hold all the objects the kd tree returns
	//std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_GOO) | (1<<OBJ_MONSTER);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects (m_pParent, PSFLAG_AABB, ReturnParams, m_vNearestObjects, ReturnBody, ReturnObjects);

	vec2f Direction;
	vec2f CP;
	for( unsigned int objindex = 0; objindex < m_vNearestObjects.size(); ++objindex )
	{
		IBaseObject* _pObject = ((IBaseObject*)m_vNearestObjects[objindex]);

		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(_pObject)->GetComponent (ECOMP_COLLISION);
		// get the collision volume of the object
		CCollisionVolume* curVolume = m_vNearestObjects[objindex]->GetCollidableObject();
		// Safety Check

		if( curVolume && curCollision )
		{
			if( curCollision->GetParent() == GetParent() )
				continue;

			CComponent_Item* colItem = (CComponent_Item*)_pObject->GetComponent(ECOMP_ITEM);
			if(colItem && colItem->GetSpawnIgnoreTime() >= 0.0f)
			{
				continue;
			}

			if( BT_DYNAMIC == curVolume->GetBodyType() )
			{
				// collision checks
				// based on volume type
				CheckDynamicCollisions(curCollision, curVolume);
			}
		}
	}
}

void CComponent_BasePipe::Render(void)
{

}

void CComponent_BasePipe::Shutdown(void)
{
	m_nActiveSpawns = 0;
	m_unMaxSpawns = 0;
	m_bActive = false;
	m_nPipeType = EPIPE_DEFAULT;
	m_pSoundCmp = nullptr;

	m_vChildren.clear();
	m_vNearestObjects.clear();

	CComponent_Collision::Shutdown();
}

void CComponent_BasePipe::AddChild(IBaseObject* _pChild)
{
	if(!_pChild)
		return;

	++m_nActiveSpawns;
	m_vChildren.push_back(_pChild);
	//_pChild->AddRef();
}

void CComponent_BasePipe::RemoveChild(IBaseObject* _pChild)
{
	if(!_pChild)
		return;

	vector<IBaseObject*>::iterator iter = m_vChildren.begin();
	for(;iter != m_vChildren.end(); iter++)
	{
		if(*iter == _pChild)
		{
			//CObjectManager::GetInstance()->AddObjectToRemove( _pChild );
			m_vChildren.erase(iter);
			--m_nActiveSpawns;
			break;
		}
	}
}

void CComponent_BasePipe::CheckDynamicCollisions(CComponent_Collision* _pCurCollision, CCollisionVolume* _pCurVolume)
{
	vec2f CP;
	vec2f Direction;
	switch( _pCurVolume->GetVolumeType() )
	{
	case VMT_AABB:
		{
			if( ((AABB*)_pCurVolume)->AABBToAABB( *((AABB*)m_pParent->GetCollidableObject()), Direction ) )
			{
				_pCurCollision->CleanOffset( Direction );
				_pCurCollision->CleanForce( Direction * 0.5f );
				// update the AABB
				((AABB*)_pCurCollision->GetParent()->GetCollidableObject())->SetWorldPoint( _pCurCollision->GetPos2DClean() );
				// set the player to grounded
				// if he is on top of us
				if( OBJ_PLAYER == _pCurCollision->GetParent()->GetType() )
				{
					if( dot_product( Direction, vec2f( 0.0f, 1.0f ) ) > 0.5f )
					{
						((CComponent_PlayerCollision*)_pCurCollision)->SetCollidingGroundAlone( true );
					}
				}
			}
			break;
		}
	case VMT_BOX:
		{
			CComponent_Item* ObjectsItemComponent = (CComponent_Item*)_pCurCollision->GetParent()->GetComponent( ECOMP_ITEM );
			if( ObjectsItemComponent )
			{
				bool bChild = false;
				for(unsigned int i = 0; i < m_vChildren.size(); i++)
				{
					if(m_vChildren[i] == _pCurCollision->GetParent())
					{
						bChild = true;
						break;
					}
				}

				if( ObjectsItemComponent->GetSpawnIgnoreTime() > 0.0f && bChild )
				{
					// If the object has an item component and the spawn ignore time has not yet reached its point, ignore collision.
					// This is a safety check to prevent boxes flying out of pipes.
					break;
				}
			}

			if( ((Box*)_pCurVolume)->BoxToAABB( *((AABB*)m_pParent->GetCollidableObject()), CP, Direction ) )
			{
				IBaseObject* colParent = _pCurCollision->GetParent();
				// get the item component of this object
				CComponent_Item* itemcomp = (CComponent_Item*)colParent->GetComponent(ECOMP_ITEM);

				// if its being pulled
				if( itemcomp && itemcomp->GetTarget() )
				{
					// stop the player from pulling
					itemcomp->SetTarget( NULL );
				}

				IBaseObject* collisionObj = _pCurCollision->GetParent();
				if(collisionObj->GetType() == OBJ_GOO)
				{
					if(((CComponent_LittleGooCollision*)(_pCurCollision))->GetVolitile() && collisionObj->GetAlive())
					{
						collisionObj->SetAlive(false);
						_pCurCollision->CleanRemoveVelocity();
					}
					break;
				}

				_pCurCollision->CleanOffset( Direction );
				_pCurCollision->CleanForce( Direction * 0.5f );
			}

			break;
		}
	case VMT_SPHERE:
		{
			break;
		}
	}
}