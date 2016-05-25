#include "CComponent_PushMeshEffect.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "..\InputLib\InputManager.h"
#include "../EffectsLib/CEffectsIncludes.h"
#include "Component_Emitter.h"
#include "CComponent_PlayerCollision.h"
#include "CComponent_Input.h"
#include "../ObjectLib/Objects/IBaseObject.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"


CComponent_PushMeshEffect::CComponent_PushMeshEffect(void) : IComponent(ECOMP_MESHEFFECT)//(ECOMP_PUSH_MESHEFFECT)
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
	fCounter = 0.0f;
	m_pPlayerArm = NULL;
	m_mGunJointMatrix.make_identity();
}

CComponent_PushMeshEffect::~CComponent_PushMeshEffect(void)
{
}

bool CComponent_PushMeshEffect::Initialize()
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pParticleMan = CParticleManager::GetInstance();
	m_pCamera = m_pGame->GetCamera();
	HasUpdatedPosition = false;
	m_pPlayerArm = NULL;
	m_mGunJointMatrix.make_identity();


	return true;
}

void CComponent_PushMeshEffect::Update( float dt )
{
	if(!m_pAnimation)
		m_pAnimation = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );
	if(!m_pEmitterComponent)
		m_pEmitterComponent = (CComponent_Emitter*)m_pParent->GetComponent( ECOMP_EMITTER );
	if(!m_pPlayer)
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();
	if( !m_pPlayerArm )
		m_pPlayerArm = CObjectManager::GetInstance()->GetPlayerArm();

#if _ANIMATIONS
	if( m_pAnimation )
	{
		m_pAnimation->GetAnimStateMachine()->ChangeState(PushEffectState::GetState());
		if(m_pAnimation->GetInterpolator().IsAnimationDone())
			CObjectManager::GetInstance()->AddObjectToRemove( m_pParent );
	}

	if( !HasUpdatedPosition )
	{
		// Grab the parents world and local matrix.
		matrix4f _worldMat = (*m_pPlayerArm->GetWorldMat());
		matrix4f _localMat = (*m_pParent->GetLocalMat());

		// Grab the World cursor Position.
		vec3f _CursorPos = m_pCamera->GetLazerEndPoint();
		// Calculate Vector from your Position to the Cursor Position.
		vec3f PosToCursor =  _CursorPos - _worldMat.axis_pos;
		PosToCursor.normalize();
		// Dot the Forward vector of the arm with the vector calculated above to determine
		// how much the arm needs to rotate by.
		float AngleBetween = dot_product( _localMat.axis_y, PosToCursor);
		_localMat.rotate_x_pre( (AngleBetween) );

		do
		{
			AngleBetween = dot_product( _localMat.axis_y, PosToCursor);
			_localMat.rotate_x_pre( (AngleBetween) );
		}while(abs(AngleBetween) >= FLT_EPSILON);

		//Set the effects position to players position and offsetting it
		//Gonna have to set it to the players arm instead of the player himself
		_localMat.axis_pos = m_pPlayerArm->GetWorldMat()->axis_pos;
		_localMat.translate_post( m_mGunJointMatrix.axis_pos );

		// Set Local Matrix.
		m_pParent->SetLocalMat( &_localMat );

		HasUpdatedPosition = true;
	}

#if _PARTICLES

#endif

	if(m_pInput->RightMouseButtonPressed())
	{
		fCounter = 0.0f;
		if(m_pEmitterComponent)
		{
			m_pEffects = m_pEmitterComponent->GetEffects();
			for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
			{
				m_pEffects->GetEmitters()[i].SetEmitterActive(true);
			}
		}
	}
	else
	{
		if(m_pEmitterComponent)
		{
			m_pEffects = m_pEmitterComponent->GetEffects();
			for(UINT i = 0; i < m_pEffects->GetEmitters().size(); i++)
				if( fCounter >= m_pEffects->GetEmitters()[i].GetMaxLifeSpan() )
				{
					m_pEffects->GetEmitters()[i].SetEmitterActive(false);
					m_pEffects->GetEmitters()[i].ResetAllParticles();
				}
		}
	}

	fCounter += dt/* * 0.5f*/;
#endif
}

void CComponent_PushMeshEffect::Render()
{
}

void CComponent_PushMeshEffect::Shutdown()
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
}

void CComponent_PushMeshEffect::SetJointMatrix( D3DXMATRIX mat )
{ 
	// Matrix passed in is the Inverse Local matrix so invert the position.

	m_mGunJointMatrix.make_identity();
	m_mGunJointMatrix.wx =  (mat._41);
	m_mGunJointMatrix.wy =  (mat._42);
	m_mGunJointMatrix.wz =  (mat._43);

}