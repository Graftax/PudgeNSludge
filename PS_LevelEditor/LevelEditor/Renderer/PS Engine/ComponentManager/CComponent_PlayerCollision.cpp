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

CComponent_PlayerCollision::CComponent_PlayerCollision()
{
	m_bPrevInGoo = false;
	m_bInGoo = false;
	m_pSoundComponent = nullptr;
	m_fInitiateWallSlideTimer = 0.0f;
	_isWallSliding = false;
	m_pAnimationComponent = NULL;
	_CollidingWallRight = false;
	_CollidingWallLeft = false;
	m_WallJumpPushOutVector = vec3f(0.0f, 0.0f, 0.0f);
	m_InputMng = nullptr;
	m_pPlayerCollisionVolume = nullptr;
	m_pHealthComponent = nullptr;
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
	m_pSoundComponent = nullptr;
	m_fInitiateWallSlideTimer = 0.0f;
	_isWallSliding = false;
	m_pAnimationComponent = NULL;
	_CollidingWallRight = false;
	_CollidingWallLeft = false;
	m_WallJumpPushOutVector = vec3f(0.0f, 0.0f, 0.0f);
	m_InputMng = nullptr;
	m_pPlayerCollisionVolume = nullptr;
	m_pHealthComponent = nullptr;
	CComponent_Collision::Initialize();
	m_InputMng = CGame::GetInstance()->GetInputManager();
	m_bPrevInGoo = false;
	m_bInGoo = false;
	m_fTimeLeftToIgnorePlayer = 0.0f;

	this->CleanRemoveVelocity();

	return true;
}

void CComponent_PlayerCollision::Update( float fDT )
{
	_CollidingWallLeft = _CollidingWallRight = false;


	if( !m_pParent )
	{
		return;
	}

	if (m_pParent->GetCollidableObject()->GetVolumeType () != VMT_AABB)
	{
		return;
	}

	if(!m_pSoundComponent)
	{
		m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));
	}
	if(!m_pAnimationComponent)
	{
		m_pAnimationComponent = (CComponent_Animation*)(m_pParent->GetComponent(ECOMP_ANIMATION));
	}
	// Get the players health component
	m_pHealthComponent = (CComponent_Health*)m_pParent->GetComponent( ECOMP_HEALTH );
	// Health Component Safety Check
	if(m_pHealthComponent)
	{
		// Set the InGooPit bool to false
		// this is done every frame before checking
		// against goo pit collision
		m_pHealthComponent->SetInGooPit(false);
	}

	// Set the pointer to the player's collision volume
	m_pPlayerCollisionVolume = (AABB*)m_pParent->GetCollidableObject();
	
	if(m_pSoundComponent)
	{
		if(m_bInGoo != m_bPrevInGoo)
		{
			m_pSoundComponent->PlaySound(SFX_HAZARD_GOOPIT_SPLASH);
		}
	}

	m_bCollidingWall = false;
	m_bCollidingGroundLast = m_bCollidingGround;
	m_bCollidingGround = false;

	vec3f fAverage;

	CComponent_Input* pInputComp = (CComponent_Input*)m_pParent->GetComponent( ECOMP_INPUT );
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

	CComponent_Input* cmpInput = (CComponent_Input*)m_pParent->GetComponent (ECOMP_INPUT);
	CComponent_Collision* pulledCol = NULL;
	/*if ((CComponent_Collision*)cmpInput->GetPulledObject ())
	pulledCol = (CComponent_Collision*)cmpInput->GetPulledObject ()->GetComponent(ECOMP_COLLISION);*/

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

	if( CollidingGround() )
	{
		/*if(m_pAnimationComponent)
		{
			State* pudgeState = (State*)m_pAnimationComponent->GetAnimStateMachine()->GetCurrentState();
			if( ( pudgeState != PudgeRunToIdleState::GetState() ) && ( pudgeState != PudgeWallSlideState::GetState() )
				&& (pudgeState != PudgeIdleState::GetState() ) && (pudgeState != PudgeRunState::GetState() ))
			{

			}
		}*/
		}

	if( CollidingGround() || !CollidingWall() )
		_isWallSliding = false;


	CKdTree::m_pMainTree->debugLeafTest.clear();

	// Update the position after everything is done.
	m_pParent->SetLocalPos(&m_fPos);

	// cap the objects z axis position and velocity
	m_fPos.z = -300.0f;
	m_fVel.z = 0.0f;

	m_bPrevInGoo = m_bInGoo;
	m_bInGoo = false;
}

void CComponent_PlayerCollision::Shutdown(void)
{
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
	vec2f Direction = vec2f(0.0f, 0.0f);
	vec3f CP = vec3f(0.0f, 0.0f, 0.0f);

	D3DXMATRIX mat;
	D3DXMatrixIdentity (&mat);

	if( pCurCollision && pCurCollision != this )
	{
		switch( pCurVolume->GetVolumeType() )
		{
		case VMT_BOX:
			{	
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
				if( abs( result ) > (1.0f - FLT_EPSILON))
				{						
					if( !CollidingGround() )
						m_fInitiateWallSlideTimer += fDT;

					if( m_fInitiateWallSlideTimer > WALLSLIDETIMER )
					{
						if( m_pAnimationComponent )
						{
							if( m_pAnimationComponent->GetAnimStateMachine()->GetCurrentState() != PudgeWallSlideState::GetState() )
								m_pAnimationComponent->GetAnimStateMachine()->ChangeState( PudgeWallSlideState::GetState() );
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

						// If the dot product result is positive then its a right wall.
						if( result > 0.0f )
						{
							_CollidingWallRight = true;
						}
						// Otherwise its a left wall.
						else
						{
							_CollidingWallLeft = true;
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
}