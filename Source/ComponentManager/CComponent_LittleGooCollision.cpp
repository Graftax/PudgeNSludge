//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CComponent_LittleGooCollision.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds the information for little goo collision
//////////////////////////////////////////////////////////////////////////////////////
#include "CComponent_LittleGooCollision.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../PS/CGame.h"
#include "CComponent_Lil_Goo_AI.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "../AnimationLib/AnimationStateMachine.h"
#include "../AI/AIStatesInclude.h"

#define PLAYER_IGNORE_TIME 0.5f

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComponent_LittleGooCollision():	Default constructor
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CComponent_LittleGooCollision::CComponent_LittleGooCollision ()
{
	this->CleanRemoveVelocity();
	m_bVolitile = false;
	m_pAnimComp = nullptr;
	m_pSoundCmp = nullptr;
	m_pItemCmp = nullptr;
	m_bAlive = false;
	m_bPreviouslyCollided = false;
	m_bCurrentlyCollided = false;
	m_bPrevAlive = false;
	m_fPlayerIgnoreTimer = 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComponent_LittleGooCollision():	Default Destructor
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CComponent_LittleGooCollision::~CComponent_LittleGooCollision ()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize():	intis the little goo collision informtion
//
// Returns:		bool
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_LittleGooCollision::Initialize ()
{
	this->CleanRemoveVelocity();
	m_bVolitile = false;
	m_pAnimComp = nullptr;
	m_pSoundCmp = nullptr;
	m_bAlive = false;
	m_aiGoo = NULL;
	m_pSpawnObject = NULL;
	m_bPulled = false;
	m_bPreviouslyCollided = false;
	m_bCurrentlyCollided = false;
	m_bPrevAlive = false;
	m_fPlayerIgnoreTimer = 0.0f;

#if _ANIMATIONS
	m_pAnimComp = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );	
#endif

	m_pSoundCmp = (CComponent_Sound*)m_pParent->GetComponent(ECOMP_SOUND);	

	m_aiGoo = (CComponent_AI*)m_pParent->GetComponent(ECOMP_AI);

	m_pItemCmp = (CComponent_Item*)m_pParent->GetComponent(ECOMP_ITEM);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown():	shutdowns the little goo collision information
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_LittleGooCollision::Shutdown ()
{
	unsigned int i = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	Updates the collison component, incrementing its physics and detecting collision with other objects.
//
// Returns:		Void
//
// Mod. Name:   Josh Fields
// Mod. Date:   5/16/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_LittleGooCollision::Update ( float _fElapsedTime )
{
	if(!m_pParent)
	{
		return;
	}

	SetPrevInGoo(GetInGoo());
	SetInGoo(false);

	// make sure the parent has a collision volume and that volume is a sphere
	m_bAlive = m_pParent->GetAlive();

	if( !m_bAlive && !m_pParent->GetCollidableObject() )
	{
		CleanRemoveVelocity();
		return;
	}

	if (m_pParent->GetCollidableObject()->GetVolumeType() != VMT_BOX)
	{
		return;
	}

	vec3f fAverage;

	if( m_pPhysicsBox )
	{
		fAverage = m_pPhysicsBox->GetCentroid();
	}

	m_pParent->SetLocalPos( &fAverage );

	// Volume for collision checks
	Box* BoxCollisionVolume = (Box*)m_pParent->GetCollidableObject();
	vec3f NewCenter = m_pParent->GetWorldPos();
	BoxCollisionVolume->SetWorld (NewCenter);
	m_tPrevPos = m_pParent->GetWorldPos();

	if ( !m_bAlive && m_bPrevAlive )
	{
		if (m_pSpawnObject)
		{
			if (m_pSpawnObject->GetType () == OBJ_PIPE_SPAWN)
			{
				CComponent_SpawnPipeCollision* spawn = ((CComponent_SpawnPipeCollision*)m_pSpawnObject->GetComponent (ECOMP_COLLISION));
				spawn->RemoveChild(GetParent());
			}			
			m_pSpawnObject = NULL;
		}

		if(m_pItemCmp && m_pItemCmp->GetTarget())
		{
			m_pItemCmp->SetTarget(nullptr);
		}
	}

	m_bPrevAlive = m_bAlive;

	if( !m_bAlive )
	{
		return;
	}

//if we're colliding with the ground, trigger things that we need to trigger
	if(m_bCollidingGround)
	{
		if( !m_bCollidingGroundLast )
		{
			CParticleManager::GetInstance()->CreateEffectOneShot( "LilGooWalkSplat", m_pParent->GetWorldPos() );
		}
	}

	// set the colliding bools
	m_bCollidingWall = false;
	m_bCollidingGroundLast = m_bCollidingGround;
	m_bCollidingGround = false;

	// cap the objects z axis position and velocity
	m_fPos.z = 0.0f;
	m_fVel.z = 0.0f;

	//// update the position with gravity
	//vec3f fAverage;
	//
	//m_fVel += vec3f( 0.0f, -20.0, 0.0f ) * _fElapsedTime;
	//
	//if ( m_fVel.y < -10.0f )
	//	m_fVel.y = -10.0f;
	//if ( m_fVel.x < -20.0f )
	//	m_fVel.x = -20.0f;
	//if ( m_fVel.x > 20.0f )
	//	m_fVel.x = 20.0f;
	//
	//m_fPos += m_fVel;
	//
	//m_pParent->SetLocalPos(&m_fPos);
	//((Box*)m_pParent->GetCollidableObject())->SetWorld (m_pParent->GetWorldMat()->axis_pos);

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> ReturnVector;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnPrams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_GOO);

	CKdTree::m_pMainTree->debugLeafTest.clear();

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects (m_pParent, PSFLAG_BOX, ReturnPrams, ReturnVector);

	// loop through all the return objects and check collision with them.
	for (unsigned int ReturnIndex = 0; ReturnIndex < ReturnVector.size(); ReturnIndex++)
	{
		IBaseObject* _pObject = ((IBaseObject*)ReturnVector[ReturnIndex]);

		if (_pObject == m_pParent)
		{
			continue;
		}

		// determine type body type of the objecet
		if (_pObject->GetCollidableObject())
		{
			switch ( _pObject->GetCollidableObject()->GetBodyType() )
			{
			case BT_DYNAMIC:
				{
					DynamicCollision ( m_pParent, _pObject);
				}
				break;
			case BT_STATIC:
				{
					StaticCollision ( m_pParent, _pObject );
				}
				break;
			}
		}
	}
	m_pParent->SetLocalPos(&m_pPhysicsBox->GetCentroid());
	m_pParent->GetFrame().Update();

	// Set the velocity of the Box.
	m_fVel = m_pPhysicsBox->GetCentroid() - m_pPhysicsBox->GetPrevCentroid();
	vec2f fTemPos = GetPos2DClean();
	m_pParent->SetLocalPos( &vec3f( fTemPos.x, fTemPos.y, 0.0f) );


	if(m_pSoundCmp && m_bCurrentlyCollided && !m_bPreviouslyCollided)
	{
		m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_LAND);
	}

	m_bPreviouslyCollided = m_bCurrentlyCollided;
	m_bCurrentlyCollided = false;

	if(m_fPlayerIgnoreTimer < PLAYER_IGNORE_TIME)
	{
		m_fPlayerIgnoreTimer += _fElapsedTime;
	}
}

void CComponent_LittleGooCollision::StaticCollision ( IBaseObject* Collider, IBaseObject* Collide )
{
	// check the volume type of the static object
	switch (Collide->GetCollidableObject()->GetVolumeType())
	{
	case VMT_AABB:
		{
			// check collision with our aabb and the static objects aabb
			vec2f Direction;
			vec2f CP;
			if(((Box*)Collider->GetCollidableObject())->BoxToAABB( *(AABB*)Collide->GetCollidableObject(), CP, Direction))
			{
				m_bCurrentlyCollided = true;
				// push our collision volume out
				// Create an offset vector.
				vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);

				////////// FRICTION
				vec3f fCross;
				cross_product( fCross, MoveDirection, vec3f( 0.0f, 0.0f, 1.0f ) );
				float3 fProj = prjPoint2Line( m_pPhysicsBox->m_pPoints[0]->m_CurrPos, m_pPhysicsBox->m_pPoints[0]->m_PrevPos, m_pPhysicsBox->m_pPoints[0]->m_PrevPos + fCross );
				float3 fPrevPush = fProj - m_pPhysicsBox->m_pPoints[0]->m_PrevPos;
				fPrevPush *= -0.3f;
				//////////

				//PushOut ( this, Direction );
				CleanOffset( vec2f( MoveDirection.x, MoveDirection.y ) );
				CleanForce( vec2f( fPrevPush.x, fPrevPush.y ) + vec2f( MoveDirection.x, MoveDirection.y ) * 0.1f );

				vec3f NormalizedDirection = vec3f(Direction.x, Direction.y, 0.0f).normalize();
				float fDotProductUp = dot_product(NormalizedDirection, vec3f(0.0f, 1.0f, 0.0f));
				float fDotProductSide = dot_product(NormalizedDirection, vec3f(1.0f, 0.0f, 0.0f));
				if( fDotProductUp > 0.5f )
				{
					m_bCollidingGround = true;
				}
				if( abs( fDotProductSide ) > 0.5f )
				{
					fPrevPush = vec3f( 0.0f, 0.0f, 0.0f );
					m_bCollidingWall = true;
				}
				// If the goo will die on contact or not.
				if( m_bVolitile )
				{
					//play the death sound for the little goo

					m_pParent->SetAlive(false);

					CleanRemoveVelocity();
				}
			}
		}
		break;
	default:
		break;
	}
}

void CComponent_LittleGooCollision::DynamicCollision ( IBaseObject* Collider, IBaseObject* Collide )
{
	// check the volume type of the dynamic object
	switch (Collide->GetCollidableObject()->GetVolumeType())
	{
	case VMT_AABB:
		{
			// check collision with with our aabb and the dynamic aabb
			vec2f Direction;
			vec2f CP;
			if ( ((Box*)Collider->GetCollidableObject())->BoxToAABB( *(AABB*)Collide->GetCollidableObject(), CP, Direction ) )
			{
				m_bCurrentlyCollided = true;
				if(Collide->GetType() == OBJ_PLAYER)
				{
					if (!m_bPulled && m_fPlayerIgnoreTimer > PLAYER_IGNORE_TIME)
					{
						// get the targets health component
						IComponent* _pTargetHealthCMP = Collide->GetComponent(ECOMP_HEALTH);

						// if the target gets a health component
						if(_pTargetHealthCMP)
						{
							//play the death sound for the little goo

							// add sludge to the target
							((CComponent_Health*)_pTargetHealthCMP)->AddSludge (10.0f);							
							m_pParent->SetAlive(false);
						}
					}
				}
			}
		}
		break;
	case VMT_BOX:
		{
			// check collision with our aabb and the dynamic box
			vec2f Direction;
			vec2f* Points = NULL;
			unsigned int NumContacts;
			if ( ((Box*)Collide->GetCollidableObject())->BoxToBox ( ((Box*)Collider->GetCollidableObject()), Points, NumContacts, Direction ) )
			{
				// get the collision volume of the box and check its velocity
				m_bCurrentlyCollided = true;
				CComponent_Collision* CollideCollision = (CComponent_Collision*)Collide->GetComponent (ECOMP_COLLISION);
				CComponent_Item* CollideItem = (CComponent_Item*)Collide->GetComponent (ECOMP_ITEM);
				if( !CollideItem )
					break;
				if ( dot_product (CollideCollision->GetVel(), CollideCollision->GetVel()) > 150.0f && !CollideItem->GetTarget())
				{
					if(Collide->GetType() != OBJ_GOO)
					{
						// if the velocity is great enough apply damage to the slime ai
						m_pParent->SetAlive(false);

//#if _ANIMATIONS
//						if( m_pAnimComp )
//						{
//							m_pAnimComp->GetAnimStateMachine()->ChangeState( LilGooDeathState::GetState() );
//						}
//#endif
					}
					return;
				}

				
				//check regular collision for when we collide against a box and we're volatile
				if(Collide->GetType() != OBJ_GOO && Collide->GetAlive())
				{
					if(GetVolitile())
					{
						//check to see if the spawn I'm colliding with spawned from something
						if(m_pSpawnObject)
						{
							if(m_pSpawnObject->GetType() == OBJ_PIPE_SPAWN)
							{
								CComponent_SpawnPipeCollision* spawn = ((CComponent_SpawnPipeCollision*)m_pSpawnObject->GetComponent(ECOMP_COLLISION));
								spawn->RemoveChild(GetParent());
							}
							SetSpawnObject(nullptr);
						}

						CComponent_Item* itemCmp = (CComponent_Item*)m_pParent->GetComponent(ECOMP_ITEM);
						if(itemCmp && itemCmp->GetTarget())
						{
							itemCmp->SetTarget(nullptr);
						}

						m_pParent->SetAlive(false);
					}
				}

				if( Collide->GetType() == OBJ_GOO && Collide->GetAlive() )
				{					
					if(GetVolitile())
					{
						//check to see if the spawn I'm colliding with spawned from something
						if(m_pSpawnObject)
						{
							if(m_pSpawnObject->GetType() == OBJ_PIPE_SPAWN)
							{
								CComponent_SpawnPipeCollision* spawn = ((CComponent_SpawnPipeCollision*)m_pSpawnObject->GetComponent(ECOMP_COLLISION));
								spawn->RemoveChild(GetParent());
							}
							SetSpawnObject(nullptr);
						}

						CComponent_Item* itemCmp = (CComponent_Item*)m_pParent->GetComponent(ECOMP_ITEM);
						if(itemCmp && itemCmp->GetTarget())
						{
							itemCmp->SetTarget(nullptr);
						}

						Collide->SetAlive(false);

						CGame::GetInstance()->GetAudioManager()->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
						CObjectManager::GetInstance()->AddObjectToRemove(m_pParent);
					}
				}

				// Collision reaction with other boxes in the game
				vec3f MoveDirection = vec3f( Direction.x, Direction.y, 0.0f );
				vec3f fCross;
				cross_product( fCross, MoveDirection, vec3f( 0.0f, 0.0f, 1.0f ) );
				float3 fProj = prjPoint2Line( m_pPhysicsBox->m_pPoints[0]->m_CurrPos, m_pPhysicsBox->m_pPoints[0]->m_PrevPos, m_pPhysicsBox->m_pPoints[0]->m_PrevPos + fCross );
				float3 fPrevPush = fProj - m_pPhysicsBox->m_pPoints[0]->m_PrevPos;
				fPrevPush *= -0.1f;

				vec3f NormalizedDirection = vec3f (Direction.x, Direction.y, 0.0f).normalize();
				float fDotProductUp = dot_product( NormalizedDirection, vec3f( 0.0f, 1.0f, 0.0f ) );
				float fDotProductSide = dot_product( NormalizedDirection, vec3f( 1.0f, 0.0f, 0.0f ) );
				if( fDotProductUp > 0.5f )
				{
					if(Collide->GetType() != OBJ_GOO)
					{
						//PushOut ( this, Direction );
						CollideCollision->CleanOffset( Direction * 0.5f );
						CollideCollision->CleanForce( Direction * 0.1f );
						Direction.negate();
						CleanOffset( Direction * 0.5f );
						CleanForce( Direction * 0.5f );
					}

					m_bCollidingGround = true;
				}
				if( fDotProductUp < -0.5f )
				{
					if(Collide->GetType() != OBJ_GOO)
					{
						//PushOut ( this, Direction );
						CollideCollision->CleanOffset( Direction * 0.5f );
						CollideCollision->CleanForce( Direction * 0.1f );
						Direction.negate();
						CleanOffset( Direction * 0.5f );
						CleanForce( Direction * 0.5f );
					}

					SetCollidingGroundAlone( true );
				}

				if( abs( fDotProductSide ) > 0.05f )
				{
					if(Collide->GetType() == OBJ_GOO)
					{
						//PushOut ( this, Direction );
						CollideCollision->CleanOffset( Direction * 0.01f );
						CollideCollision->CleanForce( Direction * 0.01f );
						Direction.negate();
						CleanOffset( Direction * 0.01f );
						CleanForce( Direction * 0.01f );
					}
					else
					{
						//PushOut ( this, Direction );
						CollideCollision->CleanOffset( Direction * 0.5f );
						CollideCollision->CleanForce( Direction * 0.1f );
						Direction.negate();
						CleanOffset( Direction * 0.5f );
						CleanForce( Direction * 0.5f );
					}

					fPrevPush = vec3f( 0.0f, 0.0f, 0.0f );
					if(!m_bPreviouslyCollided && m_aiGoo)
					{
						if(fDotProductSide > 0.05f)
						{
							CComponent_AI* CollideAI = (CComponent_AI*)Collide->GetComponent(ECOMP_AI);
							if(CollideAI)
							{
								CLilGooMoveAI* CollideMoveAI = (CLilGooMoveAI*)((CComponent_AI*)Collide->GetComponent(ECOMP_AI));
								if (CollideMoveAI)
								{
									CollideMoveAI->SetDirection(true);
								}
							}

							CLilGooMoveAI* GooMoveAI = (CLilGooMoveAI*)m_aiGoo->GetLoadedAI(AI_MOVE);
							if (GooMoveAI)
							{
								GooMoveAI->SetMoveDirection(false);
							}
						}
						else if(fDotProductSide < -0.05f)
						{
							CComponent_AI* CollideAI = (CComponent_AI*)Collide->GetComponent(ECOMP_AI);
							if(CollideAI)
							{
								CLilGooMoveAI* CollideMoveAI = (CLilGooMoveAI*)((CComponent_AI*)Collide->GetComponent(ECOMP_AI));
								if (CollideMoveAI)
								{
									CollideMoveAI->SetDirection(false);
								}
							}

							CLilGooMoveAI* GooMoveAI = (CLilGooMoveAI*)m_aiGoo->GetLoadedAI(AI_MOVE);
							if (GooMoveAI)
							{
								GooMoveAI->SetMoveDirection(true);
							}
						}

						GetVel().x = -GetVel().x;
					}
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

void CComponent_LittleGooCollision::PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction )
{
	// see if it is the the ground.
	if( dot_product( Direction, vec2f( 0.0f, 1.0f ) ) > 0.5f )
	{
		_pObjectCollision->SetCollidingGroundAlone (true);
	}
	// Check left and right for wall collision.
	if( abs( dot_product( Direction, vec2f( 1.0f, 0.0f ) ) ) > 0.5f )
	{
		_pObjectCollision->SetCollidingWall (true);
	}
}