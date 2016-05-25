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
	m_pParticleManager = NULL;
	m_pEffect = NULL;
	//m_vEffects.clear();
	m_nType = 0;
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
	if(!m_pParticleManager)
		m_pParticleManager = CParticleManager::GetInstance();
	if(!m_pCgame)
		m_pCgame = CGame::GetInstance();
	if(!m_pCamera)
		m_pCamera = m_pCgame->GetCamera();
	if(!m_pPlayer)
		m_pPlayer = CObjectManager::GetInstance()->GetPlayer();

	if(!m_pEffect)
		m_pEffect = new CEffects(*m_pParticleManager->GetParticleEffect()[m_nType]);

	if( OBJ_PULL_MESHEFFECT == m_pParent->GetType() )
	{
		if( !m_bConnectedPull )
		{
			CComponent_Input* pInTarget =  (CComponent_Input*)CObjectManager::GetInstance()->GetPlayer()->GetComponent(ECOMP_INPUT);

			if( pInTarget )
			{
				pInTarget->SetPullEmitter( this );
				m_bConnectedPull = true;
			}
		}
	}

	if( OBJ_PUSH_MESHEFFECT == m_pParent->GetType() )
	{
		if( !m_bConnectedPush )
		{
			CComponent_Input* pInTarget =  (CComponent_Input*)CObjectManager::GetInstance()->GetPlayer()->GetComponent(ECOMP_INPUT);

			if( pInTarget )
			{
				pInTarget->SetPushEmitter( this );
				m_bConnectedPush = true;
			}
		}
	}

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

#if _PARTICLES
	switch(m_pParent->GetType())
	{
	case OBJ_PLAYER:
		{
			switch(m_nType)
			{
			case FOOT_STEPS_FX:
				{
					D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_pPlayer->GetWorldPos()));
					m_pEffect->GetEmitters()[0].SetPosition(pos);
					m_pEffect->GetEmitters()[0].SetBurst(false);
					m_pEffect->Update(dt);
				}
				break;
			case JUMP_FX:
				{
					D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_pPlayer->GetWorldPos()));
					pos.y = pos.y + 50.0f;
					pos.x = pos.x + 100.0f;
					m_pEffect->GetEmitters()[0].SetPosition(pos);
					m_pEffect->GetEmitters()[0].SetBurst(false);
					m_pEffect->GetEmitters()[0].SetEmitterActive(true);
					m_pEffect->Update(dt);
				}
				break;
			}
			break;
		}
	case OBJ_PULL_MESHEFFECT:
		{
			D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_fPullStart));
			m_pEffect->GetEmitters()[0].SetPosition(pos);
			vec3f playerPos = m_pPlayer->GetLocalMat()->axis_pos;
			playerPos.y = playerPos.y + 50.0f;
			m_pEffect->UpdatePointToPoint(dt, m_fPullStart, playerPos);

			// 				D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_pCamera->GetLazerEndPoint()));
			// 				m_pEffect->GetEmitters()[0].SetPosition(pos);
			// 				vec3f playerPos = m_pPlayer->GetLocalMat()->axis_pos;
			// 				playerPos.y = playerPos.y + 50.0f;
			// 				m_pEffect->UpdatePointToPoint(dt, m_pCamera->GetLazerEndPoint(), playerPos);
		}
		break;
	case OBJ_PUSH_MESHEFFECT:
		{
			// 				D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_pPlayer->GetWorldPos()));
			vec3f playerPos = m_pPlayer->GetLocalMat()->axis_pos;

			D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(playerPos));
			pos.y = pos.y + 50.0f;
			m_pEffect->GetEmitters()[0].SetPosition(pos);
			playerPos.y = playerPos.y + 50.0f;
			m_pEffect->UpdatePointToPoint(dt, playerPos, m_fPullStart);

		}
		break;
	case OBJ_JUMP_MESHEFFECT:
		{
			switch(m_nType)
			{
			case JUMP_FX:
				{
					D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_pPlayer->GetWorldPos()));
					pos.y = pos.y + 50.0f;
					m_pEffect->GetEmitters()[0].SetPosition(pos);
					m_pEffect->GetEmitters()[0].SetBurst(false);
					//m_pEffect->GetEmitters()[0].SetEmitterActive(true);
					m_pEffect->Update(dt);
				}
				break;
			}
			break;

		}
		break;
	case OBJ_LAND_MESHEFFECT:
		{
			m_pEffect->Update(dt);
		}
		break;
	case OBJ_BIGHIT_MESHEFFECT:
		{
			if(EffectEndTimer >= 0.5f)
				CObjectManager::GetInstance()->AddObjectToRemove(m_pParent);

			int i = 0;
			//  			D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_pParent->GetLocalMat()->axis_pos));
			//  			m_pEffect->GetEmitters()[i].SetPosition(pos);
			//  			m_pEffect->GetEmitters()[i].SetDirection(m_pParent->GetLocalMat()->axis_z);
			//  			float speed = 10.0f;
			//  			m_pEffect->GetEmitters()[i].SetSpeed(speed);
			//  
			//  			m_pEffect->UpdatePointToDirection(dt);

			D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_pParent->GetLocalMat()->axis_pos));
			if(((CComponent_Input*)m_pPlayer->GetComponent(ECOMP_INPUT))->GetRotatedRight())
			{
				m_pEffect->GetEmitters()[i].SetMinVelocityX(m_pEffect->GetEmitters()[i].GetMinVelocityX() * -1.0f);
				m_pEffect->GetEmitters()[i].SetMinVelocityY(m_pEffect->GetEmitters()[i].GetMinVelocityY() * -1.0f);
				m_pEffect->GetEmitters()[i].SetMinVelocityZ(m_pEffect->GetEmitters()[i].GetMinVelocityZ() * -1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityX(m_pEffect->GetEmitters()[i].GetMaxVelocityX() * -1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityY(m_pEffect->GetEmitters()[i].GetMaxVelocityY() * -1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityZ(m_pEffect->GetEmitters()[i].GetMaxVelocityZ() * -1.0f);
			}
			else
			{
				m_pEffect->GetEmitters()[i].SetMinVelocityX(m_pEffect->GetEmitters()[i].GetMinVelocityX() * 1.0f);
				m_pEffect->GetEmitters()[i].SetMinVelocityY(m_pEffect->GetEmitters()[i].GetMinVelocityY() * 1.0f);
				m_pEffect->GetEmitters()[i].SetMinVelocityZ(m_pEffect->GetEmitters()[i].GetMinVelocityZ() * 1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityX(m_pEffect->GetEmitters()[i].GetMaxVelocityX() * 1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityY(m_pEffect->GetEmitters()[i].GetMaxVelocityY() * 1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityZ(m_pEffect->GetEmitters()[i].GetMaxVelocityZ() * 1.0f);
			}
			m_pEffect->GetEmitters()[i].SetPosition(pos);

			float speed = 10.0f;
			m_pEffect->GetEmitters()[i].SetSpeed(speed);

			m_pEffect->Update(dt);

			EffectEndTimer += dt;
		}
		break;
	case OBJ_LITTLEHIT_MESHEFFECT:
		{
			if(EffectEndTimer >= 0.8f)
				CObjectManager::GetInstance()->AddObjectToRemove(m_pParent);

			int i = 0;
			D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_pParent->GetLocalMat()->axis_pos));
			if(((CComponent_Input*)m_pPlayer->GetComponent(ECOMP_INPUT))->GetRotatedRight())
			{
				m_pEffect->GetEmitters()[i].SetMinVelocityX(m_pEffect->GetEmitters()[i].GetMinVelocityX() * -1.0f);
				m_pEffect->GetEmitters()[i].SetMinVelocityY(m_pEffect->GetEmitters()[i].GetMinVelocityY() * -1.0f);
				m_pEffect->GetEmitters()[i].SetMinVelocityZ(m_pEffect->GetEmitters()[i].GetMinVelocityZ() * -1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityX(m_pEffect->GetEmitters()[i].GetMaxVelocityX() * -1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityY(m_pEffect->GetEmitters()[i].GetMaxVelocityY() * -1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityZ(m_pEffect->GetEmitters()[i].GetMaxVelocityZ() * -1.0f);
			}
			else
			{
				m_pEffect->GetEmitters()[i].SetMinVelocityX(m_pEffect->GetEmitters()[i].GetMinVelocityX() * 1.0f);
				m_pEffect->GetEmitters()[i].SetMinVelocityY(m_pEffect->GetEmitters()[i].GetMinVelocityY() * 1.0f);
				m_pEffect->GetEmitters()[i].SetMinVelocityZ(m_pEffect->GetEmitters()[i].GetMinVelocityZ() * 1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityX(m_pEffect->GetEmitters()[i].GetMaxVelocityX() * 1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityY(m_pEffect->GetEmitters()[i].GetMaxVelocityY() * 1.0f);
				m_pEffect->GetEmitters()[i].SetMaxVelocityZ(m_pEffect->GetEmitters()[i].GetMaxVelocityZ() * 1.0f);
			}
			m_pEffect->GetEmitters()[i].SetPosition(pos);


			m_pEffect->Update(dt);

			EffectEndTimer += dt;
		}
		break;
	case OBJ_IMPACT_MESHEFFECT:
		{

			m_pEffect->Update(dt);
		}
		break;
	case OBJ_PIPE_STEAM:
		{
			for(unsigned int i = 0; i < m_pEffect->GetEmitters().size(); i++)
			{
				D3DXVECTOR3 pos = *(D3DXVECTOR3*)(&(m_pParent->GetLocalMat()->axis_pos));
				m_pEffect->GetEmitters()[i].SetPosition(pos);
				m_pEffect->GetEmitters()[i].SetDirection(m_pParent->GetLocalMat()->axis_z);
				float speed = (((CComponent_SteamPipeCollision*)m_pParent->GetComponent(ECOMP_COLLISION))->GetLength() * GAME_UNIT ) / m_pEffect->GetEmitters()[i].GetMaxLifeSpan();
				m_pEffect->GetEmitters()[i].SetSpeed(speed);
				m_pEffect->GetEmitters()[i].SetEmitterActive(((CComponent_SteamPipeCollision*)m_pParent->GetComponent(ECOMP_COLLISION))->GetSteamOn());
				m_pEffect->UpdatePointToDirection(dt);
			}

		}
		break;
	case OBJ_GOO_PARTICLE_EFFECT:
		{
			switch( m_nType )
			{
			case GOO_EXPLODE_FX:
				{
					EffectEndTimer += dt;
					if( EffectEndTimer > 0.75f )
						CObjectManager::GetInstance()->AddObjectToRemove( m_pParent );

					int NumOfEmitters = m_pEffect->GetEmitters().size();
					for( int i = 0; i < NumOfEmitters; i++)
					{
						D3DXVECTOR3 EmitterPos = (*(D3DXVECTOR3*)&m_pParent->GetLocalMat()->axis_pos);
						m_pEffect->GetEmitters()[i].SetPosition( EmitterPos );
						m_pEffect->GetEmitters()[i].SetEmitterActive( true );
						m_pEffect->Update(dt);
					}
				}
				break;
			case GOO_TRAIL_FX:
				{
					if( ActivateEffectEndTimer )
					{
						EffectEndTimer += dt;
						if( EffectEndTimer > 0.3f )
							CObjectManager::GetInstance()->AddObjectToRemove( m_pParent );
					}

					int NumOfEmitters = m_pEffect->GetEmitters().size();
					for( int i = 0; i < NumOfEmitters; i++)
					{
						D3DXVECTOR3 EmitterPos = (*(D3DXVECTOR3*)&m_pParent->GetFrame().GetWorldMat().axis_pos);
						m_pEffect->GetEmitters()[i].SetPosition( EmitterPos );
						m_pEffect->GetEmitters()[i].SetEmitterActive( true );
						m_pEffect->Update(dt);
					}
				}
				break;
			};
		}
	case OBJ_CRUSHER_PARTICLE_EFFECT:
		{
			switch( m_nType )
			{
			case CRUSHERSPARKS_FX:
				{
					EffectEndTimer += dt;
					if( EffectEndTimer > 0.3f )
						CObjectManager::GetInstance()->AddObjectToRemove( m_pParent );

					int NumOfEmitters = m_pEffect->GetEmitters().size();
					for( int i = 0; i < NumOfEmitters; i++)
					{
						D3DXVECTOR3 EmitterPos = (*(D3DXVECTOR3*)&m_pParent->GetFrame().GetWorldMat().axis_pos);
						m_pEffect->GetEmitters()[i].SetPosition( EmitterPos );
						m_pEffect->GetEmitters()[i].SetEmitterActive( true );
						m_pEffect->Update(dt);
					}
				}
				break;
			};
		}
		break;
	}
	// 	}

#endif
}

void CComponent_Emitter::Render()
{
	if( m_pEffect )
		m_pEffect->Render();
}

void CComponent_Emitter::Shutdown()
{
	m_pCgamePlay = NULL;
	m_pParticleManager = NULL;
	delete m_pEffect;
	m_pEffect = NULL;
	//m_vEffects.clear();
	m_nType = 0;
	m_pCamera = NULL;
	m_pCgame = NULL;
	m_pPlayer = NULL;
}
