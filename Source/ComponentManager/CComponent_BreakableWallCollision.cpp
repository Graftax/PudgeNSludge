//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_BreakableWallCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../RenderEngine/TextureManager.h"
#include "CComponent_Sound.h"

// Icon
static short m_sIconCount = 0;

CComponent_BreakableWallCollision::CComponent_BreakableWallCollision()
{
	IsBroken = false;
	m_pSoundComponent = nullptr;
	m_pCollisionObject = nullptr;

	this->CleanRemoveVelocity();

	// Icon
	m_pIcon = nullptr;
	m_sCount = 0;
}

CComponent_BreakableWallCollision::~CComponent_BreakableWallCollision(void)
{

}

bool CComponent_BreakableWallCollision::Initialize ()
{
	//DebugShapes::CreateSphere (10.0f, 50, 50);
	IsBroken = false;
	this->CleanRemoveVelocity();
	CComponent_Collision::Initialize();
	m_vAssociatedObjs.clear ();

	m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));		

	// Set the collision object 
	// to the parent's collision object 
	m_pCollisionObject = (AABB*)m_pParent->GetCollidableObject();		

	m_pIcon = CIcon::GetInstance();
	m_nIconTexture = TextureManager::GetInstance()->LoadTexture( "./Resources/HUDImages/iconcrate1.png" );
	m_nIconTexture2 = TextureManager::GetInstance()->LoadTexture( "./Resources/HUDImages/iconcrate2.png" );
	m_sIconCount += 1;
	m_sCount = m_sIconCount;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update():	Updates the collison component, incrementing its physics and detecting collision with other objects.
//
// Returns:		Void
//
// Mod. Name:   Josh Fields
// Mod. Date:   5/16/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BreakableWallCollision::Update( float fDT )
{	
	vec3f NewCenter = m_pParent->GetWorldPos();
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint(NewCenter);

	if( m_sCount < 2 )
	{
		D3DXMATRIX scalemat;
		D3DXMatrixIdentity( &scalemat );
		D3DXMatrixScaling( &scalemat, 150.0f, 100.0f, 0.0f );
		m_pIcon->Render( m_nIconTexture, m_nIconTexture2, m_pParent->GetFrame().GetWorldMat().axis_pos + vec3f(-175.0f, 0.0f, 0.0f), scalemat );
	}

	CheckCollisions( fDT );	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown():	destories all class objects
//
// Returns:		Void
//
// Mod. Name:   Josh Fields
// Mod. Date:   5/16/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BreakableWallCollision::Shutdown(void)
{
	IsBroken = false;
	m_pSoundComponent = nullptr;
	m_pCollisionObject = nullptr;
	m_vAssociatedObjs.clear();
	m_pIcon->DeleteInstance();
	m_pIcon = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckCollisions()
// Purpose: Check for collision	and react appropriately
// Original Author: Rueben Massey
// Creation Date: 6/13/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BreakableWallCollision::CheckCollisions( float fDT )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_GOO) | (1<<OBJ_MONSTER);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects (m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects, ReturnBody, ReturnObjects);


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
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Rueben Massey
// Creation Date: 6/11/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BreakableWallCollision::CheckDynamicCollisions( CComponent_Collision* pCurCollision, CCollisionVolume* pCurVolume, float fDT )
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
				// get the item component of this object
				CComponent_Item* itemcomp = (CComponent_Item*)colParent->GetComponent(ECOMP_ITEM);

				// if its being pulled
				if( itemcomp && itemcomp->GetTarget() )
					{
						// stop the player from pulling
						itemcomp->SetTarget( NULL );
					}

				switch( pCurCollision->GetParent()->GetType() )
				{
				case OBJ_CRATE:
					{
						// This is a crate so lets see if we can break the wall.
						float temp = dot_product( pCurCollision->GetVel(), pCurCollision->GetVel() );
						if( temp > 50.0f )
						{
							vec2f Direction;
							vec2f CP;							
							m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_BREAKABLE_WALL_CRUMBLE);
							CObjectManager::GetInstance()->AddObjectToRemove( m_pParent );

							if( GetParent()->GetAlive() )
							{
								CParticleManager::GetInstance()->CreateEffectOneShot( "BreakableWall", GetParent()->GetWorldPos() );

								D3DXMATRIX pos;
								D3DXMatrixIdentity( &pos );
								D3DXMatrixTranslation( &pos, GetPos().x, GetPos().y, GetPos().z );

								unsigned int chunkAmount = rand() % 2;
								chunkAmount++;

								for( unsigned int i = 0; i < chunkAmount; ++i )
								{
									(( CComponent_Collision*)( CObjectManager::GetInstance()->CreateObject( OBJ_WALLCHUNK, pos, GetParent()->GetLevelID() )->GetComponent( ECOMP_COLLISION )))->CleanForce( vec2f( pCurCollision->GetVel().x, pCurCollision->GetVel().y ) * 0.5f );
								}
								GetParent()->SetAlive( false );

								for (unsigned int Index = 0; Index < this->m_vAssociatedObjs.size(); Index++)
								{
									D3DXMatrixIdentity( &pos );
									D3DXMatrixTranslation( &pos, m_vAssociatedObjs[Index]->GetWorldPos().x, m_vAssociatedObjs[Index]->GetWorldPos().y, m_vAssociatedObjs[Index]->GetWorldPos().z );

									unsigned int chunkAmount = rand() % 2;
									chunkAmount++;

									for( unsigned int i = 0; i < chunkAmount; ++i )
									{
										(( CComponent_Collision*)( CObjectManager::GetInstance()->CreateObject( OBJ_WALLCHUNK, pos, GetParent()->GetLevelID() )->GetComponent( ECOMP_COLLISION )))->CleanForce( vec2f( pCurCollision->GetVel().x, pCurCollision->GetVel().y ) * 0.5f );
									}
									m_vAssociatedObjs[Index]->SetAlive( false );

									CParticleManager::GetInstance()->CreateEffectOneShot( "BreakableWall", m_vAssociatedObjs[Index]->GetWorldPos() );
									CObjectManager::GetInstance()->AddObjectToRemove (m_vAssociatedObjs[Index]);
								}
							}
							return;
						}

						// Apply push-out
						pCurCollision->CleanOffset( Direction );
						pCurCollision->CleanForce( Direction * 0.5f );
					}
					break;
				case OBJ_GOO:
					{
						// special case for Lil Goos
						CComponent_LittleGooCollision* pLilGoo = (CComponent_LittleGooCollision*)pCurCollision;
						
						if(pLilGoo->GetVolitile() && pLilGoo->GetParent()->GetAlive())
						{
							pLilGoo->GetParent()->SetAlive(false);
							pCurCollision->CleanRemoveVelocity();
						}

						// Apply push-out
						pCurCollision->CleanOffset( Direction );
						pCurCollision->CleanForce( Direction * 0.5f );
					}
					break;
				default:
					{
						// Apply push-out
						pCurCollision->CleanOffset( Direction );
						pCurCollision->CleanForce( Direction * 0.5f );
					}
				}
			}
			break;
		}
	default:
		break;
	};
}