#include "CComponent_LittleHurtMeshEffect.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "..\InputLib\InputManager.h"
#include "../EffectsLib/CEffectsIncludes.h"
#include "Component_Emitter.h"
#include "CComponent_PlayerCollision.h"
#include "CComponent_Input.h"
#include "../ObjectLib/Objects/IBaseObject.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"


CComponent_LittleHurtMeshEffect::CComponent_LittleHurtMeshEffect(void) : IComponent(ECOMP_MESHEFFECT)//(ECOMP_PULL_MESHEFFECT)
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
	_pTargetHealthCMP = NULL;
	duration = 0.0f;
}

CComponent_LittleHurtMeshEffect::~CComponent_LittleHurtMeshEffect(void)
{
}

bool CComponent_LittleHurtMeshEffect::Initialize()
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pParticleMan = CParticleManager::GetInstance();
	m_pCamera = m_pGame->GetCamera();
	m_pEffects = NULL;

	return true;
}

void CComponent_LittleHurtMeshEffect::Update( float dt )
{
	if(!m_pAnimation)
		m_pAnimation = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );
	if(!m_pEmitterComponent)
	{
		m_pEmitterComponent = (CComponent_Emitter*)m_pParent->GetComponent( ECOMP_EMITTER );
	}
	if(!m_pPlayer)
	{
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();
		_pTargetHealthCMP = m_pPlayer->GetComponent (ECOMP_HEALTH);
	}


#if _ANIMATIONS

	// Grab the parents world and local matrix.
	matrix4f _worldMat = (*m_pParent->GetWorldMat());
	matrix4f _localMat = (*m_pParent->GetLocalMat());

	_localMat = *m_pPlayer->GetLocalMat();
	_localMat.rotate_x_post(-55.0f);

 	if(_localMat.axis_x.z <= -0.0f && _localMat.axis_z.x >= 0.0f)
 		_localMat.axis_pos.x = (m_pPlayer->GetLocalMat()->axis_pos.x - 15.0f);
 	else
 		_localMat.axis_pos.x = m_pPlayer->GetLocalMat()->axis_pos.x + 15.0f;

	_localMat.axis_pos.y = m_pPlayer->GetLocalMat()->axis_pos.y + 50.0f;
	_localMat.axis_x.negate();
	_localMat.axis_y.negate();
	_localMat.axis_z.negate();
	// Set Local Matrix.
	m_pParent->SetLocalMat( &_localMat );

	//TODO: Take out when pudge can actually be damaged by things


	if( m_pAnimation )
	{
 		if(((CComponent_Health*)_pTargetHealthCMP)->GetPlayerHasTakenLightDamage() == true)
		{
			if(m_pEmitterComponent)
			{
				m_pEffects = m_pEmitterComponent->GetEffects();
				if(m_pEffects)
				{
					for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
					{
						m_pEffects->GetEmitters()[i].SetEmitterActive(true);
					}
				}
			}

			//if(duration >= 0.8f)
			if(m_pAnimation->GetAnimStateMachine()->GetCurrentState() != LittleHurtEffectState::GetState())
				m_pAnimation->GetAnimStateMachine()->ChangeState( LittleHurtEffectState::GetState() );

 			((CComponent_Health*)_pTargetHealthCMP)->SetPlayerHasTakenLightDamage(false);
 		}
	}
#endif
	duration += dt;
}

void CComponent_LittleHurtMeshEffect::Render()
{
}

void CComponent_LittleHurtMeshEffect::Shutdown()
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
	_pTargetHealthCMP = NULL;
}

