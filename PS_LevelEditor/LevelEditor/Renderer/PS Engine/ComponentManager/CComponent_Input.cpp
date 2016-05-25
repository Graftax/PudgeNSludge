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
//#include "../ObjectLib/ObjectManager/CObjectManager.h"

#define IF(x) if(x)(x)

#define WALK_TIME_CAP 0.25f

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

	m_nGlowHandle = TextureManager::GetInstance()->LoadTexture( "./Resources/Textures/PS_OBJH.png" );

	pAnimComponent = NULL;
	pColComponent = NULL;
	m_pEmitterComponent = NULL;

	m_LineToCursor.SetVolumeType(VMT_LINE);

	m_fWalkTimer = 0.0f;

	m_bPreviouslyOnGround = false;
	m_pEffects = NULL;
	m_vsWeaponState = VACS_NULL;
	m_pPullEmitterComponent = nullptr;
	m_pPushEmitterComponent = nullptr;

	m_bCreatePullOnce = false;

	return true;
}

void CComponent_Input::Update(float fDT)
{
	if ( !m_pParent->GetAlive() )
	{
		return ;
	}

	// Initialize all the components that this component will use.
	if( !pAnimComponent )
		pAnimComponent = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );
	if( !pPlayerCol )
		pPlayerCol = (CComponent_Collision*)m_pParent->GetComponent( ECOMP_COLLISION );
	if( !pColComponent )
		pColComponent = (CComponent_PlayerCollision*)pPlayerCol;
	if(!m_pEmitterComponent)
		m_pEmitterComponent = (CComponent_Emitter*)m_pParent->GetComponent( ECOMP_EMITTER );
	if(!m_pSoundComponent)
		m_pSoundComponent = (CComponent_Sound*)(m_pParent->GetComponent(ECOMP_SOUND));




	m_fWalkTimer += fDT;

	if(!m_bPreviouslyOnGround && pPlayerCol->CollidingGround())
	{
		m_bPreviouslyOnGround = true;
		m_pSoundComponent->PlaySound(SFX_PLAYER_LAND);
	}

	// set start point to about where the arm will be
	m_LineToCursor.SetStartPoint( m_pParent->GetWorldPos() + vec3f(0.0f, 50.0f, 0.0f) );
	m_LineToCursor.SetEndPoint( m_pCamera->GetWorldCursor() );

	//Get the closest object to the pull
	m_pCurrentTarget = GetClosestObject( m_pParent->GetWorldPos() + vec3f(0.0f, 50.0f, 0.0f), GetValidPlacement( MAX_PULL_DIST ) );

	if( m_pPullEmitterComponent )
		m_pPullEmitterComponent->SetPullStart( GetValidPlacement( MAX_PULL_DIST ) );
	if( m_pPushEmitterComponent )
		m_pPushEmitterComponent->SetPullStart( GetValidPlacement( MAX_PULL_DIST ) );

	if( m_tPulledObj.m_bIsAnObjectPulled )
		m_pCurrentTarget = nullptr;
	//Check to see if there is an object and what kind it is
	if( m_pCurrentTarget )
	{
		if(m_pCurrentTarget->GetType() == OBJ_CRATE || m_pCurrentTarget->GetType() == OBJ_CORK || m_pCurrentTarget->GetType() == OBJ_PLUG)
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
					// Set the Frame data that will be used to render.
					Frame glowTrans;
					glowTrans.GetLocalMat().make_scale( 300.0f, 300.0f, 1.0f );		
					glowTrans.GetLocalMat().axis_pos.x = m_pCurrentTarget->GetWorldPos().x - 150.0f;
					glowTrans.GetLocalMat().axis_pos.y = m_pCurrentTarget->GetWorldPos().y - 150.0f;
					glowTrans.GetLocalMat().axis_pos.z = m_pCurrentTarget->GetWorldPos().z + 37.5f;

					glowTrans.Update();

					// Render the Glow
					RenderEngine::AddGlowToDraw( (D3DXMATRIX)glowTrans.GetWorldMat().ma, TextureManager::GetInstance()->m_umTextureMap[m_nGlowHandle].m_lpTexture);
				}
			}
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
				m_pSoundComponent->StopSound(SFX_PLAYER_PULL_LOOP);
				m_vsWeaponState = VACS_HASOBJECT;
				break;
			}
			if(m_vsWeaponState != VACS_HASOBJECT)
			{
				if( m_pInput->LeftMouseButtonPressed())
				{
					if(m_bCreatePullOnce == false)
					{
						CObjectManager::GetInstance()->CreateObject( OBJ_PULL_MESHEFFECT, m_pParent->GetPositionMatrix().ma);
						m_bCreatePullOnce = true;
					}
				}
			}


			if(m_pInput->LeftMouseButtonDown())
			{
				// If the Drop timer is less than 0 we can pull again.
				if(m_fDropTimer <= 0.0f)
				{
					// If there is not an object in our possesion.
					if(!m_tPulledObj.m_bIsAnObjectPulled)
					{
						m_pSoundComponent->PlaySound(SFX_PLAYER_PULL_LOOP);
						// Set the Animation to Pull.
						if( pAnimComponent )
						{
							State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
							if( ( pudgeState != PudgeIdleToRunState::GetState() ) && ( pudgeState != PudgeRunState::GetState() ) &&
								( pudgeState != PudgeIdleToJumpState::GetState() ) && ( pudgeState != PudgeJumpState::GetState() ) &&
								( pudgeState != PudgeJumpToFallingState::GetState() ) && ( pudgeState != PudgeFallingState::GetState() ) &&
								( pudgeState != PudgeWallJumpState::GetState()) && ( pudgeState != PudgePushState::GetState()) && 
								( pudgeState != PudgeWallSlideState::GetState())/* && ( pudgeState != PudgeSmallDamageState::GetState()) &&
																				( pudgeState != PudgeBigDamageState::GetState())*/)
							{
								pAnimComponent->GetAnimStateMachine()->ChangeState( PudgePullState::GetState() );
							}
						}

						// Call our Pull Function which will handle all of the pulling from the object along with the Item Component.
						PlayerPull( m_pCurrentTarget );
						m_vsWeaponState = VACS_PULL;
						break;
					}
				}
			}

			if(m_pInput->RightMouseButtonPressed())
			{
				// Play the Pushing Sound and Call the Player Push Function.
				m_pSoundComponent->PlaySound(SFX_PLAYER_PUSH);
				CObjectManager::GetInstance()->CreateObject( OBJ_PUSH_MESHEFFECT, m_pParent->GetPositionMatrix().ma);
				PlayerPush( fDT );
				m_vsWeaponState = VACS_PUSH;
				break;
			}

			break;
		}
	case VACS_PULL:
		{
			if(!m_pInput->LeftMouseButtonDown() )
			{
				// Stop the sound and set our state to no longer pulling.
				m_pSoundComponent->StopSound(SFX_PLAYER_PULL_LOOP);
				// Stop playing the pull animation.
				if( pAnimComponent )
				{
					State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
					if( ( pudgeState != PudgeSmallDamageState::GetState()) && ( pudgeState != PudgeBigDamageState::GetState()))
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
				// Set the Animation to Pull.
				if( pAnimComponent )
				{
					State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
					if( ( pudgeState != PudgeIdleToRunState::GetState() ) && ( pudgeState != PudgeRunState::GetState() ) &&
						( pudgeState != PudgeIdleToJumpState::GetState() ) && ( pudgeState != PudgeJumpState::GetState() ) &&
						( pudgeState != PudgeJumpToFallingState::GetState() ) && ( pudgeState != PudgeFallingState::GetState() ) &&
						( pudgeState != PudgeWallJumpState::GetState()) && ( pudgeState != PudgePushState::GetState()) && 
						( pudgeState != PudgeWallSlideState::GetState())/* && ( pudgeState != PudgeSmallDamageState::GetState()) &&
																		( pudgeState != PudgeBigDamageState::GetState())*/)
					{
						pAnimComponent->GetAnimStateMachine()->ChangeState( PudgePullState::GetState() );
					}
				}

				// Call our Pull Function which will handle all of the pulling from the object along with the Item Component.
				//CObjectManager::GetInstance()->CreateObject(OBJ_PULL_MESHEFFECT, m_pParent->GetPositionMatrix().ma);
				PlayerPull( m_pCurrentTarget );
				break;
			}
			else
			{
				m_pSoundComponent->StopSound(SFX_PLAYER_PULL_LOOP);
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
			if(m_pInput->LeftMouseButtonPressed())
			{
				//drop the object
				// if there is a pulled object
				if( m_tPulledObj.m_bIsAnObjectPulled )
				{
					// Grab the Players Matrix and offset the box from that matrix.
					matrix4f ObjectsMatrix = m_pParent->GetPositionMatrix();// = m_pParent->GetFrame().GetWorldMat();

					ObjectsMatrix.axis_pos = GetValidPlacement( 120.0f ) + ( m_LineToCursor.GetStartPoint3D() - m_LineToCursor.GetEndPoint3D() ).normalize() * 30.0f;

					// If an object is Pulled go ahead and create it, based on the offset position from player.
					IBaseObject* pObjectPulled = CObjectManager::GetInstance()->CreateObject(m_tPulledObj.ObjectPulledType, ObjectsMatrix.ma );

					((CComponent_Collision*)pObjectPulled->GetComponent( ECOMP_COLLISION ))->SetTimeLeft( 0.75f );

					// Set a Timer up to prevent player from pulling object back in immediately.
					m_fDropTimer = 0.3f;

					// Set the object pulled variable to false and the object type to NULL to know we no longer have hold of this object.
					m_tPulledObj.m_bIsAnObjectPulled = false;
					m_tPulledObj.ObjectPulledType = OBJ_NULL;
					// Notify the Render Engine that the object is no longer in our possesion.
					RenderEngine::SetObjPulled(false);

					m_vsWeaponState = VACS_NULL;
					break;
				}
			}

			if(m_pInput->RightMouseButtonPressed())
			{
				// Play the Pushing Sound and Call the Player Push Function.
					m_pSoundComponent->PlaySound(SFX_PLAYER_PUSH);
					CObjectManager::GetInstance()->CreateObject( OBJ_PUSH_MESHEFFECT, m_pParent->GetPositionMatrix().ma);
					PlayerPush( fDT );
					m_vsWeaponState = VACS_NULL;
				break;
			}

			break;
		}
	}

	HandleMovementInput( fDT );
	
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
		//if( pColl->GetVel().x < FLT_EPSILON )
		//{
		//	if( pColl->CollidingGround() )
		//	{
		//		pColl->GetVel().x = 0.0f;
		//	}
		//}

		if( pPlayerCol->GetVel().x < 8.0f * m_fPlayerSpeedMod  )
		{
			pPlayerCol->GetVel() += vec3f(1.0f, 0.0f, 0.0f) * PLAYER_MOVE_SPEED * 2.0f * fDT * m_fPlayerSpeedMod;
		}
		/*m_bRunning = true*/
	}
	//else
	/*m_bRunning = false*/

	if( pPlayerCol )
	{
		if( pPlayerCol->CollidingGround() )
		{
			if( pAnimComponent )
			{
				State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
				if( pudgeState != PudgeRunState::GetState() && pudgeState != PudgeSmallDamageState::GetState()
					&& pudgeState != PudgeBigDamageState::GetState() && pudgeState != PudgeIdleToJumpState::GetState() )
				{
					pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeIdleToRunState::GetState() );
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
		//if( pColl->GetVel().x > FLT_EPSILON )
		//{
		//	if( pColl->CollidingGround() )
		//	{
		//		pColl->GetVel().x = 0.0f;
		//	}
		//}

		if( pPlayerCol->GetVel().x > -8.0f * m_fPlayerSpeedMod )
		{
			pPlayerCol->GetVel() += vec3f(-1.0f, 0.0f, 0.0f) * PLAYER_MOVE_SPEED * 2.0f * fDT * m_fPlayerSpeedMod;
		}
		/*m_bRunning = true*/
	}
	//else
	/*m_bRunning = false*/

	if( pPlayerCol )
	{
		if( pPlayerCol->CollidingGround() )
		{
			if( pAnimComponent )
			{
				State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
				if( pudgeState != PudgeRunState::GetState() && pudgeState != PudgeSmallDamageState::GetState()
					&& pudgeState != PudgeBigDamageState::GetState() && pudgeState != PudgeIdleToJumpState::GetState() )
				{
					pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeIdleToRunState::GetState() );

					if(m_pEmitterComponent)
					{
						m_pEffects = m_pEmitterComponent->GetEffects();
						for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
						{
							m_pEffects->GetEmitters()[i].SetEmitterActive(true);
						}
					}
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
	// Transition to Push Animation.
	if( pAnimComponent )
	{
		State* pudgeState = (State*)pAnimComponent->GetAnimStateMachine()->GetCurrentState();
		if( ( pudgeState != PudgeIdleToRunState::GetState() ) && ( pudgeState != PudgeRunState::GetState() ) &&
			( pudgeState != PudgeIdleToJumpState::GetState() ) && ( pudgeState != PudgeJumpState::GetState() ) &&
			( pudgeState != PudgeJumpToFallingState::GetState() ) && ( pudgeState != PudgeFallingState::GetState() ) &&
			( pudgeState != PudgeWallJumpState::GetState()) && ( pudgeState != PudgePullState::GetState()))
		{
			pAnimComponent->GetAnimStateMachine()->ChangeState( PudgePushState::GetState() );
		}
	}

	// if there is a pulled object
	if( m_tPulledObj.m_bIsAnObjectPulled )
	{
		// use the laser sight line to get 
		// the direction the player is aiming
		vec2f vectocursor = m_LineToCursor.GetEndPoint2D() - m_LineToCursor.GetStartPoint2D();
		vectocursor.normalize();

		//matrix4f ObjectsMatrix = m_pParent->GetFrame().GetWorldMat();
		matrix4f ObjectsMatrix = m_pParent->GetPositionMatrix();
		//ObjectsMatrix.axis_pos.y += 50.0f;
		//if( GetRotatedRight() )
		//	ObjectsMatrix.axis_pos.x += 80.0f;
		//else
		//	ObjectsMatrix.axis_pos.x -= 80.0f;
		ObjectsMatrix.axis_pos = GetValidPlacement( 120.0f ) + ( m_LineToCursor.GetStartPoint3D() - m_LineToCursor.GetEndPoint3D() ).normalize() * 30.0f;

		// If an object is Pulled go ahead and create it.
		IBaseObject* pObjectPulled = CObjectManager::GetInstance()->CreateObject(m_tPulledObj.ObjectPulledType, ObjectsMatrix.ma );
		CComponent_Collision* pObjectsCollision = (CComponent_Collision*)pObjectPulled->GetComponent( ECOMP_COLLISION );

		((CComponent_Collision*)pObjectPulled->GetComponent( ECOMP_COLLISION ) )->SetTimeLeft( 0.75f );

		// If its a little goo, let it know that its in the air.
		if( m_tPulledObj.ObjectPulledType == OBJ_GOO )
		{
			// Turns the AI off and sets the goo to die on the next contact.
			((CComponent_Lil_Goo_AI*)(pObjectPulled->GetComponent( ECOMP_AI ) ) )->SetActive( false );
			((CComponent_LittleGooCollision*)(pObjectPulled->GetComponent( ECOMP_COLLISION ) ) )->GetVolitile() = true;
			((CComponent_Animation*)(pObjectPulled->GetComponent( ECOMP_ANIMATION ) ) )->GetAnimStateMachine()->ChangeState( LilGooFlyingState::GetState() );
		}

		// Apply a force in that direction
		pObjectsCollision->CleanForce(  vec2f(vectocursor.x, vectocursor.y ) * THROW_SPEED );

		// We no longer have a pulled object so set bool to false and object type to NULL.
		m_tPulledObj.m_bIsAnObjectPulled = false;
		m_tPulledObj.ObjectPulledType = OBJ_NULL;
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

			/*char buffer[100];
			sprintf_s(buffer, "%f", fDistToTarget );
			Logger::ClearConsole();
			LOG(LOG_DEBUG," CComponent_Input::PlayerPush() : NUDGE FORCE - ", buffer  );*/

			// Object Safety Check

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
						if ( !((CComponent_Lil_Goo_AI*)PulledObjectCollisionComponent->GetParent()->GetComponent(ECOMP_AI))->GetDead () )
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
}

void CComponent_Input::PlayerJump( float fDT )
{
	if( pPlayerCol )
	{
		// If the player is touching the ground.
		if( pPlayerCol->CollidingGround() )
		{
			//m_pAudio->PlayEffect(m_pSoundObject, PLAYER_JUMP);
			m_pSoundComponent->PlaySound(SFX_PLAYER_JUMP);
			float fOldX = pPlayerCol->GetVel().x;

			// Preserve the x component of the velocity and reset the y component to up.
			pPlayerCol->GetVel() = vec3f(0.0f, 1000.0f, 0.0f)  * ( m_fPlayerSpeedMod ) * fDT;
			pPlayerCol->GetVel().x = fOldX;

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
				m_pSoundComponent->PlaySound(SFX_PLAYER_JUMP);
				// Preserve the old x velocity and reset the y to up.
				float fOldX = pPlayerCol->GetVel().x;
				pPlayerCol->GetVel() = vec3f(0.0f, 1000.0f, 0.0f) * ( m_fPlayerSpeedMod )* fDT;
				pPlayerCol->GetVel().x = fOldX;
				m_bDoubleJump = false;

				if( pAnimComponent )
				{
					pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeIdleToJumpState::GetState() );
				}
			}
			// If they pressed jump, are out of double jumps, and are colliding with a wall.
			else if( pPlayerCol->CollidingWall() )
			{
				//m_pAudio->PlayEffect(m_pSoundObject, PLAYER_JUMP);
				m_pSoundComponent->PlaySound(SFX_PLAYER_JUMP);
				// Set the up velocity.
				pPlayerCol->GetVel() = vec3f( 0.0, 1000.0f, 0.0f) * ( m_fPlayerSpeedMod ) * fDT;
				// Add in the push away from the wall.
				vec3f pushoutVector = pColComponent->GetWallJumpPushOutVector();
				pPlayerCol->GetVel().x += (pushoutVector * 500.0f  * m_fPlayerSpeedMod ).x * fDT;
				m_bWallJumping = true;

				if( pAnimComponent )
				{
					pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeWallJumpState::GetState() );
				}
			}
			/*m_bRunning = false*/
		}
	}
	/*m_bRunning = false*/
}

void CComponent_Input::Shutdown(void)
{
	//m_pAudio->StopEffect(m_pSoundObject, PLAYER_PULL_LOOP);
	m_pSoundComponent->StopSound(SFX_PLAYER_PULL_LOOP);
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetClosestWall():	This function will get the nearest wall and 
//						return it.
//
// Returns:				IBaseObject* - the closest wall, or null if no
//										walls have been collided with.
//
// Mod Date:			6/7/12
// Mod Initials:		JM
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IBaseObject* CComponent_Input::GetClosestWall(float &fDist)
{
	IBaseObject* closest = nullptr;

	CSceneObject lineObject;
	lineObject.SetCollidableObject(&m_LineToCursor);

	// create a return vector to hold all the objects the kd tree returns
	vector <CSceneObject*> ReturnVector;

	// create a unsigned int that will tell the kd tree what you want put in the return vector
	// this uses bit wise operations so you can have more then one object returned
	// use the return flags enum from the kd tree so you know what you can get back
	unsigned int ReturnPrams = (1<<RFLAG_AABB);

	//use the kd tree to get the nearest objects
	CKdTree::GetNearObjects (&lineObject, PSFLAG_LINE, ReturnPrams, ReturnVector);
	lineObject.SetCollidableObject(nullptr);

	//loop through the nearest objects and check to see if we collide with any of them
	for(unsigned int i = 0; i < ReturnVector.size(); ++i)
	{
		//if we collide return the nearest object else return null
		if(ReturnVector[i]->GetCollidableObject()->GetBodyType() == BT_STATIC)
		{
			vec3f Intersection = vec3f(FLT_MAX, FLT_MAX, FLT_MAX);
			m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
			//D3DXMATRIX mat;
			//D3DXMatrixIdentity(&mat);
			//mat._41 = Intersection.x;
			//mat._42 = Intersection.y;
			//mat._43 = -500;
			//DebugShapes::RenderSphere(mat);

			float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
			if(D < fDist)
			{
				fDist = D;
				closest = (IBaseObject*)ReturnVector[i];
			}
		}

		if(ReturnVector[i]->GetCollidableObject()->GetBodyType() == BT_DYNAMIC)
		{
			int type = ((IBaseObject*)ReturnVector[i])->GetType();
			switch(type)
			{
			case OBJ_WALL_BREAKABLE_HOR:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_WALL_BREAKABLE_VER:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_BRIDGE:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_MONSTER:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_HULK:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_BOSS:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_PIPE_STEAM:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_PIPE_INTAKE:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_PIPE_SPAWN:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_PIPE_BOX:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_CRUSHER:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			case OBJ_CHKPNT:
				{
					vec3f Intersection;
					m_LineToCursor.LineToAABB(*((AABB*)ReturnVector[i]->GetCollidableObject()), Intersection);
					//D3DXMATRIX mat;
					//D3DXMatrixIdentity(&mat);
					//mat._41 = Intersection.x;
					//mat._42 = Intersection.y;
					//mat._43 = -500;
					//DebugShapes::RenderSphere(mat);

					float D = (Intersection - m_LineToCursor.GetStartPoint3D()).magnitude();
					if(D < fDist)
					{
						fDist = D;
						closest = (IBaseObject*)ReturnVector[i];
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
	}

	return closest;
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

					if( lToCursor.LineToAABB( *((AABB*)pCurVolume), ContactPoint ) )
					{
						lToCursor.SetEndPoint( vec3f( ContactPoint.x, ContactPoint.y, -300.0f ) );
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

	if((m_pInput->KeyDown(DIK_D) || m_pInput->KeyDown(DIK_A)) && pPlayerCol)
	{
		if( pPlayerCol->CollidingGround() )
		{
			//we have the timer so the walk sound only plays when we need it
			if(m_fWalkTimer >= WALK_TIME_CAP)
			{
				m_pSoundComponent->PlaySound(SFX_PLAYER_WALK);
				m_fWalkTimer = 0.0f;
			}
		}
		else
		{
			if(m_pEmitterComponent)
			{
				if(m_pEffects)
				{
					m_pEffects = m_pEmitterComponent->GetEffects();
					for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
					{
						m_pEmitterComponent->SetType(FOOT_STEPS_FX);
						m_pEffects->GetEmitters()[i].SetEmitterActive(false);
					}
				}
			}
		}

	}

	if( m_pInput->KeyDown(DIK_D) )
	{
		PlayerMovement_Right( fDT );
		/*m_bRunning = */bMoved = true;
		if(m_pEffects && pPlayerCol->CollidingGround())
		{
			m_pEffects = m_pEmitterComponent->GetEffects();
			for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
			{
				m_pEmitterComponent->SetType(FOOT_STEPS_FX);
				m_pEffects->GetEmitters()[i].SetEmitterActive(true);
			}
		}
	}

	else if( m_pInput->KeyDown(DIK_A) )
	{
		PlayerMovement_Left( fDT );
		/*m_bRunning = */bMoved = true;
		if(m_pEffects && pPlayerCol->CollidingGround())
		{
			m_pEffects = m_pEmitterComponent->GetEffects();
			for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
			{
				m_pEmitterComponent->SetType(FOOT_STEPS_FX);
				m_pEffects->GetEmitters()[i].SetEmitterActive(true);
			}
		}
	}
	else//if(m_pEmitterComponent)
	{
		if(m_pEffects)
		{
			m_pEffects = m_pEmitterComponent->GetEffects();
			for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
			{
				//m_pEmitterComponent->SetType(FOOT_STEPS_FX);
				m_pEffects->GetEmitters()[i].SetEmitterActive(false);
			}
		}
	}

	if(m_pInput->KeyPressed(DIK_SPACE))
	{
		//Double false
		/*m_bRunning = false*/
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
					pAnimComponent->GetAnimStateMachine()->GetCurrentState() != PudgeJumpState::GetState() )
					pAnimComponent->GetAnimStateMachine()->ChangeState( PudgeFallingState::GetState() );
			}
		}
	}

	if( pColComponent && !pColComponent->GetIsWallSliding() )
	{
		// Determine whether The cursor is to the right of pudge or to the left, and have him face the direction accordingly.
		vec3f WorldX = vec3f(1.0f, 0.0f, 0.0f);
		if( HalfSpaceTest( m_pParent->GetWorldPos(), WorldX, m_pCamera->GetWorldCursor() ) )
		{
			if( !RotatedRight )
			{

				// Passed Half Space Test Set Pudge to face right.
				matrix4f _localMat = (*m_pParent->GetLocalMat());

				// Set rotated right to true.
				RotatedLeft = false;
				RotatedRight = true;

				// Create a matrix and apply the rotation.
				matrix4f rotationMatrix;
#if !_ANIMATIONS
				rotationMatrix.make_rotation_y( D3DXToRadian(90) );
#else
				rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
#endif
				// Translate the matrix by our world position.
				rotationMatrix.axis_pos = _localMat.axis_pos;
				_localMat = rotationMatrix;

				// Set the Frames local matrix to this new calculated matrix that has our rotation.
				m_pParent->SetLocalMat(&_localMat);
				m_pParent->GetWorldMat();

			}
		}
		else
		{
			if( !RotatedLeft )
			{
				// Set Rotated Left to true.
				RotatedRight = false;
				RotatedLeft = true;

				// Failed Half Space Test Set Pudge to face left
				matrix4f _localMat = (*m_pParent->GetLocalMat());

				// Create a matrix and apply the rotation.
				matrix4f rotationMatrix;
#if !_ANIMATIONS
				rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
#else
				rotationMatrix.make_rotation_y( D3DXToRadian(90) );
#endif
				// Translate the matrix by our world position.
				rotationMatrix.axis_pos = _localMat.axis_pos;
				_localMat = rotationMatrix;

				// Set the Frames local matrix to this new calculated matrix that has our rotation.
				m_pParent->SetLocalMat(&_localMat);
				m_pParent->GetWorldMat();
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

				matrix4f _localMat = (*m_pParent->GetLocalMat());

				// If colliding on the right then lets go ahead and have the wall slide with arm no the right.
				// Create a matrix and apply the rotation.
				matrix4f rotationMatrix;
#if !_ANIMATIONS
				rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
#else
				rotationMatrix.make_rotation_y( D3DXToRadian(90) );
#endif
				// Translate the matrix by our world position.
				rotationMatrix.axis_pos = _localMat.axis_pos;
				_localMat = rotationMatrix;

				// Set the Frames local matrix to this new calculated matrix that has our rotation.
				m_pParent->SetLocalMat(&_localMat);
				m_pParent->GetWorldMat();
			}
			else if( pColComponent->IsCollidingLeft() )
			{
				// If colliding on the left then lets go ahead and have the wall slide with the arm to the left.
				matrix4f _localMat = (*m_pParent->GetLocalMat());

				// Create a matrix and apply the rotation.
				matrix4f rotationMatrix;
#if !_ANIMATIONS
				rotationMatrix.make_rotation_y( D3DXToRadian(90) );
#else
				rotationMatrix.make_rotation_y( D3DXToRadian(-90) );
#endif
				// Translate the matrix by our world position.
				rotationMatrix.axis_pos = _localMat.axis_pos;
				_localMat = rotationMatrix;

				// Set the Frames local matrix to this new calculated matrix that has our rotation.
				m_pParent->SetLocalMat(&_localMat);
				m_pParent->GetWorldMat();
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