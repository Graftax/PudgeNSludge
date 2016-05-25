/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CComponent_BridgeCollision
//
// Purpose: Handle all collisions for the bridge object
//
// Original Author: Rueben Massey
//
// Creation Date: 6/4/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CComponent_BridgeCollision.h"
#include "Component_Emitter.h"
#include "CComponent_Animation.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../EffectsLib/ParticleManager.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../AnimationLib/AnimationIncludes.h"

CComponent_BridgeCollision::CComponent_BridgeCollision()
{
	m_bIsActivated = false;
	m_nChannel = -1;

	m_pSoundComponent = nullptr;
	m_pCollisionObject = nullptr;
	m_pEmitterComponent = nullptr;
	m_fDropTimer = 0.0f;
	m_pCollisionObject = nullptr;
	m_pAnimationComponent = nullptr;
	this->CleanRemoveVelocity();

	m_bPlayed = false;
}

CComponent_BridgeCollision::~CComponent_BridgeCollision()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Initialize()
// Purpose: Initialize all data members
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_BridgeCollision::Initialize( )
{
	m_bIsActivated = false;
	m_nChannel = -1;
	m_fDropTimer = 0.0f;
	m_nRotationQuadrant = 1;
	m_bRotateClockwise = false;

	this->CleanRemoveVelocity();

	CComponent_Collision::Initialize();

	m_fDropTimer = 0.0f;
	m_bPlayed = false;

	m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));


	m_pEmitterComponent = (CComponent_Emitter*)m_pParent->GetComponent( ECOMP_EMITTER );

	// Set the collision object 
	// to the parent's collision object 
	m_pCollisionObject = (AABB*)m_pParent->GetCollidableObject();

	m_pAnimationComponent = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Update()
// Purpose: Update all data members
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: RCM
// Last Modification Date: 6/9
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BridgeCollision::Update( float fDT )
{		
	if( m_bIsActivated )
	{
		if(!m_bPlayed && m_pSoundComponent)
		{
			m_bPlayed = true;
			m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_MOVING_LOOP);
		}

		m_fDropTimer += fDT;
		if( m_fDropTimer < 2.0f )
		{
			// drop the bridge
			DropBridgeAABB( fDT );
			// change animation state
#if _ANIMATIONS
			m_pAnimationComponent->GetAnimStateMachine()->ChangeState( BridgeHalfRetractState::GetState() );
#endif
		}
		else
		{
			if(!m_bPlayed && m_pSoundComponent)
			{
				m_bPlayed = true;
				m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_HAZARD_BRIDGE_MOVING_LOOP);
			}

			m_bIsActivated = false;
		}
	}
	vec3f NewCenter = m_pParent->GetWorldMat()->axis_pos;
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint( NewCenter );

	CheckCollisions( fDT );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Shutdown()
// Purpose: clean up memory
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BridgeCollision::Shutdown(void)
{
	m_bIsActivated = false;
	m_nChannel = -1;
	m_pSoundComponent = nullptr;
	m_pCollisionObject = nullptr;
	m_pEmitterComponent = nullptr;
	m_fDropTimer = 0.0f;
	m_pAnimationComponent = nullptr;
	m_nRotationQuadrant = -1;
	m_bRotateClockwise = true;

	m_bPlayed = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckCollisions()
// Purpose: Check for collision	and react appropriately
// Original Author: Rueben Massey
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BridgeCollision::CheckCollisions( float fDT )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_MONSTER) | (1<<OBJ_GOO);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects);

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
// Function Name: DropBridgeAABB()
// Purpose: Remove the AABB so the player can pass
// Original Author: Rueben Massey
// Creation Date: 6/9/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BridgeCollision::DropBridgeAABB( float fDT )
{
	// lower the bridge
	// Based on the direction	
	if( m_bRotateClockwise )
	{
		switch( m_nRotationQuadrant )
		{
		case 0:
			{
				m_pCollisionObject->SetMinPoint( vec2f( m_pCollisionObject->GetMinPoint2D().x, m_pCollisionObject->GetMinPoint2D().y + 61.0f * fDT ) );
				m_pCollisionObject->SetMaxPoint( vec2f( m_pCollisionObject->GetMaxPoint2D().x + 122.0f * fDT , m_pCollisionObject->GetMaxPoint2D().y ) );
				break;
			}
		case 1:
			{
				m_pCollisionObject->SetMinPoint( vec2f( m_pCollisionObject->GetMinPoint2D().x + 61.0f * fDT, m_pCollisionObject->GetMinPoint2D().y - 122.0f * fDT ) );
				m_pCollisionObject->SetMaxPoint( vec2f( m_pCollisionObject->GetMaxPoint2D().x, m_pCollisionObject->GetMaxPoint2D().y ) );
				break;
			}
		case 2:
			{
				m_pCollisionObject->SetMinPoint( vec2f( m_pCollisionObject->GetMinPoint2D().x - 122.0f * fDT, m_pCollisionObject->GetMinPoint2D().y ) );
				m_pCollisionObject->SetMaxPoint( vec2f( m_pCollisionObject->GetMaxPoint2D().x, m_pCollisionObject->GetMaxPoint2D().y - 61.0f * fDT ) );
				break;
			}
		case 3:
			{
				m_pCollisionObject->SetMinPoint( vec2f( m_pCollisionObject->GetMinPoint2D().x, m_pCollisionObject->GetMinPoint2D().y ) );
				m_pCollisionObject->SetMaxPoint( vec2f( m_pCollisionObject->GetMaxPoint2D().x - 61.0f * fDT, m_pCollisionObject->GetMaxPoint2D().y + 122.0f * fDT ) );
				break;
			}
		default:
			break;
		};
	}
	else
	{
		switch( m_nRotationQuadrant )
		{
		case 0:
			{
				m_pCollisionObject->SetMinPoint( vec2f( m_pCollisionObject->GetMinPoint2D().x, m_pCollisionObject->GetMinPoint2D().y ) );
				m_pCollisionObject->SetMaxPoint( vec2f( m_pCollisionObject->GetMaxPoint2D().x - 122.0f * fDT, m_pCollisionObject->GetMaxPoint2D().y + 61.0f * fDT ) );
				break;
			}
		case 1:
			{
				m_pCollisionObject->SetMinPoint( vec2f( m_pCollisionObject->GetMinPoint2D().x - 61.0f * fDT, m_pCollisionObject->GetMinPoint2D().y ) );
				m_pCollisionObject->SetMaxPoint( vec2f( m_pCollisionObject->GetMaxPoint2D().x, m_pCollisionObject->GetMaxPoint2D().y - 122.0f * fDT ) );
				break;
			}
		case 2:
			{
				m_pCollisionObject->SetMinPoint( vec2f( m_pCollisionObject->GetMinPoint2D().x - 122.0f * fDT, m_pCollisionObject->GetMinPoint2D().y - 61.0f * fDT ) );
				m_pCollisionObject->SetMaxPoint( vec2f( m_pCollisionObject->GetMaxPoint2D().x, m_pCollisionObject->GetMaxPoint2D().y ) );
				break;
			}
		case 3:
			{
				m_pCollisionObject->SetMinPoint( vec2f( m_pCollisionObject->GetMinPoint2D().x, m_pCollisionObject->GetMinPoint2D().y - 122.0f * fDT ) );
				m_pCollisionObject->SetMaxPoint( vec2f( m_pCollisionObject->GetMaxPoint2D().x + 61.0f * fDT, m_pCollisionObject->GetMaxPoint2D().y ) );
				break;
			}
		default:
			break;
		};
	}

	matrix4f* _locationMat = m_pParent->GetLocalMat();

	_locationMat->rotate_x_pre( D3DXToRadian( 45 * fDT ) );
	// Dirrty the Frame
	m_pParent->GetFrame().Update();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Rueben Massey
// Creation Date: 6/11/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BridgeCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
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
				IBaseObject* colParent = pCurCollision->GetParent();
				if ( colParent->GetType() == OBJ_GOO )
				{
					if(colParent->GetAlive() && ((CComponent_LittleGooCollision*)pCurCollision)->GetVolitile())
					{
						colParent->SetAlive(false);

						CComponent_Sound* pSound = (CComponent_Sound*)colParent->GetComponent(ECOMP_SOUND);

						if(pSound)
						{
							pSound->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_DEATH);
							pSound->PlaySound(AK::EVENTS::PLAY_DLG_LILGOO_DEATH);
						}

						pCurCollision->CleanRemoveVelocity();
					}
					break;
				}

				// get the item component of this object
				CComponent_Item* itemcomp = (CComponent_Item*)colParent->GetComponent(ECOMP_ITEM);

				// if its being pulled
				if( itemcomp && itemcomp->GetTarget() )
				{
					// stop the player from pulling
					itemcomp->SetTarget( NULL );
				}

				// Apply push-out
				pCurCollision->CleanOffset( Direction );
				pCurCollision->CleanForce( Direction * 0.5f );
			}
			break;
		}
	default:
		break;
	};
}