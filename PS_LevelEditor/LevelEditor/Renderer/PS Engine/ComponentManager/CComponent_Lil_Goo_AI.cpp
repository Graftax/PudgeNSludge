//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CComponent_Lil_Goo_AI.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds the information for the little goo AI
//////////////////////////////////////////////////////////////////////////////////////
#include "CComponent_Lil_Goo_AI.h"
#include "../Utilities/KdTree/KdTree.h"
#include "CComponent_Health.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../PS/CGame.h"
#include "../AnimationLib/AnimationIncludes.h"
//#include "Component_Emitter.h"

#define ATTACK_JUMP 300.0f
#define SUCIDE_JUMP 400.0f
#define PATROL_JUMP 300.0f
#define MOVEMENT_SPEED 125.0f
#define ATTACK_SPEED 140.0f
#define SUCIDE_SPEED 500.0f
#define ATTACK_RADIUS 160.0f
#define AGGRO_RADIUS 350.0f
#define ATTACK_TIME 0.5f
#define PATROL_TIME 0.75f
#define SUCIDE_WAIT 1.0f
#define Y_CHECK (GAME_UNIT + 61)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComponent_Lil_Goo_AI():	Default Constructor
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CComponent_Lil_Goo_AI::CComponent_Lil_Goo_AI ()
{
	m_pCollisionCmp = nullptr;
	m_bPreviouslyCollided = false;
	m_fJumpTimer = 0.0f;
	m_pSoundCmp = nullptr;
	m_bDead = false;
	m_bMoveDirection = true;
	m_nPrevDirection = 0;
	m_fJumpTimer = 0.0f;
	m_fSucideTimer = 0.0f;
	m_bInGoo = false;
	m_bJumpAttack = false;
	m_bAttacking = false;
	m_bSucideing = false;
	m_fPreviousDownVel = 0.0f;
	m_pAnimComponent = nullptr;
	m_bInRange = false;
	m_bPrevOnGround = false;
	m_bCurrentlyCollided = false;
	m_plilGooTrailEffect = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComponent_Lil_Goo_AI():	Default Destructor
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CComponent_Lil_Goo_AI::~CComponent_Lil_Goo_AI ()
{
	Shutdown();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize():	Inits all the information for the little goo AI
//
// Returns:		bool
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_Lil_Goo_AI::Initialize ()
{
	CComponent_AI::Initialize ();
	m_aiType = AI_LIL_GOO;
	m_spAggro.SetRadius(AGGRO_RADIUS);
	m_spAttack.SetRadius(ATTACK_RADIUS);
	m_bMoveDirection = false;
	m_bPreviouslyCollided = false;
	m_nPrevDirection = 0;
	m_fJumpTimer = 0.0f;
	m_fSucideTimer = 0.0f;
	m_bInGoo = false;
	m_bJumpAttack = false;
	m_bAttacking = false;
	m_bSucideing = false;
	m_fPreviousDownVel = 0.0f;
	m_pSoundCmp = nullptr;
	m_pAnimComponent = nullptr;
	m_bDead = false;
	m_bInRange = false;
	m_bPrevOnGround = false;
	m_bCurrentlyCollided = false;
	m_plilGooTrailEffect = NULL;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown():	Shutdowns the little goo AI
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Lil_Goo_AI::Shutdown ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	Updates the little goo AI
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Lil_Goo_AI::Update ( float _fElapsedTime )
{
	if(!m_pCollisionCmp)
	{
		m_pCollisionCmp = (CComponent_LittleGooCollision*)m_pParent->GetComponent( ECOMP_COLLISION ); 
	}

#if _ANIMATIONS
	if( !m_pAnimComponent )
	{
		m_pAnimComponent = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );
	}
#endif

	if( OnGround() || m_bDead)
	{
		if( m_plilGooTrailEffect )
		{
			((CComponent_Emitter*)m_plilGooTrailEffect->GetComponent(ECOMP_EMITTER))->ActivateEffectTimer();
			m_plilGooTrailEffect = NULL;
		}
	}
	else if( m_plilGooTrailEffect )
		m_plilGooTrailEffect->SetLocalMat( m_pParent->GetLocalMat() );

	if ( m_bDead )
	{
#if _ANIMATIONS
		if( m_pAnimComponent )
		{
			m_pAnimComponent->GetAnimStateMachine()->ChangeState( LilGooDeathState::GetState() );
		}
#else
			CObjectManager::GetInstance()->CreateObject( OBJ_GOO_PARTICLE_EFFECT, GetParent()->GetPositionMatrix().ma );
			CObjectManager::GetInstance()->AddObjectToRemove (m_pParent);
#endif
		return;
	}

	if( !m_bActive )
		return;

	if(!m_pSoundCmp)
	{
		m_pSoundCmp = (CComponent_Sound*)m_pParent->GetComponent(ECOMP_SOUND);
	}

	// get the elapsed time
	m_fElapsedTime = _fElapsedTime;

	if((m_pCollisionCmp->CollidingGround() && !m_bPrevOnGround) || (m_bInGoo && !m_bPrevOnGround))
	{
		m_pCollisionCmp->GetVel().x = 0.0f;
		m_fJumpTimer = 0.0f;
		if(m_pSoundCmp && m_bInRange)
		{
			m_pSoundCmp->PlaySound(SFX_ENEMY_LILGOO_LAND);
		}
	}

	//if(m_pCollisionCmp->CollidingWall() && !m_bPreviouslyCollided)
	//{
	//	m_bMoveDirection = !m_bMoveDirection;
	//	m_pCollisionCmp->GetVel().x = -m_pCollisionCmp->GetVel().x;
	//}

	if(m_pCollisionCmp->CollidingGround())// || m_bInGoo)
	{
		m_fJumpTimer += m_fElapsedTime;
	}

	if ( m_bAttacking )
	{
		m_fSucideTimer += _fElapsedTime;
	}

	// move the attack and aggro spheres with the little goo
	m_spAggro.SetCenter ( m_pParent->GetWorldPos() );
	m_spAttack.SetCenter ( m_pParent->GetWorldPos() );

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> ReturnVector;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnPrams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_GOO) | (1<<OBJ_WALL_BREAKABLE_VER) | (1<<OBJ_BRIDGE) | (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_PIPE_SPAWN) | (1<<OBJ_PIPE_STEAM) | (1<<OBJ_PIPE_INTAKE) | (1<<OBJ_PIT);

	// call the kd tree and get the near objects
	CSceneObject AggroObject;
	AggroObject.SetCollidableObject (&m_spAggro);

	CKdTree::GetNearObjects (&AggroObject, PSFLAG_SPHERE, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);
	m_vCurNearObjects = ReturnVector;

	// loop through all the return objects and check collision with them.
	for (unsigned int ReturnIndex = 0; ReturnIndex < ReturnVector.size(); ReturnIndex++)
	{
		// get the base object pointer
		IBaseObject* _pObject = ((IBaseObject*)ReturnVector[ReturnIndex]);

		// check if the base object is the player
		if (_pObject->GetType () == OBJ_PLAYER)
		{
			// check if the player is with in the aggro sphere
			vec2f Useless;
			if ( m_spAggro.SphereToAABB (*((AABB*)_pObject->GetCollidableObject()), Useless))
			{
				// set the target to the plyer and leave
				m_pTarget = _pObject;
				break;
			}
			else
			{
				m_bAttacking = false;
			}
		}

		if(m_bAttacking)
		{
			continue;
		}

		switch ( _pObject->GetCollidableObject()->GetBodyType() )
		{
		case BT_DYNAMIC:
			{
				switch(_pObject->GetCollidableObject()->GetVolumeType())
				{
				case VMT_BOX:
					{
						vec2f CPA, dir;
						unsigned int points;
						if( ((Box*)(m_pParent->GetCollidableObject()))->BoxToBox(((Box*)(_pObject->GetCollidableObject())), &CPA, points, dir) && !m_bPreviouslyCollided && m_pParent != _pObject)
						{
							//m_pCollisionCmp->CleanForce ( vec2f (-m_pCollisionCmp->GetVel().x, 0.0f) );

							if(_pObject->GetType() == OBJ_CRATE)
							{
								if(m_pCollisionCmp->GetCollidingGroundAlone())
								{
									m_bCurrentlyCollided = true;
								}
								else
								{
									m_pCollisionCmp->GetVel().x = -m_pCollisionCmp->GetVel().x;
									m_bMoveDirection = !m_bMoveDirection;
									m_bCurrentlyCollided = true;
								}
							}
							else
							{
								m_pCollisionCmp->GetVel().x = -m_pCollisionCmp->GetVel().x;
								m_bMoveDirection = !m_bMoveDirection;
								m_bCurrentlyCollided = true;
							}

							if(_pObject->GetType() == OBJ_GOO)
							{
								// Check left and right for collision.
								if(dot_product(dir, dir))
								{
									dir.normalize();
								}
								else
								{
									continue;
								}

								if( abs( dot_product( dir, vec2f( 1.0f, 0.0f ) ) ) > 0.5f )
								{
									if( dot_product( dir, vec2f( 1.0f, 0.0f ) ) > 0.5f  )
									{
										//m_pCollisionCmp->GetVel().x = -1.0f;
										((CComponent_LittleGooCollision*)(_pObject)->GetComponent(ECOMP_COLLISION))->GetVel().x = -1.0f;
										m_pCollisionCmp->GetVel().x = 1.0f;
										((CComponent_Lil_Goo_AI*)(_pObject)->GetComponent(ECOMP_AI))->SetDirection(false);
										m_bMoveDirection = true;
									}
									else
									{
										//m_pCollisionCmp->GetVel().x = 1.0f;
										((CComponent_LittleGooCollision*)(_pObject)->GetComponent(ECOMP_COLLISION))->GetVel().x = 1.0f;
										m_pCollisionCmp->GetVel().x = -1.0f;
										((CComponent_Lil_Goo_AI*)(_pObject)->GetComponent(ECOMP_AI))->SetDirection(true);
										m_bMoveDirection = false;
									}

									((CComponent_Lil_Goo_AI*)(_pObject)->GetComponent(ECOMP_AI))->SetPrevCol(true);
								}
								else
								{
									if(m_pParent->GetWorldPos().x > _pObject->GetWorldPos().x)
									{
										m_pCollisionCmp->GetVel().x = 1.0f;
										m_bMoveDirection = true;
										((CComponent_Lil_Goo_AI*)(_pObject)->GetComponent(ECOMP_AI))->SetDirection(false);
										((CComponent_Lil_Goo_AI*)(_pObject)->GetComponent(ECOMP_AI))->SetPrevCol(true);
										((CComponent_LittleGooCollision*)(_pObject)->GetComponent(ECOMP_COLLISION))->GetVel().x = -1.0f;
									}
									else
									{
										m_pCollisionCmp->GetVel().x = -1.0f;
										m_bMoveDirection = false;
										((CComponent_Lil_Goo_AI*)(_pObject)->GetComponent(ECOMP_AI))->SetDirection(true);
										((CComponent_Lil_Goo_AI*)(_pObject)->GetComponent(ECOMP_AI))->SetPrevCol(false);
										((CComponent_LittleGooCollision*)(_pObject)->GetComponent(ECOMP_COLLISION))->GetVel().x = 1.0f;
									}
								}
							}
						}
					}
					break;
				case VMT_AABB:
					{
						vec2f CPA, dir;
						//unsigned int points;
						if( ((Box*)(m_pParent->GetCollidableObject()))->BoxToAABB(*((AABB*)(_pObject->GetCollidableObject())), CPA, dir) && !m_bPreviouslyCollided)
						{
							if(_pObject->GetType() == OBJ_PIT)
							{
								m_bCurrentlyCollided = true;
								m_bInGoo = true;
							}
							else if(_pObject->GetType() == OBJ_BRIDGE)
							{
								CComponent_BridgeCollision* bridgeCol = (CComponent_BridgeCollision*)(_pObject->GetComponent(ECOMP_COLLISION));
								if(!bridgeCol->GetIsActivated())
								{	
									m_bCurrentlyCollided = true;
								}
								else
								{
									m_pCollisionCmp->GetVel().x = -m_pCollisionCmp->GetVel().x;
									m_bMoveDirection = !m_bMoveDirection;
									m_bCurrentlyCollided = true;
								}
							}
							else
							{
								//m_pCollisionCmp->CleanForce ( vec2f (-m_pCollisionCmp->GetVel().x, 0.0f) );
								m_pCollisionCmp->GetVel().x = -m_pCollisionCmp->GetVel().x;
								m_bMoveDirection = !m_bMoveDirection;
								m_bCurrentlyCollided = true;
							}
						}
					}
					break;
				default:
					{

					}
					break;
				}
			}
			break;
		case BT_STATIC:
			{
				switch(_pObject->GetCollidableObject()->GetVolumeType())
				{
				case VMT_BOX:
					{
						vec2f CPA, dir;
						unsigned int points;
						if( ((Box*)(m_pParent->GetCollidableObject()))->BoxToBox(((Box*)(_pObject->GetCollidableObject())), &CPA, points, dir) && !m_bPreviouslyCollided)
						{
							//m_pCollisionCmp->CleanForce ( vec2f (-m_pCollisionCmp->GetVel().x, 0.0f) );
							m_pCollisionCmp->GetVel().x = -m_pCollisionCmp->GetVel().x;
							m_bMoveDirection = !m_bMoveDirection;
							m_bCurrentlyCollided = true;
						}
					}
					break;
				case VMT_AABB:
					{
						vec2f CPA, dir;

						//unsigned int points;
						if((((Box*)(m_pParent->GetCollidableObject()))->BoxToAABB(*((AABB*)(_pObject->GetCollidableObject())), CPA, dir)
							|| m_pCollisionCmp->CollidingWall()) && !m_bPreviouslyCollided)
						{
							//m_pCollisionCmp->CleanForce ( vec2f (-m_pCollisionCmp->GetVel().x, 0.0f) );
							m_pCollisionCmp->GetVel().x = -m_pCollisionCmp->GetVel().x;
							m_bMoveDirection = !m_bMoveDirection;
							m_bCurrentlyCollided = true;
						}
						//else if(!m_bPreviouslyCollided)
						//{
						//	//check the side of the aabb I'm on
						//	float distToAABBLeft = abs(((AABB*)(_pObject->GetCollidableObject()))->GetMinPoint3D().x - m_pParent->GetWorldPos().x);
						//	float distToAABBRight = abs(((AABB*)(_pObject->GetCollidableObject()))->GetMaxPoint3D().x - m_pParent->GetWorldPos().x);
						//
						//	if(distToAABBLeft < distToAABBRight)
						//	{
						//		if(distToAABBLeft < 10.0f)
						//		{
						//			m_pCollisionCmp->GetVel().x = -m_pCollisionCmp->GetVel().x;
						//			m_bMoveDirection = !m_bMoveDirection;
						//			m_bCurrentlyCollided = true;
						//		}
						//	}
						//	else
						//	{
						//		if(distToAABBRight < 10.0f)
						//		{
						//			m_pCollisionCmp->GetVel().x = -m_pCollisionCmp->GetVel().x;
						//			m_bMoveDirection = !m_bMoveDirection;
						//			m_bCurrentlyCollided = true;
						//		}
						//	}
						//}
					}
					break;
				default:
					{

					}
					break;
				}
			}
			break;
		}
	}

	// if we have a target 
	if (m_pTarget)
	{
		// forget about the current platform
		m_pCurPlatform = NULL;
		// get the targets collision volume;
		CCollisionVolume* _pTargetVolume = m_pTarget->GetCollidableObject();
		// make sure the target has a collision volume and that the collision 
		// volume is an aabb
		if ( _pTargetVolume && _pTargetVolume->GetVolumeType () == VMT_AABB)
		{
			// make sure the target is within the aggro sphere
			vec2f Useless;
			bool _bAggro = m_spAggro.SphereToAABB (*((AABB*)_pTargetVolume), Useless);
	
			/// if the target is not in the aggro distance
			if (!_bAggro)
			{
				// set the target to null and leave
				m_bSucideing = false;
				m_pTarget = NULL;
				return;
			}
	
			// check if the target is within the attack sphere
			bool _bAttack = m_spAttack.SphereToAABB (*((AABB*)_pTargetVolume), Useless);
			// the the target is not in the attack sphere but is in the aggro
			// move to the target
			if ( _bAggro && !_bAttack)
			{
				m_bSucideing = false;
				Move (m_pTarget);
			}
	
			// if the target is in the attack sphere attack the target
			if (_bAttack)
			{
				Attack ();
			}
		}
	}
	else
	{
		m_bSucideing = false;
		// move around the level if there is no target
		Move ();

		// avoid certain hazards located in the level
		//Avoide ( );
	}

	// set on ground to false
	//m_bOnGround = false;

	m_bPrevOnGround = m_pCollisionCmp->GetCollidingGroundAlone();
	m_bPreviouslyCollided = m_bCurrentlyCollided;
	m_bCurrentlyCollided = false;

	if(m_pSoundCmp)
	{
		//check the distance between me and the listener position
		vec3f lineTo = m_pSoundCmp->GetAudio()->GetListenerPosition() - m_pParent->GetWorldPos();
		float dist = dot_product(lineTo, lineTo);

		//if 
		if(dist > SOUND_RANGE)
		{
			m_bInRange = false;
		}
		else
		{
			m_bInRange = true;
		}
	}

	if(m_bMoveDirection)
	{
		//rotate me to face right
		matrix4f _localMat = (*m_pParent->GetLocalMat());
		matrix4f rotationMatrix;
		rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
		rotationMatrix.translate_post( _localMat.axis_pos );
		_localMat = rotationMatrix;

		m_pParent->SetLocalMat(&_localMat);
	}
	else
	{
		matrix4f _localMat = (*m_pParent->GetLocalMat());
		matrix4f rotationMatrix;
		rotationMatrix.make_rotation_y( D3DXToRadian(90) );
		rotationMatrix.translate_post( _localMat.axis_pos );
		_localMat = rotationMatrix;

		m_pParent->SetLocalMat(&_localMat);
	}
}

void CComponent_Lil_Goo_AI::Attack ()
{
	// see if we are on the ground so we can jump before attacking
	if(m_pCollisionCmp->CollidingGround())
	{
		vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * SUCIDE_JUMP;

		// make the little goo jump
		m_pCollisionCmp->GetVel() += Jump * m_fElapsedTime;	
		m_bSucideing = false;

		if(m_pSoundCmp && m_bInRange)
		{
			m_pSoundCmp->PlaySound(SFX_ENEMY_LILGOO_HOP);
		}

	}


	// wait tell the peak of the jump then fly at the player
	if ( m_fPreviousDownVel >= m_pCollisionCmp->GetVel().y && !m_pCollisionCmp->CollidingGround() && m_bAttacking && !m_bSucideing)
	{
#if _ANIMATIONS
		if( m_pAnimComponent )
		{
			m_pAnimComponent->GetAnimStateMachine()->ChangeState( LilGooAttackState::GetState() );
		}
#endif

 		m_bSucideing = true;

		vec3f TargetPos = m_pTarget->GetWorldPos();
		// get the direction to move
		vec3f fVelocity = (TargetPos - m_pParent->GetWorldPos());
		//fVelocity.y = 0.0f;
		// normalize the velocity
		fVelocity.normalize();

		if(dot_product(fVelocity, vec3f(1.0f, 0.0f, 0.0f)) < -0.25f)
		{
			m_bMoveDirection = false;
		}
		else
		{
			m_bMoveDirection = true;
		}

		m_pCollisionCmp->GetVel().x += fVelocity.x * SUCIDE_SPEED * m_fElapsedTime;
		m_pCollisionCmp->GetVel().y += -200 * m_fElapsedTime;
	}

	// kill the little goo and hurt the player
	vec2f Useless;
	vec2f Garbage;
	if ( ((Box*)m_pParent->GetCollidableObject())->BoxToAABB( *((AABB*)m_pTarget->GetCollidableObject()), Garbage, Useless) )
	{
		m_bSucideing = false;
#if _ANIMATIONS
		if( m_pAnimComponent )
			m_pAnimComponent->GetAnimStateMachine()->ChangeState( LilGooDeathState::GetState() );
#endif
		// attack the target
		// get the targets health component
		IComponent* _pTargetHealthCMP = m_pTarget->GetComponent (ECOMP_HEALTH);
		CComponent_LittleGooCollision* collision = (CComponent_LittleGooCollision*)m_pTarget->GetComponent (ECOMP_COLLISION);

		// if the target gets a health component
		if ( _pTargetHealthCMP && collision)
		{
			collision->SetDead (true);
			//play the death sound for the little goo
			if(m_pSoundCmp && m_bInRange)
			{
				m_pSoundCmp->PlaySound(SFX_ENEMY_LILGOO_DEATH);
			}

			// add sludge to the target
			((CComponent_Health*)_pTargetHealthCMP)->AddSludge (10.0f);
			// remove our parent
			m_bDead = true;
		}
	}

	m_fPreviousDownVel = m_pCollisionCmp->GetVel().y;

	m_bAttacking = true;
}

// this moves around the level
void CComponent_Lil_Goo_AI::Move ()
{
	// do basic movement and edge detection

	//// make sure we have a platform to move on
	//if ( !m_pCurPlatform && !m_bInGoo && !m_bOnGround )
	//{
	//	return;
	//}


	// check if the collision component is valid
	if(!m_pCollisionCmp)
	{
		return; 
	}

	//SimulateMoveLeft();
	//SimulateMoveRight();

	// create a velocity and a jump vector;
	vec3f fVelocity = vec3f (0.0f, 0.0f, 0.0f);
	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;

	// make sure the little goo is on the ground
	if(m_fJumpTimer > PATROL_TIME && m_pCollisionCmp->CollidingGround())// && !m_bInGoo)/* && !m_bPreviouslyCollided*/
	{
		m_pCollisionCmp->GetVel().x = 0.0f;

		// make the little goo jump
		m_pCollisionCmp->GetVel() += Jump * m_fElapsedTime;
		
#if _ANIMATIONS
		if( m_pAnimComponent )
		{
			m_pAnimComponent->GetAnimStateMachine()->ChangeState( LilGooWalkState::GetState() );
		}
#endif
		
		//// check into the futrue moving right and left
		//bool MoveRight = SimulateMoveRight ();
		//bool MoveLeft = SimulateMoveLeft ();
		//
		//// if we are moving right and the direction we were moving is right then
		//// continue moving right
		//if (MoveRight && m_bMoveDirection)
		//{
		//	fVelocity = vec3f (1.0f, 0.0f, 0.0f) * 110.0f;
		//}
		//else
		//{
		//	// if we are not going to move right set the parents velocity on the x axis
		//	// to 0
		//	// and set the move direction to false so the little goo will move to the left
		//	m_pCollisionCmp->GetVel().x = 0.0f;
		//	m_bMoveDirection = false;
		//}
		//
		//// if we are moving to the left and the direction we were moving is to the left then
		//// continue moving to the left
		//if ( MoveLeft && !m_bMoveDirection)
		//{
		//	fVelocity = vec3f (-1.0f, 0.0f, 0.0f) * 110.0f; 
		//}
		//else
		//{
		//	// if we are not going to move left set the parents velocity on the x axis
		//	// to 0 and set the move direction to true so the little goo will move to the right
		//	m_pCollisionCmp->GetVel().x = 0.0f;
		//	fVelocity = vec3f (1.0f, 0.0f, 0.0f) * 110.0f;
		//	m_bMoveDirection = true;
		//}

		// if we are moving right, simulate my next movement and then check if I should 
		// move that way.

		if(m_bMoveDirection)
		{
			if(SimulateMoveRight())
			{
				fVelocity = vec3f (1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
			}
			else
			{
				//I just failed moving to the right. Let's see if we can go the other way.
				if(SimulateMoveLeft())
				{
					//m_pCollisionCmp->GetVel().x = 0.0f;
					fVelocity = vec3f (-1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
					m_bMoveDirection = false;
				}
				else
				{
					//both ways are horrible. I'll trust my first instinct and move right.
					fVelocity = vec3f (1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
					//or not...
					//m_pCollisionCmp->GetVel().x = 0.0f;
					m_bMoveDirection = true;
				}
			}
		}
		else
		{
			//we must have been moving left.
			if(SimulateMoveLeft())
			{
				fVelocity = vec3f (-1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
			}
			else
			{
				//I just failed moving to the left. Let's see if we can go the other way.
				if(SimulateMoveRight())
				{
					//m_pCollisionCmp->GetVel().x = 0.0f;
					fVelocity = vec3f (1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
					m_bMoveDirection = true;
				}
				else
				{
					//both ways are horrible. I'll trust my first instinct and move left.
					fVelocity = vec3f (-1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
					//or not...
					//m_pCollisionCmp->GetVel().x = 0.0f;
					m_bMoveDirection = false;
				}
			}
		}

		if(m_pSoundCmp && m_bInRange)
		{
			m_pSoundCmp->PlaySound(SFX_ENEMY_LILGOO_HOP);
		}

		// update the parents velocity
		m_pCollisionCmp->GetVel() += fVelocity * m_fElapsedTime;

		//m_fJumpTimer = 0.0f;
	}
	//else if(m_fJumpTimer > PATROL_TIME && m_bInGoo)
	//{
	//	m_pCollisionCmp->GetVel().x = 0.0f;
	//
	//	// make the little goo jump
	//	m_pCollisionCmp->GetVel() += Jump  * m_fElapsedTime;
	//
	//	if(m_bMoveDirection)
	//	{
	//		fVelocity = vec3f (1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
	//	}
	//	else
	//	{
	//		fVelocity = vec3f (-1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
	//	}
	//
	//	// update the parents velocity
	//	m_pCollisionCmp->GetVel() += fVelocity * m_fElapsedTime;
	//	m_bInGoo = false;
	//	//m_fJumpTimer = 0.0f;
	//}

	//if(m_pCollisionCmp->CollidingWall() && !m_bPreviouslyCollided)
	//{
	//	m_bMoveDirection = !m_bMoveDirection;
	//	m_pCollisionCmp->GetVel().x = -m_pCollisionCmp->GetVel().x;
	//}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Move():	Takes in a IBaseObject pointer (_pTarget). this will move the little goo toward the target passed in
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Lil_Goo_AI::Move (IBaseObject* _pTarget)
{
	// check if the collision component is valid
	if(!m_pCollisionCmp)
	{
		return; 
	}

	// move towards the target

	// get the targets position
	vec3f TargetPos = m_pTarget->GetWorldPos();

	// make sure the little goo is on the ground
	if(m_fJumpTimer > ATTACK_TIME && m_pCollisionCmp->CollidingGround()/* && !m_bPreviouslyCollided*/)
	{
#if _ANIMATIONS
		if( m_pAnimComponent )
		{
			m_pAnimComponent->GetAnimStateMachine()->ChangeState( LilGooWalkState::GetState() );
		}
#endif

		// get the direction to move
		vec3f fVelocity = (TargetPos- m_pParent->GetWorldPos());
		// set the velocity to 0 so the little goo doesnt jump down
		fVelocity.y = 0.0f;
		// normalize the velocity
		fVelocity.normalize();
			
		if(fVelocity.x < -0.5f)
		{
			m_bMoveDirection = false;
		}
		
		if(fVelocity.x > 0.5f)
		{
			m_bMoveDirection = true;
		}
			
		// check if the little goo was moving one way and is now moving another way 
		// if so set the parents velocity to 0
		if ((m_nPrevDirection < 0.0f && fVelocity.x > 0.0f) || (fVelocity.x < 0.0f && m_nPrevDirection > 0.0f))
		{
			m_pCollisionCmp->GetVel().x = 0.0f;
		}
		// set the prev direction to the current velocity
		m_nPrevDirection = (int)fVelocity.x;

		// update the velocity and the jump
		fVelocity *= ATTACK_SPEED;
		vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * ATTACK_JUMP;

		// update the parents velocity
		m_pCollisionCmp->GetVel() += fVelocity * m_fElapsedTime; 
		m_pCollisionCmp->GetVel() += Jump * m_fElapsedTime;	

		if(m_pSoundCmp && m_bInRange)
		{
			m_pSoundCmp->PlaySound(SFX_ENEMY_LILGOO_HOP);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Avoide():	the little goo should try and avoid crushers
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Lil_Goo_AI::Avoide (IBaseObject* _pHazard)
{
	// avoid crushers
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SimulateMoveRight():	Checks if the little goo will be on the same platform when he jumps and moves to the right
//
// Returns:		bool
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_Lil_Goo_AI::SimulateMoveRight ()
{
	// check if the collision component is valid
	if(!m_pCollisionCmp)
	{
		return false; 
	}

	// get the parents collision sphere
	Box NewCenter = *(Box*)m_pParent->GetCollidableObject();
	
	// create the direction, gravity and jump velocitys
	vec3f DirVelocity = vec3f (1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
	vec3f Gravity = vec3f( 0.0f, -20.0f, 0.0f );
	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;

	// get the parents velocity
	vec3f NewVelocity = m_pCollisionCmp->GetVel();

	// if the parent is on the ground 
	// update its velocity
	if(m_pCollisionCmp->CollidingGround()/* && !m_bPreviouslyCollided*/)
	{
		NewVelocity += DirVelocity * m_fElapsedTime;
		NewVelocity += Jump * m_fElapsedTime;
	}

	// loop several times and check where the little goo will be
	vec2f Useless;
	vec2f Useless2;
	int Interation = 0;
	bool ReturnBool = true;
	bool bOnGround = false;
	while ( Interation < 120 )
	{
		// update the little goes velocity
		NewVelocity += Gravity * m_fElapsedTime;

		// cap that velocity
		if ( NewVelocity.y < -10.0f )
			NewVelocity.y = -10.0f;
		if ( NewVelocity.x < -20.0f )
			NewVelocity.x = -20.0f;
		if ( NewVelocity.x > 20.0f )
			NewVelocity.x = 20.0f;

		// update the future goos position
		NewCenter.SetWorld (NewCenter.GetWorld3D() + NewVelocity);

		////check to see if my position is greater than the platform I'm on.
		//if(m_pCurPlatform->GetCollidableObject()->GetVolumeType() == VMT_AABB)
		//{
		//	if(((AABB*)(m_pCurPlatform->GetCollidableObject()))->GetMaxPoint2D().x <
		//		m_pParent->GetWorldPos().x)
		//	{
		//		ReturnBool = false;
		//		//break;
		//	}
		//}

		if(NewCenter.GetWorld3D().y < m_pParent->GetWorldPos().y - Y_CHECK && !bOnGround)
		{
			ReturnBool = false;
			break;
		}

		// this shows where the little goo will be
		//D3DXMATRIX mat;
		//D3DXMatrixIdentity (&mat);
		//
		//mat._41 = NewCenter.GetWorld3D().x;
		//mat._42 = NewCenter.GetWorld3D().y;
		//mat._43 = -500.0f;
		//
		//DebugShapes::RenderSphere (mat);

		vec2f Garbage;

		// loop through the objects that were found within the aggro range
		for (unsigned int NearIndex = 0; NearIndex < m_vCurNearObjects.size(); NearIndex++)
		{
			// get the back object
			IBaseObject* pObject = (IBaseObject*)m_vCurNearObjects[NearIndex];
			
			if(pObject->GetType() == OBJ_WORLD_COLLISION)
			{
				// check the sphere against that aabb.
				if ( NewCenter.BoxToAABB ( *((AABB*)pObject->GetCollidableObject()), Garbage, Useless) )
				{
					//// if the little goos future sphere is colliding with the current platform
					//if(pObject == m_pCurPlatform)
					//{
					//	// return ture so the little goo moves to the right
					//	ReturnBool = true;
					//	break;
					//}
					//
					//if(pObject->GetType() == OBJ_PIT)
					//{
					//	// return ture so the little goo moves into a goo pit
					//	ReturnBool = true;
					//	break;
					//}
					////if (pObject != m_pCurPlatform)
					////{
					////	ReturnBool = false;
					////}
		
					//// Check wall collision
					//if(NewCenter.GetWorld3D().y - m_pParent->GetWorldPos().y > 10.0f)
					//{
					//	ReturnBool = false;
					//}
		
					//// Check left for wall collision.
					//if( dot_product( Useless.normalize(), vec2f( 1.0f, 0.0f ) ) < -0.5f )
					//{
					//	ReturnBool = false;
					//	break;
					//}
					//else
					{
						bOnGround = true;
					}
				}
			}
			else if(pObject->GetType() == OBJ_PIT)
			{
				if(NewCenter.BoxToAABB(*((AABB*)pObject->GetCollidableObject()), Garbage, Useless))
				{
					ReturnBool = false;
				}
			}
		}

		if(ReturnBool == false)
		{
			break;
		}

		if(bOnGround)
		{
			break;
		}

		Interation++;
	}

	// if the little goo made it through the iterations without hitting the current platform return false
	return ReturnBool;
}

bool CComponent_Lil_Goo_AI::SimulateMoveLeft ()
{
	// check if the collision component is valid
	if(!m_pCollisionCmp)
	{
		return false; 
	}

	// get the parents collision sphere
	Box NewCenter = *(Box*)m_pParent->GetCollidableObject();
	
	// create the direction, gravity and jump velocitys
	vec3f DirVelocity = vec3f (-1.0f, 0.0f, 0.0f) * MOVEMENT_SPEED;
	vec3f Gravity = vec3f( 0.0f, -20.0f, 0.0f );
	vec3f Jump = (vec3f (0.0f , 1.0f, 0.0f)) * PATROL_JUMP;

	// get the parents velocity
	vec3f NewVelocity = m_pCollisionCmp->GetVel();

	// if the parent is on the ground 
	// update its velocity
	if(m_pCollisionCmp->CollidingGround()/* && !m_bPreviouslyCollided*/)
	{
		NewVelocity += DirVelocity * m_fElapsedTime;
		NewVelocity += Jump * m_fElapsedTime;
	}

	// loop several times and check where the little goo will be
	vec2f Useless;
	vec2f Useless2;
	int Interation = 0;
	bool ReturnBool = true;
	bool bOnGround = false;
	while ( Interation < 120 )
	{
		// update the little goes velocity
		NewVelocity += Gravity * m_fElapsedTime;

		// cap that velocity
		if ( NewVelocity.y < -10.0f )
			NewVelocity.y = -10.0f;
		if ( NewVelocity.x < -20.0f )
			NewVelocity.x = -20.0f;
		if ( NewVelocity.x > 20.0f )
			NewVelocity.x = 20.0f;

		// update the future goos position
		NewCenter.SetWorld (NewCenter.GetWorld3D() + NewVelocity);

		////check to see if my position is greater than the platform I'm on.
		//if(m_pCurPlatform->GetCollidableObject()->GetVolumeType() == VMT_AABB)
		//{
		//	if(((AABB*)(m_pCurPlatform->GetCollidableObject()))->GetMinPoint2D().x > 
		//		m_pParent->GetWorldPos().x)
		//	{
		//		ReturnBool = false;
		//		//break;
		//	}
		//}

		if(NewCenter.GetWorld3D().y < m_pParent->GetWorldPos().y - Y_CHECK && !bOnGround)
		{
			ReturnBool = false;
			break;
		}

		//D3DXMATRIX mat;
		//D3DXMatrixIdentity (&mat);
		//
		//mat._41 = NewCenter.GetWorld3D().x;
		//mat._42 = NewCenter.GetWorld3D().y;
		//mat._43 = -500.0f;
		//
		//DebugShapes::RenderSphere (mat);

		vec2f Garbage;
		// loop through the objects that were found within the aggro range
		for (unsigned int NearIndex = 0; NearIndex < m_vCurNearObjects.size(); NearIndex++)
		{
			IBaseObject* pObject = (IBaseObject*)m_vCurNearObjects[NearIndex];
			
			// check the sphere against that aabb.
			if(pObject->GetType() == OBJ_WORLD_COLLISION)
			{
				if(NewCenter.BoxToAABB(*((AABB*)pObject->GetCollidableObject()), Garbage, Useless))
				{
					//// if the little goos future sphere is colliding with the current platform
					//if(pObject == m_pCurPlatform)
					//{
					//	// return ture so the little goo moves to the right
					//	ReturnBool = true;
					//	break;
					//}
					//
					//if(pObject->GetType() == OBJ_PIT)
					//{
					//	// return ture so the little goo moves into a goo pit
					//	ReturnBool = true;
					//	break;
					//}
					////if(pObject != m_pCurPlatform)
					////{
					////	ReturnBool = false;
					////}

					// Check wall collision
					//if(NewCenter.GetWorld3D().y - m_pParent->GetWorldPos().y > 10.0f)
				//{
				//	ReturnBool = false;
				//}
		
					//// Check left and right for wall collision.
					//if( dot_product( Useless.normalize(), vec2f( 1.0f, 0.0f ) ) > 0.5f )
					//{
					//	ReturnBool = false;
					//	break;
					//}
					//else
					{
						bOnGround = true;
					}
				}
			}
			else if(pObject->GetType() == OBJ_PIT)
			{
				if(NewCenter.BoxToAABB(*((AABB*)pObject->GetCollidableObject()), Garbage, Useless))
				{
					ReturnBool = false;
				}
			}
			}
		
		if(ReturnBool == false)
		{
			break;
		}

		if(bOnGround)
		{
			break;
		}

		Interation++;
	}

	// if the little goo made it through the iterations without hitting the current platform return false
	return ReturnBool;
}