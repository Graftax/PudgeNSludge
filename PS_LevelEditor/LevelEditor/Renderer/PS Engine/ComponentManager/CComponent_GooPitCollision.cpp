//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_GooPitCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "..//PS/States/CWinGameState.h"
#include "../Utilities/KdTree/KdTree.h"
#include "CComponent_Health.h"
#include "../RenderEngine/DebugShapes.h"

CComponent_GooPitCollision::CComponent_GooPitCollision()
{
	//m_fDamageTimer = 0.0f;
	m_bSoundLoop = false;
	m_pSoundComponent = nullptr;
	m_fElapsedTime = 0.0f;
	m_bInRange = false;
	m_bSoundSplash = false;
	this->CleanRemoveVelocity();
}

CComponent_GooPitCollision::~CComponent_GooPitCollision(void)
{

}

bool CComponent_GooPitCollision::Initialize ()
{
	m_bSoundLoop = false;
	m_pSoundComponent = nullptr;
	m_fElapsedTime = 0.0f;
	m_bSoundSplash = false;
	this->CleanRemoveVelocity();
	//DebugShapes::CreateSphere (10.0f, 50, 50);
	CComponent_Collision::Initialize();
	m_bInRange = false;
	return true;
}

void CComponent_GooPitCollision::Shutdown(void)
{
	m_pSoundComponent->StopSound(SFX_HAZARD_GOOPIT_BLOOP);
}

void CComponent_GooPitCollision::Update( float fDT )
{
	if ( !m_pParent->GetCollidableObject() )
	{
		return;
	}

	m_fElapsedTime = fDT;

	if(!m_pSoundComponent && m_pParent)
	{
		m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));
	}

	if(m_pSoundComponent && !m_bSoundLoop && m_bInRange)
	{
		if(m_pSoundComponent->PlaySound(SFX_HAZARD_GOOPIT_BLOOP))
		{
			m_bSoundLoop = true;
		}
	}

	// set the collision volumes position
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (m_pParent->GetWorldPos());

	// this can be removed when we no longer need the debug leafs to show were and object is in the kd tree
	CKdTree::m_pMainTree->debugLeafTest.clear();


	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> ReturnVector;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnPrams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_MONSTER) | (1<<OBJ_GOO);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects (m_pParent, PSFLAG_AABB, ReturnPrams, ReturnVector, ReturnBody, ReturnObjects);

	// This is debug stuff for the kd tree please leave this here for I (Josh Fields) use this from time to time Thank you.
	/*for (unsigned int LeafIndex = 0; LeafIndex < CKdTree::m_pMainTree->debugLeafTest.size(); LeafIndex++)
	{
	D3DXMATRIX mat;
	D3DXMatrixIdentity (&mat);

	mat._41 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMaxPoint2D().x;
	mat._42 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMaxPoint2D().y;
	mat._43 = -500.0f;

	DebugShapes::RenderSphere (mat);												 

	mat._41 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMinPoint2D().x;
	mat._42 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMinPoint2D().y;
	mat._43 = -500.0f;

	DebugShapes::RenderSphere (mat);												 

	mat._41 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMinPoint2D().x;
	mat._42 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMaxPoint2D().y;
	mat._43 = -500.0f;

	DebugShapes::RenderSphere (mat);												 

	mat._41 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMaxPoint2D().x;
	mat._42 = CKdTree::m_pMainTree->debugLeafTest[LeafIndex]->GetBounds().GetMinPoint2D().y;
	mat._43 = -500.0f;

	DebugShapes::RenderSphere (mat);	
	}	

	CKdTree::m_pMainTree->debugLeafTest.clear();*/

	//float closest1, closest2 = closest1 = FLT_MAX;

	// loop through all the return objects and check collision with them.
	for (unsigned int ReturnIndex = 0; ReturnIndex < ReturnVector.size(); ReturnIndex++)
	{
		IBaseObject* _pObject = ((IBaseObject*)ReturnVector[ReturnIndex]);

		if ( _pObject->GetCollidableObject() )
		{
			switch ( _pObject->GetCollidableObject()->GetBodyType() )
			{
			case BT_STATIC:
				{
					StaticCollision (m_pParent, _pObject);
				}
				break;
			case BT_DYNAMIC:
				{
					DynamicCollision (m_pParent, _pObject);
				}
				break;
			}
		}
	}

	if(m_pSoundComponent)
	{
		//check the distance between me and the listener position
		vec3f lineTo = m_pSoundComponent->GetAudio()->GetListenerPosition() - m_pParent->GetWorldPos();
		float dist = dot_product(lineTo, lineTo);

		//if 
		if(dist > SOUND_RANGE/* || (dist > closest1 && dist > closest2)*/)
		{
			if(m_bSoundLoop)
			{
 				m_pSoundComponent->StopSound(SFX_HAZARD_GOOPIT_BLOOP);
				m_bSoundLoop = false;
			}
			m_bInRange = false;
		}
		else //if(dist < SOUND_RANGE && !m_bInRange/* || (dist < closest1 && dist < closest2)*/)
		{
			m_bInRange = true;
		}
	}
}

void CComponent_GooPitCollision::StaticCollision ( IBaseObject* Collider, IBaseObject* Collide )
{
	// check the volume type of the static object
	/*switch (Collide->GetCollidableObject()->GetVolumeType())
	{
	default:
		break;
	}*/
}

void CComponent_GooPitCollision::DynamicCollision ( IBaseObject* Collider, IBaseObject* Collide )
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
				if ( Collide->GetType () == OBJ_PLAYER)
				{
					CComponent_Health* playerHealth = (CComponent_Health*)Collide->GetComponent(ECOMP_HEALTH);
					CComponent_PlayerCollision* playerCollision = (CComponent_PlayerCollision*)Collide->GetComponent(ECOMP_COLLISION);
					if (playerHealth)
					{
						//playerCollision->SetPrevInGoo(playerHealth->GetInGooPit());
						playerCollision->SetInGoo(true);
						playerHealth->SetInGooPit (true);
						//playerCollision->SetSplashed (true);
					}
				}
				else
				{
					((CComponent_Collision*)Collide->GetComponent (ECOMP_COLLISION))->CleanForce ( vec2f( 0.0f, 1.0f ) * 30.0f * m_fElapsedTime);
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
				CComponent_Collision* CollideCollision = (CComponent_Collision*)(Collide->GetComponent (ECOMP_COLLISION));

				if ( Collide->GetType() == OBJ_GOO )
				{
					CComponent_Lil_Goo_AI* pLilGoo = (CComponent_Lil_Goo_AI*)Collide->GetComponent(ECOMP_AI);
					if(pLilGoo)
					{
						pLilGoo->SetInGoo(true);
					}
					//CollideCollision->SetCollidingGroundAlone (true);
					//PushOut (CollideCollision, Direction);
					//CollideCollision->CleanForce ( NegateVel * 4.0f * m_fElapsedTime);
					//CollideCollision->CleanForce ( vec2f( 0.0f, 1.0f ) * 2.0f * m_fElapsedTime);
					break;
				}

				vec3f temp = CollideCollision->GetVel().negate();
				vec2f NegateVel = vec2f (temp.x, temp.y);
				CollideCollision->CleanForce ( NegateVel * 4.0f * m_fElapsedTime);
				CollideCollision->CleanForce ( vec2f( 0.0f, 1.0f ) * 30.0f * m_fElapsedTime);

				// Apply some fluid friction.
				//CollideCollision->GetPhysicsBox()->ApplyUniformForce( ( CollideCollision->GetPhysicsBox()->GetCentroid() - CollideCollision->GetPhysicsBox()->GetPrevCentroid() ).negate(), 4.0f * m_fElapsedTime );
				// Then apply the upward push.
				//CollideCollision->GetPhysicsBox()->ApplyUniformForce( vec3f( 0.0f, 1.0f, 0.0f ), 30.0f * m_fElapsedTime );
			}
		}
		break;
	default:
		break;
	}
}

void CComponent_GooPitCollision::PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction )
{
	// Move them by that vector.
	_pObjectCollision->CleanOffset (Direction);

	// Create a nortmal of that movement.
	if (dot_product (Direction, Direction))
		Direction.normalize();

	// see if it is the the ground.
	if( dot_product( Direction, vec2f( 0.0f, 1.0f ) ) > 0.5f )
	{
		//_pObjectCollision->GetVel().y = 0.0f;
		_pObjectCollision->SetCollidingGroundAlone (true);
	}
	// Check left and right for wall collision.
	if( abs( dot_product( Direction, vec2f( 1.0f, 0.0f ) ) ) > 0.5f )
	{
		_pObjectCollision->GetVel().x = 0.0f;
		_pObjectCollision->SetCollidingWall (true);
	}
}