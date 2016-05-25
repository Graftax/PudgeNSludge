#include "CComponent_PullMeshEffect.h"
#include "../AnimationLib/AnimationIncludes.h"
#include "..\InputLib\InputManager.h"
#include "../EffectsLib/CEffectsIncludes.h"
#include "Component_Emitter.h"
#include "CComponent_PlayerCollision.h"
#include "CComponent_CrateCollision.h"
#include "CComponent_Input.h"
#include "../ObjectLib/Objects/IBaseObject.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../RenderEngine/DebugShapes.h"

CComponent_ImpactMeshEffect::CComponent_ImpactMeshEffect(void) : IComponent(ECOMP_MESHEFFECT)//(ECOMP_IMPACT_MESHEFFECT)
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
	//m_pPlayer = NULL;
	m_pCrateComponent = NULL;
	_worldMat.make_identity();

	//DebugShapes::CreateSphere(200.0f, 5, 5);
}

CComponent_ImpactMeshEffect::~CComponent_ImpactMeshEffect(void)
{
}

bool CComponent_ImpactMeshEffect::Initialize()
{
	m_pGame = CGame::GetInstance();
	m_pInput = m_pGame->GetInputManager();
	m_pParticleMan = CParticleManager::GetInstance();
	m_pCamera = m_pGame->GetCamera();

	return true;
}

void CComponent_ImpactMeshEffect::Update( float dt )
{
// 	if (!m_pParent || !m_pParent->GetCollidableObject() || m_pParent->GetCollidableObject()->GetVolumeType () != VMT_AABB)
// 	{
// 		return;
// 	}

	D3DXMATRIX mat;
	D3DXMatrixIdentity( &mat);

	mat._41 = m_pParent->GetWorldPos().x;
	mat._42 = m_pParent->GetWorldPos().y;
	mat._43 = -500.0f;

	//DebugShapes::RenderSphere(mat);
	m_pParent->SetLocalPos(&m_fPos);

	if(!m_pAnimation)
		m_pAnimation = (CComponent_Animation*)m_pParent->GetComponent( ECOMP_ANIMATION );
	if(!m_pEmitterComponent)
		m_pEmitterComponent = (CComponent_Emitter*)m_pParent->GetComponent( ECOMP_EMITTER );
// 	if(!m_pPlayer)
// 		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();

// 	std::list <IBaseObject*> objects = CObjectManager::GetInstance()->GetAllObjects();
// 
// 	for (std::list<IBaseObject*>::iterator itr = objects.begin(); itr != objects.end(); itr++)
// 	{
// 		CComponent_Collision* cmp = (CComponent_Collision*)(*itr)->GetComponent (ECOMP_COLLISION);
// 		CComponent_Item* item = (CComponent_Item*)(*itr)->GetComponent (ECOMP_ITEM);
// 
// 		if (cmp->GetParent()->GetCollidableObject()->GetVolumeType () == VMT_BOX)
// 		{
// 			Box* crate = (Box*)(cmp->GetParent()->GetCollidableObject());
// 
// 			vec2f Direction;
// 			vec2f CP;
// 			if ((crate)-> BoxToAABB(*(AABB*)m_pParent->GetCollidableObject(), CP, Direction))
// 			{
// 
// 			}
// 		}
// 	}


#if _ANIMATIONS
#else

	//vec3f pos = _worldMat.axis_pos;
	//m_pParent->SetLocalMat( &_worldMat );
// 	LOG(LOG_DEBUG, "CComponent_ImpactMeshEffect::Update()", "Bx: %.2f, By: %.2f, Bz: %.2f", _worldMat.axis_pos.x, _worldMat.axis_pos.y, _worldMat.axis_pos.z);
// 	LOG(LOG_DEBUG, "CComponent_ImpactMeshEffect::Update()", "Px: %.2f, Py: %.2f, Pz: %.2f",
// 		m_pParent->GetLocalMat()->axis_pos.x, m_pParent->GetLocalMat()->axis_pos.y, m_pParent->GetLocalMat()->axis_pos.z);

	if( m_pAnimation )
	{
		//if( m_pAnimation->GetAnimStateMachine()->GetCurrentState() != ImpactEffectState::GetState() )
			m_pAnimation->GetAnimStateMachine()->ChangeState( ImpactEffectState::GetState() );


		if( m_pAnimation->GetInterpolator().IsAnimationDone())
		{
			m_pParent->GetLocalMat()->axis_pos = vec3f(0.0f, 0.0f, 0.0f);
			CObjectManager::GetInstance ()->AddObjectToRemove(m_pParent);
		}
	}

#endif
}

void CComponent_ImpactMeshEffect::Render()
{
	//m_pEmitterComponent->GetParticleManager()->GetParticleEffect()[PUSH_PULL_FX]->Render();
}

void CComponent_ImpactMeshEffect::Shutdown()
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
	//m_pPlayer = NULL;
	m_pCrateComponent = NULL;
	_worldMat.make_identity();
}

