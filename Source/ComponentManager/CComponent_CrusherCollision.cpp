#include "CComponent_CrusherCollision.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../Utilities/CollisionLib/CollisionAABB.h"
#include "CComponent_Health.h"
#include "CComponent_PlayerCollision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "CComponent_Sound.h"

#include "../AnimationLib/AnimationIncludes.h"
#include "../EffectsLib/ParticleManager.h"

#define CLICK_TIME 0.1f

CComponent_CrusherCollision::CComponent_CrusherCollision(void)
{
	m_bIsCrusherUp = true;								// Crusher starts on its up state.
	m_bIsCrusherCrushing = false;						// Determines whether the crusher is falling and rising.
	m_fLengthInGameUnits = m_fCrusherSpeed = m_fCrusherDropDuration = 0.0f;
	m_fDurationTimer = m_fFrequencyTimer = 0.0f;		// Reset the Timers.

	this->CleanRemoveVelocity();

	m_pSoundCmp = nullptr;
	m_pCollisionComp = nullptr;
	m_bCollided = false;
	m_fClickTimer = 0.0f;

	m_bCrusherSparksCreated = false;
	m_fEffectToFrequencyTimerMax = 0.0f;
}

CComponent_CrusherCollision::~CComponent_CrusherCollision(void)
{

}

bool CComponent_CrusherCollision::Initialize(void)
{
	m_bIsCrusherUp = true;								// Crusher starts on its up state.
	m_bIsCrusherCrushing = false;						// Determines whether the crusher is falling and rising.
	m_fLengthInGameUnits = m_fCrusherSpeed = m_fCrusherDropDuration = 0.0f;
	m_fDurationTimer = m_fFrequencyTimer = 0.0f;		// Reset the Timers.
	// Hardcoding variable values.
	m_fCrusherDuration = 2.0f;							// Crusher drops and rises for 2 seconds.
	m_fCrusherFrequency = 1.0f;							// Crusher waits 2 seconds before drops again.
	m_nLength = 4;										// Crusher drops for a total of this Game Units.
	m_nDirection = 1;

	m_bCrusherSparksCreated = false;
	m_fEffectToFrequencyTimerMax = 0.0f;
	this->CleanRemoveVelocity();

	m_bCollided = false;
	m_fClickTimer = 0.0f;

	m_pSoundCmp = (CComponent_Sound*)m_pParent->GetComponent(ECOMP_SOUND);

	m_pCollisionComp = (CComponent_Collision*)m_pParent->GetComponent(ECOMP_COLLISION);
	m_pSteamEffect = CParticleManager::GetInstance()->CreateEffect( "SteamPipe", GetParent()->GetWorldPos() + vec3f( 0.0f, 0.0f, 0.0f ) );
	m_pSteamEffect->m_fScale /= 2.0f;
	m_pSteamEffect->m_fRampScaleMax /= 2.0f;
	m_pSteamEffect->m_fRampScaleMin /= 2.0f;
	m_fSteamDurationTimer = 10.0f;
	return true;
}

void CComponent_CrusherCollision::Shutdown( void )
{
	if(m_pSoundCmp)
	{
		m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP);
	}
	m_bIsCrusherUp = true;								// Crusher starts on its up state.
	m_bIsCrusherCrushing = false;						// Determines whether the crusher is falling and rising.
	m_fLengthInGameUnits = m_fCrusherSpeed = m_fCrusherDropDuration = 0.0f;
	m_fDurationTimer = m_fFrequencyTimer = 0.0f;		// Reset the Timers.

	this->CleanRemoveVelocity();

	m_pSoundCmp = nullptr;
	m_pCollisionComp = nullptr;
	m_bCollided = false;
	m_fClickTimer = 0.0f;

	m_bCrusherSparksCreated = false;
	m_fEffectToFrequencyTimerMax = 0.0f;

	CParticleManager::GetInstance()->DestroyEffect( m_pSteamEffect );
}

void CComponent_CrusherCollision::Update( float fDT )
{
	// Increment the timers and Handle State Changing.
	if( m_bIsCrusherCrushing )
	{
		m_fDurationTimer += fDT;
	}
	else
	{
		m_fFrequencyTimer += fDT;
	}

	m_fSteamDurationTimer += fDT;

	if( m_fSteamDurationTimer < 1.0f )
	{
		m_pSteamEffect->m_fPosition = GetParent()->GetWorldPos() + vec3f( 100.0f, 0.0f, 0.0f );
		m_pSteamEffect->m_fDirection = vec3f( 0.3f, -0.7f, 0.0f );
		m_pSteamEffect->Emit( fDT );
		m_pSteamEffect->m_fPosition = GetParent()->GetWorldPos() - vec3f( 100.0f, 0.0f, 0.0f );
		m_pSteamEffect->m_fDirection = vec3f( -0.3f, -0.7f, 0.0f );
		m_pSteamEffect->Emit( fDT );
	}

	// If the crusher is crushing.
	if( m_bIsCrusherCrushing )
	{

		// If the Crusher is Up.
		if( m_bIsCrusherUp )
		{
			// Calculate the ratio at which the Crusher is Crushing.
			m_fCrusherDropDuration = (m_fCrusherDuration * 0.25f);
			// Calculate the Length of how much the Crusher should move.
			m_fLengthInGameUnits = float(m_nLength * GAME_UNIT);
			// Calculate the Velocity at which the Crusher should move.
			m_fCrusherSpeed = -(m_fLengthInGameUnits / m_fCrusherDropDuration);
		}
		// If the Crusher is Down
		else
		{
			// Calculate the ratio at which the Crusher is Rising.
			float fCrusherRiseDuration = (m_fCrusherDuration - m_fCrusherDropDuration);
			// Calculate the Length of how much the Crusher should move.
			m_fLengthInGameUnits = float(m_nLength * GAME_UNIT);
			// Calculate the Velocity at which the Crusher should move.
			m_fCrusherSpeed = (m_fLengthInGameUnits / (fCrusherRiseDuration));

			//increment the click sound timer, and play when appropriate.
			m_fClickTimer += fDT;
			if(m_fClickTimer > CLICK_TIME)
			{
				if(m_pSoundCmp)
				{
					m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_RISING_LOOP);
				}
				m_fClickTimer = 0.0f;
			}

			m_bCollided = false;
		}

		// If the Crusher is Up and the Duration Timer is greater or equal to the Drop duration, then Stop dropping.
		if( m_bIsCrusherUp && (m_fDurationTimer >= m_fCrusherDropDuration ) )
		{
			m_bIsCrusherUp = false;
		}

		if( m_fCrusherDuration != 0.0f )
		{
			vec3f Velocity = m_pParent->GetFrame().GetLocalMat().axis_z;
			m_pParent->GetFrame().GetLocalMat().axis_pos += (Velocity * m_fCrusherSpeed) * fDT;
			m_pParent->GetFrame().Update();
		}
	}


	((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (m_pParent->GetWorldPos());
	m_bCrusherVolume.SetWorldPoint( m_pParent->GetWorldPos() );	

	// Declare a vector which will be passed in to the KDTree and will contain all the Objects near
	// the scene objects passed in.
	vector<CSceneObject*> ReturnVector;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnPrams = 0;
	int ReturnBody = 0;
	int ReturnObjects = (1<<OBJ_PLAYER) | (1<<OBJ_CRATE) | (1<<OBJ_CORK) | (1<<OBJ_PLUG) | (1<<OBJ_MONSTER) | (1<<OBJ_GOO) | (1<<OBJ_WORLD_COLLISION);

	// Grab the nearest objects from the KD Tree.
	CKdTree::GetNearObjects( m_pParent, PSFLAG_AABB, ReturnPrams, ReturnVector);
	// Loop through all the return objects and check collision with them.
	for (unsigned int ReturnIndex = 0; ReturnIndex < ReturnVector.size(); ReturnIndex++)
	{
		// Create a Pointer to the Object.
		IBaseObject* _pObject = ((IBaseObject*)ReturnVector[ReturnIndex]);

		if (_pObject == m_pParent)
		{
			continue;
		}

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

	if( m_bIsCrusherCrushing )
	{


		// If the duration is greater than the Crushers duration change the state.
		if( m_fDurationTimer > (m_fCrusherDuration + 0.2f) )
		{
			CParticleManager::GetInstance()->CreateEffectOneShot("Sparks", m_pParent->GetWorldPos(), vec3f( 0.5f, 0.5f, 0.0f ) );
			CParticleManager::GetInstance()->CreateEffectOneShot("Sparks", m_pParent->GetWorldPos(), vec3f( -0.5f, 0.5f, 0.0f ) );

			// Reset Timer
			m_fDurationTimer = 0.0f;

			// Else if the crusher is crushing and duration was reached.
			m_bIsCrusherCrushing = false;
			// If the Crusher is not up and the Duration Timer is >= the Duration, then set to Up.
			if( !m_bIsCrusherUp )
			{
				m_bIsCrusherUp = true;
				if(m_pSoundCmp)
				{
					m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP);

					m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_STEAM);
					m_fSteamDurationTimer = 0.0f;
					
				}
				m_bCollided = false;
			}
		}
	}
	else
	{
		// If the frequency is greater than the time the crusher should wait change the state.
		if( m_fFrequencyTimer >= m_fCrusherFrequency )
		{
			// Reset Timer
			m_fFrequencyTimer = 0.0f;

			// If the crusher is not crushing.
			// Set it to Crush.
			m_bIsCrusherCrushing = true;
			m_bCrusherSparksCreated = false;
		}
	}
}

void CComponent_CrusherCollision::StaticCollision ( IBaseObject* Collider, IBaseObject* Collide )
{
	// check the volume type of the static object
	switch (Collide->GetCollidableObject()->GetVolumeType())
	{
	case VMT_AABB:
		{
			// check collision with with our aabb and the dynamic aabb
			vec2f Direction;
			if ( m_bCrusherVolume.AABBToAABB( *(AABB*)Collide->GetCollidableObject(), Direction ) )
			{
				// Prepare a movement vector.
				vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);

				m_pParent->GetFrame().GetLocalMat().axis_pos += (MoveDirection);
				m_pParent->GetFrame().Update();

				if(m_pSoundCmp && !m_bCollided && m_bIsCrusherUp)
				{
					m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP);

					m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH);
					m_bCollided = true;
				}

				if( !m_bCrusherSparksCreated )
				{
					// Create the Effect.
					CParticleManager::GetInstance()->CreateEffectOneShot("Sparks", CrusherSparkPositions[0] + m_pParent->GetWorldPos(), vec3f( 1.0f, 0.0f, 0.0f ) );
					CParticleManager::GetInstance()->CreateEffectOneShot("ImpactSmoke", CrusherSparkPositions[0] + m_pParent->GetWorldPos(), vec3f( 1.0f, 0.0f, 0.0f ) );
					CParticleManager::GetInstance()->CreateEffectOneShot("Sparks",m_pParent->GetWorldPos() + vec3f( 0.0f, CrusherSparkPositions[0].y, 0.0f ) , vec3f( 0.0f, 1.0f, 0.0f ) );
					CParticleManager::GetInstance()->CreateEffectOneShot("ImpactSmoke", CrusherSparkPositions[1] + m_pParent->GetWorldPos(), vec3f( -1.0f, 0.0f, 0.0f ) );
					CParticleManager::GetInstance()->CreateEffectOneShot("Sparks",CrusherSparkPositions[1] + m_pParent->GetWorldPos(), vec3f( -1.0f, 0.0f, 0.0f ) );

					m_bCrusherSparksCreated = true;
				}
			}
		}
		break;
	default:
		break;
	}
}

void CComponent_CrusherCollision::DynamicCollision ( IBaseObject* Collider, IBaseObject* Collide )
{

	// check the volume type of the dynamic object
	switch (Collide->GetCollidableObject()->GetVolumeType())
	{
	case VMT_AABB:
		{
			// check collision with with our aabb and the dynamic aabb
			vec2f Direction;
			if ( ((AABB*)Collide->GetCollidableObject())->AABBToAABB ( m_bCrusherVolume, Direction ) )
			{
				if( Collide->GetType() == OBJ_PLAYER )
				{
					if( Collide->GetAlive() )
					{
						CComponent_Collision* CollideCollision = ((CComponent_Collision*)Collide->GetComponent(ECOMP_COLLISION));
						float magnitude = dot_product( Direction, Direction);
						if( (CollideCollision->CollidingGround() || CollideCollision->CollidingWall()) && magnitude > 400.0f)
						{
							Direction.y = -Direction.y;
							// Prepare a movement vector.
							vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);

							m_pParent->GetFrame().GetLocalMat().axis_pos += (MoveDirection);
							m_pParent->GetFrame().Update();

							CComponent_Health* health = (CComponent_Health*)Collide->GetComponent (ECOMP_HEALTH);
							if (health)
							{
								health->TakeDamage (100.0f);
								CollideCollision->GetVel().x = 0.0f;

								CComponent_Animation* pAnimComponent = (CComponent_Animation*)Collide->GetComponent( ECOMP_ANIMATION );
								if( pAnimComponent )
									pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeDeathByCrusherState::GetState() );
							}
						}
						else
						{
							vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);
							CComponent_PlayerCollision* pPlayerCol = (CComponent_PlayerCollision*) Collide->GetComponent( ECOMP_COLLISION );
							if( dot_product( MoveDirection, vec3f( 0.0f, 1.0f, 0.0f)) > 0.5f)
								pPlayerCol->SetCollidingGroundAlone(true);

							PushOut( CollideCollision, Direction);
						}


					}
				}
				else if( Collide->GetType() == OBJ_CRUSHERBASE )
				{
					Direction = Direction.negate();
					// Prepare a movement vector.
					vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);

					m_pParent->GetFrame().GetLocalMat().axis_pos += (MoveDirection);
					m_pParent->GetFrame().Update();

					if(m_pSoundCmp && !m_bCollided && m_bIsCrusherUp)
					{
						m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP);

						m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH);
						m_bCollided = true;
					}
				}
				else
					PushOut ( ((CComponent_Collision*)Collide->GetComponent(ECOMP_COLLISION)), Direction );

				((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (m_pParent->GetWorldPos());
				m_bCrusherVolume.SetWorldPoint( m_pParent->GetWorldPos() );
			}
			if( ((AABB*)Collide->GetCollidableObject())->AABBToAABB ( (*(AABB*)m_pParent->GetCollidableObject()), Direction ))
			{
				switch( Collide->GetType() )
				{
				case OBJ_PLAYER:
					{
						if( Collide->GetAlive() )
						{
							vec3f MoveDirection = vec3f (Direction.x, Direction.y, 0.0f);
							CComponent_PlayerCollision* pPlayerCol = (CComponent_PlayerCollision*) Collide->GetComponent( ECOMP_COLLISION );
							if( dot_product( MoveDirection, vec3f( 0.0f, 1.0f, 0.0f)) > 0.5f)
								pPlayerCol->SetCollidingGroundAlone(true);

							PushOut( pPlayerCol, Direction);
						}
						break;
					}
				};				
			}
		}
		break;
	case VMT_BOX:
		{
			// check collision with our aabb and the dynamic box
			vec2f Direction;
			vec2f CP;
			if ( ((Box*)Collide->GetCollidableObject())->BoxToAABB ( m_bCrusherVolume, CP, Direction ) )
			{
				IBaseObject* colParent = Collide;
				// get the item component of this object
				CComponent_Item* itemcomp = (CComponent_Item*)colParent->GetComponent(ECOMP_ITEM);

				// if its being pulled
				if( itemcomp && itemcomp->GetTarget() )
				{
					// stop the player from pulling
					itemcomp->SetTarget( NULL );
				}

				CComponent_Collision* CollideCollision = ((CComponent_Collision*)Collide->GetComponent(ECOMP_COLLISION));
				if (Collide->GetType () == OBJ_GOO)
				{
					CComponent_LittleGooCollision* pCollision = (CComponent_LittleGooCollision*)Collide->GetComponent(ECOMP_COLLISION);

					if(Collide->GetAlive())
					{
						Collide->SetAlive(false);
						pCollision->CleanRemoveVelocity();
					}
				}
				else
				{
					float magnitude = dot_product (Direction, Direction);
					if ( (CollideCollision->CollidingGround() || CollideCollision->CollidingWall()) && magnitude > 50.0f)
					{
						CollideCollision->GetVel().y = 0.0f;

						PushOut ( CollideCollision, vec2f (Direction.x, 0.0f) );

						Direction.y = -Direction.y;
						// Prepare a movement vector.
						vec3f MoveDirection = vec3f (0.0f, Direction.y, 0.0f);

						m_pParent->GetFrame().GetLocalMat().axis_pos += (MoveDirection);
						m_pParent->GetFrame().Update();	

						if(m_pSoundCmp && !m_bCollided && m_bIsCrusherUp)
						{
							m_pSoundCmp->StopSound(AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP);

							m_pSoundCmp->PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH);
							m_bCollided = true;
						}
					}
					else
					{
						PushOut ( CollideCollision, Direction );
					}
				}

				((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (m_pParent->GetWorldPos());
				m_bCrusherVolume.SetWorldPoint( m_pParent->GetWorldPos() );
			}
			else if(((Box*)Collide->GetCollidableObject())->BoxToAABB ( (*(AABB*)m_pParent->GetCollidableObject()), CP, Direction ))
			{
				CComponent_Collision* CollideCollision = ((CComponent_Collision*)Collide->GetComponent(ECOMP_COLLISION));
				PushOut ( CollideCollision, Direction );

				((AABB*)m_pParent->GetCollidableObject())->SetWorldPoint (m_pParent->GetWorldPos());
				m_bCrusherVolume.SetWorldPoint( m_pParent->GetWorldPos() );
			}
		}
		break;
	default:
		break;
	}
}

void CComponent_CrusherCollision::PushOut ( CComponent_Collision* _pObjectCollision, vec2f Direction )
{
	// Move them by that vector.
	_pObjectCollision->CleanOffset (Direction);
	_pObjectCollision->CleanForce( Direction );

	float magnitude = dot_product (Direction, Direction);
	// Create a nortmal of that movement.
	if (magnitude)
		Direction.normalize();

	// See if colliding with the bottom of the crusher pushes you down or up.
	if( dot_product( Direction, vec2f( 0.0f, -1.0f ) ) > 0.0f )
	{
		_pObjectCollision->GetVel().y = 0.0f;
	}

	if( dot_product( Direction, vec2f( 0.0f, 1.0f ) ) > 0.0f )
	{
		_pObjectCollision->GetVel().y = 0.0f;
	}

	if( abs(dot_product(Direction, vec2f( 1.0f, 0.0f) ) > 0.0f) )
	{
		_pObjectCollision->GetVel().x = 0.0f;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: BuildCrusherAABB()
// Purpose: Create an AABB for the Crusher
// Original Author: Andy Madruga
// Creation Date: 6/12/2012
// Last Modification By: 
// Last Modification Date: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_CrusherCollision::BuildCrusherAABB(void)
{
	switch( m_nDirection )
	{
		// Up
	case 0:
		{
			m_bCrusherVolume.SetMaxPoint( vec3f( 100.0f, 0.0f, 200.0f)   );
			m_bCrusherVolume.SetMinPoint( vec3f( -100.0f, -35.0f, -200.0f) );

			CrusherSparkPositions[0] = vec3f( 130.0f,   10.0f, 0.0f);
			CrusherSparkPositions[1] = vec3f( -130.0f,  10.0f, 0.0f);

			AABB* temp = (AABB*)m_pParent->GetCollidableObject();
			vec3f max = temp->GetMaxPoint3D();
			max.x -= 90.0f;
			temp->SetMaxPoint(max);

			vec3f min = temp->GetMinPoint3D();
			min.x += 90.0f;
			temp->SetMinPoint( min );


			break;
		}
		// Down
	case 1:
		{
			m_bCrusherVolume.SetMaxPoint( vec3f( 100.0f, 0.0f, 200.0f)   );
			m_bCrusherVolume.SetMinPoint( vec3f( -100.0f, -35.0f, -200.0f) );

			CrusherSparkPositions[0] = vec3f( 130.0f,   10.0f, 0.0f);
			CrusherSparkPositions[1] = vec3f( -130.0f,  10.0f, 0.0f);

			AABB* temp = (AABB*)m_pParent->GetCollidableObject();
			vec3f max = temp->GetMaxPoint3D();
			max.x -= 90.0f;
			temp->SetMaxPoint(max);

			vec3f min = temp->GetMinPoint3D();
			min.x += 90.0f;
			temp->SetMinPoint( min );

			break;
		}
		// Left
	case 2:
		{
			m_bCrusherVolume.SetMaxPoint( vec3f( 0.0f, 100.0f, 200.0f)     );
			m_bCrusherVolume.SetMinPoint( vec3f( -35.0f, -100.0f, -200.0f) );

			CrusherSparkPositions[0] = vec3f( 10.0f,   130.0f, 0.0f);
			CrusherSparkPositions[1] = vec3f( 10.0f,  -130.0f, 0.0f);

			AABB* temp = (AABB*)m_pParent->GetCollidableObject();
			vec3f max = temp->GetMaxPoint3D();
			max.y -= 90.0f;
			temp->SetMaxPoint(max);

			vec3f min = temp->GetMinPoint3D();
			min.y += 90.0f;
			temp->SetMinPoint( min );

			break;
		}
		// Right
	case 3:
		{
			m_bCrusherVolume.SetMaxPoint( vec3f( 0.0f, 100.0f, 200.0f)   );
			m_bCrusherVolume.SetMinPoint( vec3f( -35.0f, -100.0f, -200.0f) );

			CrusherSparkPositions[0] = vec3f( 10.0f,   130.0f, 0.0f);
			CrusherSparkPositions[1] = vec3f( 10.0f,  -130.0f, 0.0f);

			AABB* temp = (AABB*)m_pParent->GetCollidableObject();
			vec3f max = temp->GetMaxPoint3D();
			max.y -= 90.0f;
			temp->SetMaxPoint(max);

			vec3f min = temp->GetMinPoint3D();
			min.y += 90.0f;
			temp->SetMinPoint( min );

			break;
		}
	};

	m_bCrusherVolume.SetBodyType( BT_DYNAMIC );
	m_bCrusherVolume.SetVolumeType( VMT_AABB );
}