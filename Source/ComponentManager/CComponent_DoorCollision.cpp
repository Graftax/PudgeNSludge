/////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CComponent_DoorCollision
//
// Purpose: Handle all collisions for the bridge object
//
// Original Author: Rueben Massey
//
// Creation Date: 6/15/2012
//
// Last Modification By: 
// Last Modification Date: 
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CComponent_DoorCollision.h"
#include "Component_Emitter.h"
#include "CComponent_Animation.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../EffectsLib/ParticleManager.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../AnimationLib/AnimationIncludes.h"

CComponent_DoorCollision::CComponent_DoorCollision()
{
	m_pSoundComponent = nullptr;
	m_pCollisionObject = nullptr;
	m_pEmitterComponent = nullptr;
	m_pCollisionObject = nullptr;
	m_pAnimationComponent = nullptr;
	m_bButtonPress = false;
	m_bPrevPress = false;
	this->CleanRemoveVelocity();
	m_bOpening = false;

	m_pAnimationComponent = NULL;
	m_bHasAssociatedButton = false;
}

CComponent_DoorCollision::~CComponent_DoorCollision()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Initialize()
// Purpose: Initialize all data members
// Original Author: Rueben Massey
// Creation Date: 6/15/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_DoorCollision::Initialize( )
{
	CComponent_Collision::Initialize();
	
	m_bIsOpen = false;
	m_bButtonPress = false;
	m_bPrevPress = false;
	m_bOpening = false;
	this->CleanRemoveVelocity();

	m_pAnimationComponent = NULL;
	m_bHasAssociatedButton = false;

	// Create the AABB to trigger the door to open
	m_DoorOpenAABB.SetMaxPoint( vec2f( 150.0f, 122.0f ) );
	m_DoorOpenAABB.SetMinPoint( vec2f( -150.0f, -122.0f ) );
	m_DoorOpenAABB.SetBodyType( BT_DYNAMIC );
	m_DoorOpenAABB.SetVolumeType( VMT_AABB );

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
// Creation Date: 6/15/2012
// Last Modification By: JM
// Last Modification Date: 7/8
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_DoorCollision::Update( float fDT )
{
	vec3f NewCenter = m_pParent->GetWorldMat()->axis_pos;
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint( NewCenter );
	m_DoorOpenAABB.SetWorldPoint( NewCenter );

	if(m_bButtonPress && !m_bPrevPress)
	{
		m_bOpening = true;

		if( m_pAnimationComponent )
		{
			m_pAnimationComponent->SetAnimationBackwards(false);
		}

		if(m_pSoundComponent)
		{
			m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_DOOR_OPEN);
		}
	}
	else if(!m_bButtonPress && m_bPrevPress)
	{
		if( m_bOpening || m_bIsOpen )
		{
			m_bOpening = false;
			m_bIsOpen = false;

			if( m_pAnimationComponent )
			{
				m_pAnimationComponent->SetAnimationBackwards(true);
			}

			if(m_pSoundComponent)
			{
				m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_DOOR_OPEN);
			}
		}
	}

	if ( m_bOpening && !m_bIsOpen )
	{
#if _ANIMATIONS
		m_pAnimationComponent->GetAnimStateMachine()->ChangeState( DoorOpenState::GetState() );
		// Kill the AABB on the door
		if( m_pAnimationComponent->GetInterpolator().IsAnimationDone() )
		{
			m_bIsOpen = true;
			m_bOpening = false;
		}
#else
		{
			m_bIsOpen = true;
			m_bOpening = false;
		}
#endif
	}

	if( !m_bIsOpen )
	{
		CheckCollisions( fDT );
	}

	m_bPrevPress = m_bButtonPress;

	if( !m_bHasAssociatedButton )
		CheckOpenCollision( fDT );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: Shutdown()
// Purpose: clean up memory
// Original Author: Rueben Massey
// Creation Date: 6/15/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_DoorCollision::Shutdown(void)
{
	m_pSoundComponent = nullptr;
	m_pCollisionObject = nullptr;
	m_pEmitterComponent = nullptr;
	m_pCollisionObject = nullptr;
	m_bIsOpen = false;
	this->CleanRemoveVelocity();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckCollisions()
// Purpose: Check for collision	and react appropriately
// Original Author: Rueben Massey
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_DoorCollision::CheckCollisions( float fDT )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_PLAYER) | (1<<OBJ_GOO);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects );

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
// Function Name: CheckOpenCollision()
// Purpose: Check for collision	with the player and open the door
// Original Author: Rueben Massey
// Creation Date: 6/15/2012
// Last Modification By: JM
// Last Modification Date: 7/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_DoorCollision::CheckOpenCollision( float fDT )
{
	// Variables to catch returns
	vec2f Direction;

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER);

	CSceneObject _door;
	_door.SetCollidableObject( &m_DoorOpenAABB );

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( &_door, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects );

	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();
		// Safety Check
		if( curVolume && curCollision && curCollision != this )
		{
			IBaseObject* player = (IBaseObject*)NearestObjects[objindex];
			// Half Space check between Players Position and the Doors Position.
			vec3f DoorToPlayer = player->GetFrame().GetWorldMat().axis_pos - m_pParent->GetFrame().GetWorldMat().axis_pos;
			// Grab the result of the dot product in the direction of the worlds left. 
			float Distance = dot_product( vec3f( -1.0f, 0.0f, 0.0f), DoorToPlayer);
			// If the distance is negative then it failed the half space test.
			if( Distance < -50.0f )
			{
				m_bButtonPress = false;
				return;
			}

			if( ((AABB*)curVolume)->AABBToAABB( m_DoorOpenAABB, Direction ) )
			{
				m_bButtonPress = true;
			}
			else if( m_bOpening || m_bIsOpen )
			{
				m_bButtonPress = false;
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
void CComponent_DoorCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
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
			IBaseObject* colParent = pCurCollision->GetParent();
			// get the item component of this object
			CComponent_Item* itemcomp = (CComponent_Item*)colParent->GetComponent(ECOMP_ITEM);	

			// Use the current volume to check against this collision volume
			// This returns the proper direction to push out the current object
			if( ((Box*)pCurVolume)->BoxToAABB( *m_pCollisionObject, CP,Direction ) )
			{
				// if its being pulled
				if( itemcomp && itemcomp->GetTarget() )
				{
					// stop the player from pulling
					itemcomp->SetTarget( NULL );
					//pCurCollision->CleanRemoveVelocity();
				}
				// special case for Lil Goos
				if(pCurCollision->GetParent()->GetType() == OBJ_GOO)
				{
					CComponent_LittleGooCollision* pLilGoo = (CComponent_LittleGooCollision*)pCurCollision;
					CComponent_Sound* pSound = (CComponent_Sound*)pCurCollision->GetParent()->GetComponent(ECOMP_SOUND);

					if(pLilGoo->GetVolitile() && pLilGoo->GetParent()->GetAlive())
					{
						pLilGoo->GetParent()->SetAlive(false);
						pCurCollision->CleanRemoveVelocity();
					}
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