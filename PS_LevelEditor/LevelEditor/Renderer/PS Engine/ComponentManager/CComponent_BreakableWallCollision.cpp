//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_BreakableWallCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../Utilities/KdTree/KdTree.h"
#include "CComponent_Sound.h"

CComponent_BreakableWallCollision::CComponent_BreakableWallCollision()
{
	IsBroken = false;
	m_pSoundComponent = nullptr;
	m_pCollisionObject = nullptr;

	this->CleanRemoveVelocity();

}

CComponent_BreakableWallCollision::~CComponent_BreakableWallCollision(void)
{

}

bool CComponent_BreakableWallCollision::Initialize ()
{
	//DebugShapes::CreateSphere (10.0f, 50, 50);
	IsBroken = false;
	m_pSoundComponent = nullptr;
	m_pCollisionObject = nullptr;
	this->CleanRemoveVelocity();
	CComponent_Collision::Initialize();
	return true;
}

void CComponent_BreakableWallCollision::Update( float fDT )
{
	if( m_pParent )
	{
		if(!m_pSoundComponent)
		{
			m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));
		}

		// Set the collision object 
		// to the parent's collision object 
		if( !m_pCollisionObject )
		{
			m_pCollisionObject = (AABB*)m_pParent->GetCollidableObject();
		}
	}
	vec3f NewCenter = m_pParent->GetWorldPos();
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint(NewCenter);

	CheckCollisions( fDT );

	
}

void CComponent_BreakableWallCollision::Shutdown(void)
{
	IsBroken = false;
	m_pSoundComponent = nullptr;
	m_pCollisionObject = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckCollisions()
// Purpose: Check for collision	and react appropriately
// Original Author: Rueben Massey
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BreakableWallCollision::CheckCollisions( float fDT )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = (1<<RFLAG_AABB);
	ReturnParams |= (1<<RFLAG_BOX);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects);

	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();
		// Safety Check
		if( curVolume && curCollision && curCollision != this )
		{
			// Check if the body type is dynamic
			// meaning it's a moving object in the game
			if( BT_DYNAMIC == curVolume->GetBodyType() )
			{
				CheckDynamicCollisions( curCollision, curVolume, fDT );
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Rueben Massey
// Creation Date: 6/11/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BreakableWallCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
{
	// Variables to catch returns
	vec2f Direction;
	vec2f CP;

	// Check collisions
	// based on the current objects collision volume
	switch( pCurVolume->GetVolumeType() )
	{
	case VMT_AABB:
		{
			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( ((AABB*)pCurVolume)->AABBToAABB( *m_pCollisionObject, Direction ) )
			{
				// Apply push-out
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
			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( ((Box*)pCurVolume)->BoxToAABB( *m_pCollisionObject, CP,Direction ) )
			{
				switch( pCurCollision->GetParent()->GetType() )
				{
				case OBJ_CRATE:
					{
						// This is a crate so lets see if we can break the wall.
						float temp = dot_product( pCurCollision->GetVel(), pCurCollision->GetVel() );
						if( temp > 50.0f )
						{
							vec2f Direction;
							vec2f CP;							
							m_pSoundComponent->PlaySound( SFX_HAZARD_BREAKABLE_WALL );
							CObjectManager::GetInstance()->AddObjectToRemove( m_pParent );
							return;
						}
					}
				default:
					{
						// Apply push-out
						pCurCollision->CleanOffset( Direction );
						pCurCollision->CleanForce( Direction * 0.5f );
					}
				}
			}
			break;
		}
	default:
		break;
	};
}