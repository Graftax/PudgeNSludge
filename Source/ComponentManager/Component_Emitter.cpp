#include "Component_Emitter.h"
#include "../PS/States/GamePlayState.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"


CComponent_Emitter::CComponent_Emitter(void) : IComponent(ECOMP_EMITTER)
{
}

CComponent_Emitter::~CComponent_Emitter(void)
{
}

bool CComponent_Emitter::Initialize()
{
	m_pCgamePlay = NULL;
	m_pCamera = NULL;
	m_pCgame = NULL;
	m_pPlayer = NULL;
	m_fPullStart = vec3f( 0.0f, 0.0f, 0.0f );

	m_bConnectedPull = false;
	m_bConnectedPush = false;
	EffectEndTimer = 0.0f;
	ActivateEffectEndTimer = false;

	return true;
}

void CComponent_Emitter::Update( float dt )
{
	if(!m_pCgamePlay)
		m_pCgamePlay = CGamePlayState::GetInstance();
	if(!m_pCgame)
		m_pCgame = CGame::GetInstance();
	if(!m_pCamera)
		m_pCamera = m_pCgame->GetCamera();
	if(!m_pPlayer)
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();

	//if(m_pEffect)
	{
		// 		if(m_nType == STEAM_FX)
		// 		{
		// 			/*CEffects om_pEffect*/m_pEffect = m_vEffects[m_nType];
		// 			m_pEffect->GetEmitters().push_back(m_pEffect->GetEmitters()[0]);
		// 		}
		//m_pEffect = 
		// 		for(int i = 0; i < m_vEffects.size(); i++)
		// 			//if(m_vEffects[m_nType].GetEmitters()[i])
		// 		m_pEffect->GetEmitters().push_back(m_pEffect->GetEmitters()[0]);
	}


	//m_pParticleManager->Update(dt);
}

void CComponent_Emitter::Render()
{

}

void CComponent_Emitter::Shutdown()
{
	m_pCgamePlay = NULL;
	//m_vEffects.clear();
	m_nType = 0;
	m_pCamera = NULL;
	m_pCgame = NULL;
	m_pPlayer = NULL;
}
