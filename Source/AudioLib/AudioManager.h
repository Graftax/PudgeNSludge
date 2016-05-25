//////////////////////////////////////////////////////////////////////////
// Title: AudioManager.h
// Author: Josh Morgan
// Date: 5/3/12
// Purpose:	This class will handle intitializing Wwise, and will
//			handle the communication with the Wwise API. It will
//			also be used to send the events to Wwise to play the
//			different audio the game will be using.
//////////////////////////////////////////////////////////////////////////
#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_

#include <vector>
#include <queue>
using namespace std;

#include "../Utilities/MathLib/vec3.h"
#include "AKIncludes/AK/SoundEngine/Common/AkTypes.h"

//include for the WWiseIDs
#include "../../Resources/Sounds/Wwise_IDs.h"

//enum EFFECT {
//	/*PLAYER*/
//	SFX_PLAYER_JUMP = 0,
//	SFX_PLAYER_LAND,
//	SFX_PLAYER_WALK,
//	SFX_PLAYER_PUSH,
//	SFX_PLAYER_NUDGE,
//	SFX_PLAYER_PULL_LOOP,
//
//	/*ENEMIES*/
//	SFX_ENEMY_LILGOO_HOP,
//	SFX_ENEMY_LILGOO_LAND,
//	SFX_ENEMY_LILGOO_ATTACK,
//	SFX_ENEMY_LILGOO_DEATH,
//	SFX_ENEMY_SLIMEMONSTER_HOP,
//	SFX_ENEMY_SLIMEMONSTER_LAND,
//	SFX_ENEMY_SLIMEMONSTER_ATTACK,
//	SFX_ENEMY_SLIMEMONSTER_SURPRISE,
//	SFX_ENEMY_SLIMEMONSTER_DEATH,
//	SFX_ENEMY_JUNKHULK_PATROL,
//	SFX_ENEMY_JUNKHULK_CHARGE,
//	SFX_ENEMY_JUNKHULK_DEATH,
//	
//
//	/*MENU*/
//	SFX_MENU_ACCEPT,
//	SFX_MENU_CANCEL,
//	SFX_MENU_HIGHLIGHT,
//	SFX_MENU_PAUSE,
//	SFX_MENU_UNPAUSE,
//
//	/*HAZARDS*/
//	SFX_HAZARD_STEAM_PIPE,
//	SFX_HAZARD_CRUSHER_CRUSH,
//	SFX_HAZARD_CRUSHER_STEAM,
//	SFX_HAZARD_CRUSHER_RISING,
//	SFX_HAZARD_GOOPIT_SPLASH,
//	SFX_HAZARD_GOOPIT_BLOOP,
//	SFX_HAZARD_BREAKABLE_WALL,
//	SFX_HAZARD_BRIDGE,
//
//	/*PLAYER INTERACTION*/
//	SFX_CRATE_CRASH,
//	SFX_CORK_CRASH,
//	SFX_WATERPOOL_HEAL,
//};
//
//enum MUSIC {
//	BGM_TITLE = 100,
//	BGM_GAMEPLAY_LEVEL1,
//	BGM_VICTORY,
//	BGM_GAMEOVER,
//	BGM_CREDITS,
//};
//
//enum DIALOGUE {
//	DLG_PLAYER_HURT = 200,
//	DLG_PLAYER_DEATH,
//};

__declspec(align(8))
struct TSoundObject
{
	AkSoundPosition	ObjectPosition;
	AkGameObjectID nObjectId;
	char* szObjectName;
	bool bFree;

	TSoundObject() {}
	~TSoundObject() {}
};

__declspec(align(32))
class CAudioManager
{
private:
	static CAudioManager* m_pInstance;
	int m_nMasterVolume;
	int m_nEffectsVolume;
	int m_nMusicVolume;
	int m_nDialogueVolume;
	int m_nScaledEffectsVolume;
	int m_nScaledMusicVolume;
	int m_nScaledDialogueVolume;
	unsigned int m_nCurrentMusicID;
	bool m_bMusicPlaying;

	vector<TSoundObject*> m_vSoundObjects;
	queue<TSoundObject*> m_qFreeObjects;
	AkGameObjectID m_nCurrentId;
	AkListenerPosition m_ListenerPosition;
	bool m_bMuted;

	float m_fClosestGooPit;

private:
	

public:
	CAudioManager(void);
	~CAudioManager(void);
	//static CAudioManager* GetInstance(void);
	//static void DeleteInstance(void);

	/////////////////////////////////////////////////////////////////////
	//Initialize():		Initializes Wwise and sets up any members that
	//					need to be set up here.
	//
	//Returns:			bool - true if Wwise was successfully initialized
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool Initialize(void);

	/////////////////////////////////////////////////////////////////////
	//LoadBanks():		Loads in the audio banks that hold the sounds
	//					that the game will be using.
	//
	//Returns:			bool - true if the banks were loaded successfully.
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool LoadBanks(void);

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
	TSoundObject* RegisterSoundObject(char* szObjectName, vec3f tPosition);

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
	void Update(void);


	/////////////////////////////////////////////////////////////////////
	//PlayEffect():		Plays an effect specified by nEffectId, which is
	//					supposed to be in the EFFECTS enum.
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void PlayEffect(TSoundObject* pSoundOject, unsigned int nEffectId);

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
	void PlayEffect(unsigned int nEffectId);

	/////////////////////////////////////////////////////////////////////
	//PlayMusic():		Plays music specified by nMusicId which is
	//					specified in the MUSIC enum.
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void PlayMusic(unsigned int nMusicId);

	/////////////////////////////////////////////////////////////////////
	//PlayDialogue():	Plays dialogue specified by nDialogueID, which is
	//					in the DIALOGUE enum.
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void PlayDialogue(TSoundObject* pSoundOject, unsigned int nDialogueId);

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
	void PlayDialogue(unsigned int nDialogueId);


	/////////////////////////////////////////////////////////////////////
	//StopEffect():		Stops the effect specified by nEffectId, which is
	//					in the EFFECT enum.
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void StopEffect(TSoundObject* pSoundObject, unsigned int nEffectId); 

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
	void StopEffect(unsigned int nEffectId);

	/////////////////////////////////////////////////////////////////////
	//StopMusic():		Stops the music specified by nMusicId, which is
	//					in the MUSIC enum
	//
	//Returns:			void
	//
	//Mod Date:			7/20/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void StopMusic(void);

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
	void StopDialogue(TSoundObject* pSoundObject, unsigned int nDialogueId);

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
	void StopDialogue(unsigned int nDialogueId);

	/////////////////////////////////////////////////////////////////////
	//StopAll():		Stops all the sounds currently playing.
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void StopAll(void);

	/////////////////////////////////////////////////////////////////////
	//PauseAll():		Pauses all the sounds currently playing.
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void PauseAll(void);

	/////////////////////////////////////////////////////////////////////
	//ResumeAll():		Resumes all the sounds that were paused.
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void ResumeAll(void);

	/////////////////////////////////////////////////////////////////////
	//Mute():			Mutes the sounds 
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void Mute(void);

	/////////////////////////////////////////////////////////////////////
	//UnMute():			Unmutes the sound and sets it back to the
	//					volume it was before it was muted.
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void UnMute(void);

	/////////////////////////////////////////////////////////////////////
	//MuteMusic():		Mutes the music of the id passed in.
	//
	//Returns:			void
	//
	//Mod Date:			7/20/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void MuteMusic(unsigned int _nMusicID);

	/////////////////////////////////////////////////////////////////////
	//UnMute():			Unmutes the music of the ID passed in so it plays
	//					over the already playing music.
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void UnMuteMusic(unsigned int _nMusicID);

	/////////////////////////////////////////////////////////////////////
	//UnLoadBanks():	Unloads the banks from Wwise
	//
	//Returns:			void
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void UnLoadBanks(void);

	/////////////////////////////////////////////////////////////////////
	//UnRegisterSoundObject():This function unregisters a sound object
	//					from Wwise.
	//
	//Returns:			void
	//
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void UnRegisterSoundObject(TSoundObject* pSoundObject);

	/////////////////////////////////////////////////////////////////////
	//UnRegisterAll():	Unregisters all the sound objects from Wwise.
	//
	//Returns:			void
	//
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void UnRegisterAll(void);

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
	void Shutdown(void);

	////////////////////////
	//		ACCESSORS	  //
	////////////////////////
	int GetMasterVolume(void) { return m_nMasterVolume; }
	int GetFXVolume(void) { return m_nEffectsVolume; }
	int GetBGMVolume(void) { return m_nMusicVolume; }
	int GetDLGVolume(void) { return m_nDialogueVolume; }
	vec3f GetListenerPosition(void) { return vec3f(m_ListenerPosition.Position.X, m_ListenerPosition.Position.Y, m_ListenerPosition.Position.Z); }

	////////////////////////
	//		MUTATORS	  //
	////////////////////////
	void SetMasterVolume(int nVolume);
	void SetFXVolume(int nVolume);
	void SetBGMVolume(int nVolume);
	void SetDLGVolume(int nVolume);
	void SetListenerPosition(vec3f position);
	void SetSoundObjectPosition(TSoundObject* pSoundObject, vec3f tPosition);
};

#endif //IAUDIOMANAGER_H_