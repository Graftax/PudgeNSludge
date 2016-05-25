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
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../RenderEngine/TextureManager.h"
#include "../EffectsLib/ParticleManager.h"

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
	m_fHealAmount = 10.0f;
	m_pSoundCmp = nullptr;
	m_bPlayerHeal = false;
	m_fAmountHealed = 0.0f;
	m_szMeshNodeName = "";
	m_bTransitioned = false;
	m_pOwner = nullptr;
	m_pSparkleEmitter = nullptr;
	m_pPlayerHealth = nullptr;
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
	m_fHealAmount = 10.0f;
	m_fAmountHealed = 0.0f;
	m_bPlayerHeal = false;
	m_pSoundCmp = nullptr;
	m_bTransitioned = false;
	m_pSparkleEmitter = CParticleManager::GetInstance()->CreateEffect( "FullHealthSparkles", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_vAssociatedObjs.clear();

	m_pSoundCmp = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));

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
	m_vAssociatedObjs.clear ();

	if( m_pSparkleEmitter )
		CParticleManager::GetInstance()->DestroyEffect( m_pSparkleEmitter );

	m_fHealAmount = 10.0f;
	m_pSoundCmp = nullptr;
	m_bPlayerHeal = false;
	m_fAmountHealed = 0.0f;
	m_szMeshNodeName = "";
	m_bTransitioned = false;
	m_pOwner = nullptr;
	m_pSparkleEmitter = nullptr;
	m_pPlayerHealth = nullptr;
}
void CComponent_WaterPoolCollision::SetMeshName( string _mesh )
{ 
	m_szMeshNodeName = _mesh;	
	m_pOwner = MeshManager::GetInstance()->GetRenderNode( m_szMeshNodeName.c_str() );
	if( m_pOwner )
	{
		CComponent_Render* test = (CComponent_Render*)m_pParent->GetComponent( ECOMP_RENDER );
		test->SetRenderNode( m_pOwner );
		m_pOwner->GetRenderData().m_uiTextureHandles[BURNDOWN_TEX] = TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/T_WaterPoolDirty_D.png" );
		m_pOwner->SetExtraData( 1.0f );
	} 
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

	if( CObjectManager::GetInstance()->GetPlayer() && !m_pPlayerHealth )
	{
		m_pPlayerHealth = (CComponent_Health*)CObjectManager::GetInstance()->GetPlayer()->GetComponent( ECOMP_HEALTH );
	}

	// update the parents position
	m_pParent->GetWorldMat()->axis_pos = m_fPos;
	// update the collision volumes postion
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (m_pParent->GetWorldMat()->axis_pos);

	if( m_bPlayerHeal )
	{ 		
		if( m_fAmountHealed > m_fHealAmount )
		{
			if(m_pSoundCmp)
			{
				m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_WATERPOOL_HEAL_LOOP);
			}
			m_pOwner->SetExtraData( -1.0f );
			return;
		}

		if( m_pOwner == nullptr )
			m_pOwner = MeshManager::GetInstance()->GetRenderNode( m_szMeshNodeName.c_str() );
		m_pOwner->AddFlags( RE_BURNTEX | RE_BURNOWNER );
		m_fAmountHealed += m_fHealAmount * _fElapsedTime;
		m_pOwner->SetExtraData( 1.0f - (m_fAmountHealed / m_fHealAmount) );

		for (unsigned int Index = 0; Index < this->m_vAssociatedObjs.size(); Index++)
		{
			CComponent_WaterPoolCollision* _AssWaterPoolCol = (CComponent_WaterPoolCollision*)m_vAssociatedObjs[Index]->GetComponent (ECOMP_COLLISION);
			_AssWaterPoolCol->SetAmountHealed (m_fAmountHealed);
			_AssWaterPoolCol->SetPlayerHealed (false);
		}

		m_pPlayerHealth->RemoveSludge( m_fHealAmount * _fElapsedTime );

		m_bPlayerHeal = false;
	}
	// this can be removed when we no longer need the debug leafs to show were and object is in the kd tree
	CKdTree::m_pMainTree->debugLeafTest.clear();

	if( m_pSparkleEmitter )
	{
		m_pSparkleEmitter->m_nParticleRate = (int)(m_fHealAmount - m_fAmountHealed);
		m_pSparkleEmitter->m_fPosition = GetParent()->GetWorldPos() + vec3f( 0.0f, 85.0f, 0.0f );
		m_pSparkleEmitter->Emit( _fElapsedTime );
	}
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
					//StaticCollision ( m_pParent, _pObject );
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
					CComponent_Collision* CollideCollision = (CComponent_Collision*)(Collide->GetComponent (ECOMP_COLLISION));
					CComponent_Sound* pSound = (CComponent_Sound*)Collide->GetComponent(ECOMP_SOUND);

					CollideCollision->SetInGoo(true);

					if(pSound && !CollideCollision->GetPrevInGoo())
					{
						pSound->PlaySound(AK::EVENTS::PLAY_SFX_PLAYER_SPLASH);
					}

					CComponent_Health* PlayerHealth = ((CComponent_Health*)Collide->GetComponent (ECOMP_HEALTH));
					if (PlayerHealth && !m_bPlayerHeal && PlayerHealth->GetHealth() < 100.0f)
					{
						m_bPlayerHeal = true;

						if(m_pSoundCmp)
						{
							m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_WATERPOOL_HEAL_LOOP);
						}
					}
					else
					{
						if(m_pSoundCmp)
						{
							m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_WATERPOOL_HEAL_LOOP);
						}
					}
				}
				else
				{
					// push out the other collision volume
					CComponent_Collision* CollideCollision = (CComponent_Collision*)Collide->GetComponent (ECOMP_COLLISION);
					PushOut ( CollideCollision, Direction );
				}
			}
			else
			{
				if(m_pSoundCmp)
				{
					m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_WATERPOOL_HEAL_LOOP);
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

				CComponent_Sound* pSound = (CComponent_Sound*)Collide->GetComponent(ECOMP_SOUND);

				CollideCollision->SetInGoo(true);

				if(pSound && !CollideCollision->GetPrevInGoo())
				{
					pSound->PlaySound(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH);
				}

				// special case for Lil Goos
				if(Collide->GetType() == OBJ_GOO)
				{
					CComponent_LittleGooCollision* pCollision = (CComponent_LittleGooCollision*)Collide->GetComponent(ECOMP_COLLISION);

					if(Collide->GetAlive())
					{
						Collide->SetAlive(false);
						pCollision->CleanRemoveVelocity();
					}
				}	
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