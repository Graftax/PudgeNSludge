//////////////////////////////////////////////////////////////////////////////////////
// Filename:	CComponent_SlimeMonsterCollision.cpp
// Author:		Josh Fields
// Date:		5/24/12
// Purpose:		This class holds the information for little goo collision
//////////////////////////////////////////////////////////////////////////////////////
#include "CComponent_WaterPoolCollision.h"
#include "CComponent_Health.h"
#include "CComponent_Sound.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../PS/CGame.h"

#define SOUND_CAP 0.75f

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComponent_SlimeMonsterCollision():	Default constructor
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CComponent_WaterPoolCollision::CComponent_WaterPoolCollision ()
{
	m_fHealAmount = 0.0f;
	m_fSoundTimer = 0.0f;
	m_pSoundCmp = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComponent_SlimeMonsterCollision():	Default Destructor
//
// Returns:		Void
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CComponent_WaterPoolCollision::~CComponent_WaterPoolCollision ()
{
	Shutdown ();
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize():	intis the little goo collision informtion
//
// Returns:		bool
//
// Mod. Name: JF
// Mod. Date: 6/2/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_WaterPoolCollision::Initialize ()
{
	//DebugShapes::CreateSphere (20.0f, 5, 5);
	m_fHealAmount = 0.0f;
	m_fSoundTimer = 0.0f;
	m_pSoundCmp = nullptr;
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
void CComponent_WaterPoolCollision::Shutdown ()
{
	m_fHealAmount = 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	Updates the collison component, incrementing its physics and detecting collision with other objects.
//
// Returns:		Void
//
// Mod. Name:   Josh Fields
// Mod. Date:   5/16/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_WaterPoolCollision::Update ( float _fElapsedTime )
{
	// make sure the parent is alive and that it has a collision volume
 	if ( !m_pParent->GetAlive() && !m_pParent->GetCollidableObject())
	{
		return;
	}

	if(!m_pSoundCmp)
	{
		m_pSoundCmp = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));
	}

	m_fSoundTimer += _fElapsedTime;

	// update the parents position
	m_pParent->GetWorldMat()->axis_pos = m_fPos;
	// update the collision volumes postion
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (m_pParent->GetWorldMat()->axis_pos);


	// this can be removed when we no longer need the debug leafs to show were and object is in the kd tree
	CKdTree::m_pMainTree->debugLeafTest.clear();


	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> ReturnVector;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnPrams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_GOO) | (1<<OBJ_MONSTER);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects (m_pParent, PSFLAG_AABB, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);

	// loop through all the return objects and check collision with them.
	for (unsigned int ReturnIndex = 0; ReturnIndex < ReturnVector.size(); ReturnIndex++)
	{
		IBaseObject* _pObject = ((IBaseObject*)ReturnVector[ReturnIndex]);

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

	// update the parents position
	m_pParent->GetWorldMat()->axis_pos = m_fPos;
	// update the collision volumes postion
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (m_pParent->GetWorldMat()->axis_pos);
}

void CComponent_WaterPoolCollision::StaticCollision ( IBaseObject* Collider, IBaseObject* Collide )
{
	// check the volume type of the static object
	/*switch (Collide->GetCollidableObject()->GetVolumeType())
	{
	default:
		break;
	}*/
}

void CComponent_WaterPoolCollision::DynamicCollision ( IBaseObject* Collider, IBaseObject* Collide )
{
	// check the volume type of the dynamic object
	switch (Collide->GetCollidableObject()->GetVolumeType())
	{
	case VMT_AABB:
		{
			// check collision with with our aabb and the dynamic aabb
			vec2f Direction;
			if ( ((AABB*)Collider->GetCollidableObject())->AABBToAABB( *(AABB*)Collide->GetCollidableObject(), Direction ) )
			{
				if (Collide->GetType() == OBJ_PLAYER)
				{
					CComponent_Health* PlayerHealth = ((CComponent_Health*)Collide->GetComponent (ECOMP_HEALTH));
					if (PlayerHealth)
					{
						PlayerHealth->SetHealing (true);
					}
					if(m_pSoundCmp && m_fSoundTimer > SOUND_CAP && PlayerHealth->GetHealth() < 100.0f)
					{
						m_pSoundCmp->PlaySound(SFX_WATERPOOL_HEAL);
						m_fSoundTimer = 0.0f;
					}
				}
				else
				{
					// push out the other collision volume
					CComponent_Collision* CollideCollision = (CComponent_Collision*)Collide->GetComponent (ECOMP_COLLISION);
					PushOut ( CollideCollision, Direction );
				}
			}
		}
		break;
	case VMT_BOX:
		{
			// check collision with our aabb and the dynamic box
			vec2f Direction;
			vec2f CP;
			if ( ((Box*)Collide->GetCollidableObject())->BoxToAABB ( *((AABB*)Collider->GetCollidableObject()), CP, Direction ) )
			{
				// get the collision volume of the box and check its velocity
				CComponent_Collision* CollideCollision = (CComponent_Collision*)Collide->GetComponent (ECOMP_COLLISION);
				PushOut ( CollideCollision, Direction );
			}
		}
		break;
	default:
		break;
	}
}

void CComponent_WaterPoolCollision::PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction )
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
		//_pObjectCollision->GetVel().x = 0.0f;
		_pObjectCollision->SetCollidingWall (true);
	}
}