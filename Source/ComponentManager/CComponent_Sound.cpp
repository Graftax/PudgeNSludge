#include "CComponent_Sound.h"
//#include "../PS/CGame.h"
#include "../ObjectLib/ObjectManager/CObjectManager.h"
#include "../Utilities/KdTree/KdTree.h"

#define NEAR_MUSIC_DIST 650.0f * 650.0f
#define SOUND_RANGE 700.0f

CComponent_Sound::CComponent_Sound(void) : IComponent(ECOMP_SOUND)
{
	m_pGame = nullptr;
	m_pAudio = nullptr;
	m_pSoundObject = nullptr;
	m_pClosestGooPit = nullptr;

	m_bLilGooNear = false;
	m_bSlimeNear = false;
	m_bPrevLilGooNear = false;
	m_bPrevSlimeNear = false;
	m_pParentCollision = nullptr;

	m_bSoundLoop1 = false;
	m_bSoundLoop2 = false;
	m_bInRange = false;
}

CComponent_Sound::~CComponent_Sound(void)
{

}

bool CComponent_Sound::Initialize(void)
{
	m_pGame = CGame::GetInstance();
	m_pAudio = m_pGame->GetAudioManager();
	m_pClosestGooPit = nullptr;

	m_bLilGooNear = false;
	m_bSlimeNear = false;
	m_bPrevLilGooNear = false;
	m_bPrevSlimeNear = false;
	m_pParentCollision = nullptr;

	m_bSoundLoop1 = false;
	m_bSoundLoop2 = false;
	m_bInRange = false;

	m_pAudio = CGame::GetInstance()->GetAudioManager();
	RegisterSoundObject();

	return true;
}

bool CComponent_Sound::RegisterSoundObject(void)
{
	if(!m_pParent || !m_pAudio)
	{
		return false;
	}

	switch( m_pParent->GetType() )
	{
	case OBJ_PLAYER:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Player", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_GOO:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Lil_Goo", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_MONSTER:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Slime_Monster", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_HULK:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Junk_Hulk", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_BOSS:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Sludge", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_CRUSHER:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Crusher", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_BRIDGE:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Bridge", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_PLUG:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Plug", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_PIT:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Goo_Pit", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_WALL_BREAKABLE:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Breakable_Wall", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_PIPE_STEAM:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Steam_Pipe", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_PIPE_SPAWN:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Spawn_Pipe", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_PIPE_BOX:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Box_Pipe", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_CRATE:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Crate", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_CORK:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Cork", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_POOL:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Healing_Pool", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_DOORBUTTON:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Door_Button", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_BOXBUTTON:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Box_Button", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_OUTLET:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Outlet", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_DOOR:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Door", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_PIPE_INTAKE:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Intake_Pipe", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_CHECKPOINT:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Checkpoint", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	default:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("WTF_M8?!", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	}

	if(!m_pSoundObject)
	{
		return false;
	}

	return true;
}

void CComponent_Sound::Update(float)
{
	//if(m_pParent->GetType() == OBJ_CRATE && m_pSoundObject)
	//{
	//	LOG(LOG_DEBUG, "CComponent_Sound::Update()", "I'm a Crate.");
	//}
	//else if(m_pParent->GetType() == OBJ_CRATE && !m_pSoundObject)
	//{
	//	LOG(LOG_DEBUG, "CComponent_Sound::Update()", "I'm a Crate. But I'm not Registered.");
	//}
	//else if(m_pParent->GetType() == OBJ_NULL)
	//{
	//	LOG(LOG_DEBUG, "CComponent_Sound::Update()", "I'm not a Crate. WTF?!");
	//}



	if(m_pSoundObject)
	{
		m_pSoundObject->ObjectPosition.Position.X = m_pParent->GetWorldPos().x;
		m_pSoundObject->ObjectPosition.Position.Y = m_pParent->GetWorldPos().y;
		if(m_pAudio && m_pParent->GetType() == OBJ_BOSS)
		{
			m_pAudio->SetSoundObjectPosition(m_pSoundObject, vec3f(m_pSoundObject->ObjectPosition.Position.X, m_pSoundObject->ObjectPosition.Position.Y, 0.0f));
		}
	}

	m_bInRange = false;
	m_bLilGooNear = false;
	m_bSlimeNear = false;
	
	if(!m_pParentCollision)
	{
		m_pParentCollision = (CComponent_Collision*)m_pParent->GetComponent(ECOMP_COLLISION);
	}

	//inside here, we'll be checking to see if the player is close to us. We play sounds if the player is close enough
	if(m_pParentCollision)
	{
		//creating the sound sphere
		Sphere soundRadius;
		soundRadius.SetRadius(SOUND_RANGE);
		if((CComponent_Item*)m_pParent->GetComponent(ECOMP_ITEM))
		{
			soundRadius.SetCenter(m_pParentCollision->GetPhysicsBox()->GetCentroid());
		}
		else
		{
			soundRadius.SetCenter(m_pParent->GetWorldPos());
		}

		IBaseObject* pObject = CObjectManager::GetInstance()->GetPlayer();

		if (pObject->GetType() == OBJ_PLAYER)
		{
			vec2f fPlayerPos = m_pParentCollision->GetPos2DClean();
			vec3f lineToPlayer = vec3f( fPlayerPos.x, fPlayerPos.y, 0.0f ) - pObject->GetWorldPos();
			m_bInRange = ( (lineToPlayer.x * lineToPlayer.x + lineToPlayer.y * lineToPlayer.y) < SOUND_RANGE * SOUND_RANGE );

			//music is different than sound effects, and are based on the player position instead of the listener position
			switch (m_pParent->GetType())
			{
			case OBJ_GOO:
			{
				vec3f lineTo = m_pParentCollision->GetPhysicsBox()->GetCentroid() - pObject->GetWorldPos();
				float distTo = abs(dot_product(lineTo, lineTo));

				if (!m_bLilGooNear && distTo < NEAR_MUSIC_DIST)
				{
					m_bLilGooNear = true;
				}
			}
				break;
			case OBJ_MONSTER:
			{
				vec3f lineTo = m_pParentCollision->GetParent()->GetWorldPos() - pObject->GetWorldPos();
				float distTo = abs(dot_product(lineTo, lineTo));

				if (!m_bSlimeNear && distTo < NEAR_MUSIC_DIST)
				{
					m_bSlimeNear = true;
				}
			}
				break;
			case OBJ_PIT:
			{
				CComponent_Sound* pPlayerSound = (CComponent_Sound*)pObject->GetComponent(ECOMP_SOUND);
				if (!pPlayerSound->GetClosestGooPit() || pPlayerSound->GetClosestGooPit()->GetType() == OBJ_NULL)
				{
					PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_GOOPIT_BLOOP_LOOP);
					pPlayerSound->SetClosestGooPit(pObject);
				}
				else
				{
					CComponent_Sound* pClosestSound = (CComponent_Sound*)pPlayerSound->GetClosestGooPit()->GetComponent(ECOMP_SOUND);
					vec3f lineToGooPit = m_pAudio->GetListenerPosition() - m_pParent->GetWorldPos();
					vec3f lineToClosestGooPit = m_pAudio->GetListenerPosition() - pPlayerSound->GetClosestGooPit()->GetWorldPos();
					float distToGoo = abs(dot_product(lineToGooPit, lineToGooPit));
					float distToClosestGoo = abs(dot_product(lineToClosestGooPit, lineToClosestGooPit));

					if (distToGoo < distToClosestGoo)
					{
						if (pClosestSound)
						{
							pClosestSound->StopSound(AK::EVENTS::STOP_SFX_HAZARD_GOOPIT_BLOOP_LOOP);
						}
						else
							LOG(LOG_ERROR, "CComponent_Sound::Update", "Sound Component is Null, this is an error.");

						PlaySound(AK::EVENTS::PLAY_SFX_HAZARD_GOOPIT_BLOOP_LOOP);
						pPlayerSound->SetClosestGooPit(m_pParent);
					}
				}
			}
				break;
			default:
			{

			}
				break;
			}
		}


		//shut off any loops that are out of range
		if(m_pParent->GetType() == OBJ_PIT && m_pParent == m_pClosestGooPit && !m_bInRange)
		{
			StopSound(AK::EVENTS::STOP_SFX_HAZARD_GOOPIT_BLOOP_LOOP);
		}

		if(m_bLilGooNear && !m_bPrevLilGooNear)
		{
			CGame::GetInstance()->GetAudioManager()->UnMuteMusic(AK::EVENTS::UNMUTE_PS_ENEMY_LILGOO);
		}

		if(m_bSlimeNear && !m_bPrevSlimeNear)
		{
			CGame::GetInstance()->GetAudioManager()->UnMuteMusic(AK::EVENTS::UNMUTE_PS_ENEMY_SLIMEMONSTER);
		}

		if(!m_bLilGooNear && m_bPrevLilGooNear)
		{
			CGame::GetInstance()->GetAudioManager()->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_LILGOO);
		}

		if(!m_bSlimeNear && m_bPrevSlimeNear)
		{
			CGame::GetInstance()->GetAudioManager()->MuteMusic(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER);
		}

		m_bPrevLilGooNear = m_bLilGooNear;
		m_bPrevSlimeNear = m_bSlimeNear;
	}
}

void CComponent_Sound::Shutdown(void)
{
	if(m_pGame)
	{
		m_pGame = nullptr;
	}

	if(m_pAudio)
	{
		if(m_pSoundObject)
		{
			m_pAudio->UnRegisterSoundObject(m_pSoundObject);
			m_pSoundObject = nullptr;
		}

		m_pAudio = nullptr;
	}

	m_pClosestGooPit = nullptr;

	m_bLilGooNear = false;
	m_bSlimeNear = false;
	m_bPrevLilGooNear = false;
	m_bPrevSlimeNear = false;
	m_pParentCollision = nullptr;

	m_bSoundLoop1 = false;
	m_bSoundLoop2 = false;
	m_bInRange = false;
}

bool CComponent_Sound::PlaySound(unsigned int nSound)
{	
	if(!m_pAudio || !m_pSoundObject || !m_pParent)
	{
		//if(m_pParent && m_pParent->GetType() == OBJ_NULL)
		//{
		//	LOG(LOG_DEBUG, "CComponent_Sound::PlaySound()", "I'm not what I'm Supposed to be?!?");
		//}

		return false;
	}

	switch( m_pParent->GetType() )
	{
	case OBJ_PLAYER:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_PLAYER_JUMP:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PLAYER_JUMP);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_PLAYER_LAND:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PLAYER_LAND);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_PLAYER_WALK:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PLAYER_WALK);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_PLAYER_PULL_LOOP:
				{
					if(!m_bSoundLoop1)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PLAYER_PULL_LOOP);
						m_bSoundLoop1 = true;
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_PLAYER_PUSH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PLAYER_PUSH);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_PLAYER_SPLASH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PLAYER_SPLASH);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_PLAYER_SPLASH_GOOPIT:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PLAYER_SPLASH_GOOPIT);
					}
				}
				break;
			case AK::EVENTS::PLAY_DLG_PLAYER_DEATH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_PLAYER_DEATH);
					}
				}
				break;
			case AK::EVENTS::PLAY_DLG_PLAYER_HURT_BIG:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_PLAYER_HURT_BIG);
					}
				}
				break;
			case AK::EVENTS::PLAY_DLG_PLAYER_HURT_SMALL:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_PLAYER_HURT_SMALL);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_VACUUM_PICKUP_OBJECT:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_VACUUM_PICKUP_OBJECT);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_PUDGE_WALL_GRAB:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PUDGE_WALL_GRAB);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_PUDGE_WALL_SLIDE_LOOP:
				{
					if(!m_bSoundLoop2)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PUDGE_WALL_SLIDE_LOOP);
						m_bSoundLoop2 = true;
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_GOO:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_LAND:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_LAND);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_DEATH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_DEATH);
					}
				}
				break;
			//case AK::EVENTS::PLAY_DLG_LILGOO_IDLE:
			//	{
			//		if(m_bInRange)
			//		{
			//			m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_LILGOO_IDLE);
			//		}
			//	}
			//	break;
			case AK::EVENTS::PLAY_LILGOO_SUCKEDUP:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_LILGOO_SUCKEDUP);
					}
				}
				break;
			case AK::EVENTS::PLAY_DLG_LILGOO_DEATH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_LILGOO_DEATH);
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_MONSTER:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_HOP:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_HOP);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_LAND:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_LAND);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_ATTACK:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_ATTACK);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_DEATH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_DEATH);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_SPLASH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_SPLASH);
					}
				}
				break;
			case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_IDLE:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_SLIMEMONSTER_IDLE);
					}
				}
				break;
			//case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_SUPRISE:
			//	{
			//		if(m_bInRange)
			//		{
			//			m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_SLIMEMONSTER_SUPRISE);
			//		}
			//	}
			//	break;
			case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_DEATH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_SLIMEMONSTER_DEATH);
					}
				}
				break;
			case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_ATTACK:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_SLIMEMONSTER_ATTACK);
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_HULK:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_CHARGE:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_CHARGE);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_DEATH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_DEATH);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_PATROL:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_PATROL);
					}
				}
				break;
			case AK::EVENTS::PLAY_DLG_JUNKHULK_ROAR:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_JUNKHULK_ROAR);
					}
				}
				break;
			case AK::EVENTS::PLAY_DLG_JUNKHULK_DEATH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_JUNKHULK_DEATH);
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_BOSS:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_DLG_BOSS_DEATH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_BOSS_DEATH);
					}
				}
				break;
			case AK::EVENTS::PLAY_DLG_BOSS_HURT:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_BOSS_HURT);
					}
				}
				break;
			case AK::EVENTS::PLAY_DLG_BOSS_LAUGH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayDialogue(m_pSoundObject, AK::EVENTS::PLAY_DLG_BOSS_LAUGH);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_BOSS_DEATH_EXPLOSION:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_BOSS_DEATH_EXPLOSION);
					}
				}
				break;
			//case AK::EVENTS::PLAY_SFX_BOSS_HOVER_LOOP:
			//	{
			//		if(!m_bSoundLoop2)
			//		{
			//			m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_BOSS_HOVER_LOOP);
			//			m_bSoundLoop2 = true;
			//		}
			//	}
			//	break;
			case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_STEAM:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_STEAM);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_HAZARD_STEAM_PIPE_LOOP:
				{
					if(!m_bSoundLoop1)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_STEAM_PIPE_LOOP);
						m_bSoundLoop1 = true;
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_LILGOO_DISPENSOR:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_LILGOO_DISPENSOR);
					}
				}
				break;
			}
		}
		break;
	case OBJ_CRUSHER:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_RISING_LOOP:
				{
					if(!m_bSoundLoop1)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_RISING_LOOP);
						m_bSoundLoop1 = true;
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_STEAM:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_STEAM);
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_BRIDGE:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_MOVING_LOOP:
				{
					if(!m_bSoundLoop1)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_MOVING_LOOP);
						m_bSoundLoop1 = true;
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_FINISH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_FINISH);
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_PLUG:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_PLUG_COLLIDE:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PLUG_COLLIDE);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_PLUG_OUTLET:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_PLUG_OUTLET);
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_PIT:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_HAZARD_GOOPIT_BLOOP_LOOP:
				{
					if(!m_bSoundLoop1)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_GOOPIT_BLOOP_LOOP);
						m_bSoundLoop1 = true;
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_WALL_BREAKABLE:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_HAZARD_BREAKABLE_WALL_CRUMBLE:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_BREAKABLE_WALL_CRUMBLE);
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_PIPE_STEAM:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_HAZARD_STEAM_PIPE_LOOP:
				{
					if(!m_bSoundLoop1)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_STEAM_PIPE_LOOP);
						m_bSoundLoop1 = true;
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_PIPE_SPAWN:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_LILGOO_DISPENSOR:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_LILGOO_DISPENSOR);
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_PIPE_BOX:
		{
		}
		break;
	case OBJ_CRATE:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_CRATE_COLLIDE:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_CRATE_COLLIDE);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH);
					}
				}
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_CORK:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_CORK_PIPE:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_CORK_PIPE);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_CORK_COLLIDE:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_CORK_COLLIDE);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH);
					}
				}
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_BOXBUTTON:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_BUTTON_CLICK:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_BUTTON_CLICK);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH);
					}
				}
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_DOORBUTTON:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_BUTTON_CLICK:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_BUTTON_CLICK);
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_DOOR:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_DOOR_OPEN:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_DOOR_OPEN);
					}
				}
				break;
			case AK::EVENTS::PLAY_SFX_DOOR_CLOSE:
				{
					if(m_bInRange)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_DOOR_CLOSE);
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_OUTLET:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_HAZARD_OUTLET_SPARK_LOOP:
				{
					if(!m_bSoundLoop1)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_HAZARD_OUTLET_SPARK_LOOP);
						m_bSoundLoop1 = true;
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_POOL:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_WATERPOOL_HEAL_LOOP:
				{
					if(!m_bSoundLoop1)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_WATERPOOL_HEAL_LOOP);
						m_bSoundLoop1 = true;
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_CHECKPOINT:
		{
			switch(nSound)
			{
			case AK::EVENTS::PLAY_SFX_SPRINKLERS:
				{
					if(!m_bSoundLoop1)
					{
						m_pAudio->PlayEffect(m_pSoundObject, AK::EVENTS::PLAY_SFX_SPRINKLERS);
						m_bSoundLoop1 = true;
					}
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	default:
		break;
	}

	return true;
}

void CComponent_Sound::StopSound(unsigned int nSound)
{
	if(!m_pAudio || !m_pSoundObject)
	{
		return;
	}

	switch(nSound)
	{
	case AK::EVENTS::STOP_SFX_HAZARD_BRIDGE_MOVING_LOOP:
		{
			if(m_bSoundLoop1)
			{
				m_pAudio->StopEffect(m_pSoundObject, AK::EVENTS::STOP_SFX_HAZARD_BRIDGE_MOVING_LOOP);
				m_bSoundLoop1 = false;
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP:
		{
			if(m_bSoundLoop1)
			{
				m_pAudio->StopEffect(m_pSoundObject, AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP);
				m_bSoundLoop1 = false;
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_GOOPIT_BLOOP_LOOP:
		{
			if(m_bSoundLoop1)
			{
				m_pAudio->StopEffect(m_pSoundObject, AK::EVENTS::STOP_SFX_HAZARD_GOOPIT_BLOOP_LOOP);
				m_bSoundLoop1 = false;
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_OUTLET_SPARK_LOOP:
		{
			if(m_bSoundLoop1)
			{
				m_pAudio->StopEffect(m_pSoundObject, AK::EVENTS::STOP_SFX_HAZARD_OUTLET_SPARK_LOOP);
				m_bSoundLoop1 = false;
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_STEAM_PIPE_LOOP:
		{
			if(m_bSoundLoop1)
			{
 				m_pAudio->StopEffect(m_pSoundObject, AK::EVENTS::STOP_SFX_HAZARD_STEAM_PIPE_LOOP);
				m_bSoundLoop1 = false;
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_WATERPOOL_HEAL_LOOP:
		{
			if(m_bSoundLoop1)
			{
				m_pAudio->StopEffect(m_pSoundObject, AK::EVENTS::STOP_SFX_WATERPOOL_HEAL_LOOP);
				m_bSoundLoop1 = false;
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP:
		{
			if(m_bSoundLoop1)
			{
				m_pAudio->StopEffect(m_pSoundObject, AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP);
				m_bSoundLoop1 = false;
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_PUDGE_WALL_SLIDE_LOOP:
		{
			if(m_bSoundLoop2)
			{
				m_pAudio->StopEffect(m_pSoundObject, AK::EVENTS::STOP_SFX_PUDGE_WALL_SLIDE_LOOP);
				m_bSoundLoop2 = false;
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_SPRINKLERS:
		{
			if(m_bSoundLoop1)
			{
				m_pAudio->StopEffect(m_pSoundObject, AK::EVENTS::STOP_SFX_SPRINKLERS);
				m_bSoundLoop1 = false;
			}
		}
		break;
	//case AK::EVENTS::STOP_SFX_BOSS_HOVER_LOOP:
	//	{
	//		if(m_bSoundLoop2)
	//		{
	//			m_pAudio->StopEffect(m_pSoundObject, AK::EVENTS::STOP_SFX_BOSS_HOVER_LOOP);
	//			m_bSoundLoop2 = false;
	//		}
	//	}
	//	break;
	default:
		{

		}
		break;
	}
}