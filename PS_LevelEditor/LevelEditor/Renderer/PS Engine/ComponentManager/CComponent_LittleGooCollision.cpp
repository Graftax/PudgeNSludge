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
	//DebugShapes::CreateSphere (20.0f, 5, 5);
	m_bVolitile = false;
	m_pAnimComp = nullptr;
	m_pSoundCmp = nullptr;
	m_bDead = false;
	m_fSpawnIngore = 0.0f;
	m_fSpawnIngoreTimer = 0.0f;

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
	//DebugShapes::CreateSphere (20.0f, 5, 5);
	m_bVolitile = false;
	m_pAnimComp = nullptr;
	m_pSoundCmp = nullptr;
	m_bDead = false;
	m_aiGoo = NULL;
	m_fSpawnIngoreTimer = 0.0f;
	m_pSpawnObject = NULL;
	m_bPulled = false;
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
	// make sure the parent has a collision volume and that volume is a sphere
	if( !m_pParent->GetAlive() && !m_pParent->GetCollidableObject() )
	{
		return;
	}

	if ( !m_aiGoo )
	{
		m_aiGoo = (CComponent_Lil_Goo_AI*)m_pParent->GetComponent (ECOMP_AI);
	}

	if ( m_bDead || m_bPulled || (m_aiGoo && m_aiGoo->GetDead()))
	{		
		if (m_pSpawnObject)
		{
			if (m_pSpawnObject->GetType () == OBJ_PIPE_SPAWN)
			{
				CComponent_SpawnPipeCollision* spawn = ((CComponent_SpawnPipeCollision*)m_pSpawnObject->GetComponent (ECOMP_COLLISION));
				spawn->SetActiveEnemies (spawn->GetActiveEnemies() - 1);
			}
			else if (m_pSpawnObject->GetType () == OBJ_MONSTER)
			{
				CComponent_Slime_Monster_AI* monster = ((CComponent_Slime_Monster_AI*)m_pSpawnObject->GetComponent (ECOMP_AI));
				monster->SetActiveEnemies (monster->GetActiveEnemies() - 1);
			}
			m_pSpawnObject = NULL;
		}
	}

	if ( m_bDead )
	{
		return;
	}

	
	// ingore collision with our parent while this is on
	if (m_fSpawnIngore >= 0.0f)
	{
		m_fSpawnIngore -= _fElapsedTime;
	}

#if _ANIMATIONS
	if( !m_pAnimComp )
	{
		m_pAnimComp = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );
	}
#endif

	if(!m_pSoundCmp)
	{
		m_pSoundCmp = (CComponent_Sound*)m_pParent->GetComponent(ECOMP_SOUND);
	}

	// set the colliding bools
	m_bCollidingWall = false;
	m_bCollidingGroundLast = m_bCollidingGround;
	m_bCollidingGround = false;

	// cap the objects z axis position and velocity
	m_fPos.z = -300.0f;
	m_fVel.z = 0.0f;

	// update the position with gravity
	vec3f fAverage;

	m_fVel += vec3f( 0.0f, -20.0, 0.0f ) * _fElapsedTime;
	
	if ( m_fVel.y < -10.0f )
		m_fVel.y = -10.0f;
	if ( m_fVel.x < -20.0f )
		m_fVel.x = -20.0f;
	if ( m_fVel.x > 20.0f )
		m_fVel.x = 20.0f;
	
	m_fPos += m_fVel;

	m_pParent->SetLocalPos(&m_fPos);
	((Box*)m_pParent->GetCollidableObject())->SetWorld (m_pParent->GetWorldMat()->axis_pos);

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> ReturnVector;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnPrams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PIT) | (1<<OBJ_BRIDGE);

	CKdTree::m_pMainTree->debugLeafTest.clear();

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects (m_pParent, PSFLAG_BOX, ReturnPrams, ReturnVector);

	// loop through all the return objects and check collision with them.
	for (unsigned int ReturnIndex = 0; ReturnIndex < ReturnVector.size(); ReturnIndex++)
	{
		IBaseObject* _pObject = ((IBaseObject*)ReturnVector[ReturnIndex]);

		if (_pObject == m_pParent || _pObject->GetType() == OBJ_GOO)
		{
			continue;
		}

		// determine type body type of the objecet
		if ( _pObject->GetCollidableObject() )
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

	m_pParent->SetLocalPos(&m_fPos);
	((Box*)m_pParent->GetCollidableObject())->SetWorld (m_pParent->GetWorldMat()->axis_pos);
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
			if ( ((Box*)Collider->GetCollidableObject())->BoxToAABB( *(AABB*)Collide->GetCollidableObject(), CP, Direction ) )
			{
				// push our collision volume out
				PushOut ( this, Direction );
				// If the goo will die on contact or not.
				if( m_bVolitile )
				{
					//play the death sound for the little goo
					if(m_pSoundCmp)
					{
						m_pSoundCmp->PlaySound(SFX_ENEMY_LILGOO_DEATH);
					}

					m_bDead = true;
					if ( m_aiGoo )
					{
						m_aiGoo->SetDead(true);
					}

					//CObjectManager::GetInstance()->AddObjectToRemove( m_pParent );
				}

				if (m_bCollidingGround)
				{
					if ( m_aiGoo )
					{
						m_aiGoo->SetCurPlatform (Collide);
						m_aiGoo->SetOnGround ( true );
					}
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
			if(Collide->GetType() == OBJ_PIT)
			{
				// check collision with with our aabb and the dynamic aabb
				vec2f Direction;
				vec2f CP;
				if ( ((Box*)Collider->GetCollidableObject())->BoxToAABB( *(AABB*)Collide->GetCollidableObject(), CP, Direction ) )
				{
					// push out the other collision volume
					//PushOut ( this, Direction );
					if(m_pSoundCmp && !m_bDead)
					{
						m_pSoundCmp->PlaySound(SFX_ENEMY_LILGOO_DEATH);	
					}

					m_bDead = true;
					if (m_aiGoo)
					{
						m_aiGoo->SetDead(true);
					}

#if _ANIMATIONS
					if( m_pAnimComp )
						m_pAnimComp->GetAnimStateMachine()->ChangeState( LilGooDeathState::GetState() );
#endif
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
				CComponent_Collision* CollideCollision = (CComponent_Collision*)Collide->GetComponent (ECOMP_COLLISION);
				CComponent_Item* CollideItem = (CComponent_Item*)Collide->GetComponent (ECOMP_ITEM);
				if ( dot_product (CollideCollision->GetVel(), CollideCollision->GetVel()) > 150.0f && !CollideItem->GetTarget())
				{
					// if the velocity is great enough apply damage to the slime ai
					if(m_pSoundCmp && !m_bDead)
					{
						m_pSoundCmp->PlaySound(SFX_ENEMY_LILGOO_DEATH);	
					}

					m_bDead = true;
					if (m_aiGoo)
					{
						m_aiGoo->SetDead(true);
					}

#if _ANIMATIONS
					if( m_pAnimComp )
						m_pAnimComp->GetAnimStateMachine()->ChangeState( LilGooDeathState::GetState() );
#endif
					return;
				}

				if( m_bVolitile )
				{
					//play the death sound for the little goo
					if(m_pSoundCmp)
					{
						m_pSoundCmp->PlaySound(SFX_ENEMY_LILGOO_DEATH);
					}
					CObjectManager::GetInstance()->AddObjectToRemove( m_pParent );
					if( Collide->GetType() == OBJ_GOO )
						CObjectManager::GetInstance()->AddObjectToRemove( Collide );
				}

				// if we are trying to push the box down we need to push our self up
				if (Direction.y < 0.0f)
				{
					Direction.y = -Direction.y;
					PushOut ( this, Direction );
				}
				else
				{
					// else push the box away from us
					PushOut ( CollideCollision, Direction );
				}

				if (m_bCollidingGround)
				{
					if ( m_aiGoo )
					{
						if(GetCollidingGroundLast())
						{
							m_aiGoo->SetOnGround ( true );
						}
					}
				}
				
			}
		}
		break;
	default:
		break;
	}
}

void CComponent_LittleGooCollision::PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction )
{
	// Move them by that vector.
	_pObjectCollision->CleanOffset (Direction);

	// Create a nortmal of that movement.
	if (dot_product (Direction, Direction))
		Direction.normalize();

	// Check the last movement to see if its the cieling.
	if( dot_product( Direction, vec2f( 0.0f, -1.0f ) ) > 0.5f )
	{
		_pObjectCollision->CleanForce ( vec2f (0.0f, -_pObjectCollision->GetVel().y) );
	}
	// see if it is the the ground.
	if( dot_product( Direction, vec2f( 0.0f, 1.0f ) ) > 0.5f )
	{
		_pObjectCollision->CleanForce ( vec2f (0.0f, -_pObjectCollision->GetVel().y) );
		_pObjectCollision->SetCollidingGroundAlone (true);
	}
	// Check left and right for wall collision.
	if( abs( dot_product( Direction, vec2f( 1.0f, 0.0f ) ) ) > 0.5f )
	{
		_pObjectCollision->CleanForce ( vec2f (-_pObjectCollision->GetVel().x, 0.0f) );
		_pObjectCollision->SetCollidingWall (true);
	}
}