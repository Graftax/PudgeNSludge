#include "CComponent_LandMeshEffect.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "..\InputLib\InputManager.h"
#include "../EffectsLib/CEffectsIncludes.h"
#include "Component_Emitter.h"
#include "CComponent_PlayerCollision.h"
#include "CComponent_Input.h"
#include "../ObjectLib/Objects/IBaseObject.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"


CComponent_LandMeshEffect::CComponent_LandMeshEffect(void) : IComponent(ECOMP_MESHEFFECT)//(ECOMP_PULL_MESHEFFECT)
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
	m_bCollidingGround = false;
	isCollidingLast = false;
}

CComponent_LandMeshEffect::~CComponent_LandMeshEffect(void)
{
}

bool CComponent_LandMeshEffect::Initialize()
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pParticleMan = CParticleManager::GetInstance();
	m_pCamera = m_pGame->GetCamera();
	m_pEffects = NULL;

	return true;
}

void CComponent_LandMeshEffect::Update( float dt )
{
	if(!m_pAnimation)
		m_pAnimation = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );
	if(!m_pEmitterComponent)
	{
		m_pEmitterComponent = (CComponent_Emitter*)m_pParent->GetComponent( ECOMP_EMITTER );
	}
	if(!m_pPlayer)
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();

#if _ANIMATIONS
	// Grab the parents world and local matrix.
	matrix4f _worldMat = (*m_pParent->GetWorldMat());
	matrix4f _localMat = (*m_pParent->GetLocalMat());

	//_localMat = *m_pPlayer->GetLocalMat();
	_localMat.axis_pos.x = m_pPlayer->GetWorldPos().x;
	_localMat.axis_pos.y = m_pPlayer->GetWorldPos().y+15.0f;
	_localMat.axis_pos.z = m_pPlayer->GetWorldPos().z-40.0f;

	m_bCollidingGround = ((CComponent_PlayerCollision*)m_pPlayer->GetComponent(ECOMP_COLLISION))->GetCollidingGroundAlone();
	isCollidingLast = ((CComponent_PlayerCollision*)m_pPlayer->GetComponent(ECOMP_COLLISION))->GetCollidingGroundLast();

	//If player is not dead. Allow player to Land 
	if(((CComponent_Health*)m_pPlayer->GetComponent(ECOMP_HEALTH))->GetHealth() >= 0.1f)
	{
		if(m_bCollidingGround && !isCollidingLast)
		{
			if( m_pAnimation )
			{
				// Set Local Matrix.
				m_pParent->SetLocalMat( &_localMat );
				if( m_pAnimation->GetAnimStateMachine()->GetCurrentState() != LandEffectState::GetState())
				{
					m_pAnimation->GetAnimStateMachine()->ChangeState(LandEffectState::GetState());
				}
			}
		}

		if(m_pInput->KeyPressed(DIK_SPACE))
		{

			if(m_bCollidingGround)
			{
				if( m_pAnimation )
				{
					// Set Local Matrix.
					m_pParent->SetLocalMat( &_localMat );
					if( m_pAnimation->GetAnimStateMachine()->GetCurrentState() != LandEffectState::GetState())
					{
						m_pAnimation->GetAnimStateMachine()->ChangeState(LandEffectState::GetState());
					}
					if(m_pAnimation->GetInterpolator().IsAnimationDone())
						m_pAnimation->GetAnimStateMachine()->ChangeState( LandIdleEffectState::GetState() );
				}
			}
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
			m_pAnimation->GetAnimStateMachine()->ChangeState( LandIdleEffectState::GetState() );
		}
	}
#endif
}

void CComponent_LandMeshEffect::Render()
{
}

void CComponent_LandMeshEffect::Shutdown()
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
	m_bCollidingGround = false;
	isCollidingLast = false;
}

