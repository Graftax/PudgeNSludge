#include "AudioManager.h"

#define SOUND_SCALE 10

#ifndef _DEBUG
#define AK_OPTIMIZED
#endif //DEBUG

//#include "../PS/config.h"
//#include "../Utilities/Logger/Logger.h"

//sound engine
#include "AKIncludes/AK/SoundEngine/Common/AkSoundEngine.h"
#include "AKIncludes/AK/IBytes.h"

//memory manager
#include "AKIncludes/AK/SoundEngine/Common/AkMemoryMgr.h"
#include "AKIncludes/AK/SoundEngine/Common/AkModule.h"

//stream manager
#include "AKIncludes/AK/SoundEngine/Common/IAkStreamMgr.h"
#include "AKIncludes/AK/Tools/Common/AkPlatformFuncs.h"
#include "AKIncludes/Win32/AkFilePackageLowLevelIOBlocking.h"

//music manager
#include "AKIncludes/AK/MusicEngine/Common/AkMusicEngine.h"

//include for communication between Wwise and the game
#ifndef AK_OPTIMIZED
	#include "AKIncludes/AK/Comm/AkCommunication.h"
#endif //AK_OPTIMIZED

//other wise includes needed for functionality
#include "AKIncludes/AK/Plugin/AkVorbisFactory.h"
#include "AKIncludes/AK/SoundEngine/Common/AkDynamicSequence.h"
#include "AKIncludes/AK/SoundEngine/Common/AkQueryParameters.h"

#ifdef _DEBUG
#pragma comment(lib, "../AudioLib/lib_d/AkSoundEngine.lib")
#pragma comment(lib, "../AudioLib/lib_d/AkMusicEngine.lib")
#pragma comment(lib, "../AudioLib/lib_d/AkMemoryMgr.lib")
#pragma comment(lib, "../AudioLib/lib_d/AkStreamMgr.lib")
#pragma comment(lib, "../AudioLib/lib_d/CommunicationCentral.lib")
#pragma comment(lib, "../AudioLib/lib_d/AkAudioInputSource.lib")
#pragma comment(lib, "../AudioLib/lib_d/dxguid.lib")
#pragma comment(lib, "../AudioLib/lib_d/AkVorbisDecoder.lib")
#pragma comment(lib, "Ws2_32.lib")
#else				 
#pragma comment(lib, "../AudioLib/lib/AkMemoryMgr.lib")
#pragma comment(lib, "../AudioLib/lib/AkSoundEngine.lib")
#pragma comment(lib, "../AudioLib/lib/AkStreamMgr.lib")
#pragma comment(lib, "../AudioLib/lib/AkMusicEngine.lib")
#pragma comment(lib, "../AudioLib/lib/AkAudioInputSource.lib")
#pragma comment(lib, "../AudioLib/lib/dxguid.lib")
#pragma comment(lib, "../AudioLib/lib/AkVorbisDecoder.lib")
#endif //_DEBUG

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
#ifdef WIN32
    void * AllocHook( size_t in_size )
    {
        return malloc( in_size );
    }
    void FreeHook( void * in_ptr )
    {
        free( in_ptr );
    }
    // Note: VirtualAllocHook() may be used by I/O pools of the default implementation
    // of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
    // - refer to the Windows SDK documentation for more details). This is NOT mandatory;
    // you may implement it with a simple malloc().
    void * VirtualAllocHook(	void * in_pMemAddress,
								size_t in_size,
								DWORD in_dwAllocationType,
								DWORD in_dwProtect)
    {
        return VirtualAlloc( in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect );
    }
    void VirtualFreeHook(	void * in_pMemAddress,
							size_t in_size,
							DWORD in_dwFreeType)
    {
        VirtualFree( in_pMemAddress, in_size, in_dwFreeType );
    }
#endif //WIN32
}

#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_SOUNDBANK L"SoundBank.bnk"

CAudioManager* CAudioManager::m_pInstance = NULL;

CAudioManager::CAudioManager()
{
	m_nMasterVolume = 50;
	m_nEffectsVolume = 100;
	m_nMusicVolume = 100;
	m_nDialogueVolume = 100;
	m_nScaledEffectsVolume = 100;
	m_nScaledMusicVolume = 100;
	m_nScaledDialogueVolume = 100;
	m_nCurrentId = 10;
	m_bMuted = false;
	m_bMusicPlaying = false;
}

CAudioManager::~CAudioManager()
{

}

//CAudioManager* CAudioManager::GetInstance()
//{
//	if(m_pInstance == NULL)
//	{
//		m_pInstance = new CAudioManager();
//	}
//
//	return m_pInstance;
//}
//
//void CAudioManager::DeleteInstance()
//{
//	if(m_pInstance != NULL)
//	{
//		delete m_pInstance;
//		m_pInstance = NULL;
//	}
//}

/////////////////////////////////////////////////////////////////////
//Initialize():		Initializes Wwise and sets up any members that
//					need to be set up here.
//
//Returns:			bool - true if Wwise was successfully initialized
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CAudioManager::Initialize()
{
	m_nMasterVolume = 50;
	m_nEffectsVolume = 100;
	m_nMusicVolume = 100;
	m_nDialogueVolume = 100;
	m_nScaledEffectsVolume = 100;
	m_nScaledMusicVolume = 100;
	m_nScaledDialogueVolume = 100;
	m_nCurrentId = 10;
	m_bMuted = false;
	m_bMusicPlaying = false;

	//create and init an instance of the default memory manager
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 64;

	if(AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		//TODO: report error in the logger
		return false;
	}

	//create and init an instance of the default streaming manager
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	if(!AK::StreamMgr::Create(stmSettings))
	{
		//TODO: report error in the logger
		return false;
	}

	//create a streaming device with blocking low-level I/O handshaking.
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	//create a streaming device in the stream manager
	if(g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		//TODO: report error in the logger
		return false;
	}

	//Create the sound engine using default initialization params
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	initSettings.uDefaultPoolSize = 4 * 1024 * 1024;
	initSettings.uMaxNumPaths = 64;
	initSettings.uMaxNumTransitions = 128;

	platformInitSettings.uLEngineDefaultPoolSize = 4 * 1024 * 1024;

	if(AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		//TODO: report error in the logger
		return false;
	}

	//initialize the music engine
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if(AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		//TODO: report error in the logger
		return false;
	}

#ifndef AK_OPTIMIZED
	//initialize communications
	/**THIS SHOULD NOT BE ENABLED IN RELEASE MODE**/
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if(AK::Comm::Init(commSettings) != AK_Success)
	{
		//TODO: report error in the logger
		return false;
	}
#endif //AK_OPTIMIZED

	AK::SoundEngine::RegisterCodec(AKCOMPANYID_AUDIOKINETIC,
								   AKCODECID_VORBIS,
								   CreateVorbisFilePlugin,
								   CreateVorbisBankPlugin);

	AK::SoundEngine::SetListenerScalingFactor(0, SOUND_SCALE);

	//SetListenerPosition(vec3f(0.0f, 0.0f, 0.0f));

	LoadBanks();

	//SetEffectsVolume(100);
	//SetMusicVolume(100);
	//SetDialogueVolume(100);
	//SetMasterVolume(50);

	//if nothing went wrong in the init return true
	return true;
}

/////////////////////////////////////////////////////////////////////
//LoadBanks():		Loads in the audio banks that hold the sounds
//					that the game will be using.
//
//Returns:			bool - true if the banks were loaded successfully.
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CAudioManager::LoadBanks(void)
{
	g_lowLevelIO.SetBasePath(L"./Resources/Sounds/Windows/");

	AkBankID init, soundBank;
	if(AK::SoundEngine::LoadBank(BANKNAME_INIT, AK_DEFAULT_POOL_ID, init) != 
		AK_Success)
	{
		return false;
	}

	if(AK::SoundEngine::LoadBank(BANKNAME_SOUNDBANK, AK_DEFAULT_POOL_ID, soundBank) !=
		AK_Success)
	{
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//RegisterSoundObject():Registers a sound object to Wwise that will
//					be used to update the position of the sounds the
//					object will produce.
//
//TSoundObject:		returns the sound object that was registered.
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
TSoundObject* CAudioManager::RegisterSoundObject(char* szObjectName, vec3f tPosition)
{
	TSoundObject* pSoundObject;
	if(m_qFreeObjects.size() != 0)
	{
		pSoundObject = m_qFreeObjects.front();
		m_qFreeObjects.pop();
		pSoundObject->szObjectName = szObjectName;
	}
	else
	{
		if(!AK::SoundEngine::IsInitialized())
		{
			return NULL;
		}

		pSoundObject = new TSoundObject();
		pSoundObject->nObjectId = m_nCurrentId;
		pSoundObject->szObjectName = szObjectName;
		if(AK::SoundEngine::RegisterGameObj(pSoundObject->nObjectId, pSoundObject->szObjectName)
			!= AK_Success)
		{
			assert(!"Could not register the game object!");
		}
		m_nCurrentId += 10;
		m_vSoundObjects.push_back(pSoundObject);
	}
	
	pSoundObject->bFree = false;
	
	pSoundObject->ObjectPosition.Position.X = tPosition.x;
	pSoundObject->ObjectPosition.Position.Y = tPosition.y;
	pSoundObject->ObjectPosition.Position.Z = tPosition.z;
	pSoundObject->ObjectPosition.Orientation.Z = 1.0f;
	pSoundObject->ObjectPosition.Orientation.X = pSoundObject->ObjectPosition.Orientation.Y = 0.0f;

	SetSoundObjectPosition(pSoundObject, tPosition);

	return pSoundObject;
}

/////////////////////////////////////////////////////////////////////
//Update():			Calls the function to render the audio every
//					frame, and Wwise will process any events it has
//					so the audio gets updated properly.
//
//Returns:			void
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::Update()
{
	//this processes the request, events, positions, RTC, etc.. that
	// have been sent to wise. This is required to update the audio
	// and play sounds.
	AK::SoundEngine::RenderAudio();
}

/////////////////////////////////////////////////////////////////////
//PlayEffect():		Plays an effect specified by nEffectId, which is
//					supposed to be in the EFFECTS enum.
//
//Returns:			void
//
//Mod Date:			6/9/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::PlayEffect(TSoundObject* pSoundObject, unsigned int nEffectId)
{
	switch(nEffectId)
	{
	case AK::EVENTS::PLAY_SFX_PLAYER_WALK:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_WALK, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_PULL_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_PULL_LOOP, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_PUSH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_PUSH, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_JUMP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_JUMP, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_LAND:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_LAND, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_SPLASH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_SPLASH, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_SPLASH_GOOPIT:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_SPLASH_GOOPIT, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_PUDGE_WALL_GRAB:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PUDGE_WALL_GRAB, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_PUDGE_WALL_SLIDE_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PUDGE_WALL_SLIDE_LOOP, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_VACUUM_PICKUP_OBJECT:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_VACUUM_PICKUP_OBJECT, pSoundObject->nObjectId);
			}
		}
		break;

	/*ENEMIES*/
	case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_LAND:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_LAND, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_DEATH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_DEATH, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_HOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_HOP, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_LAND:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_LAND, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_ATTACK:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_ATTACK, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_SUPRISE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_SUPRISE, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_SPLASH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_SPLASH, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_DEATH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_DEATH, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_PATROL:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_PATROL, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_CHARGE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_CHARGE, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_DEATH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_DEATH, pSoundObject->nObjectId);
			}
		}
		break;

	//MENU
	case AK::EVENTS::PLAY_SFX_MENU_ACCEPT:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_MENU_CANCEL:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_MENU_CANCEL, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_MENU_PAUSE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_MENU_PAUSE, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_MENU_UNPAUSE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_MENU_UNPAUSE, pSoundObject->nObjectId);
			}
		}
		break;

	//HAZARDS
	case AK::EVENTS::PLAY_SFX_HAZARD_STEAM_PIPE_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_STEAM_PIPE_LOOP, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_LILGOO_DISPENSOR:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_LILGOO_DISPENSOR, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_STEAM:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_STEAM, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_RISING_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_RISING_LOOP, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_GOOPIT_BLOOP_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_GOOPIT_BLOOP_LOOP, pSoundObject->nObjectId);	//this one loops
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_BREAKABLE_WALL_CRUMBLE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_BREAKABLE_WALL_CRUMBLE, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_MOVING_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_MOVING_LOOP, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_FINISH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_FINISH, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_OUTLET_SPARK_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_FINISH, pSoundObject->nObjectId);
			}
		}
		break;

	//PLAYER INTERACTIONS
	case AK::EVENTS::PLAY_SFX_CRATE_COLLIDE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_CRATE_COLLIDE, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_CORK_COLLIDE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_CORK_COLLIDE, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_CORK_PIPE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_CORK_PIPE, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_WATERPOOL_HEAL_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_WATERPOOL_HEAL_LOOP, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_BUTTON_CLICK:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_BUTTON_CLICK, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_DOOR_OPEN:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_DOOR_OPEN, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_DOOR_CLOSE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_DOOR_CLOSE, pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_SFX_SPRINKLERS:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_SPRINKLERS, pSoundObject->nObjectId);
			}
		}
		break;
	//BOSS
	case AK::EVENTS::PLAY_SFX_BOSS_DEATH_EXPLOSION:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_BOSS_DEATH_EXPLOSION, pSoundObject->nObjectId);
			}
		}
		break;
	//case AK::EVENTS::PLAY_SFX_BOSS_HOVER_LOOP:
	//	{
	//		if(pSoundObject)
	//		{
	//			AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
	//			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_BOSS_HOVER_LOOP, pSoundObject->nObjectId);
	//		}
	//	}
	//	break;
	default:
		{

		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////
//PlayEffect():		Plays an effect specified by nEffectId, which is
//					supposed to be in the EFFECTS enum. This will not
//					use a sound object, so the sounds won't have a
//					position to play at, they will just play
//
//Returns:			void
//
//Mod Date:			6/9/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::PlayEffect(unsigned int nEffectId)
{
	switch(nEffectId)
	{
	case AK::EVENTS::PLAY_SFX_PLAYER_WALK:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_WALK, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_PULL_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_PULL_LOOP, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_PUSH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_PUSH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_JUMP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_JUMP, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_LAND:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_LAND, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_PLAYER_SPLASH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_PLAYER_SPLASH, NULL);
		}
		break;

	/*ENEMIES*/
	case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_HOP, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_LAND:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_LAND, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_SPLASH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_DEATH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_LILGOO_DEATH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_HOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_HOP, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_LAND:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_LAND, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_ATTACK:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_ATTACK, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_SUPRISE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_SUPRISE, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_SPLASH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_SPLASH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_DEATH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_SLIMEMONSTER_DEATH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_PATROL:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_PATROL, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_CHARGE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_CHARGE, NULL);
			
		}
		break;
	case AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_DEATH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_ENEMY_JUNKHULK_DEATH, NULL);
		}
		break;

	//MENU
	case AK::EVENTS::PLAY_SFX_MENU_ACCEPT:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_MENU_ACCEPT, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_MENU_CANCEL:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_MENU_CANCEL, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_MENU_HIGHLIGHT, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_MENU_PAUSE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_MENU_PAUSE, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_MENU_UNPAUSE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_MENU_UNPAUSE, NULL);
		}
		break;

	//HAZARDS
	case AK::EVENTS::PLAY_SFX_HAZARD_STEAM_PIPE_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_STEAM_PIPE_LOOP, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_CRUSH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_STEAM:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_STEAM, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_RISING_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_CRUSHER_RISING_LOOP, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_GOOPIT_BLOOP_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_GOOPIT_BLOOP_LOOP, NULL);	//this one loops
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_BREAKABLE_WALL_CRUMBLE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_BREAKABLE_WALL_CRUMBLE, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_MOVING_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_MOVING_LOOP, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_FINISH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_FINISH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_HAZARD_OUTLET_SPARK_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_HAZARD_BRIDGE_FINISH, NULL);
		}
		break;

	//PLAYER INTERACTIONS
	case AK::EVENTS::PLAY_SFX_CRATE_COLLIDE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_CRATE_COLLIDE, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_CORK_COLLIDE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_CORK_COLLIDE, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_CORK_PIPE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_CORK_PIPE, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_WATERPOOL_HEAL_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_WATERPOOL_HEAL_LOOP, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_BUTTON_CLICK:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_BUTTON_CLICK, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_DOOR_OPEN:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_DOOR_OPEN, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_DOOR_CLOSE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_DOOR_CLOSE, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_SPRINKLERS:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_SPRINKLERS, NULL);
		}
		break;
	/*** Boss ***/
	case AK::EVENTS::PLAY_SFX_BOSS_DEATH_EXPLOSION:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_BOSS_DEATH_EXPLOSION, NULL);
		}
		break;
	//case AK::EVENTS::PLAY_SFX_BOSS_HOVER_LOOP:
	//	{
	//		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_BOSS_HOVER_LOOP, NULL);
	//	}
	//	break;
	// MISC...
	case AK::EVENTS::PLAY_SFX_SPLASHSCREEN_INFINITEHP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_SPLASHSCREEN_INFINITEHP, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_WWISESPLASH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_WWISESPLASH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_SFX_AMBIENCE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_SFX_AMBIENCE, NULL);
		}
		break;
	default:
		{

		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////
//PlayMusic():		Plays music specified by nMusicId which is
//					specified in the MUSIC enum.
//
//Returns:			void
//
//Mod Date:			6/9/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::PlayMusic(unsigned int nMusicId)
{
	if(m_nCurrentMusicID == nMusicId)
	{
		return;
	}

	//check to see if the music is playing, and play it if it's not
	if(!m_bMusicPlaying)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_MX_MUSIC, NULL);
		m_bMusicPlaying = true;
	}

	switch(nMusicId)
	{
	case AK::SWITCHES::MUSIC::SWITCH::PS_MENU:
		{
			AK::SoundEngine::SetSwitch(AK::SWITCHES::MUSIC::GROUP, 
				AK::SWITCHES::MUSIC::SWITCH::PS_MENU, NULL);
		}
		break;
	case AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL1:
		{
			AK::SoundEngine::SetSwitch(AK::SWITCHES::MUSIC::GROUP, 
				AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL1, NULL);
		}
		break;
	case AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL2:
		{
			AK::SoundEngine::SetSwitch(AK::SWITCHES::MUSIC::GROUP, 
				AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL2, NULL);
		}
		break;
	case AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL3:
		{
			AK::SoundEngine::SetSwitch(AK::SWITCHES::MUSIC::GROUP, 
				AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL3, NULL);
		}
		break;
	case AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL4:
		{
			AK::SoundEngine::SetSwitch(AK::SWITCHES::MUSIC::GROUP, 
				AK::SWITCHES::MUSIC::SWITCH::PS_LEVEL4, NULL);
		}
		break;
	case AK::SWITCHES::MUSIC::SWITCH::PS_LEVELBOSS:
		{
			AK::SoundEngine::SetSwitch(AK::SWITCHES::MUSIC::GROUP, 
				AK::SWITCHES::MUSIC::SWITCH::PS_LEVELBOSS, NULL);
		}
		break;
	case AK::SWITCHES::MUSIC::SWITCH::PS_CREDITS:
		{
			AK::SoundEngine::SetSwitch(AK::SWITCHES::MUSIC::GROUP, 
				AK::SWITCHES::MUSIC::SWITCH::PS_CREDITS, NULL);
		}
		break;
	default:
		{

		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////
//PlayDialogue():	Plays dialogue specified by nDialogueID, which is
//					in the DIALOGUE enum.
//
//Returns:			void
//
//Mod Date:			6/9/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::PlayDialogue(TSoundObject* pSoundObject, unsigned int nDialogueId)
{
	switch(nDialogueId)
	{
	/*** Pudge ***/
	case AK::EVENTS::PLAY_DLG_PLAYER_HURT_SMALL:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_PLAYER_HURT_SMALL, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_DLG_PLAYER_HURT_BIG:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_PLAYER_HURT_BIG, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_DLG_PLAYER_DEATH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_PLAYER_DEATH, 
					pSoundObject->nObjectId);
			}
		}
		break;
	/*** Enemies ***/
	//case AK::EVENTS::PLAY_DLG_LILGOO_IDLE:
	//	{
	//		if(pSoundObject)
	//		{
	//			AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
	//			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_LILGOO_IDLE, 
	//				pSoundObject->nObjectId);
	//		}
	//	}
	//	break;
	case AK::EVENTS::PLAY_LILGOO_SUCKEDUP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_LILGOO_SUCKEDUP, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_DLG_LILGOO_DEATH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_LILGOO_DEATH, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_IDLE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_IDLE, 
					pSoundObject->nObjectId);
			}
		}
		break;
	/*case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_SUPRISE:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_SUPRISE, 
					pSoundObject->nObjectId);
			}
		}
		break;*/
	case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_ATTACK:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_ATTACK, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_DEATH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_DEATH, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_DLG_JUNKHULK_ROAR:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_JUNKHULK_ROAR, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_DLG_JUNKHULK_DEATH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_JUNKHULK_DEATH, 
					pSoundObject->nObjectId);
			}
		}
		break;
	/*** Boss ***/
	case AK::EVENTS::PLAY_DLG_BOSS_DEATH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_BOSS_DEATH, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_DLG_BOSS_HURT:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_BOSS_HURT, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::PLAY_DLG_BOSS_LAUGH:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
				AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_BOSS_LAUGH, 
					pSoundObject->nObjectId);
			}
		}
		break;
	default:
		{

		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////
//PlayDialogue():	Plays dialogue specified by nDialogueID, which is
//					in the DIALOGUE enum. This will not use a sound
//					object, so the sounds won't have a position to
//					play at, they will just play.
//
//Returns:			void
//
//Mod Date:			6/9/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::PlayDialogue(unsigned int nDialogueId)
{
	switch(nDialogueId)
	{
	/*** Pudge ***/
	case AK::EVENTS::PLAY_DLG_PLAYER_HURT_SMALL:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_PLAYER_HURT_SMALL, NULL);
		}
		break;
	case AK::EVENTS::PLAY_DLG_PLAYER_HURT_BIG:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_PLAYER_HURT_BIG, NULL);
		}
		break;
	case AK::EVENTS::PLAY_DLG_PLAYER_DEATH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_PLAYER_DEATH, NULL);
		}
		break;
	/*** Enemies ***/
	/*case AK::EVENTS::PLAY_DLG_LILGOO_IDLE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_LILGOO_IDLE, NULL);
		}
		break;
	case AK::EVENTS::PLAY_DLG_LILGOO_SCREAM:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_LILGOO_SCREAM, NULL);
		}
		break;*/
	case AK::EVENTS::PLAY_DLG_LILGOO_DEATH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_LILGOO_DEATH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_IDLE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_IDLE, NULL);
		}
		break;
	/*case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_SUPRISE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_SUPRISE, NULL);
		}
		break;*/
	case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_ATTACK:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_ATTACK, NULL);
		}
		break;
	case AK::EVENTS::PLAY_DLG_SLIMEMONSTER_DEATH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_SLIMEMONSTER_DEATH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_DLG_JUNKHULK_ROAR:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_JUNKHULK_ROAR, NULL);
		}
		break;
	case AK::EVENTS::PLAY_DLG_JUNKHULK_DEATH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_JUNKHULK_DEATH, NULL);
		}
		break;
	/*** Boss ***/
	case AK::EVENTS::PLAY_DLG_BOSS_DEATH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_BOSS_DEATH, NULL);
		}
		break;
	case AK::EVENTS::PLAY_DLG_BOSS_HURT:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_BOSS_HURT, NULL);
		}
		break;
	case AK::EVENTS::PLAY_DLG_BOSS_LAUGH:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_DLG_BOSS_LAUGH, NULL);
		}
		break;
	default:
		{

		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////
//StopEffect():		Stops the effect specified by nEffectId, which is
//					in the EFFECT enum.
//
//Returns:			void
//
//Mod Date:			6/9/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::StopEffect(TSoundObject* pSoundObject, unsigned int nEffectId)
{
	switch(nEffectId)
	{
	case AK::EVENTS::STOP_SFX_HAZARD_BRIDGE_MOVING_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_HAZARD_BRIDGE_MOVING_LOOP, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_GOOPIT_BLOOP_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_HAZARD_GOOPIT_BLOOP_LOOP, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_OUTLET_SPARK_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_HAZARD_OUTLET_SPARK_LOOP, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_STEAM_PIPE_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_HAZARD_STEAM_PIPE_LOOP, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_PLAYER_PULL_LOOP, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_WATERPOOL_HEAL_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_WATERPOOL_HEAL_LOOP, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_PUDGE_WALL_SLIDE_LOOP:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_PUDGE_WALL_SLIDE_LOOP, 
					pSoundObject->nObjectId);
			}
		}
		break;
	case AK::EVENTS::STOP_SFX_SPRINKLERS:
		{
			if(pSoundObject)
			{
				AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_SPRINKLERS, 
					pSoundObject->nObjectId);
			}
		}
		break;
	//case AK::EVENTS::STOP_SFX_BOSS_HOVER_LOOP:
	//	{
	//		if(pSoundObject)
	//		{
	//			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_BOSS_HOVER_LOOP, 
	//				pSoundObject->nObjectId);
	//		}
	//	}
	//	break;
	default:
		{

		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////
//StopEffect():		Stops the effect specified by nEffectId, which is
//					in the EFFECT enum. This will not use a sound
//					object, so the sounds won't have a position to
//					play at, they will just play.
//
//Returns:			void
//
//Mod Date:			6/9/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::StopEffect(unsigned int nEffectId)
{
	switch(nEffectId)
	{
	case AK::EVENTS::STOP_SFX_HAZARD_BRIDGE_MOVING_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_HAZARD_BRIDGE_MOVING_LOOP, NULL);
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_HAZARD_CRUSHER_RISING_LOOP, NULL);
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_GOOPIT_BLOOP_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_HAZARD_GOOPIT_BLOOP_LOOP, NULL);
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_OUTLET_SPARK_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_HAZARD_OUTLET_SPARK_LOOP, NULL);
		}
		break;
	case AK::EVENTS::STOP_SFX_HAZARD_STEAM_PIPE_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_HAZARD_STEAM_PIPE_LOOP, NULL);
		}
		break;
	case AK::EVENTS::STOP_SFX_PUDGE_WALL_SLIDE_LOOP:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_PUDGE_WALL_SLIDE_LOOP, NULL);
		}
		break;
	case AK::EVENTS::STOP_SFX_AMBIENCE:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_AMBIENCE, NULL);
		}
		break;
	case AK::EVENTS::STOP_SFX_SPRINKLERS:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_SPRINKLERS, NULL);
		}
		break;
	//case AK::EVENTS::STOP_SFX_BOSS_HOVER_LOOP:
	//	{
	//		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_SFX_BOSS_HOVER_LOOP, NULL);
	//	}
	//	break;
	default:
		{

		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////
//StopMusic():		Stops the music specified by nMusicId, which is
//					in the MUSIC enum
//
//Returns:			void
//
//Mod Date:			7/20/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::StopMusic(void)
{
	//check to see if the music is playing, and play it if it's not
	if(m_bMusicPlaying)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MX_MUSIC, NULL);
		m_bMusicPlaying = false;
	}
}

/////////////////////////////////////////////////////////////////////
//StopDialogue():	Stops the dialogue specified by nMusicId, which 
//					is in the DIALOGUE enum.
//					
//
//Returns:			void
//
//Mod Date:			6/9/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void StopDialogue(TSoundObject* pSoundObject, unsigned int nDialogueId)
{
	//switch(nDialogueId)
	//{
	//default:
	//	{
	//
	//	}
	//	break;
	//}
}

/////////////////////////////////////////////////////////////////////
//StopDialogue():	Stops the dialogue specified by nMusicId, which 
//					is in the DIALOGUE enum. This doesn't use a 
//					sound object to work.
//
//Returns:			void
//
//Mod Date:			6/9/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void StopDialogue(unsigned int nDialogueId)
{
	//switch(nDialogueId)
	//{
	//default:
	//	{
	//
	//	}
	//	break;
	//}
}

/////////////////////////////////////////////////////////////////////
//StopAll():		Stops all the sounds currently playing.
//
//Returns:			void
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::StopAll(void)
{
	AK::SoundEngine::PostEvent(AK::EVENTS::STOP_ALL, NULL);
}

/////////////////////////////////////////////////////////////////////
//PauseAll():		Pauses all the sounds currently playing.
//
//Returns:			void
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::PauseAll(void)
{
	AK::SoundEngine::PostEvent(AK::EVENTS::PAUSE_ALL, NULL);
}

/////////////////////////////////////////////////////////////////////
//ResumeAll():		Resumes all the sounds that were paused.
//
//Returns:			void
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::ResumeAll(void)
{
	AK::SoundEngine::PostEvent(AK::EVENTS::RESUME_ALL, NULL);
}

/////////////////////////////////////////////////////////////////////
//Mute():			Mutes the sounds 
//
//Returns:			void
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::Mute(void)
{

}

/////////////////////////////////////////////////////////////////////
//UnMute():			Unmutes the sound and sets it back to the
//					volume it was before it was muted.
//
//Returns:			void
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::UnMute(void)
{

}

/////////////////////////////////////////////////////////////////////
//MuteMusic():		Mutes the music of the id passed in.
//
//Returns:			void
//
//Mod Date:			7/20/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::MuteMusic(unsigned int _nMusicID)
{
	if(!m_bMusicPlaying)
	{
		return;
	}

	switch(_nMusicID)
	{
	case AK::EVENTS::MUTE_PS_CHECKPOINT:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::MUTE_PS_CHECKPOINT, NULL);
		}
		break;
	case AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::MUTE_PS_ENEMY_JUNKHULK, NULL);
		}
		break;
	case AK::EVENTS::MUTE_PS_ENEMY_LILGOO:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::MUTE_PS_ENEMY_LILGOO, NULL);
		}
		break;
	case AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::MUTE_PS_ENEMY_SLIMEMONSTER, NULL);
		}
		break;
	default:
		{

		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////
//UnMute():			Unmutes the music of the ID passed in so it plays
//					over the already playing music.
//
//Returns:			void
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::UnMuteMusic(unsigned int _nMusicID)
{
	if(!m_bMusicPlaying)
	{
		return;
	}

	switch(_nMusicID)
	{
	case AK::EVENTS::UNMUTE_PS_CHECKPOINT:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::UNMUTE_PS_CHECKPOINT, NULL);
		}
		break;
	case AK::EVENTS::UNMUTE_PS_ENEMY_JUNKHULK:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::UNMUTE_PS_ENEMY_JUNKHULK, NULL);
		}
		break;
	case AK::EVENTS::UNMUTE_PS_ENEMY_LILGOO:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::UNMUTE_PS_ENEMY_LILGOO, NULL);
		}
		break;
	case AK::EVENTS::UNMUTE_PS_ENEMY_SLIMEMONSTER:
		{
			AK::SoundEngine::PostEvent(AK::EVENTS::UNMUTE_PS_ENEMY_SLIMEMONSTER, NULL);
		}
		break;
	default:
		{

		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////
//UnLoadBanks():	Unloads the banks from Wwise
//
//Returns:			void
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::UnLoadBanks(void)
{
	//AK::SoundEngine::StopAll();

	AK::SoundEngine::UnloadBank(BANKNAME_INIT);
	AK::SoundEngine::UnloadBank(BANKNAME_SOUNDBANK);
}

/////////////////////////////////////////////////////////////////////
//UnRegisterSoundObject():This function unregisters a sound object
//					from Wwise.
//
//Returns:			void
//
//Mod Date:			5/21/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::UnRegisterSoundObject(TSoundObject* pSoundObject)
{
	pSoundObject->bFree = true;

	//SetSoundObjectPosition(pSoundObject, vec3f(0.0f, 0.0f, 0.0f));
	m_qFreeObjects.push(pSoundObject);
}

/////////////////////////////////////////////////////////////////////
//UnRegisterAll():	Unregisters all the sound objects from Wwise.
//
//Returns:			void
//
//Mod Date:			5/21/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::UnRegisterAll(void)
{
	AK::SoundEngine::UnregisterAllGameObj();

	while(!m_qFreeObjects.empty())
	{
		m_qFreeObjects.pop();
	}

	for(unsigned int i = 0; i < m_vSoundObjects.size(); ++i)
	{
		if(!m_vSoundObjects[i])
		{
			continue;
		}
	
		delete m_vSoundObjects[i];
	}

	m_vSoundObjects.clear();

	m_nCurrentId = 10;
}

/////////////////////////////////////////////////////////////////////
//Shutdown():		Shuts down the Audio manager, freeing up any
//					memory that was allocated, and shutting down
//					Wwise
//
//Returns:			void
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CAudioManager::Shutdown()
{
	UnLoadBanks();
	UnRegisterAll();

#ifndef AK_OPTIMIZED
	//terminate the communication services
	/**THIS SHOULD ONLY BE DONE IN RELEASE MODE**/
	AK::Comm::Term();
#endif //AK_OPTIMIZED

	//terminate the music engine
	AK::MusicEngine::Term();

	//terminate the sound engine
	AK::SoundEngine::Term();

	//terminate the streaming device
	//this will destroy the associated streaming devic
	// that lives in the stream manager, and unregister
	// itself as the file location resolver
	g_lowLevelIO.Term();
	if(AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	//last but not least.. terminate the memory manager
	AK::MemoryMgr::Term();
}

void CAudioManager::SetSoundObjectPosition(TSoundObject* pSoundObject, vec3f tPosition)
{
	pSoundObject->ObjectPosition.Position.X = tPosition.x;
	pSoundObject->ObjectPosition.Position.Y = tPosition.y;
	pSoundObject->ObjectPosition.Position.Z = tPosition.z;

	AK::SoundEngine::SetPosition(pSoundObject->nObjectId, pSoundObject->ObjectPosition);
}

void CAudioManager::SetListenerPosition(vec3f tPosition)
{
	m_ListenerPosition.Position.X = tPosition.x;
	m_ListenerPosition.Position.Y = tPosition.y;
	m_ListenerPosition.Position.Z = tPosition.z;
	m_ListenerPosition.OrientationTop.Y = 1.0f;
	m_ListenerPosition.OrientationTop.X = m_ListenerPosition.OrientationTop.Z = 0.0f;
	m_ListenerPosition.OrientationFront.Z = 1.0f;
	m_ListenerPosition.OrientationFront.X = m_ListenerPosition.OrientationFront.Y = 0.0f;

	AK::SoundEngine::SetListenerPosition(m_ListenerPosition);
}

void CAudioManager::SetMasterVolume(int nVolume)
{
	//constrain the volume between 0 and 100
	if(nVolume > 100)
	{
		nVolume = 100;
	}

	if(nVolume < 0)
	{
		nVolume = 0;
	}

	m_nMasterVolume = nVolume;

	//set the volumes scaled off of the master volume
	m_nScaledEffectsVolume = (int)((0.01f * m_nMasterVolume) * m_nEffectsVolume);
	m_nScaledMusicVolume = (int)((0.01f * m_nMasterVolume) * m_nMusicVolume);
	m_nScaledDialogueVolume = (int)((0.01f * m_nMasterVolume) * m_nDialogueVolume);

	//set the volume in Wwise to the scaled value
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::SFX_VOLUME, (AkRtpcValue)m_nScaledEffectsVolume);
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::MX_VOLUME, (AkRtpcValue)m_nScaledMusicVolume);
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::DX_VOLUME, (AkRtpcValue)m_nScaledDialogueVolume);
}

void CAudioManager::SetFXVolume(int nVolume)
{
	//constrain the volume between 0 and 100
	if(nVolume > 100)
	{
		nVolume = 100;
	}
	if(nVolume < 0)
	{
		nVolume = 0;
	}

	m_nEffectsVolume = nVolume;

	//scale the effects volume off the master volume
	m_nScaledEffectsVolume = (int)((0.01f * m_nMasterVolume) * m_nEffectsVolume);

	//set the volume in Wwise to the scaled value
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::SFX_VOLUME, (AkRtpcValue)m_nScaledEffectsVolume);
}

void CAudioManager::SetBGMVolume(int nVolume)
{
	//constrain the volume between 0 and 100
	if(nVolume > 100)
	{
		nVolume = 100;
	}
	if(nVolume < 0)
	{
		nVolume = 0;
	}

	m_nMusicVolume = nVolume;
	
	//scale the music volume off the master volume
	m_nScaledMusicVolume = (int)((0.01f * m_nMasterVolume) * m_nMusicVolume);

	//set the volume in Wwise to the scaled value
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::MX_VOLUME, (AkRtpcValue)m_nScaledMusicVolume);
}

void CAudioManager::SetDLGVolume(int nVolume)
{
	//constrain the volume between 0 and 100
	if(nVolume > 100)
	{
		nVolume = 100;
	}
	if(nVolume < 0)
	{
		nVolume = 0;
	}

	m_nDialogueVolume = nVolume;

	//scale the dialogue volume off the master volume
	m_nScaledDialogueVolume = (int)((0.01f * m_nMasterVolume) * m_nDialogueVolume);

	//set the volume in Wwise to the scaled value
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::DX_VOLUME, (AkRtpcValue)m_nScaledDialogueVolume);
}