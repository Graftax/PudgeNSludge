//////////////////////////////////////////////////////////////////////////////////////
// Filename:            CComponent_Collision.h
// Author:              Ethan Pendergraft
// Date:                5/9/12
// Purpose:             Defines the collisions and physics.
//////////////////////////////////////////////////////////////////////////////////////

#include "CComponent_BossCollision.h"

#include "CComponent_Health.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "..//PS/States/GamePlayState.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "../RenderEngine/TextureManager.h"

CComponent_BossCollision::CComponent_BossCollision()
{
	m_bPrevInGoo = false;
	m_bInGoo = false;
	m_pSoundComponent = nullptr;
	m_pHealthComponent = nullptr;
	m_pAnimationComponent = nullptr;
	m_UFOAABB = nullptr;
	this->CleanRemoveVelocity();
}

CComponent_BossCollision::~CComponent_BossCollision(void)
{

}

bool CComponent_BossCollision::Initialize ()
{
	m_bPrevInGoo = false;
	m_bInGoo = false;

	CComponent_Collision::Initialize();
	m_bPrevInGoo = false;
	m_bInGoo = false;
	m_fTimeLeftToIgnorePlayer = 0.0f;

	m_UFOAABB = (AABB*)m_pParent->GetCollidableObject();
	m_SludgeAABB.SetWorldPoint ( m_UFOAABB->GetWorldPoint() );
	m_SludgeAABB.SetMaxPoint ( vec3f (25.0f, 110.0f, 0.0f ) );
	m_SludgeAABB.SetMinPoint ( vec3f (-25.0f, -14.0f, 0.0f ) );

	m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));

	m_pAnimationComponent = (CComponent_Animation*)(m_pParent->GetComponent(ECOMP_ANIMATION));

	// Get the players health component
	m_pHealthComponent = (CComponent_Health*)m_pParent->GetComponent( ECOMP_HEALTH );

	CleanRemoveVelocity();

	RedTexture = TextureManager::GetInstance()->LoadTexture ("./Resources/Textures/RedTexture.png");

	CComponent_Render* rendComp = (CComponent_Render*)m_pParent->GetComponent(ECOMP_RENDER);
	m_pShipNode = rendComp->GetRenderNode ();
	OrignalShipTexture = m_pShipNode->GetRenderData().m_uiTextureHandles[0];

	IBaseObject* sludge = CObjectManager::GetInstance()->GetSludge();
	if (sludge)
	{
		rendComp = (CComponent_Render*)sludge->GetComponent(ECOMP_RENDER);
		m_pSludgeNode = rendComp->GetRenderNode ();
		OrignalSludgeTexture = m_pSludgeNode->GetRenderData().m_uiTextureHandles[0];
	}

	m_fFlashTimer = 0.0f;
	m_fElapsedTime = 0.0f;
	m_bFlashSludge = false;
	m_bFlashShip = false;

	m_pSmokeEffect = CParticleManager::GetInstance()->CreateEffect( "UFODamage", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_pExplosionFireEffect = CParticleManager::GetInstance()->CreateEffect( "UfoExplosion", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_pExplosionSparksEffect = CParticleManager::GetInstance()->CreateEffect( "UfoExplosionSparks", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_pExplosionSmokeEffect = CParticleManager::GetInstance()->CreateEffect( "UfoExplosionSmoke", vec3f( 0.0f, 0.0f, 0.0f ) );

	m_fSparksTimerCap = 0.0f;
	m_fSparksTimer = 0.0f;

	return true;
}

void CComponent_BossCollision::Update( float fDT )
{
	if( !m_pParent || !m_pParent->GetCollidableObject() || m_pParent->GetCollidableObject()->GetVolumeType () != VMT_AABB)
	{
		return;
	}

	m_fElapsedTime = fDT;

	//m_pHealthComponent->AddSludge( 5.0f * fDT );

	/*if( GetAsyncKeyState( VK_TAB ) )
	{
		CParticleManager::GetInstance()->CreateEffectOneShot( "UfoExplosion", GetParent()->GetWorldPos() );
		CParticleManager::GetInstance()->CreateEffectOneShot( "UfoExplosionSparks", GetParent()->GetWorldPos() );
		CParticleManager::GetInstance()->CreateEffectOneShot( "UfoExplosionSmoke", GetParent()->GetWorldPos() + vec3f( 0.0f, 0.0f, 80.0f ) );					
	}*/

	float percentage = 0.0f;
	if (m_pHealthComponent)
	{
		percentage = m_pHealthComponent->GetHealth() / 100.0f;

		m_pSmokeEffect->m_nParticleRate = (int)( ( 1.0f - percentage ) * ( 150.0f ));
		m_fSparksTimerCap = 2.0f * ( percentage );
		if( m_fSparksTimerCap < 0.3f )
			m_fSparksTimerCap = 0.3f;

		// Max Position
		vec3f MaxPos = vec3f( GetParent()->GetWorldPos() + vec3f( 30.0f, 10.0f, 0.0f));
		// Min Position
		vec3f MinPos = vec3f( GetParent()->GetWorldPos() - vec3f( 30.0f, 10.0f, 0.0f));

		m_pSmokeEffect->m_fPosition = GetParent()->GetWorldPos();
		m_pSmokeEffect->Emit( fDT );

		m_fSparksTimer += m_fElapsedTime;


		if( m_fSparksTimer >= m_fSparksTimerCap )
		{
			vec3f Position = vec3f( RAND_FLOAT(MinPos.x, MaxPos.x ),
									RAND_FLOAT(MinPos.y, MaxPos.y ),
									RAND_FLOAT(MinPos.z, MaxPos.z ) );
			m_fSparksTimer = 0.0f;
			CParticleManager::GetInstance()->CreateEffectOneShot( "Sparks", Position, vec3f( 0.5f, 0.5f, 0.0f ) );
			CParticleManager::GetInstance()->CreateEffectOneShot( "Sparks", Position, vec3f( -0.5f, 0.5f, 0.0f ) );

		}

		/*m_pShipSparks->m_fPosition.x = RAND_FLOAT(MinPos.x, MaxPos.x );
		m_pShipSparks->m_fPosition.y = RAND_FLOAT(MinPos.y, MaxPos.y );
		m_pShipSparks->m_fPosition.z = RAND_FLOAT(MinPos.z, MaxPos.z );*/

		/*m_pShipSparks->Emit( fDT );*/

		if( m_pHealthComponent->GetHealth() < 1 )
		{
			m_pExplosionFireEffect->m_fPosition =  GetParent()->GetWorldPos();
			m_pExplosionFireEffect->Emit( fDT );

			m_pExplosionSparksEffect->m_fPosition = GetParent()->GetWorldPos();
			m_pExplosionSparksEffect->Emit( fDT );

			m_pExplosionSmokeEffect->m_fPosition = GetParent()->GetWorldPos();
			m_pExplosionSmokeEffect->Emit( fDT );
		}

	}

	if( m_bFlashShip && m_fFlashTimer > 0.0f )
	{
		m_fFlashTimer -= fDT;
		if( m_fFlashTimer <= 0.0f ) 
		{
			m_pShipNode->GetRenderData().m_uiTextureHandles[0] = OrignalShipTexture;
			m_bFlashShip = false;
		}
	}

	if( m_bFlashSludge && m_fFlashTimer > 0.0f )
	{
		m_fFlashTimer -= fDT;
		if( m_fFlashTimer <= 0.0f ) 
		{
			m_pSludgeNode->GetRenderData().m_uiTextureHandles[0] = OrignalSludgeTexture;
			m_bFlashSludge = false;
		}
	}

	m_bCollidingWall = false;
	m_bCollidingGroundLast = m_bCollidingGround;
	m_bCollidingGround = false;

	vec3f fAverage;
	m_fPos = m_pParent->GetWorldPos ();

	m_fVel += vec3f( 0.0f, 4.0f, 0.0f ) * fDT;

	if( m_fVel.y < -20.0f )
		m_fVel.y = -20.0f;
	if( m_fVel.y >  5.0f )
		m_fVel.y =  5.0f;

	if( m_fVel.x < -6.0f )
		m_fVel.x = -6.0f;
	if( m_fVel.x >  6.0f )
		m_fVel.x =  6.0f;

	m_fPos += m_fVel;
	fAverage = m_fPos;

	m_pParent->SetLocalPos(&fAverage);

	vec3f NewCenter = m_pParent->GetWorldPos();
	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (NewCenter);
	m_SludgeAABB.SetWorldPoint (NewCenter);

	CKdTree::m_pMainTree->debugLeafTest.clear();

	CComponent_Collision* pulledCol = NULL;
	/*if ((CComponent_Collision*)cmpInput->GetPulledObject ())
	pulledCol = (CComponent_Collision*)cmpInput->GetPulledObject ()->GetComponent(ECOMP_COLLISION);*/

	Sphere CollisionSphere;
	CollisionSphere.SetCenter (m_pParent->GetWorldPos());
	CollisionSphere.SetRadius (400.0f);

	CSceneObject CollisionTest;
	CollisionTest.SetCollidableObject(&CollisionSphere);

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObject = (1<<OBJ_WORLD_COLLISION) | (1<<OBJ_PLAYER) | (1<<OBJ_GOO) | (1<<OBJ_CRATE);

	// call the kd tree and get the near objects
	CKdTree::GetNearObjects ( &CollisionTest, PSFLAG_SPHERE, ReturnParams, NearestObjects, ReturnBody, ReturnObject);

	CollisionTest.SetCollidableObject(NULL);

	// loop through all the return objects and check collision with them.
	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the collision component of the object
		CComponent_Collision* curCollision = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		IBaseObject* curObj = (IBaseObject*)NearestObjects[objindex];
		// get the collision volume of the object
		CCollisionVolume* curVolume = NearestObjects[objindex]->GetCollidableObject();		

		if (curObj == m_pParent || !curObj->GetAlive())
		{
			continue;
		}

		// Safety Check
		if( curVolume )
		{
			// Is the object from the Kd Tree Static or Dynamic
			switch( curVolume->GetBodyType() )
			{
			case BT_STATIC:
				{
					CheckStaticCollision ( curObj );
					break;
				}
			case BT_DYNAMIC:
				{
					CheckDynamicCollision ( curObj );
					break;
				}
			default:
				break;
			};
		}

		// Update the player collision volume every time the player collides with something
		((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (m_fPos);
	}

	// Update the position after everything is done.
	m_pParent->SetLocalPos(&m_fPos);

	// cap the objects z axis position and velocity
	m_fPos.z = 0.0f;
	m_fVel.z = 0.0f;
}

void CComponent_BossCollision::Shutdown(void)
{
	if (m_pAnimationComponent)
		m_pAnimationComponent->Shutdown();

	CParticleManager::GetInstance()->DestroyEffect( m_pSmokeEffect );
	CParticleManager::GetInstance()->DestroyEffect( m_pExplosionFireEffect );
	CParticleManager::GetInstance()->DestroyEffect( m_pExplosionSparksEffect );
	CParticleManager::GetInstance()->DestroyEffect( m_pExplosionSmokeEffect );

	m_bPrevInGoo = false;
	m_bInGoo = false;
	m_pSoundComponent = nullptr;
	m_pHealthComponent = nullptr;
	m_pAnimationComponent = nullptr;
	m_UFOAABB = nullptr;
	this->CleanRemoveVelocity();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckDynamicCollisions()
// Purpose: Check collisions with all dynamic objects
// Original Author: Rueben Massey
// Creation Date: 6/4/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_BossCollision::CheckDynamicCollision ( IBaseObject* _pCollide )
{
	vec2f Direction = vec2f(0.0f, 0.0f);
	vec2f CP = vec2f(0.0f, 0.0f);

	D3DXMATRIX mat;
	D3DXMatrixIdentity (&mat);

	if (!_pCollide->GetCollidableObject())
	{
		return;
	}

	switch (_pCollide->GetCollidableObject()->GetVolumeType ())
	{
	case VMT_BOX:
		{
			// if a little goo hit us, take some damage
			// if a box hits us just move
			if ( ((Box*)_pCollide->GetCollidableObject())->BoxToAABB ( (m_SludgeAABB), CP, Direction) )
			{
				CComponent_Collision* _pCollideCollision = (CComponent_Collision*)_pCollide->GetComponent (ECOMP_COLLISION);
				if (_pCollide->GetType() == OBJ_GOO)
				{
					if ( ((CComponent_LittleGooCollision*)_pCollideCollision)->GetVolitile () )
					{
						m_fFlashTimer = 0.2f;
						m_bFlashSludge = true;
						m_pSludgeNode->GetRenderData().m_uiTextureHandles[0] = RedTexture;
						if (m_pHealthComponent)
							m_pHealthComponent->TakeDamage (3.0f);

						if(m_pSoundComponent)
						{
							m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_DLG_BOSS_HURT);			
						}

						_pCollide->SetAlive (false);
					}

					CComponent_Item* _CollideItem = ((CComponent_Item*)_pCollide->GetComponent (ECOMP_ITEM));
					if ( _CollideItem && _CollideItem->GetSpawnIgnoreTime() > 0.0f)
					{
						return;
					}
				}
				PushOut ( _pCollideCollision, Direction );
				return;
			}

			if ( ((Box*)_pCollide->GetCollidableObject())->BoxToAABB ( *(m_UFOAABB), CP, Direction) )
			{
				CComponent_Collision* _pCollideCollision = (CComponent_Collision*)_pCollide->GetComponent (ECOMP_COLLISION);
				if (_pCollide->GetType() == OBJ_GOO)
				{
					if ( ((CComponent_LittleGooCollision*)_pCollideCollision)->GetVolitile () )
					{
						m_fFlashTimer = 0.2f;
						m_bFlashShip = true;
						m_pShipNode->GetRenderData().m_uiTextureHandles[0] = RedTexture;
						if (m_pHealthComponent)
							m_pHealthComponent->TakeDamage (1.6f);

						if(m_pSoundComponent)
						{
							m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_DLG_BOSS_HURT);			
						}

						_pCollide->SetAlive (false);
					}

					CComponent_Item* _CollideItem = ((CComponent_Item*)_pCollide->GetComponent (ECOMP_ITEM));
					if ( _CollideItem && _CollideItem->GetSpawnIgnoreTime() > 0.0f)
					{
						return;
					}
				}

				PushOut ( _pCollideCollision, Direction );
			}
			
		}
		break;
	case VMT_AABB:
		{
			vec3f ClosestPoint = vec3f (0.0f, 0.0f, 0.0f);
			// if anything else hits us move
			if ( ((AABB*)_pCollide->GetCollidableObject())->AABBToAABB ( (m_SludgeAABB), Direction) )
			{
				CComponent_Collision* CollideCollision = (CComponent_Collision*)_pCollide->GetComponent (ECOMP_COLLISION);

				// relpace this with a knock back so pudge is not on the ship
				if (Direction.y > 0.0f)
				{
					CComponent_Health* health = (CComponent_Health*)_pCollide->GetComponent(ECOMP_HEALTH);
					if ( health &&  _pCollide->GetType() == OBJ_PLAYER )
					{
						//TODO: Place Spark effect or something here
						health->AddSludge( 10.0f * m_fElapsedTime );
					}
					float fSided =  _pCollide->GetWorldPos().x - m_pParent->GetWorldPos().x;
					// Normalize the float!
					if( fSided > 0.0f )
						fSided = 1.0f;
					else
						fSided = -1.0f;

					Direction.x += fSided * 10.0f;
				}

				PushOut (CollideCollision , Direction);
			}

			ClosestPoint = vec3f (0.0f, 0.0f, 0.0f);
			// if anything else hits us move
			if ( ((AABB*)_pCollide->GetCollidableObject())->AABBToAABB ( *(m_UFOAABB), Direction) )
			{
				CComponent_Collision* CollideCollision = (CComponent_Collision*)_pCollide->GetComponent (ECOMP_COLLISION);

				// replace this with a knock back so pudge is not on the ship
				if (Direction.y > 0.0f)
				{
					CComponent_Health* health = (CComponent_Health*)_pCollide->GetComponent(ECOMP_HEALTH);
					if ( health &&  _pCollide->GetType() == OBJ_PLAYER )
					{
						//TODO: Place Spark effect or something here
						health->AddSludge( 10.0f * m_fElapsedTime );
					}
					float fSided =  _pCollide->GetWorldPos().x - m_pParent->GetWorldPos().x;
					// Normalize the float!
					if( fSided > 0.0f )
						fSided = 1.0f;
					else
						fSided = -1.0f;

					Direction.x += fSided * 10.0f;
				}
					PushOut (CollideCollision , Direction);
				}
		}
		break;
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
void CComponent_BossCollision::CheckStaticCollision( IBaseObject* _pCollide )
{
	vec2f Direction;
	vec3f CP;

	if (!_pCollide->GetCollidableObject())
	{
		return;
	}

	switch (_pCollide->GetCollidableObject()->GetVolumeType ())
	{
	case VMT_AABB:
		{
				if( (m_UFOAABB)->AABBToAABB( *((AABB*)_pCollide->GetCollidableObject()), Direction) )
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

					if( m_pHealthComponent )
					{
						if( m_pHealthComponent->GetHealth() < 1 )
						{
							//CParticleManager::GetInstance()->CreateEffectOneShot( "UFODamage", GetParent()->GetWorldPos() );
							/*CParticleManager::GetInstance()->CreateEffectOneShot( "UfoExplosion", GetParent()->GetWorldPos() );
							CParticleManager::GetInstance()->CreateEffectOneShot( "UfoExplosionSparks", GetParent()->GetWorldPos() );
							CParticleManager::GetInstance()->CreateEffectOneShot( "UfoExplosionSmoke", GetParent()->GetWorldPos() + vec3f( 0.0f, 0.0f, 80.0f )  );*/
							/*if(m_pSoundComponent)
							{			
								m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_BOSS_DEATH_EXPLOSION);			
							}*/
						}
					}
				}
		}
		break;
	}
}

void CComponent_BossCollision::PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction )
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
		//_pObjectCollision->SetCollidingWall (true);
	}
}