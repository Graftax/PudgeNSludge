//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

// AkWinSoundEngine.h

/// \file 
/// Main Sound Engine interface, specific WIN32.

#ifndef _AK_WIN_SOUND_ENGINE_H_
#define _AK_WIN_SOUND_ENGINE_H_

#include "../../Common/AkTypes.h"
#include "../../../Tools/Common/AkPlatformFuncs.h"

/// Sound quality option
/// (available in the PC version only)
enum AkSoundQuality
{
	AkSoundQuality_High,	///< High quality (48 kHz sampling rate, default value)
	AkSoundQuality_Low,		///< Reduced quality (24 kHz sampling rate)
};

/// Platform specific initialization settings
/// \sa AK::SoundEngine::Init
/// \sa AK::SoundEngine::GetDefaultPlatformInitSettings
/// - \ref soundengine_initialization_advanced_soundengine_using_memory_threshold
struct AkPlatformInitSettings
{
    // Direct sound.
    HWND			    hWnd;					///< Handle to the window associated to the audio.
												///< Each game must specify the HWND that will be passed to DirectSound initialization.
												///< The value returned by GetDefaultPlatformInitSettings is the foreground HWND at 
												///< the moment of the initialization of the sound engine and may not be the correct one for your game.
												///< It is required that each game provides the correct HWND to be used.
									

    // Threading model.
    AkThreadProperties  threadLEngine;			///< Lower engine threading properties
	AkThreadProperties  threadBankManager;		///< Bank manager threading properties (its default priority is AK_THREAD_PRIORITY_NORMAL)

    // Memory.
    AkUInt32            uLEngineDefaultPoolSize;///< Lower Engine default memory pool size
	AkReal32            fLEngineDefaultPoolRatioThreshold;	///< 0.0f to 1.0f value: The percentage of occupied memory where the sound engine should enter in Low memory mode. \ref soundengine_initialization_advanced_soundengine_using_memory_threshold

	// Voices.
	AkUInt16            uNumRefillsInVoice;		///< Number of refill buffers in voice buffer. 2 == double-buffered, defaults to 4.
	AkSoundQuality		eAudioQuality;			///< Quality of audio processing, default = AkSoundQuality_High.
	
	AkChannelMask		uChannelMask;			///< One of AK_SPEAKER_SETUP_5POINT1, AK_SPEAKER_SETUP_STEREO or AK_INVALID_CHANNELMASK 
												///< (AK_INVALID_CHANNELMASK  will use the speaker setup choosen in windows control panel). 
												///< Note: Setting a valid channel mask might not be possible in terms of the number of channel of the soundcard, if this is the case downmixing will be happen.

	bool				bGlobalFocus;			///< Corresponding to DSBCAPS_GLOBALFOCUS. Sounds will be muted if set to false when the game loses the focus.

	AkThreadProperties  threadMonitor;			///< Monitor threading properties (its default priority is AK_THREAD_PRIORITY_ABOVENORMAL). This parameter is not used in Release build.
};

struct IDirectSound8;

namespace AK
{
	/// Get instance of DirectSound created by the sound engine at initialization.
	/// \return Non-addref'd pointer to DirectSound interface. NULL if sound engine is not initialized.
	AK_EXTERNFUNC( IDirectSound8 *, GetDirectSoundInstance )();
};

#endif //_AK_WIN_SOUND_ENGINE_H_
