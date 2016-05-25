#include "CComponent_Input.h"
#include "CComponent_Collision.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "CComponent_Item.h"
#include "../RenderEngine/RenderEngine.h"
#include "../RenderEngine/TextureManager.h"
#include "CComponent_CorkCollision.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "CComponent_PlayerCollision.h"
#include "../EffectsLib/CEffectsIncludes.h"
#include "CComponent_Sound.h"
#include "../Utilities/KdTree/KdTree.h"
#include "../RenderEngine/DebugShapes.h"
#include "../EffectsLib/ParticleManager.h"
#include "..//HUD/CTrajectory.h"
#include "../Utilities/Interpolate.h"
#include "CComponent_LittleGooCollision.h"
#include "../AI/AIStatesInclude.h"

#define IF(x) if(x)(x)

#define WALK_TIME_CAP 0.25f
#define ROTATION_TIME_CAP 0.1f

static short m_sPullIconCount = 0;
static short m_sPushIconCount = 0;

CComponent_Input::CComponent_Input(void) : IComponent(ECOMP_INPUT)
{
	m_bJumping = false;
	m_bDoubleJump = false;
	m_bWallJumping = false;
	m_fDropTimer = 0.0f;
	m_fPlayerSpeedMod = 1.0f;
	m_pGame = NULL;
	m_pInput = NULL;
	m_pCamera = NULL;
	m_pObjectManager = NULL;
	m_pSoundComponent = NULL;
	pPlayerCol = NULL;
	pColComponent = NULL;
	m_fWalkTimer = 0.0f;
	m_bPreviouslyOnGround = false;

	m_tPulledObj.m_bIsAnObjectPulled = false;
	m_tPulledObj.ObjectPulledType = OBJ_NULL;
	m_tPulledObj.m_Object = nullptr;
	RotatedRight = RotatedLeft = false;

	m_pJumpParticleEffect = nullptr;
	m_fJumpParticleTimer = 0.0f;
	m_fWalkParticleTimer = 0.0f;

	m_pPullSmokeParticleEffect = nullptr;
	m_pPullSparkleParticleEffect = nullptr;

	m_fRotationTimer = 0.0f;
	SelectionMesh = NULL;

	// Rueben's awesome stuff
	// Trajectory
	m_pTrajectory = nullptr;

	// Icon
	m_pIcon = nullptr;
	m_nPullIconTexture = 0;
	m_nPullIconTexture2 = 0;
	m_nPushIconTexture = 0;
	m_nPushIconTexture2 = 0;
}

bool CComponent_Input::Initialize(void)
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pCamera = m_pGame->GetCamera();
	m_pObjectManager = CObjectManager::GetInstance();

	m_tPulledObj.m_bIsAnObjectPulled = false;
	m_tPulledObj.ObjectPulledType = OBJ_NULL;
	m_tPulledObj.m_Object = nullptr;

	RotatedRight = RotatedLeft = false;

	m_pJumpParticleEffect = CParticleManager::GetInstance()->CreateEffect( "JumpSteam", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_fJumpParticleTimer = FLT_MAX;

	m_PSparkleParticleEffect = CParticleManager::GetInstance()->CreateEffect( "FullHealthSparkles", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_pGooDripParticleEffect = CParticleManager::GetInstance()->CreateEffect( "GooDripping", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_pPullSmokeParticleEffect = CParticleManager::GetInstance()->CreateEffect( "PullSmokeCloud", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_pPullSmokeParticleEffect->m_bInterpolate = true;
	m_pPullSparkleParticleEffect = CParticleManager::GetInstance()->CreateEffect( "PullSmokeSparkles", vec3f( 0.0f, 0.0f, 0.0f ) );
	m_pPullSparkleParticleEffect->m_bInterpolate = true;

	pAnimComponent = NULL;
	pColComponent = NULL;
	m_pEmitterComponent = NULL;
	m_pSoundComponent = nullptr;

	m_LineToCursor.SetVolumeType(VMT_LINE);

	m_fWalkTimer = 0.0f;

	m_bPreviouslyOnGround = false;
	m_vsWeaponState = VACS_NULL;
	m_pPullEmitterComponent = nullptr;
	m_pPushEmitterComponent = nullptr;


	m_bCreatePullOnce = false;

	m_fWalkParticleTimer = 0.0f;

	m_fRotationTimer = 0.0f;
	SelectionMesh = NULL;

	// Rueben's awesome stuff
	// Trajectory
	m_fTrajectoryTimer = 0.0f;
	m_pTrajectory = new CTrajectory();
	m_pTrajectory->Initialize();

	// Icon
	m_pIcon = CIcon::GetInstance();
	m_nPullIconTexture = TextureManager::GetInstance()->LoadTexture( "./Resources/HUDImages/mouse.png" );
	m_nPullIconTexture2 = TextureManager::GetInstance()->LoadTexture( "./Resources/HUDImages/mousepull.png" );
	m_nPushIconTexture = m_nPullIconTexture;
	m_nPushIconTexture2 = TextureManager::GetInstance()->LoadTexture( "./Resources/HUDImages/mousepush.png" );


	m_bStopInput = false;

	// Initialize all the components that this component will use.
	pAnimComponent = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );

	pPlayerCol = (CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION );

	pColComponent = (CComponent_PlayerCollision*)pPlayerCol;

	m_pEmitterComponent = (CComponent_Emitter*)m_pParent->GetComponent( ECOMP_EMITTER );

	m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));	

	return true;
}

void CComponent_Input::Update(float fDT)
{
	if ( !m_pParent->GetAlive() )
	{
		CComponent_Collision* _pCol = (CComponent_Collision*)m_pParent->GetComponent (ECOMP_COLLISION);
		if (_pCol)
		{
			_pCol->CleanRemoveVelocity();
		}

		if(m_pSoundComponent)
		{
			m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP);
		}
		return ;
	}

	m_fWalkTimer += fDT;

	if(m_pSoundComponent && !m_bPreviouslyOnGround && pPlayerCol->CollidingGround())
	{
		m_bPreviouslyOnGround = true;
		m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_PLAYER_LAND);
	}


	// UPDATE THE PARTICLE LINE!
	vec3f fPullVector = GetValidPlacement( MAX_PULL_DIST ) - ( GetParent()->GetWorldPos() + vec3f( 0.0f, 50.0f, 0.0f ) );
	float fPullLength = fPullVector.magnitude();
	vec3f fPullNormalized = ( fPullVector ).normalize();

	m_pPullSmokeParticleEffect->m_fRampScaleMax = 1.0f + ( fPullLength / MAX_PULL_DIST ) * ( 45.0f - 1.0f );
	m_pPullSmokeParticleEffect->m_fMaxDistance = ( fPullLength / MAX_PULL_DIST ) * 92.0f;
	m_pPullSmokeParticleEffect->m_fLifespan = 0.5f;
	m_pPullSmokeParticleEffect->m_fPosition = GetValidPlacement( MAX_PULL_DIST );
	m_pPullSmokeParticleEffect->m_fTargetPosition = GetParent()->GetWorldPos() + vec3f( 0.0f, 50.0f, 0.0f );


	m_pPullSparkleParticleEffect->m_fRampScaleMax		= m_pPullSmokeParticleEffect->m_fRampScaleMax;
	m_pPullSparkleParticleEffect->m_fMaxDistance		= m_pPullSmokeParticleEffect->m_fMaxDistance;
	m_pPullSparkleParticleEffect->m_fLifespan			= m_pPullSmokeParticleEffect->m_fLifespan;
	m_pPullSparkleParticleEffect->m_fPosition			= m_pPullSmokeParticleEffect->m_fPosition;
	m_pPullSparkleParticleEffect->m_fTargetPosition		= m_pPullSmokeParticleEffect->m_fTargetPosition;

	// set start point to about where the arm will be
	m_LineToCursor.SetStartPoint( m_pParent->GetWorldPos() + vec3f(0.0f, 50.0f, 0.0f) );
	m_LineToCursor.SetEndPoint( m_pCamera->GetWorldCursor() );

	//Get the closest object to the pull
	m_pCurrentTarget = GetClosestObject( m_pParent->GetWorldPos() + vec3f(0.0f, 50.0f, 0.0f), GetValidPlacement( MAX_PULL_DIST ) );

	if( m_pPullEmitterComponent )
		m_pPullEmitterComponent->SetPullStart( GetValidPlacement( MAX_PULL_DIST ) );
	if( m_pPushEmitterComponent )
		m_pPushEmitterComponent->SetPullStart( GetValidPlacement( MAX_PULL_DIST ) );

	if( m_tPulledObj.m_bIsAnObjectPulled || CheckForInbetweens(m_pCurrentTarget) )
		m_pCurrentTarget = nullptr;
	//Check to see if there is an object and what kind it is
	if( m_pCurrentTarget )
	{
		if(m_pCurrentTarget->GetType() == OBJ_CRATE || m_pCurrentTarget->GetType() == OBJ_CORK || m_pCurrentTarget->GetType() == OBJ_PLUG || m_pCurrentTarget->GetType() == OBJ_GOO )
		{
			bool DontRenderGlow = false;
			if( m_pCurrentTarget->GetType() == OBJ_CORK )
			{
				//if it is a cork, check to see if it is corking something
				CComponent_CorkCollision* cork = (CComponent_CorkCollision*)(m_pCurrentTarget)->GetComponent( ECOMP_COLLISION );
				if( cork->GetCorked() )
					DontRenderGlow = true;
			}
			else if( m_pCurrentTarget->GetType() == OBJ_PLUG )
			{
				// If it is a Plug, check to see if it is plugging something
				CComponent_PlugCollision* plug = (CComponent_PlugCollision*)(m_pCurrentTarget)->GetComponent( ECOMP_COLLISION );
				if( plug->GetPlugged() )
					DontRenderGlow = true;
			}
			if( !DontRenderGlow )
			{
				//if we are not corked glow
				CComponent_Item* item = (CComponent_Item*)(m_pCurrentTarget)->GetComponent (ECOMP_ITEM);
				if( !item->GetAttached() )
				{
					if( SelectionMesh == NULL )
					{
						matrix4f Identity;
						Identity.make_identity();

						if( m_pCurrentTarget->GetType() == OBJ_GOO )
						{
							Identity.make_rotation_y( D3DXToRadian( -90.0f ));
							Identity.axis_pos.y += 25.0f;
						}

						SelectionMesh = CObjectManager::GetInstance()->CreateObject(OBJ_OBJECT_SELECTION, Identity.ma, GetParent()->GetLevelID() );
						if( SelectionMesh )
							item->GetParent()->GetFrame().AddChild( (Node*)&SelectionMesh->GetFrame() );

						D3DXMATRIX scalemat;
						D3DXMatrixIdentity( &scalemat );
						D3DXMatrixScaling( &scalemat, 100.0f, 100.0f, 0.0f );
						m_pIcon->Render( m_nPullIconTexture, m_nPullIconTexture2, item->GetParent()->GetFrame().GetWorldMat().axis_pos + vec3f( -50.0f, 100.0f, 0.0f ), scalemat );						
					}
					else
					{
						Node* SelectionFrame = &SelectionMesh->GetFrame();
						Node* ItemFrame = &item->GetParent()->GetFrame();

						if( SelectionFrame->GetParent() != ItemFrame )
						{
							SelectionMesh->GetFrame().GetParent()->RemoveChild( (Node*)&SelectionMesh->GetFrame() );
							CObjectManager::GetInstance()->AddObjectToRemove( SelectionMesh );
							SelectionMesh = NULL;
						}

						D3DXMATRIX scalemat;
						D3DXMatrixIdentity( &scalemat );
						D3DXMatrixScaling( &scalemat, 100.0f, 100.0f, 0.0f );
						m_pIcon->Render( m_nPullIconTexture, m_nPullIconTexture2, item->GetParent()->GetFrame().GetWorldMat().axis_pos + vec3f( -50.0f, 100.0f, 0.0f ), scalemat );						
					}
				}
			}
		}
	}
	else
	{
		if( SelectionMesh )
		{
			SelectionMesh->GetFrame().GetParent()->RemoveChild( (Node*)&SelectionMesh->GetFrame() );
			CObjectManager::GetInstance()->AddObjectToRemove( SelectionMesh );
			SelectionMesh = NULL;
		}
	}

	// Increment Drop Timer
	m_fDropTimer -= fDT;

	switch( m_vsWeaponState )
	{
	case VACS_NULL:
		{
			if(m_tPulledObj.m_bIsAnObjectPulled)
			{
				if(m_pSoundComponent)
				{
					m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP);
					m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_VACUUM_PICKUP_OBJECT);
				}
				m_vsWeaponState = VACS_HASOBJECT;
				break;
			}
			if(m_vsWeaponState != VACS_HASOBJECT)
			{
				if(m_pInput->LeftMouseButtonPressed())
				{
					if(m_bCreatePullOnce == false)
					{
						m_bCreatePullOnce = true;
					}
				}
			}

			//in case someone sets our pulled object's target to null, but didn't tell us, check to see if it's still aiming at us
			if( m_tPulledObj.m_bIsAnObjectPulled )
			{
				CComponent_Item* itemCmp = ((CComponent_Item*)m_tPulledObj.m_Object->GetComponent(ECOMP_ITEM));
				if(itemCmp && !itemCmp->GetTarget())
				{
					//we aren't being aimed at anymore, so we aren't pulling anything anymore. Stop the sound.
					m_tPulledObj.m_Object = nullptr;
					if(m_pSoundComponent)
					{
						m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP);
					}
				}
			}

			if(m_pInput->LeftMouseButtonDown() && !m_bStopInput)
			{
				// If the Drop timer is less than 0 we can pull again.
				if(m_fDropTimer <= 0.0f)
				{
					// If there is not an object in our possesion.
					if(!m_tPulledObj.m_bIsAnObjectPulled)
					{
						if(m_pSoundComponent)
						{
							m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_PLAYER_PULL_LOOP);
						}

						// Call our Pull Function which will handle all of the pulling from the object along with the Item Component.
						PlayerPull( m_pCurrentTarget );
						m_vsWeaponState = VACS_PULL;
						break;
					}
				}
			}

			if(m_pInput->RightMouseButtonPressed() && !m_bStopInput)
			{
				if(m_tPulledObj.m_Object)
				{
					break;
				}

				// Play the Pushing Sound and Call the Player Push Function.
				if(m_pSoundComponent)
				{
					m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_PLAYER_PUSH);
				}
				PlayerPush( fDT );
				m_vsWeaponState = VACS_PUSH;
				break;
			}

			break;
		}
	case VACS_PULL:
		{
			m_pPullSmokeParticleEffect->Emit( fDT );
			m_pPullSparkleParticleEffect->Emit( fDT );

			if(!m_pInput->LeftMouseButtonDown() )
			{
				// Stop the sound and set our state to no longer pulling.
				if(m_pSoundComponent && m_tPulledObj.m_Object)
				{
					CComponent_Item* pulledItemCmp = (CComponent_Item*)m_tPulledObj.m_Object->GetComponent(ECOMP_ITEM);
					if(pulledItemCmp && !pulledItemCmp->GetTarget())
					{
						m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP);
					}
				}
				if(m_pSoundComponent && !m_tPulledObj.m_Object)
				{
					m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP);
				}
				// Stop playing the pull animation.
				if( pAnimComponent )
				{
					State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
					if( ( pudgeState != PudgeIdleToRunState::GetState() ) && ( pudgeState != PudgeRunState::GetState() ) &&
						( pudgeState != PudgeIdleToJumpState::GetState() ) && ( pudgeState != PudgeJumpState::GetState() ) &&
						( pudgeState != PudgeJumpToFallingState::GetState() ) && ( pudgeState != PudgeFallingState::GetState() ) &&
						( pudgeState != PudgeWallJumpState::GetState()) && ( pudgeState != PudgeWallSlideState::GetState()) && 
						( pudgeState != PudgeBackPeddleState::GetState()) )
					{
						pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeIdleState::GetState() );
					}
				}

				m_vsWeaponState = VACS_NULL;
				break;
			}

			// If there is not an object in our possesion.
			if(!m_tPulledObj.m_bIsAnObjectPulled)
			{
				//if the item I was pulling doesn't know I was pulling it, I'm not pulling it.
				if(m_tPulledObj.m_Object && (CComponent_Item*)m_tPulledObj.m_Object->GetComponent(ECOMP_ITEM))
				{
					if(!((CComponent_Item*)m_tPulledObj.m_Object->GetComponent(ECOMP_ITEM))->GetTarget())
					{
						m_tPulledObj.m_Object = nullptr;
					}
				}

				// Call our Pull Function which will handle all of the pulling from the object along with the Item Component.
				PlayerPull( m_pCurrentTarget );
				break;
			}
			else
			{
				if(m_pSoundComponent)
				{
					m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_VACUUM_PICKUP_OBJECT);
					m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP);
				}

				m_vsWeaponState = VACS_HASOBJECT;
				break;
			}

			break;
		}
	case VACS_PUSH:
		{
			m_vsWeaponState = VACS_NULL;
			break;
		}
	case VACS_HASOBJECT:
		{
			if(m_pInput->LeftMouseButtonPressed() && !m_bStopInput)
			{
				//drop the object
				// if there is a pulled object
				if( m_tPulledObj.m_bIsAnObjectPulled )
				{
					// Grab the Players Matrix and offset the box from that matrix.
					matrix4f ObjectsMatrix = m_pParent->GetPositionMatrix();// = m_pParent->GetFrame().GetWorldMat();

					ObjectsMatrix.axis_pos = GetValidPlacement( 120.0f ) + ( m_LineToCursor.GetStartPoint3D() - m_LineToCursor.GetEndPoint3D() ).normalize() * 30.0f;


					if(m_tPulledObj.m_Object)
					{
						CComponent_Collision* pulledCollision = ((CComponent_Collision*)m_tPulledObj.m_Object->GetComponent( ECOMP_COLLISION ));
						CComponent_Item* pulledItem = ((CComponent_Item*)m_tPulledObj.m_Object->GetComponent(ECOMP_ITEM));
						if(pulledCollision)
						{
							pulledCollision->GetPhysicsBox()->SetPosition(vec3f(ObjectsMatrix.wx, ObjectsMatrix.wy, ObjectsMatrix.wz));
							pulledCollision->CleanRemoveVelocity();
							pulledCollision->SetTimeLeft( 0.75f );
						}

						if(pulledItem)
						{
							pulledItem->SetTarget(nullptr);
							pulledItem->SetAttached(false);
						}

						if(m_tPulledObj.m_Object->GetType() == OBJ_GOO)
						{
							CComponent_AI* pulledGooAI = ((CComponent_AI*)(m_tPulledObj.m_Object->GetComponent(ECOMP_AI)));

							if(pulledCollision)
							{
								((CComponent_LittleGooCollision*)pulledCollision)->GetVolitile() = false;
								((CComponent_LittleGooCollision*)pulledCollision)->SetPulled(false);
								((CComponent_LittleGooCollision*)pulledCollision)->SetPlayerIgnoreTimer(0.0f);
							}

							if(pulledGooAI)
							{
								pulledGooAI->SetActive(true);
							}

#if _ANIMATIONS
							CComponent_Animation* pulledGooAnimation = ((CComponent_Animation*)(m_tPulledObj.m_Object->GetComponent(ECOMP_ANIMATION)));

							if(pulledGooAnimation)
							{
								pulledGooAnimation->GetAnimStateMachine()->ChangeState(LilGooWalkState::GetState());
								pulledGooAnimation->GetInterpolator().SetTime(0.0f);
							}
#endif //_ANIMATIONS
						}

						// Set a Timer up to prevent player from pulling object back in immediately.
						m_fDropTimer = 0.3f;

						// Set the object pulled variable to false and the object type to NULL to know we no longer have hold of this object.
						m_tPulledObj.m_bIsAnObjectPulled = false;
						m_tPulledObj.ObjectPulledType = OBJ_NULL;
						m_tPulledObj.m_Object = NULL;
						// Notify the Render Engine that the object is no longer in our possesion.
						RenderEngine::SetObjPulled(false);

						m_vsWeaponState = VACS_NULL;
						break;
					}
				}
			}

			if(m_pInput->RightMouseButtonPressed() && !m_bStopInput)
			{
				// Play the Pushing Sound and Call the Player Push Function.
				if(m_pSoundComponent)
				{
					m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_PLAYER_PUSH);
				}
				PlayerPush( fDT );
				m_vsWeaponState = VACS_NULL;
				break;
			}
			break;
		}
	}

	// If there is a pulled object
	// create the trajectory objects and
	// send them along the shot path
	if( m_tPulledObj.m_bIsAnObjectPulled )
	{		
		// use the laser sight line to get 
		// the direction the player is aiming
		vec2f vectocursor = m_LineToCursor.GetEndPoint2D() - m_LineToCursor.GetStartPoint2D();
		vectocursor.normalize();
		// set the trajectory vector
		m_pTrajectory->SetVecToCursor( vectocursor );

		matrix4f ObjectsMatrix = m_pParent->GetPositionMatrix();
		ObjectsMatrix.axis_pos = GetValidPlacement( 120.0f ) + ( m_LineToCursor.GetStartPoint3D() - m_LineToCursor.GetEndPoint3D() ).normalize() * 30.0f;
		// Set the trajectory start position
		m_pTrajectory->SetStartPos( ObjectsMatrix.axis_pos );

		// Update the trajectory to simulate the objects movement
		m_pTrajectory->Update( fDT );

		// Render the Trajectory Line
		m_pTrajectory->Render();

		D3DXMATRIX scalemat;
		D3DXMatrixIdentity( &scalemat );
		D3DXMatrixScaling( &scalemat, 100.0f, 100.0f, 0.0f );
		m_pIcon->Render( m_nPushIconTexture, m_nPushIconTexture2, m_pParent->GetFrame().GetWorldMat().axis_pos + vec3f( -50.0f, 200.0f, 0.0f ), scalemat );		
	}

	HandleMovementInput( fDT );

	if( m_fJumpParticleTimer < 0.15f )
	{
		m_pJumpParticleEffect->m_fPosition = GetParent()->GetWorldPos();
		m_pJumpParticleEffect->m_bGravityWell = true;
		m_pJumpParticleEffect->m_fTargetPosition = GetParent()->GetWorldPos();

		if( RotatedRight )
		{
			m_pJumpParticleEffect->m_fDirection = vec3f( -0.25f, -0.75f, 0.0f ).normalize();
			m_pJumpParticleEffect->m_fPosition += vec3f( -25.0f, 20.0f, 0.0f );
		}
		else
		{
			m_pJumpParticleEffect->m_fDirection = vec3f( 0.25f, -0.75f, 0.0f ).normalize();
			m_pJumpParticleEffect->m_fPosition += vec3f( 25.0f, 20.0f, 0.0f );
		}

		m_pJumpParticleEffect->Emit( fDT );
	}

	m_fJumpParticleTimer += fDT;
	m_fWalkParticleTimer += fDT;

	float fCurrentHealth = ((CComponent_Health*)(GetParent()->GetComponent( ECOMP_HEALTH )))->GetHealth();
	if( fCurrentHealth == MAX_HEALTH )
	{
		m_PSparkleParticleEffect->m_fPosition = GetParent()->GetWorldPos();
		m_PSparkleParticleEffect->m_fPosition.y += 85.0f;
		m_PSparkleParticleEffect->Emit( fDT );
	}
	RenderEngine::SetPlayerHealth( fCurrentHealth );

	m_pGooDripParticleEffect->m_nParticleRate = (int)(( 1.0f - ( fCurrentHealth / (MAX_HEALTH) ) ) * 16.0f);
	m_pGooDripParticleEffect->m_fPosition = GetParent()->GetWorldPos() + vec3f( 0.0f, 40.0f, 0.0f );
	m_pGooDripParticleEffect->Emit( fDT );
}

void CComponent_Input::Render()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: PlayerMovement_Right()
// Purpose: Move the player to the right
// Original Author: Rueben Massey
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Input::PlayerMovement_Right( float fDT )
{
	// Collision Component Safety Check
	if( pPlayerCol )
	{
		if( pPlayerCol->GetVel().x < 8.0f * m_fPlayerSpeedMod  )
		{
			pPlayerCol->GetVel() += vec3f(1.0f, 0.0f, 0.0f) * PLAYER_MOVE_SPEED * 2.0f * fDT * m_fPlayerSpeedMod;
		}
		else
		{
			pPlayerCol->GetVel().x = 8.0f * m_fPlayerSpeedMod;
		}
		m_fPlayerSpeedMod = 1.0f;
	}

	if( pPlayerCol )
	{
		if( pPlayerCol->CollidingGround() )
		{
			if( m_fWalkParticleTimer > 0.2f )
			{
				CParticleManager::GetInstance()->CreateEffectOneShot( string( "RunSmoke" ), GetParent()->GetWorldPos() );
				m_fWalkParticleTimer = 0.0f;
			}

			if( pAnimComponent )
			{
				State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
				if( pudgeState != PudgeSmallDamageState::GetState() && pudgeState != PudgeBigDamageState::GetState() 
					&& pudgeState != PudgeIdleToJumpState::GetState() )
				{
					if( RotatedLeft )
						pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeBackPeddleState::GetState() );
					else
						pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeRunState::GetState() );
				}

			}

			m_bRunning = true;
		}
		else
		{
			m_bRunning = false;
		}
	}

	if( pPlayerCol->CollidingGround() || pColComponent->IsCollidingLeft())
		pColComponent->ResetWallJumpTimer();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: PlayerMovement_Left()
// Purpose: Move the player to the left
// Original Author: Rueben Massey
// Creation Date: 5/12/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Input::PlayerMovement_Left( float fDT )
{
	// Collision Component Safety Check
	if( pPlayerCol )
	{
		if( pPlayerCol->GetVel().x > -8.0f * m_fPlayerSpeedMod )
		{
			pPlayerCol->GetVel() += vec3f(-1.0f, 0.0f, 0.0f) * PLAYER_MOVE_SPEED * 2.0f * fDT * m_fPlayerSpeedMod;
		}
		else
		{
			pPlayerCol->GetVel().x = -8.0f * m_fPlayerSpeedMod;	
		}
		m_fPlayerSpeedMod = 1.0f;
	}

	if( pPlayerCol )
	{
		if( pPlayerCol->CollidingGround() )
		{
			if( m_fWalkParticleTimer > 0.2f )
			{
				CParticleManager::GetInstance()->CreateEffectOneShot( string( "RunSmoke" ), GetParent()->GetWorldPos() );
				m_fWalkParticleTimer = 0.0f;
			}

			if( pAnimComponent )
			{
				State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
				if( pudgeState != PudgeSmallDamageState::GetState() && pudgeState != PudgeBigDamageState::GetState() 
					&& pudgeState != PudgeIdleToJumpState::GetState() )
				{
					if( RotatedRight )
						pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeBackPeddleState::GetState() );
					else
						pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeRunState::GetState() );
				}
			}
			m_bRunning = true;
		}
		else
		{
			m_bRunning = false;

		}
	}


	if( pPlayerCol->CollidingGround() || pColComponent->IsCollidingRight() )
		pColComponent->ResetWallJumpTimer();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: PlayerPush()
// Purpose: Apply force to the closest object
// Original Author: Rueben Massey
// Creation Date: 5/15/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CComponent_Input::PlayerPush( float fDT )
{
	vec3f fFireDirection = ( m_pCamera->GetWorldCursor() - GetParent()->GetWorldPos() ).normalize();
	CParticleManager::GetInstance()->CreateEffectOneShot( "PushInside", GetParent()->GetWorldPos() + vec3f( 0.0f, 50.0f, 0.0f ), fFireDirection ); 
	CParticleManager::GetInstance()->CreateEffectOneShot( "PushOutside", GetParent()->GetWorldPos() + vec3f( 0.0f, 50.0f, 0.0f ), fFireDirection );
	CParticleManager::GetInstance()->CreateEffectOneShot( "PushSparkles", GetParent()->GetWorldPos() + vec3f( 0.0f, 50.0f, 0.0f ), fFireDirection ); 

	// Transition to Push Animation.
	if( pAnimComponent )
	{
		State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
		if( ( pudgeState != PudgeIdleToRunState::GetState() ) && ( pudgeState != PudgeRunState::GetState() ) &&
			( pudgeState != PudgeIdleToJumpState::GetState() ) && ( pudgeState != PudgeJumpState::GetState() ) &&
			( pudgeState != PudgeJumpToFallingState::GetState() ) && ( pudgeState != PudgeFallingState::GetState() ) &&
			( pudgeState != PudgeWallJumpState::GetState()) && ( pudgeState != PudgePullState::GetState()) &&
			( pudgeState != PudgeWallSlideState::GetState()) && pudgeState != PudgeBackPeddleState::GetState() )
		{
			pAnimComponent->GetAnimStateMachine()->ChangeState( PudgePushState::GetState() );
		}
	}

	// if there is a pulled object
	if( m_tPulledObj.m_bIsAnObjectPulled )
	{
		if(m_tPulledObj.m_Object == nullptr)
		{
			m_tPulledObj.m_bIsAnObjectPulled = false;
			m_tPulledObj.ObjectPulledType = OBJ_NULL;
			m_tPulledObj.m_Object = nullptr;
			return;
		}

		// use the laser sight line to get 
		// the direction the player is aiming
		vec2f vectocursor = m_LineToCursor.GetEndPoint2D() - m_LineToCursor.GetStartPoint2D();
		vectocursor.normalize();

		matrix4f ObjectsMatrix = m_pParent->GetPositionMatrix();
		ObjectsMatrix.axis_pos = GetValidPlacement( 120.0f ) + ( m_LineToCursor.GetStartPoint3D() - m_LineToCursor.GetEndPoint3D() ).normalize() * 30.0f;

		// If an object is Pulled go ahead and create it.
		CComponent_Collision* pObjectsCollision = (CComponent_Collision*)m_tPulledObj.m_Object->GetComponent( ECOMP_COLLISION );
		pObjectsCollision->CleanRemoveVelocity();
		if(pObjectsCollision->GetPhysicsBox())
		{
			pObjectsCollision->GetPhysicsBox()->SetPosition(vec3f(ObjectsMatrix.wx, ObjectsMatrix.wy, ObjectsMatrix.wz));
		}
		pObjectsCollision->SetTimeLeft( 0.75f );

		((CComponent_Item*)m_tPulledObj.m_Object->GetComponent(ECOMP_ITEM))->SetTarget(nullptr);
		((CComponent_Item*)m_tPulledObj.m_Object->GetComponent(ECOMP_ITEM))->SetAttached(false);

		// Apply a force in that direction
		pObjectsCollision->CleanForce(  vec2f(vectocursor.x, vectocursor.y ) * THROW_SPEED );

		// If its a little goo, let it know that its in the air.
		if( m_tPulledObj.ObjectPulledType == OBJ_GOO )
		{
			CComponent_AI* GooAIComp = (CComponent_AI*)m_tPulledObj.m_Object->GetComponent( ECOMP_AI );
			// Turns the AI off and sets the goo to die on the next contact.
			GooAIComp->SetActive(true);
			CComponent_LittleGooCollision* gooCol = ((CComponent_LittleGooCollision*)(m_tPulledObj.m_Object->GetComponent( ECOMP_COLLISION ) ) );
			if(gooCol)
			{
				gooCol->GetVolitile() = true;
				gooCol->SetPulled(false);
				// Reset the ignore timer for the player.
				gooCol->SetPlayerIgnoreTimer( 0.0f );
				CComponent_Sound* pGooSound = (CComponent_Sound*)m_tPulledObj.m_Object->GetComponent(ECOMP_SOUND);
				if(pGooSound)
				{
					pGooSound->SetInRange(true);
					pGooSound->PlaySound(AK::EVENTS::PLAY_LILGOO_SUCKEDUP);
				}
			}
#if _ANIMATIONS
			((CComponent_Animation*)(m_tPulledObj.m_Object->GetComponent( ECOMP_ANIMATION ) ) )->GetAnimStateMachine()->ChangeState( LilGooFlyingState::GetState() );
#endif //_ANIMATIONS
			float XVel = dot_product(pObjectsCollision->GetVel(), vec3f(1.0f, 0.0f, 0.0f));
			if(XVel > 0.0f)
			{
				((CLilGooMoveAI*)GooAIComp->GetLoadedAI(AI_MOVE))->SetDirectionTimer( 1.1f );
				((CLilGooMoveAI*)GooAIComp->GetLoadedAI(AI_MOVE))->SetMoveDirection( true );

				//set it to face right
				matrix4f _localMat = (*m_tPulledObj.m_Object->GetLocalMat());
				matrix4f rotationMatrix;
				rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
				rotationMatrix.axis_pos = _localMat.axis_pos;
				_localMat = rotationMatrix;
				m_tPulledObj.m_Object->SetLocalMat(&_localMat);
			}
			else if(XVel < 0.0f)
			{
				((CLilGooMoveAI*)GooAIComp->GetLoadedAI(AI_MOVE))->SetDirectionTimer( 1.1f );
				((CLilGooMoveAI*)GooAIComp->GetLoadedAI(AI_MOVE))->SetMoveDirection( false );

				//set it to face left
				matrix4f _localMat = (*m_tPulledObj.m_Object->GetLocalMat());
				matrix4f rotationMatrix;
				rotationMatrix.make_rotation_y( D3DXToRadian(90) );
				rotationMatrix.axis_pos = _localMat.axis_pos;
				_localMat = rotationMatrix;
				m_tPulledObj.m_Object->SetLocalMat(&_localMat);
			}		
		}

		// We no longer have a pulled object so set bool to false and object type to NULL.
		m_tPulledObj.m_bIsAnObjectPulled = false;
		m_tPulledObj.ObjectPulledType = OBJ_NULL;
		//m_tPulledObj.m_Object->Release();
		m_tPulledObj.m_Object = NULL;

		// Set the Drop Timer to prevent Yo-Yo use of a Object.
		m_fDropTimer = 0.3f;

		// Notify Render Engine.
		RenderEngine::SetObjPulled(false);
	}
	// this else is for the nudge
	else
	{
		if( m_pCurrentTarget )
		{
			// this will be inverted
			float fDistToTarget = ( m_pParent->GetWorldPos() - m_pCurrentTarget->GetWorldPos() ).magnitude();

			// Use the Max Push distance and the Distance to the target to calculate How Much Force to apply.
			float Nudge_Force_Ratio = fDistToTarget / MAX_PUSH_DIST;
			// Subtract the Nudge_Ratio from 1 to calculate the ratio of how much force to apply to push an object 8 units.
			Nudge_Force_Ratio = (1.0f - Nudge_Force_Ratio);

			// Calculate how much force to apply over time on the object.
			float Nudge_Force_Speed = MAX_PUSH_DIST * Nudge_Force_Ratio;
			Nudge_Force_Speed *= fDT;

			// get the collision component
			CComponent_Collision* pColl = ((CComponent_Collision*)m_pCurrentTarget->GetComponent( ECOMP_COLLISION ));
			// Collsion Component Safety Check
			if( pColl )
			{
				// Get the physics box
				tPhyObject* pPhy = pColl->GetPhysicsBox();
				// Physics Box Safety Check
				if( pPhy )
				{
					// use the laser sight line to get 
					// the direction the player is aiming
					vec2f vectocursor = m_LineToCursor.GetEndPoint2D() - m_LineToCursor.GetStartPoint2D();
					vectocursor.normalize();
					// apply force in that direction based on distance
					pPhy->ApplyUniformForce( vec3f(vectocursor.x, vectocursor.y, 0.0f), Nudge_Force_Speed );
					m_tPulledObj.m_bIsAnObjectPulled = false;
					m_tPulledObj.m_Object = nullptr;
				}
				// Notify Render Engine.
				RenderEngine::SetObjPulled(false);
			}
		}
	}
}

void CComponent_Input::PlayerPull( IBaseObject* toPull )
{
	IBaseObject* closestObj = toPull;

	// If there is no set object being pulled, go ahead and set it as the Object being pulled.
	if( NULL == m_tPulledObj.m_Object )
		m_tPulledObj.m_Object = closestObj;
	else
		return;

	if(m_tPulledObj.m_Object && !CheckForInbetweens(m_tPulledObj.m_Object))
	{
		CComponent_Item* objItem = (CComponent_Item*)m_tPulledObj.m_Object->GetComponent(ECOMP_ITEM);
		if(objItem)
		{
			objItem->SetTarget(nullptr);
		}
		m_tPulledObj.m_Object = nullptr;
		m_tPulledObj.m_bIsAnObjectPulled = false;
		return;
	}

	if(closestObj)
	{
		// If there is a closest Object.
		if( closestObj == m_tPulledObj.m_Object )
		{
			// Grab the Item component of this object.
			CComponent_Item* closestItem = ((CComponent_Item*)(closestObj->GetComponent(ECOMP_ITEM)));
			// Set the Closest Items Target to the Player.
			closestItem->SetTarget(GetParent());

			// Turn off the AI if necesary.
			IF( (CComponent_AI*)closestObj->GetComponent( ECOMP_AI ) )->SetActive( false );

			// If this Object is a Cork or a Plug
			if( closestObj->GetType() == OBJ_CORK || closestObj->GetType() == OBJ_PLUG || closestObj->GetType() == OBJ_GOO)
			{
				// Grab its collision Component.
				CComponent_Collision* PulledObjectCollisionComponent = (CComponent_Collision*)closestObj->GetComponent( ECOMP_COLLISION );
				// If this object is a cork and it is not corked then go ahead and pull it.
				if( PulledObjectCollisionComponent )
				{
					// If this is a cork.
					if( CCMP_CORK == PulledObjectCollisionComponent->GetCollType() )
					{
						// If this cork is not corked go ahead and pull it.
						if( !((CComponent_CorkCollision*)PulledObjectCollisionComponent)->GetCorked() )
							((CComponent_CorkCollision*)PulledObjectCollisionComponent)->SetPulled( true );
						else
						{
							m_tPulledObj.m_Object = NULL;
							return;
						}
					}
					// If this object is a plug.
					else if( CCMP_PLUG == PulledObjectCollisionComponent->GetCollType() )
					{
						// If this Plug is plugging something, then do not pull it.
						if( ((CComponent_PlugCollision*)PulledObjectCollisionComponent)->GetPlugged() )
						{
							m_tPulledObj.m_Object = NULL;
							return;
						}
					}
					else if( CCMP_GOO == PulledObjectCollisionComponent->GetCollType() )
					{
						// If this Plug is plugging something, then do not pull it.
						if(PulledObjectCollisionComponent->GetParent()->GetAlive())
						{
							((CComponent_LittleGooCollision*)PulledObjectCollisionComponent)->SetPulled ( true );
						}
						else
						{
							((CComponent_LittleGooCollision*)PulledObjectCollisionComponent)->SetPulled ( false );
							m_tPulledObj.m_Object = NULL;
							closestItem->SetTarget(NULL);
							return;
						}
					}
				}

			}

			// Notify the Render Engine that we have an object.
			RenderEngine::SetObjPulled(true);
		}
	}

	// Set the Animation to Pull.
	if( pAnimComponent )
	{
		State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
		if( ( pudgeState != PudgeIdleToRunState::GetState() ) && ( pudgeState != PudgeRunState::GetState() ) &&
			( pudgeState != PudgeIdleToJumpState::GetState() ) && ( pudgeState != PudgeJumpState::GetState() ) &&
			( pudgeState != PudgeJumpToFallingState::GetState() ) && ( pudgeState != PudgeFallingState::GetState() ) &&
			( pudgeState != PudgeWallJumpState::GetState()) && ( pudgeState != PudgePushState::GetState()) && 
			( pudgeState != PudgeWallSlideState::GetState()) && pudgeState != PudgeBackPeddleState::GetState())
		{
			pAnimComponent->GetAnimStateMachine()->ChangeState( PudgePullState::GetState() );
		}
	}
}

void CComponent_Input::PlayerJump( float fDT )
{
	if( pPlayerCol )
	{
		// If the player is touching the ground.
		if( pPlayerCol->CollidingGround() )
		{
			//m_pAudio->PlayEffect(m_pSoundObject, PLAYER_JUMP);
			m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_PLAYER_JUMP);
			float fOldX = pPlayerCol->GetVel().x;

			// Preserve the x component of the velocity and reset the y component to up.
			pPlayerCol->GetVel() = vec3f(0.0f, 1000.0f, 0.0f)  * fDT;
			pPlayerCol->GetVel().x = fOldX;
			m_fJumpParticleTimer = 0.0f;

			if( pAnimComponent )
			{
				pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeIdleToJumpState::GetState() );
			}
			/*m_bRunning = false*/
		}
		else
			// If not colliding with the ground.
		{
			// If the player has a double jump left.
			if(  m_bDoubleJump && (!pPlayerCol->CollidingWall()) )
			{
				//m_pAudio->PlayEffect(m_pSoundObject, PLAYER_JUMP);
				m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_PLAYER_JUMP);
				// Preserve the old x velocity and reset the y to up.
				float fOldX = pPlayerCol->GetVel().x;
				pPlayerCol->GetVel() = vec3f(0.0f, 1000.0f, 0.0f) * fDT;
				pPlayerCol->GetVel().x = fOldX;
				m_bDoubleJump = false;
				m_fJumpParticleTimer = 0.0f;

				if( pAnimComponent )
				{
					pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeIdleToJumpState::GetState() );
				}
			}
			// If they pressed jump, are out of double jumps, and are colliding with a wall.
			else if( pPlayerCol->CollidingWall() )
			{
				//m_pAudio->PlayEffect(m_pSoundObject, PLAYER_JUMP);
				m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_PLAYER_JUMP);
				// Set the up velocity.
				pPlayerCol->GetVel() = vec3f( 0.0, 1000.0f, 0.0f) * ( m_fPlayerSpeedMod ) * fDT;
				// Add in the push away from the wall.
				vec3f pushoutVector = pColComponent->GetWallJumpPushOutVector();
				pPlayerCol->GetVel().x += (pushoutVector * 500.0f  * m_fPlayerSpeedMod ).x * fDT;
				m_bWallJumping = true;
				m_fJumpParticleTimer = 0.0f;

				if( pAnimComponent )
				{
					pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeWallJumpState::GetState() );
				}
			}
		}
	}
}

void CComponent_Input::Shutdown(void)
{
	if(m_pSoundComponent)
	{
		m_pSoundComponent->StopSound(AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP);
	}
	delete m_pTrajectory;

	CParticleManager::GetInstance()->DestroyEffect( m_pJumpParticleEffect );
	CParticleManager::GetInstance()->DestroyEffect( m_PSparkleParticleEffect );
	CParticleManager::GetInstance()->DestroyEffect( m_pGooDripParticleEffect );
	CParticleManager::GetInstance()->DestroyEffect( m_pPullSmokeParticleEffect );
	CParticleManager::GetInstance()->DestroyEffect( m_pPullSparkleParticleEffect );

	m_fTrajectoryTimer = 0.0f;
	m_bJumping = false;
	m_bDoubleJump = false;
	m_bWallJumping = false;
	m_fDropTimer = 0.0f;
	m_fPlayerSpeedMod = 1.0f;
	m_pGame = NULL;
	m_pInput = NULL;
	m_pCamera = NULL;
	m_pObjectManager = NULL;
	m_pSoundComponent = NULL;
	pPlayerCol = NULL;
	pColComponent = NULL;
	m_fWalkTimer = 0.0f;
	m_bPreviouslyOnGround = false;

	m_tPulledObj.m_bIsAnObjectPulled = false;
	m_tPulledObj.ObjectPulledType = OBJ_NULL;
	m_tPulledObj.m_Object = nullptr;
	RotatedRight = RotatedLeft = false;

	m_pJumpParticleEffect = nullptr;
	m_fJumpParticleTimer = 0.0f;
	m_fWalkParticleTimer = 0.0f;

	m_pPullSmokeParticleEffect = nullptr;
	m_pPullSparkleParticleEffect = nullptr;

	m_fRotationTimer = 0.0f;
	SelectionMesh = NULL;

	// Rueben's awesome stuff
	// Trajectory
	m_pTrajectory = nullptr;

	// Icon
	m_pIcon->DeleteInstance();
	m_pIcon = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: GetClosestObject()
// Purpose: returns the closest object along the laser sight
// Original Author: Rueben Massey
// Creation Date: 5/15/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IBaseObject* CComponent_Input::GetClosestObject( vec3f _start, vec3f _end )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = (1<<RFLAG_BOX);

	// call the kd tree and get the near objects
	vec2f vectocursor = m_LineToCursor.GetEndPoint2D() - m_LineToCursor.GetStartPoint2D();
	vectocursor.normalize();

	CSceneObject soLineSceneObject;
	Line lToCursor;
	lToCursor.SetVolumeType( VMT_LINE );
	lToCursor.SetStartPoint( _start );
	lToCursor.SetEndPoint( _end );
	soLineSceneObject.SetCollidableObject( &lToCursor );

	CKdTree::GetNearObjects( &soLineSceneObject, PSFLAG_LINE, ReturnParams, NearestObjects ); 

	soLineSceneObject.SetCollidableObject( nullptr );

	//loop through all the return objects and check collision with them.
	IBaseObject* closestObj = nullptr;
	float fDist = FLT_MAX;
	vec3f cp;

	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		// get the necessary information about the object
		CComponent_Collision* colcomp = (CComponent_Collision*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_COLLISION ));
		CComponent_Item* item = (CComponent_Item*)(((IBaseObject*)NearestObjects[objindex])->GetComponent( ECOMP_ITEM ));

		// Component Safety Check
		if(item && colcomp)
		{
			Box* boxB = (Box*)(colcomp->GetParent()->GetCollidableObject());

			// if the laser sight collided with an object 
			if( lToCursor.LineToBox(*boxB, cp) )
			{
				// Get a vector from the play to the object
				vec3f vectoobj = cp - lToCursor.GetStartPoint3D();

				float dist = vectoobj.magnitude();

				// if the distance to the object is less than the previous distance
				if( dist < fDist )
				{
					// Set the distance
					fDist = dist;

					// Catch the object
					closestObj = (IBaseObject*)NearestObjects[objindex];;
				}
			}
		}
	}

	return closestObj;
}

// Half Space test which will be helpful to determine the direction Player should face.
bool CComponent_Input::HalfSpaceTest( vec3f PlanePos, vec3f PlaneNormal, vec3f TestPoint )
{
	// Calculate Vector from the Plane to the Test Point
	vec3f VectorFromPointOnPlaneToTestPoint = (TestPoint - PlanePos);

	// Calculate the distance of the vector in the direction of the normal.
	float Distance = dot_product( PlaneNormal, VectorFromPointOnPlaneToTestPoint );

	// If Distance is Positive or right on it then It passed.
	if( Distance >= 0)
		return true;
	// If Distance is Negative that it failed.
	return false;
};

vec3f CComponent_Input::GetValidPlacement( float _fMaxRange )
{
	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> NearestObjects;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnParams = (1<<RFLAG_AABB);

	// call the kd tree and get the near objects
	//CKdTree::GetNearObjects ( m_pParent, PSFLAG_AABB, ReturnParams, NearestObjects);
	vec2f vectocursor = m_LineToCursor.GetEndPoint2D() - m_LineToCursor.GetStartPoint2D();
	vectocursor.normalize();

	CSceneObject soLineSceneObject;
	Line lToCursor;
	lToCursor.SetVolumeType( VMT_LINE );
	lToCursor.SetStartPoint( m_pParent->GetWorldPos() + vec3f(0.0f, 50.0f, 0.0f) );
	lToCursor.SetEndPoint( m_pParent->GetWorldPos() + vec3f(0.0f, 50.0f, 0.0f) + vec3f( vectocursor.x, vectocursor.y, 0.0f ) * _fMaxRange );
	soLineSceneObject.SetCollidableObject( &lToCursor );

	CKdTree::GetNearObjects( &soLineSceneObject, PSFLAG_LINE, ReturnParams, NearestObjects ); 

	soLineSceneObject.SetCollidableObject( nullptr );

	//loop through all the return objects and check collision with them.

	vec3f ContactPoint = lToCursor.GetEndPoint3D();

	for( unsigned int objindex = 0; objindex < NearestObjects.size(); ++objindex )
	{
		CCollisionVolume* pCurVolume = NearestObjects[objindex]->GetCollidableObject();		

		// Safety Check
		if( pCurVolume )
		{
			// Is the object from the Kd Tree Static or Dynamic
			if( BT_STATIC == pCurVolume->GetBodyType() || BT_DYNAMIC == pCurVolume->GetBodyType() )
			{
				if( VMT_AABB == pCurVolume->GetVolumeType() )
				{
					// Special Ignoore cases.
					if( GetParent() == ((IBaseObject*)NearestObjects[objindex]) )
						continue;
					if( ((IBaseObject*)NearestObjects[objindex])->GetType() == OBJ_PIT )
						continue;
					if( ((IBaseObject*)NearestObjects[objindex])->GetType() == OBJ_POOL )
						continue;
					if( ((IBaseObject*)NearestObjects[objindex])->GetType() == OBJ_DOOR )
					{
						IBaseObject* Door = (IBaseObject*)NearestObjects[objindex];
						CComponent_DoorCollision* pDoorCollision = (CComponent_DoorCollision*)Door->GetComponent( ECOMP_COLLISION );

						if( pDoorCollision->IsDoorOpen() )
						{
							continue;
						}
					}

					if( lToCursor.LineToAABB( *((AABB*)pCurVolume), ContactPoint ) )
					{
						lToCursor.SetEndPoint( vec3f( ContactPoint.x, ContactPoint.y, 0.0f ) );
					}
				}
			}
		}
	}
	return lToCursor.GetEndPoint3D();
}

void CComponent_Input::HandleMovementInput( float fDT )
{
	bool bMoved = false;
	m_bRunning = bMoved;

	if( pColComponent && !pColComponent->GetIsWallSliding() )
	{
		// Determine whether The cursor is to the right of pudge or to the left, and have him face the direction accordingly.
		vec3f WorldX = vec3f(1.0f, 0.0f, 0.0f);
		if( HalfSpaceTest( m_pParent->GetWorldPos(), WorldX, m_pCamera->GetWorldCursor() ) )
		{
			if( m_fRotationTimer < ROTATION_TIME_CAP )
				m_fRotationTimer += fDT;

			// Passed Half Space Test Set Pudge to face right.
			matrix4f _localMat = (*m_pParent->GetLocalMat());

			float Lambda = (m_fRotationTimer / ROTATION_TIME_CAP);
			if( Lambda > 1.0f )
				Lambda = 1.0f;

			// Create a matrix and apply the rotation.
			matrix4f RightRotationMatrix;
			matrix4f LeftRotationMatrix;

			RightRotationMatrix.make_rotation_y( D3DXToRadian(-92) );
			LeftRotationMatrix.make_rotation_y( D3DXToRadian(90) );

			// Translate the matrix by our world position.
			RightRotationMatrix.axis_pos = _localMat.axis_pos;
			LeftRotationMatrix.axis_pos = _localMat.axis_pos;

			// Interpolate from the left Rotation to the Right Rotation.
			D3DXMATRIX interpMatrix = Interpolate(LeftRotationMatrix.ma, RightRotationMatrix.ma, -Lambda );
			_localMat = (*(matrix4f*)&interpMatrix);

			// Set the Frames local matrix to this new calculated matrix that has our rotation.
			m_pParent->SetLocalMat(&_localMat);
			m_pParent->GetWorldMat();

			if( Lambda >= 1.0f )
			{
				// Set rotated right to true.
				RotatedLeft = false;
				RotatedRight = true;
			}
		}
		else
		{
			if( m_fRotationTimer > 0.0f )
				m_fRotationTimer -= fDT;

			// Failed Half Space Test Set Pudge to face left
			matrix4f _localMat = (*m_pParent->GetLocalMat());

			float Lambda = (m_fRotationTimer / ROTATION_TIME_CAP);
			if( Lambda < 0.0f )
				Lambda = 0.0f;

			// Create a matrix and apply the rotation.
			matrix4f RightRotationMatrix;
			matrix4f LeftRotationMatrix;

			RightRotationMatrix.make_rotation_y( D3DXToRadian(-92) );
			LeftRotationMatrix.make_rotation_y( D3DXToRadian(90) );

			// Translate the matrix by our world position.
			RightRotationMatrix.axis_pos = _localMat.axis_pos;
			LeftRotationMatrix.axis_pos = _localMat.axis_pos;

			// Interpolate from the left Rotation to the Right Rotation.
			D3DXMATRIX interpMatrix = Interpolate(LeftRotationMatrix.ma, RightRotationMatrix.ma, Lambda );
			_localMat = (*(matrix4f*)&interpMatrix);

			// Set the Frames local matrix to this new calculated matrix that has our rotation.
			m_pParent->SetLocalMat(&_localMat);
			m_pParent->GetWorldMat();

			if( Lambda <= 0.0f )
			{
				// Set Rotated Left to true.
				RotatedRight = false;
				RotatedLeft = true;
			}
		}
	}
	// This section of code is used to set the rotation and prevent player from changing rotation freely.
	else
	{
		RotatedLeft = RotatedRight = false;
		if( pColComponent )
		{
			if( pColComponent->IsCollidingRight() )
			{
				if( m_fRotationTimer > 0.0f )
					m_fRotationTimer -= fDT;

				matrix4f _localMat = (*m_pParent->GetLocalMat());

				float Lambda = (m_fRotationTimer / ROTATION_TIME_CAP);
				if( Lambda < 0.0f )
					Lambda = 0.0f;

				// If colliding on the right then lets go ahead and have the wall slide with arm on the right.
				matrix4f RightRotationMatrix;
				matrix4f LeftRotationMatrix;

				RightRotationMatrix.make_rotation_y( D3DXToRadian(-92) );
				LeftRotationMatrix.make_rotation_y( D3DXToRadian(90) );

				// Translate the matrix by our world position.
				RightRotationMatrix.axis_pos = _localMat.axis_pos;
				LeftRotationMatrix.axis_pos = _localMat.axis_pos;

				// Interpolate from the left Rotation to the Right Rotation.
				D3DXMATRIX interpMatrix;
				if( RotatedLeft )
				{
					interpMatrix = Interpolate(RightRotationMatrix.ma, LeftRotationMatrix.ma, -Lambda );
				}
				else
				{
					interpMatrix = Interpolate(LeftRotationMatrix.ma, RightRotationMatrix.ma, -Lambda );
				}
				_localMat = (*(matrix4f*)&interpMatrix);

				// Set the Frames local matrix to this new calculated matrix that has our rotation.
				m_pParent->SetLocalMat(&_localMat);
				m_pParent->GetWorldMat();
			}
			else if( pColComponent->IsCollidingLeft() )
			{
				if( m_fRotationTimer < ROTATION_TIME_CAP )
					m_fRotationTimer += fDT;

				// If colliding on the left then lets go ahead and have the wall slide with the arm to the left.
				matrix4f _localMat = (*m_pParent->GetLocalMat());

				float Lambda = (m_fRotationTimer / ROTATION_TIME_CAP);
				if( Lambda > 1.0f )
					Lambda = 1.0f;

				// Create a matrix and apply the rotation.
				matrix4f RightRotationMatrix;
				matrix4f LeftRotationMatrix;


				RightRotationMatrix.make_rotation_y( D3DXToRadian(-92) );
				LeftRotationMatrix.make_rotation_y( D3DXToRadian(90) );


				// Translate the matrix by our world position.
				RightRotationMatrix.axis_pos = _localMat.axis_pos;
				LeftRotationMatrix.axis_pos = _localMat.axis_pos;

				// Interpolate from the left Rotation to the Right Rotation.
				D3DXMATRIX interpMatrix;
				if( RotatedRight )
				{
					interpMatrix = Interpolate(RightRotationMatrix.ma, LeftRotationMatrix.ma, Lambda );
				}
				else
				{
					interpMatrix = Interpolate(LeftRotationMatrix.ma, RightRotationMatrix.ma, Lambda );
				}
				_localMat = (*(matrix4f*)&interpMatrix);

				// Set the Frames local matrix to this new calculated matrix that has our rotation.
				m_pParent->SetLocalMat(&_localMat);
				m_pParent->GetWorldMat();
			}
		}
	}

	if((m_pInput->KeyDown(DIK_D) || m_pInput->KeyDown(DIK_A)) && pPlayerCol)
	{
		if( pPlayerCol->CollidingGround() )
		{
			//we have the timer so the walk sound only plays when we need it
			if(m_fWalkTimer >= WALK_TIME_CAP)
			{
				m_pSoundComponent->PlaySound(AK::EVENTS::PLAY_SFX_PLAYER_WALK);
				m_fWalkTimer = 0.0f;
			}
		}
	}

	if( m_pInput->KeyDown(DIK_D) && !m_bStopInput)
	{
		PlayerMovement_Right( fDT );
		bMoved = true;
	}

	else if( m_pInput->KeyDown(DIK_A) && !m_bStopInput)
	{
		PlayerMovement_Left( fDT );
		bMoved = true;
	}
	else
	{

	}

	if(m_pInput->KeyPressed(DIK_SPACE) && !m_bStopInput)
	{
		//Double false
		PlayerJump( fDT );
	}

	if( pPlayerCol )
	{
		// If player is colliding with the ground set double jump to true and wall jumping to false.
		if( pPlayerCol->CollidingGround() )
		{
			m_bDoubleJump = true;
			m_bWallJumping = false;
		}

		// If I have not moved.
		if( !bMoved )
		{
			// If im not wall jumping
			if( !m_bWallJumping )
				// Apply friction.
				pPlayerCol->GetVel().x *= 0.70f;

			// If im on the ground and im not moving go back to Idle.
			if( pPlayerCol->CollidingGround() )
			{
				if( pAnimComponent )
				{
					State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
					if( ( pudgeState != PudgeRunToIdleState::GetState() ) && ( pudgeState != PudgeIdleState::GetState() )
						&& (pudgeState != PushEffectState::GetState() ) && (pudgeState != PullEffectState::GetState())
						&& (pudgeState != PudgePullState::GetState() ) && (pudgeState != PudgePushState::GetState() ) 
						&& (pudgeState != PudgeIdleToJumpState::GetState() ) && ( pudgeState != PudgeSmallDamageState::GetState())
						&& (pudgeState != PudgeBigDamageState::GetState()))
					{
						pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeRunToIdleState::GetState() );
					}
				}
			}
		}

		if( !pPlayerCol->CollidingGround() && !pPlayerCol->CollidingWall() )
		{
			// If im not colliding with the ground or the Wall then im in the air.
			if( pAnimComponent )
			{
				if( pAnimComponent->GetAnimStateMachine()->GetCurrentState() != PudgeJumpToFallingState::GetState() && 
					pAnimComponent->GetAnimStateMachine()->GetCurrentState() != PudgeJumpState::GetState() &&
					pAnimComponent->GetAnimStateMachine()->GetCurrentState() != PudgeIdleToJumpState::GetState())
					pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeFallingState::GetState() );
			}
		}
	}

	// Reset wall jump timer if im not colliding with a wall.
	if( !pPlayerCol->CollidingWall() && (pColComponent->GetWallTimer() >= WALLSLIDETIMER) )
		pColComponent->ResetWallJumpTimer();

	if(!pPlayerCol->CollidingGround() && m_bPreviouslyOnGround)
	{
		m_bPreviouslyOnGround = false;
	}
}

bool CComponent_Input::GetPulling()
{
	if( VACS_PULL == m_vsWeaponState )
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: CheckForInbetweens
// Purpose: This function tells us if there's any AABBs between the player and what he's trying to pull. Returns true
//			if there's something blocking my pull.
// Original Author: Josh Morgan
// Creation Date: 8/16/2012
// Last Modification By:
// Last Modification Date:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CComponent_Input::CheckForInbetweens(IBaseObject* _pTarget)
{
	if(!_pTarget)
	{
		return false;
	}

	//creating the sound sphere
	Sphere LoSRadius;
	LoSRadius.SetRadius(MAX_PULL_DIST);
	LoSRadius.SetCenter(m_pParent->GetWorldPos());
	CSceneObject LoSSphere;
	LoSSphere.SetCollidableObject(&LoSRadius);

	// create a return vector to hold all the objects the kd tree returns
	std::vector <CSceneObject*> ReturnVector;
	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	int ReturnParams = 0;
	int ReturnBody = 0;
	int ReturnObjects = 1<<OBJ_CRATE | 1<<OBJ_CORK | 1<<OBJ_PLUG | 1<<OBJ_GOO | 1<<OBJ_WORLD_COLLISION;

	CKdTree::GetNearObjects(&LoSSphere, PSFLAG_SPHERE, ReturnParams, ReturnVector, ReturnBody, ReturnObjects);

	//bool for checking if there's the player, and the position of the player
	bool bPulledObjectInRange = false;

	for(unsigned int i = 0; i < ReturnVector.size(); i++)
	{
		IBaseObject* pObject = ((IBaseObject*)ReturnVector[i]);

		if(m_tPulledObj.m_Object && pObject == m_tPulledObj.m_Object)
		{
			bPulledObjectInRange = true;
			break;
		}
	}

	if(!bPulledObjectInRange)
	{
		return false;
	}

	CComponent_Collision* boxCol = (CComponent_Collision*)_pTarget->GetComponent(ECOMP_COLLISION);
	vec3f tObjectPos = boxCol->GetPhysicsBox()->GetCentroid();

	//we make a line to the player since he's in aggro range
	CSceneObject soLineSceneObject;
	Line LineToPlayer;
	LineToPlayer.SetVolumeType(VMT_LINE);
	LineToPlayer.SetStartPoint(vec3f(m_pParent->GetWorldPos().x, m_pParent->GetWorldPos().y + 50.0f, m_pParent->GetWorldPos().z));
	LineToPlayer.SetEndPoint(vec3f(tObjectPos.x, tObjectPos.y, tObjectPos.z));
	soLineSceneObject.SetCollidableObject(&LineToPlayer);

	CKdTree::GetNearObjects(&soLineSceneObject, PSFLAG_LINE, ReturnParams, ReturnVector, ReturnBody, ReturnObjects); 

	soLineSceneObject.SetCollidableObject(nullptr);

	//loop through all the return objects again and check collision with them.
	for(unsigned int i = 0; i < ReturnVector.size(); ++i)
	{
		IBaseObject* pObject = ((IBaseObject*)ReturnVector[i]);

		if(pObject->GetType() == OBJ_WORLD_COLLISION)
		{
			//check to see if our line to the player is obstructed by this ocject
			vec3f Intersection = vec3f(FLT_MAX, FLT_MAX, FLT_MAX);
			if(LineToPlayer.LineToAABB(*((AABB*)pObject->GetCollidableObject()), Intersection))
			{
				//D3DXMATRIX mat;
				//D3DXMatrixIdentity(&mat);
				//mat._41 = Intersection.x;
				//mat._42 = Intersection.y;
				//mat._43 = -500;
				//DebugShapes::RenderSphere(mat);

				//we see that there's something between us so I don't have line of sight
				return false;
			}
		}
	}
	return true;
}