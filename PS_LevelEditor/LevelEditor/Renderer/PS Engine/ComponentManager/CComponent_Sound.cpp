#include "CComponent_Sound.h"
#include "../PS/CGame.h"

CComponent_Sound::CComponent_Sound(void) : IComponent(ECOMP_SOUND)
{
	m_pGame = nullptr;
	m_pAudio = nullptr;
	m_pSoundObject = nullptr;
	m_pParent = nullptr;
}
 
CComponent_Sound::~CComponent_Sound(void)
{

}

bool CComponent_Sound::Initialize(void)
{
	m_pGame = CGame::GetInstance();
	if(!m_pGame)
	{
		return false;
	}

	m_pAudio = m_pGame->GetAudioManager();
	if(!m_pAudio)
	{
		return false;
	}

	m_pParent = GetParent();
	if(!m_pParent)
	{
		return false;
	}

	if(RegisterSoundObject() == false)
	{
		return false;
	}

	return true;
}

bool CComponent_Sound::RegisterSoundObject(void)
{
	if(!m_pParent)
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
			m_pSoundObject = m_pAudio->RegisterSoundObject("Button", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_PIT:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Goo_Pit", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_WALL_BREAKABLE_VER:
		{
			m_pSoundObject = m_pAudio->RegisterSoundObject("Breakable_Wall", vec3f(0.0f, 0.0f, 0.0f));
		}
		break;
	case OBJ_WALL_BREAKABLE_HOR:
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
	default:
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
	if(!m_pParent)
	{
		m_pParent = GetParent();
	}

	if(m_pParent && !m_pSoundObject)
	{
		RegisterSoundObject();
	}

	if(m_pSoundObject)
	{
		m_pSoundObject->ObjectPosition.Position.X = GetParent()->GetWorldPos().x;
		m_pSoundObject->ObjectPosition.Position.Y = GetParent()->GetWorldPos().y;
	}
}

void CComponent_Sound::Shutdown(void)
{
	if(m_pGame)
	{
		m_pGame = NULL;
	}

	if(m_pAudio)
	{
		if(m_pSoundObject)
		{
			m_pAudio->UnRegisterSoundObject(m_pSoundObject);
			m_pSoundObject = nullptr;
		}

		m_pAudio = NULL;
	}

	if(m_pParent)
	{
		m_pParent = NULL;
	}
}

bool CComponent_Sound::PlaySound(int nSound)
{	
	if(!m_pAudio || !m_pSoundObject || !m_pParent)
	{
		return false;
	}

	switch( m_pParent->GetType() )
	{
	case OBJ_PLAYER:
		{
			switch(nSound)
			{
			case SFX_PLAYER_JUMP:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_PLAYER_JUMP);
				}
				break;
			case SFX_PLAYER_LAND:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_PLAYER_LAND);
				}
				break;
			case SFX_PLAYER_WALK:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_PLAYER_WALK);
				}
				break;
			case SFX_PLAYER_PUSH:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_PLAYER_PUSH);
				}
				break;
			case SFX_PLAYER_NUDGE:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_PLAYER_NUDGE);
				}
				break;
			case SFX_PLAYER_PULL_LOOP:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_PLAYER_PULL_LOOP);
				}
				break;
			case DLG_PLAYER_HURT:
				{
					m_pAudio->PlayDialogue(m_pSoundObject, DLG_PLAYER_HURT);
				}
				break;
			case DLG_PLAYER_DEATH:
				{
					m_pAudio->PlayDialogue(m_pSoundObject, DLG_PLAYER_DEATH);
				}
				break;
			case SFX_HAZARD_GOOPIT_SPLASH:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_HAZARD_GOOPIT_SPLASH);
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
			case SFX_ENEMY_LILGOO_HOP:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_ENEMY_LILGOO_HOP);
				}
				break;
			case SFX_ENEMY_LILGOO_LAND:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_ENEMY_LILGOO_LAND);
				}
				break;
			case SFX_ENEMY_LILGOO_ATTACK:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_ENEMY_LILGOO_ATTACK);
				}
				break;
			case SFX_ENEMY_LILGOO_DEATH:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_ENEMY_LILGOO_DEATH);
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
			case SFX_ENEMY_SLIMEMONSTER_HOP:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_ENEMY_SLIMEMONSTER_HOP);
				}
				break;
			case SFX_ENEMY_SLIMEMONSTER_LAND:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_ENEMY_SLIMEMONSTER_LAND);
				}
				break;
			case SFX_ENEMY_SLIMEMONSTER_ATTACK:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_ENEMY_SLIMEMONSTER_ATTACK);
				}
				break;
			case SFX_ENEMY_SLIMEMONSTER_DEATH:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_ENEMY_SLIMEMONSTER_DEATH);
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
		}
		break;
	case OBJ_BOSS:
		{
		}
		break;
	case OBJ_CRUSHER:
		{
			switch(nSound)
			{
			case SFX_HAZARD_CRUSHER_CRUSH:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_HAZARD_CRUSHER_CRUSH);
				}
				break;
			case SFX_HAZARD_CRUSHER_STEAM:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_HAZARD_CRUSHER_STEAM);
				}
				break;
			case SFX_HAZARD_CRUSHER_RISING:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_HAZARD_CRUSHER_RISING);
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
			case SFX_HAZARD_BRIDGE:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_HAZARD_BRIDGE);
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
		}
		break;
	case OBJ_PIT:
		{
			switch(nSound)
			{
			case SFX_HAZARD_GOOPIT_BLOOP:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_HAZARD_GOOPIT_BLOOP);
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_WALL_BREAKABLE_VER:
		{
			switch(nSound)
			{
			case SFX_HAZARD_BREAKABLE_WALL:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_HAZARD_BREAKABLE_WALL);
				}
				break;
			default:
				{

				}
				break;
			}
		}
		break;
	case OBJ_WALL_BREAKABLE_HOR:
		{
			switch(nSound)
			{
			case SFX_HAZARD_BREAKABLE_WALL:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_HAZARD_BREAKABLE_WALL);
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
			case SFX_HAZARD_STEAM_PIPE:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_HAZARD_STEAM_PIPE);
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
			case SFX_CRATE_CRASH:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_CRATE_CRASH);
				}
				break;
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
			case SFX_CORK_CRASH:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_CORK_CRASH);
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
			case SFX_WATERPOOL_HEAL:
				{
					m_pAudio->PlayEffect(m_pSoundObject, SFX_WATERPOOL_HEAL);
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

void CComponent_Sound::StopSound(int nSound)
{
	if(!m_pAudio || !m_pSoundObject)
	{
		return;
	}

	switch(nSound)
	{
		case SFX_PLAYER_PULL_LOOP:
			{
				m_pAudio->StopEffect(m_pSoundObject, SFX_PLAYER_PULL_LOOP);
			}
			break;
		case SFX_HAZARD_GOOPIT_BLOOP:
			{
				m_pAudio->StopEffect(m_pSoundObject, SFX_HAZARD_GOOPIT_BLOOP);
			}
			break;
		case SFX_HAZARD_STEAM_PIPE:
			{
				m_pAudio->StopEffect(m_pSoundObject, SFX_HAZARD_STEAM_PIPE);
			}
			break;
		default:
			{

			}
			break;
	}
}