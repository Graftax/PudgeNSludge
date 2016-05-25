//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#define IF(x) if(x)(x)
#define WALL_SLIDE_CAP -2.0f

#include "CComponent_PlayerCollision.h"
#include "CComponent_Health.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "..//PS/States/GamePlayState.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../InputLib/InputManager.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "../EffectsLib/ParticleManager.h"

CComponent_PlayerCollision::CComponent_PlayerCollision()
{
	m_bPrevInGoo = false;
	m_bInGoo = false;
	m_pSoundComponent = nullptr;
	m_fInitiateWallSlideTimer = 0.0f;
	_isWallSliding = false;
	m_pAnimationComponent = nullptr;
	_CollidingWallRight = false;
	_CollidingWallLeft = false;
	m_WallJumpPushOutVector = vec3f(0.0f, 0.0f, 0.0f);
	m_InputMng = nullptr;
	m_pPlayerCollisionVolume = nullptr;
	m_pHealthComponent = nullptr;
	m_pInputComp = nullptr;
	this->CleanRemoveVelocity();
}

CComponent_PlayerCollision::~CComponent_PlayerCollision(void)
{

}

bool CComponent_PlayerCollision::Initialize ()
{
	//DebugShapes::CreateSphere (10.0f, 50, 50);
	m_bPrevInGoo = false;
	m_bInGoo = false;
	m_fInitiateWallSlideTimer = 0.0f;
	_isWallSliding = false;
	_CollidingWallRight = false;
	_CollidingWallLeft = false;
	m_WallJumpPushOutVector = vec3f(0.0f, 0.0f, 0.0f);
	CComponent_Collision::Initialize();
	m_InputMng = CGame::GetInstance()->GetInputManager();
	m_bPrevInGoo = false;
	m_bInGoo = false;
	m_fTimeLeftToIgnorePlayer = 0.0f;

	this->CleanRemoveVelocity();

	m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));

	m_pAnimationComponent = (CComponent_Animation*)(m_pParent->GetComponent(ECOMP_ANIMATION));

	// Get the players health component
	m_pHealthComponent = (CComponent_Health*)m_pParent->GetComponent( ECOMP_HEALTH );

	m_pInputComp = (CComponent_Input*)m_pParent->GetComponent( ECOMP_INPUT );

	m_pWallSlideEffect = CParticleManager::GetInstance()->CreateEffect( "WallSmoke", vec3f( 0.0f, 0.0f, 0.0f ) );

	return true;
}

void CComponent_PlayerCollision::Update( float fDT )
{
	if( !m_pPlayerCollisionVolume )
	{
		// Set the pointer to the player's collision volume
		m_pPlayerCollisionVolume = (AABB*)m_pParent->GetCollidableObject();
	}
	_CollidingWallLeft = _CollidingWallRight = false;

	if( m_pParent->GetWorldPos().x > 5000 )
	{
		// Do stuff
		int x = 50;
	}

	if( !m_pParent )
	{
		return;
	}

	if (m_pParent->GetCollidableObject()->GetVolumeType () != VMT_AABB)
	{
		return;
	}


	m_fPos = m_pParent->GetFrame().GetLocalMat().axis_pos;

	m_bCollidingWall = false;
	m_bCollidingGroundLast = m_bCollidingGround;
	m_bCollidingGround = false;

	vec3f fAverage;

	m_fVel += vec3f( 0.0f, -40.0f, 0.0f ) * fDT;

	if( m_fVel.y < -30.0f )
		m_fVel.y = -30.0f;
	if( m_fVel.y > 20.0f )
		m_fVel.y = 20.0f;

	if( m_fVel.x < -100.0f )
		m_fVel.x = -100.0f;
	if( m_fVel.x >  100.0f )
		m_fVel.x =  100.0f;

	m_fPos += m_fVel;
	fAverage = m_fPos;

	m_pParent->SetLocalPos(&fAverage);

	vec3f NewCenter = m_pParent->GetWorldPos();
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (NewCenter);

	CKdTree::m_pMainTree->debugLeafTest.clear();

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = (1<<RFLAG_AABB);
	ReturnParams |= (1<<RFLAG_BOX);
	ReturnParams |= (1<<RFLAG_SPHERE);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects);

	// loop through all the return objects and check collision with them.
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		IBaseObject* curObj = (IBaseObject*)NearestObjects[objindex];
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();		

		// Safety Check
		if( curVolume )
		{
			// Is the object from the Kd Tree Static or Dynamic
			switch( curVolume->GetBodyType() )
			{
			case BT_STATIC:
				{
					CheckStaticCollisions( curCollision, curVolume, fDT );
					break;
				}
			case BT_DYNAMIC:
				{
					CheckDynamicCollisions( curCollision, curVolume, fDT );
					break;
				}
			default:
				break;
			};
		}

		// Update the player collision volume every time the player collides with something
		((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (m_fPos);
	}

	if( CollidingGround() || !CollidingWall() )
	{
		_isWallSliding = false;
		if(m_pSoundComponent)
		{
			m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_PUDGE_WALL_SLIDE_LOOP);
		}
	}

	CKdTree::m_pMainTree->debugLeafTest.clear();

	// Update the position after everything is done.
	m_pParent->SetLocalPos(&m_fPos);

	// cap the objects z axis position and velocity
	m_fPos.z = 0.0f;
	m_fVel.z = 0.0f;

	m_bPrevInGoo = m_bInGoo;
	m_bInGoo = false;

	if( m_pParent->GetWorldPos().x > 5000 )
	{
		// Do stuff
		int x = 50;
	}
}

void CComponent_PlayerCollision::Shutdown(void)
{

	m_bPrevInGoo = false;
	m_bInGoo = false;
	m_pSoundComponent = nullptr;
	m_fInitiateWallSlideTimer = 0.0f;
	_isWallSliding = false;
	m_pAnimationComponent = nullptr;
	_CollidingWallRight = false;
	_CollidingWallLeft = false;
	m_WallJumpPushOutVector = vec3f(0.0f, 0.0f, 0.0f);
	m_InputMng = nullptr;
	m_pPlayerCollisionVolume = nullptr;
	m_pHealthComponent = nullptr;
	m_pInputComp = nullptr;
	this->CleanRemoveVelocity();

	CParticleManager::GetInstance()->DestroyEffect( m_pWallSlideEffect );

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_PlayerCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
{
	if( m_pParent->GetWorldPos().x > 5000 )
	{
		// Do stuff
		int x = 50;
	}
	vec2f Direction = vec2f(0.0f, 0.0f);
	vec3f CP = vec3f(0.0f, 0.0f, 0.0f);

	D3DXMATRIX mat;
	D3DXMatrixIdentity (&mat);

	if( pCurCollision && pCurCollision != this )
	{
		switch( pCurVolume->GetVolumeType() )
		{
		//case VMT_AABB:
		//	{
		//		if(pCurCollision->GetParent()->GetType() == OBJ_PIT)
		//		{
		//			CComponent_Sound* pPitSound = (CComponent_Sound*)pCurCollision->GetParent()->GetComponent(ECOMP_SOUND);
		//			if(!pPitSound->GetClosestGooPit())
		//			{
		//				pPitSound->SetClosestGooPit(m_pParent);
		//			}
		//			else
		//			{
		//				vec3f lineToGooPit = pPitSound->GetAudio()->GetListenerPosition() - m_pParent->GetWorldPos();
		//				vec3f lineToClosestGooPit = pPitSound->GetAudio()->GetListenerPosition() - pPitSound->GetClosestGooPit()->GetWorldPos();
		//				float distToGoo = dot_product(lineToGooPit, lineToGooPit);
		//				float distToClosestGoo = dot_product(lineToClosestGooPit, lineToClosestGooPit);
		//
		//				if(distToGoo < distToClosestGoo)
		//				{
 		//					pPitSound->SetClosestGooPit(pCurCollision->GetParent());
		//
		//					char message[128];
		//					sprintf(message, "Closest Goo Pit X: %f		Y: %f", pPitSound->GetClosestGooPit()->GetWorldPos().x, pPitSound->GetClosestGooPit()->GetWorldPos().y);
		//					LOG(LOG_DEBUG, "CComponent_GooPitCollision::DynamicCollision()", message);
		//				}
		//			}
		//		}
		//	}
		//	break;
		case VMT_BOX:
			{	
				//check the item component target to see if the box is being pulled.
				CComponent_Item* boxItemCmp = (CComponent_Item*)pCurCollision->GetParent()->GetComponent(ECOMP_ITEM);
				if(boxItemCmp)
				{
					if(boxItemCmp->GetTarget() == m_pParent || boxItemCmp->GetAttached())
					{
						//we are pulling the box so don't check collision with it
						return;
					}
				}

				switch( pCurCollision->GetParent()->GetType() )
				{
				case OBJ_GOO:
					{
						return;
					}
				case OBJ_PLUG:
					{
						if( ((CComponent_PlugCollision*)pCurCollision)->GetPlugged() )
						{
							return;
						}
						break;
					}
				case OBJ_CORK:
					{
						if( ((CComponent_CorkCollision*)pCurCollision)->GetCorked() )
						{
							return;
						}
						break;
					}
				};

				// make sure there is a collision component and that it is not us, also make sure that the object we 
				// are checking against is a box.
				// get a pointer to that box
				Box* crate = (Box*)( pCurCollision->GetParent()->GetCollidableObject() );

				CComponent_Item* pCurItem = (CComponent_Item*)pCurCollision->GetParent()->GetComponent( ECOMP_ITEM );
				if( pCurItem )
				{
					if( pCurItem->GetTarget() )
						return;
				}

				// check the players aabb against the crate or cork
				if (crate->BoxToAABB ( *m_pPlayerCollisionVolume, vec2f( CP.x, CP.y ) , Direction))
				{
					m_fLastOffset = vec3f( Direction.x, Direction.y, 0.0f );
					vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f).normalize();

					float fTimePercentage = 1.0f - ( pCurCollision->GetTimeLeft() / 0.5f );
					if( fTimePercentage > 1.0f ) fTimePercentage = 1.0f;
					if( fTimePercentage < 0.0f ) fTimePercentage = 0.0f;

					float fDirection = dot_product( MoveDirection, vec3f( 0.0f, -1.0f, 0.0f ) );
					if( fDirection > 0.5f )
					{
						CleanOffset( vec2f( Direction.x, Direction.y ) * -1.0f * fTimePercentage );
						if( !( pCurCollision->GetTimeLeft() > 0.0f ) )
							CleanForce( vec2f( Direction.x, Direction.y ) * -1.0f );
						if( pCurCollision->CollidingGround() )
							m_bCollidingGround = true;
					}
					else if( fDirection < -0.5f )
					{
						float fSided = pCurCollision->GetPos2DClean().x - GetPos2DClean().x;
						// Normalize the float.
						if( fSided > 0.0f )
							fSided = 1.0f;
						else
							fSided = -1.0f;

						pCurCollision->CleanOffset( vec2f( Direction.x + fSided * 4.0f, Direction.y ) * 0.75f * fTimePercentage );
						pCurCollision->CleanForce( vec2f( Direction.x + fSided * 2.0f, Direction.y ) * 0.25f * fTimePercentage );
					}
					else
					{
						pCurCollision->CleanOffset( vec2f( Direction.x, Direction.y ) * 0.5f * fTimePercentage );
						pCurCollision->CleanForce( vec2f( Direction.x, Direction.y ) * 0.5f * fTimePercentage );
						CleanOffset( vec2f( Direction.x, Direction.y ) * -0.5f * fTimePercentage );
						CleanForce( vec2f( Direction.x, Direction.y ) * -0.5f * fTimePercentage );
					}
				}
				break;
			}
		case VMT_SPHERE:
			{
				break;
			}
		default:
			break;
		};

	}
	if( m_pParent->GetWorldPos().x > 5000 )
	{
		// Do stuff
		int x = 50;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckStaticCollisions()
// Purpose: Check collisions with all static objects
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_PlayerCollision::CheckStaticCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
{
	if( m_pParent->GetWorldPos().x > 5000 )
	{
		// Do stuff
		int x = 50;
	}
	vec2f Direction;
	vec3f CP;
	switch( pCurVolume->GetVolumeType() )
	{
	case VMT_AABB:
		{
			//TODO
			// There's two different AABtoAABB checks here
			// what the hell are we doing?
			if( m_pPlayerCollisionVolume->AABBToAABB( (*(AABB*)pCurVolume), CP, Direction ) )
			{
				CP.z = m_fPos.z;
				vec3f ObjectPosToCollidedPos = CP - m_pParent->GetWorldPos();
				ObjectPosToCollidedPos.normalize();

				// Prepare a movement vector.
				vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);

				// Create a normal of that movement.
				vec3f pushDirection = MoveDirection;
				pushDirection.normalize();

				// Move them by that vector.
				//m_fPos += MoveDirection;
				vec3f _WorldRight = vec3f( 1.0f, 0.0f, 0.0f );

				//// Check left and right for wall collision.
				float result = dot_product( ObjectPosToCollidedPos, _WorldRight );
				if( abs( result ) > (0.4f))
				{		
					if( !GetInGoo() )
					{
						if( !CollidingGround() )
							m_fInitiateWallSlideTimer += fDT;

						if( m_fInitiateWallSlideTimer >= WALLSLIDETIMER )
						{
							if( !CollidingGround() )
							{
								if( m_pAnimationComponent )
								{
									m_pAnimationComponent->GetAnimStateMachine()->ChangeState( PudgeWallSlideState::GetState() );
								}

								if(m_pSoundComponent)
								{
									if(!_isWallSliding)
									{
										m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_PUDGE_WALL_GRAB);
									}
									m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_PUDGE_WALL_SLIDE_LOOP);
								}
							}

							// Set the vector to push out later.
							m_WallJumpPushOutVector = ObjectPosToCollidedPos;
							// Negate to push in opposite direction.
							m_WallJumpPushOutVector.negate();
							_isWallSliding = true;
							m_bCollidingWall = true;
							// Cap the Wall side gravity to slow player down.
							if( m_fVel.y < WALL_SLIDE_CAP )
								m_fVel.y = WALL_SLIDE_CAP;

							m_pWallSlideEffect->m_fPosition = m_pParent->GetWorldPos();

							// If the dot product result is positive then its a right wall.
							if( result > 0.0f )
							{
								_CollidingWallRight = true;
								m_pWallSlideEffect->m_fDirection = vec3f( -1.0f, 0.0f, 0.0f );
								m_pWallSlideEffect->m_fPosition += vec3f( 30.0f, 0.0f, 0.0f );
							}
							// Otherwise its a left wall.
							else
							{
								_CollidingWallLeft = true;
								m_pWallSlideEffect->m_fDirection = vec3f( 1.0f, 0.0f, 0.0f );
								m_pWallSlideEffect->m_fPosition += vec3f( -30.0f, 0.0f, 0.0f );
							}


							m_pWallSlideEffect->Emit( fDT );
						}	
					}
				}

				if( m_pPlayerCollisionVolume->AABBToAABB( (*(AABB*)pCurVolume), Direction) )
				{
					// Prepare a movement vector.
					vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);

					vec3f pushDirection = MoveDirection;
					pushDirection.normalize();

					// Move them by that vector.
					m_fPos += MoveDirection;

					// Check the last movement to see if its the cieling.
					if( dot_product( pushDirection, vec3f( 0.0f, -1.0f, 0.0f ) ) > 0.5f )
					{
						m_fVel.y = 0.0f;
					}
					// see if it is the the ground.
					if( dot_product( pushDirection, vec3f( 0.0f, 1.0f, 0.0f ) ) > 0.5f )
					{
						m_fVel.y = 0.0f;

						// Ground dust effect.
						if( m_bCollidingGroundLast == false )
							CParticleManager::GetInstance()->CreateEffectOneShot( "LandingSmoke", GetParent()->GetWorldPos() );

						m_bCollidingGround = true;
						
					}
					if( abs(dot_product( pushDirection, vec3f( 1.0f, 0.0f, 0.0f ) ) ) > 0.5f )
					{
						m_fVel.x = 0.0f;
					}
				}
			}

			break;
		}
	case VMT_BOX:
		{
			// TODO
			// handle player collision
			// with world boxes

			break;
		}
	case VMT_SPHERE:
		{
			// TODO
			// handle player collision
			// with world spheres

			break;
		}
	default:
		break;
	};
	if( m_pParent->GetWorldPos().x > 5000 )
	{
		// Do stuff
		int x = 50;
	}
}