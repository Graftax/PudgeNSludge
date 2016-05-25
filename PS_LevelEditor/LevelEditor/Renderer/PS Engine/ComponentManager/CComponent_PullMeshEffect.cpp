#include "CComponent_PullMeshEffect.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "..\InputLib\InputManager.h"
#include "../EffectsLib/CEffectsIncludes.h"
#include "Component_Emitter.h"
#include "CComponent_PlayerCollision.h"
#include "CComponent_Input.h"
#include "../ObjectLib/Objects/IBaseObject.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"


CComponent_PullMeshEffect::CComponent_PullMeshEffect(void) : IComponent(ECOMP_MESHEFFECT)//(ECOMP_PULL_MESHEFFECT)
{
	m_pGame = NULL;
	m_pAnimation = NULL;
	m_pEmitterComponent = NULL;
	m_pInput = NULL;
	m_pEmitter = NULL;
	m_pParticleMan = NULL;
	m_pEffects = NULL;
	m_pEmitter = NULL;
	m_pCamera = NULL;
	m_pPlayer = NULL;
	m_bEffectHasPlayed = false;
	m_pPlayerArm = NULL;
	m_mGunJointMatrix.make_identity();
}

CComponent_PullMeshEffect::~CComponent_PullMeshEffect(void)
{
}

bool CComponent_PullMeshEffect::Initialize()
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pParticleMan = CParticleManager::GetInstance();
	m_pCamera = m_pGame->GetCamera();
	m_pEffects = NULL;
	m_pPlayerArm = NULL;
	m_mGunJointMatrix.make_identity();

	return true;
}

void CComponent_PullMeshEffect::Update( float dt )
{
	if(!m_pAnimation)
		m_pAnimation = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );
	if(!m_pEmitterComponent)
	{
		m_pEmitterComponent = (CComponent_Emitter*)m_pParent->GetComponent( ECOMP_EMITTER );
	}
	if(!m_pPlayer)
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();
	if( !m_pPlayerArm )
		m_pPlayerArm = CObjectManager::GetInstance()->GetPlayerArm();


	//D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_pParent->GetWorldPos()));
	//m_pParticleMan->GetParticleEffect()[PUSH_PULL_FX]->GetEmitters()[0].SetPosition(pos);


#if _ANIMATIONS
  	// Grab the parents world and local matrix.
  	matrix4f _worldMat = (*m_pParent->GetWorldMat());
  	matrix4f _localMat = (*m_pParent->GetLocalMat());
  
  	// Grab the World cursor Position.
  	vec3f _CursorPos = m_pCamera->GetWorldCursor();
  	// Calculate Vector from your Position to the Cursor Position.
  	vec3f PosToCursor =  _CursorPos - _worldMat.axis_pos;
  	PosToCursor.normalize();
  	// Dot the Forward vector of the arm with the vector calculated above to determine
  	// how much the arm needs to rotate by.
  	float AngleBetween = dot_product( _worldMat.axis_y.negate(), PosToCursor);
  	_localMat.rotate_x_pre( (AngleBetween) );
  
  	//Set the effects position to players position and offsetting it
  	//Gonna have to set it to the players arm instead of the player himself
  	_localMat.axis_pos = m_pPlayer->GetLocalMat()->axis_pos;
  	_localMat.axis_pos.y = _localMat.axis_pos.y + 50.0f;
  
  	// Set Local Matrix.
  	m_pParent->SetLocalMat( &_localMat );

	//If player is not dead. Allow player to pull 
	if(((CComponent_Health*)m_pPlayer->GetComponent(ECOMP_HEALTH))->GetHealth() >= 0.1f)
	{
		if(m_pInput->LeftMouseButtonDown())
		{
			if(((CComponent_Input*)m_pPlayer->GetComponent(ECOMP_INPUT))->GetWeaponState() != VACS_HASOBJECT)
			{
				if( m_pAnimation )   
				{
					if( m_pAnimation->GetAnimStateMachine()->GetCurrentState() != PullEffectPullForeverState::GetState() )
						m_pAnimation->GetAnimStateMachine()->ChangeState( PullEffectState::GetState() );

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
			else
			{
				//CObjectManager::GetInstance()->AddObjectToRemove(m_pParent);
				if(m_pEmitterComponent)
				{
					m_pEffects = m_pEmitterComponent->GetEffects();
					for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
					{
						m_pEffects->GetEmitters()[i].SetEmitterActive(false);
						//m_pEffects->GetEmitters()[i].ResetAllParticles();
					}
				}
				m_pAnimation->GetAnimStateMachine()->ChangeState( PullEffectIdleState::GetState() );
			}

		}
		else
		{
			if(m_pEmitterComponent)
			{
				m_pEffects = m_pEmitterComponent->GetEffects();
				for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
				{
					m_pEffects->GetEmitters()[i].SetEmitterActive(false);
					m_pEffects->GetEmitters()[i].ResetAllParticles();
				}
			}
 			if( m_pAnimation )
 			{
 				m_pAnimation->GetAnimStateMachine()->ChangeState( PullEffectIdleState::GetState() );
 			}
		}
	}
	else //Player is dead turn off particles, turn off mesh effect
	{
		if(m_pEmitterComponent)
		{
			m_pEffects = m_pEmitterComponent->GetEffects();
			for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
			{
				m_pEffects->GetEmitters()[i].SetEmitterActive(false);
				m_pEffects->GetEmitters()[i].ResetAllParticles();
			}
		}
		CObjectManager::GetInstance()->AddObjectToRemove(m_pParent);
		if( m_pAnimation )
		{
			m_pAnimation->GetAnimStateMachine()->ChangeState( PullEffectIdleState::GetState() );
		}
	}
#endif
}

void CComponent_PullMeshEffect::Render()
{

}

void CComponent_PullMeshEffect::Shutdown()
{
	m_pGame = NULL;
	m_pAnimation = NULL;
	m_pEmitterComponent = NULL;
	m_pInput = NULL;
	m_pEmitter = NULL;
	m_pParticleMan = NULL;
	m_pEffects = NULL;
	m_pEmitter = NULL;
	m_pCamera = NULL;
	m_pPlayer = NULL;
	m_bEffectHasPlayed = false;
}

