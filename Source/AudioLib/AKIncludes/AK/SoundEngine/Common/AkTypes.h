//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

// AkTypes.h

/// \file 
/// Data type definitions.

#ifndef _AK_DATA_TYPES_H_
#define _AK_DATA_TYPES_H_

// Platform-specific section.
//----------------------------------------------------------------------------------------------------
#include "../../AkPlatforms.h"
#if defined( AK_3DS )
	#include <AK/SoundEngine/Platforms/3DS/AkTypes.h>
#elif defined( AK_XBOX360 )
	#include <AK/SoundEngine/Platforms/XBox360/AkTypes.h>
#elif defined( AK_WIN )
	#include "../Platforms/Windows/AkTypes.h"
#elif defined( AK_APPLE )
	#include "../Platforms/Mac/AkTypes.h"
#elif defined ( AK_PS3 )
	#include <AK/SoundEngine/Platforms/PS3/AkTypes.h>
#elif defined( AK_WII_FAMILY )
	#include <AK/SoundEngine/Platforms/WiiFamily/AkTypes.h>
#elif defined( AK_NGP )
	#include <AK/SoundEngine/Platforms/NGP/AkTypes.h>
#elif defined( AK_ANDROID )
	#include <AK/SoundEngine/Platforms/Android/AkTypes.h>
#elif defined( AK_NACL )
	#include <AK/SoundEngine/Platforms/nacl/AkTypes.h>
#else
	#error Unsupported platform, or platform-specific symbols not defined
#endif

//----------------------------------------------------------------------------------------------------

#include "../Common/AkSoundEngineExport.h"

typedef AkUInt32		AkUniqueID;			 		///< Unique 32-bit ID
typedef AkUInt32		AkStateID;			 		///< State ID
typedef AkUInt32		AkStateGroupID;		 		///< State group ID
typedef AkUInt32		AkPlayingID;		 		///< Playing ID
typedef	AkInt32			AkTimeMs;			 		///< Time in ms
typedef AkReal32		AkPitchValue;		 		///< Pitch value
typedef AkReal32		AkVolumeValue;		 		///< Volume value( also apply to LFE )
typedef AkUIntPtr		AkGameObjectID;		 		///< Game object ID
typedef AkReal32		AkLPFType;			 		///< Low-pass filter type
typedef AkInt32			AkMemPoolId;		 		///< Memory pool ID
typedef AkUInt32		AkPluginID;			 		///< Source or effect plug-in ID
typedef AkUInt32		AkCodecID;			 		///< Codec plug-in ID
typedef AkUInt32		AkEnvID;			 		///< Environmental ID
typedef AkInt16			AkPluginParamID;	 		///< Source or effect plug-in parameter ID
typedef AkInt8			AkPriority;			 		///< Priority
typedef AkUInt16        AkDataCompID;		 		///< Data compression format ID
typedef AkUInt16        AkDataTypeID;		 		///< Data sample type ID
typedef AkUInt8			AkDataInterleaveID;	 		///< Data interleaved state ID
typedef AkUInt32        AkSwitchGroupID;	 		///< Switch group ID
typedef AkUInt32        AkSwitchStateID;	 		///< Switch ID
typedef AkUInt32        AkRtpcID;			 		///< Real time parameter control ID
typedef AkReal32        AkRtpcValue;		 		///< Real time parameter control value
typedef AkUInt32        AkBankID;			 		///< Run time bank ID
typedef AkUInt32        AkFileID;			 		///< Integer-type file identifier
typedef AkUInt32        AkDeviceID;			 		///< I/O device ID
typedef AkUInt32		AkTriggerID;		 		///< Trigger ID
typedef AkUInt32		AkArgumentValueID;			///< Argument value ID
typedef AkUInt32		AkChannelMask;				///< Channel mask (similar to WAVE_FORMAT_EXTENSIBLE). Bit values are defined in AkSpeakerConfig.h.

// Constants.
static const AkPluginID					AK_INVALID_PLUGINID					= (AkPluginID)-1;		///< Invalid FX ID
static const AkGameObjectID				AK_INVALID_GAME_OBJECT				= (AkGameObjectID)-1;	///< Invalid game object (may also mean all game objects)
static const AkUniqueID					AK_INVALID_UNIQUE_ID				=  0;					///< Invalid unique 32-bit ID
static const AkRtpcID					AK_INVALID_RTPC_ID					=  AK_INVALID_UNIQUE_ID;///< Invalid RTPC ID
static const AkUInt32					AK_INVALID_LISTENER_INDEX			= (AkUInt32)-1;			///< Invalid listener index
static const AkPlayingID				AK_INVALID_PLAYING_ID				=  AK_INVALID_UNIQUE_ID;///< Invalid playing ID
static const AkUInt32					AK_DEFAULT_SWITCH_STATE				=  0;					///< Switch selected if no switch has been set yet
static const AkMemPoolId				AK_INVALID_POOL_ID					= -1;					///< Invalid pool ID
static const AkMemPoolId				AK_DEFAULT_POOL_ID					= -1;					///< Default pool ID, same as AK_INVALID_POOL_ID
static const AkEnvID					AK_INVALID_ENV_ID					=  AK_INVALID_UNIQUE_ID;///< Invalid environment ID (or no environment ID)
static const AkFileID					AK_INVALID_FILE_ID					= (AkFileID)-1;			///< Invalid file ID
static const AkDeviceID					AK_INVALID_DEVICE_ID				= (AkDeviceID)-1;		///< Invalid device ID
static const AkBankID					AK_INVALID_BANK_ID					=  AK_INVALID_UNIQUE_ID;///< Invalid bank ID
static const AkArgumentValueID			AK_FALLBACK_ARGUMENTVALUE_ID		=  0;					///< Fallback argument value ID
static const AkChannelMask				AK_INVALID_CHANNELMASK				=  0;					///< Invalid channel mask


// Priority.
static const AkPriority        AK_DEFAULT_PRIORITY          = 50;   ///< Default sound / I/O priority
static const AkPriority        AK_MIN_PRIORITY		        = 0;    ///< Minimal priority value [0,100]
static const AkPriority        AK_MAX_PRIORITY		        = 100;  ///< Maximal priority value [0,100]

// Default bank I/O settings.
static const AkPriority     AK_DEFAULT_BANK_IO_PRIORITY = AK_DEFAULT_PRIORITY; 	///<  Default bank load I/O priority
static const AkReal32       AK_DEFAULT_BANK_THROUGHPUT  = 1*1024*1024/1000.f;     	///<  Default bank load throughput (1 Mb/ms)

/// Standard function call result.
enum AKRESULT
{
    AK_NotImplemented			= 0,	///< This feature is not implemented.
    AK_Success					= 1,	///< The operation was successful.
    AK_Fail						= 2,	///< The operation failed.
    AK_PartialSuccess			= 3,	///< The operation succeeded partially.
    AK_NotCompatible			= 4,	///< Incompatible formats
    AK_AlreadyConnected			= 5,	///< The stream is already connected to another node.
    AK_NameNotSet				= 6,	///< Trying to open a file when its name was not set
    AK_InvalidFile				= 7,	///< An unexpected value causes the file to be invalid.
    AK_CorruptedFile			= 8,	///< The file is missing some exprected data.
    AK_MaxReached				= 9,	///< The maximum was reached.
    AK_InputsInUsed				= 10,	///< Inputs are currently used.
    AK_OutputsInUsed			= 11,	///< Outputs are currently used.
    AK_InvalidName				= 12,	///< The name is invalid.
    AK_NameAlreadyInUse			= 13,	///< The name is already in use.
    AK_InvalidID				= 14,	///< The ID is invalid.
    AK_IDNotFound				= 15,	///< The ID was not found.
    AK_InvalidInstanceID		= 16,	///< The InstanceID is invalid.
    AK_NoMoreData				= 17,	///< No more data is available from the source.
    AK_NoSourceAvailable		= 18,	///< There is no child (source) associated with the node.
	AK_StateGroupAlreadyExists	= 19,	///< The StateGroup already exists.
	AK_InvalidStateGroup		= 20,	///< The StateGroup is not a valid channel.
	AK_ChildAlreadyHasAParent	= 21,	///< The child already has a parent.
	AK_InvalidLanguage			= 22,	///< The language is invalid (applies to the Low-Level I/O).
	AK_CannotAddItseflAsAChild	= 23,	///< It is not possible to add itself as its own child.
	AK_TransitionNotFound		= 24,	///< The transition is not in the list.
	AK_TransitionNotStartable	= 25,	///< Start allowed in the Running and Done states.
	AK_TransitionNotRemovable	= 26,	///< Must not be in the Computing state.
	AK_UsersListFull			= 27,	///< No one can be added any more, could be AK_MaxReached.
	AK_UserAlreadyInList		= 28,	///< This user is already there.
	AK_UserNotInList			= 29,	///< This user is not there.
	AK_NoTransitionPoint		= 30,	///< Not in use.
	AK_InvalidParameter			= 31,	///< Something is not within bounds.
	AK_ParameterAdjusted		= 32,	///< Something was not within bounds and was relocated to the nearest OK value.
	AK_IsA3DSound				= 33,	///< The sound has 3D parameters.
	AK_NotA3DSound				= 34,	///< The sound does not have 3D parameters.
	AK_ElementAlreadyInList		= 35,	///< The item could not be added because it was already in the list.
	AK_PathNotFound				= 36,	///< This path is not known.
	AK_PathNoVertices			= 37,	///< Stuff in vertices before trying to start it
	AK_PathNotRunning			= 38,	///< Only a running path can be paused.
	AK_PathNotPaused			= 39,	///< Only a paused path can be resumed.
	AK_PathNodeAlreadyInList	= 40,	///< This path is already there.
	AK_PathNodeNotInList		= 41,	///< This path is not there.
	AK_VoiceNotFound			= 42,	///< Unknown in our voices list
	AK_DataNeeded				= 43,	///< The consumer needs more.
	AK_NoDataNeeded				= 44,	///< The consumer does not need more.
	AK_DataReady				= 45,	///< The provider has available data.
	AK_NoDataReady				= 46,	///< The provider does not have available data.
	AK_NoMoreSlotAvailable		= 47,	///< Not enough space to load bank.
	AK_SlotNotFound				= 48,	///< Bank error.
	AK_ProcessingOnly			= 49,	///< No need to fetch new data.
	AK_MemoryLeak				= 50,	///< Debug mode only.
	AK_CorruptedBlockList		= 51,	///< The memory manager's block list has been corrupted.
	AK_InsufficientMemory		= 52,	///< Memory error.
	AK_Cancelled				= 53,	///< The requested action was cancelled (not an error).
	AK_UnknownBankID			= 54,	///< Trying to load a bank using an ID which is not defined.
    AK_IsProcessing             = 55,   ///< Asynchronous pipeline component is processing.
	AK_BankReadError			= 56,	///< Error while reading a bank.
	AK_InvalidSwitchType		= 57,	///< Invalid switch type (used with the switch container)
	AK_VoiceDone				= 58,	///< Internal use only.
	AK_UnknownEnvironment		= 59,	///< This environment is not defined.
	AK_EnvironmentInUse			= 60,	///< This environment is used by an object.
	AK_UnknownObject			= 61,	///< This object is not defined.
	AK_NoConversionNeeded		= 62,	///< Audio data already in target format, no conversion to perform.
    AK_FormatNotReady           = 63,   ///< Source format not known yet.
	AK_WrongBankVersion			= 64,	///< The bank version is not compatible with the current bank reader.
	AK_DataReadyNoProcess		= 65,	///< The provider has some data but does not process it (virtual voices).
    AK_FileNotFound             = 66,   ///< File not found.
    AK_DeviceNotReady           = 67,   ///< IO device not ready (may be because the tray is open)
    AK_CouldNotCreateSecBuffer	= 68,   ///< The direct sound secondary buffer creation failed.
	AK_BankAlreadyLoaded		= 69,	///< The bank load failed because the bank is already loaded.
	AK_RenderedFX				= 71,	///< The effect on the node is rendered.
	AK_ProcessNeeded			= 72,	///< A routine needs to be executed on some CPU.
	AK_ProcessDone				= 73,	///< The executed routine has finished its execution.
	AK_MemManagerNotInitialized	= 74,	///< The memory manager should have been initialized at this point.
	AK_StreamMgrNotInitialized	= 75,	///< The stream manager should have been initialized at this point.
	AK_SSEInstructionsNotSupported = 76,///< The machine does not support SSE instructions (required on PC).
	AK_Busy						= 77,	///< The system is busy and could not process the request.
	AK_UnsupportedChannelConfig = 78,	///< Channel configuration is not supported in the current execution context.
	AK_PluginMediaNotAvailable  = 79,	///< Plugin media is not available for effect.
	AK_MustBeVirtualized		= 80,	///< Sound was Not Allowed to play.
	AK_CommandTooLarge			= 81	///< SDK command is too large to fit in the command queue.
};

/// Game sync group type
enum AkGroupType
{
	// should stay set as Switch = 0 and State = 1
	AkGroupType_Switch	= 0, ///< Type switch
	AkGroupType_State	= 1  ///< Type state
};

/// This structure allows the game to provide audio files to fill the external sources. See \ref AK::SoundEngine::PostEvent
/// You can specify a streaming file or a file in-memory, regardless of the "Stream" option in the Wwise project.  
/// \akwarning
/// Make sure that only one of szFile, pInMemory or idFile is non-null.  The other 2 must be null.
/// \endakwarning
/// \if Wii \aknote On the Wii, only mono files can be played from memory. \endaknote \endif
struct AkExternalSourceInfo
{
	AkUInt32 iExternalSrcCookie;	///< Cookie identifying the source, given by hashing the name of the source given in the project.  See \ref AK::SoundEngine::GetIDFromString. \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures. \endaknote
	AkCodecID idCodec;				///< Codec ID for the file.  One of the audio formats defined in AkTypes.h (AKCODECID_XXX)
	AkOSChar * szFile;				///< File path for the source.  If not NULL, the source will be streaming from disk.  Set pInMemory and idFile to NULL.
	void* pInMemory;				///< Pointer to the in-memory file.  If not NULL, the source will be read from memory.  Set szFile and idFile to NULL.
	AkUInt32 uiMemorySize;			///< Size of the data pointed by pInMemory
	AkFileID idFile;				///< File ID.  If not zero, the source will be streaming from disk.  This ID can be anything.  Note that you must override the low-level IO to resolve this ID to a real file.  See \ref streamingmanager_lowlevel for more information on overriding the Low Level IO.

	/// Default constructor.
	AkExternalSourceInfo()
		: iExternalSrcCookie( 0 )
		, idCodec( 0 )
		, szFile( 0 )
		, pInMemory( 0 )
		, uiMemorySize( 0 )
		, idFile( 0 ) {}

	/// Constructor: specify source by memory.
	AkExternalSourceInfo( 
		void* in_pInMemory,				///< Pointer to the in-memory file.
		AkUInt32 in_uiMemorySize,		///< Size of data.
		AkUInt32 in_iExternalSrcCookie,	///< Cookie.
		AkCodecID in_idCodec			///< Codec ID.
		)
		: iExternalSrcCookie( in_iExternalSrcCookie )
		, idCodec( in_idCodec )
		, szFile( 0 )
		, pInMemory( in_pInMemory )
		, uiMemorySize( in_uiMemorySize )
		, idFile( 0 ) {}

	/// Constructor: specify source by streaming file name.
	AkExternalSourceInfo( 
		AkOSChar * in_pszFileName,		///< File name.
		AkUInt32 in_iExternalSrcCookie,	///< Cookie.
		AkCodecID in_idCodec			///< Codec ID.
		)
		: iExternalSrcCookie( in_iExternalSrcCookie )
		, idCodec( in_idCodec )
		, szFile( in_pszFileName )
		, pInMemory( 0 )
		, uiMemorySize( 0 )
		, idFile( 0 ) {}

	/// Constructor: specify source by streaming file ID.
	AkExternalSourceInfo( 
		AkFileID in_idFile,				///< File ID.
		AkUInt32 in_iExternalSrcCookie,	///< Cookie.
		AkCodecID in_idCodec			///< Codec ID.
		)
		: iExternalSrcCookie( in_iExternalSrcCookie )
		, idCodec( in_idCodec )
		, szFile( 0 )
		, pInMemory( 0 )
		, uiMemorySize( 0 )
		, idFile( in_idFile ) {}
};

/// 3D vector.
struct AkVector
{
    AkReal32		X;	///< X Position
    AkReal32		Y;	///< Y Position
    AkReal32		Z;	///< Z Position
};

/// Positioning information for a sound.
struct AkSoundPosition
{
	AkVector		Position;		///< Position of the sound
	AkVector		Orientation;	///< Orientation of the sound
};

/// Positioning information for a listener.
struct AkListenerPosition
{
	AkVector		OrientationFront;	///< Orientation of the listener
	AkVector		OrientationTop;		///< Top orientation of the listener
	AkVector		Position;			///< Position of the listener
};

/// Per-speaker volume information for up to 7.1 audio.
struct AkSpeakerVolumes
{
	AkReal32 fFrontLeft;				///< Front-Left volume
	AkReal32 fFrontRight;				///< Front-Right volume
#ifdef AK_LFECENTER
	AkReal32 fCenter;					///< Center volume
#endif // AK_LFECENTER
#ifdef AK_REARCHANNELS
	AkReal32 fRearLeft;					///< Rear-Left volume
	AkReal32 fRearRight;				///< Rear-Right volume
#endif // AK_REARCHANNELS
#ifdef AK_71AUDIO
	AkReal32 fExtraLeft;				///< Extra-Left volume
	AkReal32 fExtraRight;				///< Extra-Right volume
#endif // AK_71AUDIO
#ifdef AK_LFECENTER
	AkReal32 fLfe;						///< LFE volume
#endif // AK_LFECENTER
};

/// Curve interpolation types
enum AkCurveInterpolation
{
//DONT GO BEYOND 15! (see below for details)
//Curves from 0 to LastFadeCurve NEED TO BE A MIRROR IMAGE AROUND LINEAR (eg. Log3 is the inverse of Exp3)
    AkCurveInterpolation_Log3			= 0, ///< Log3
    AkCurveInterpolation_Sine			= 1, ///< Sine
    AkCurveInterpolation_Log1			= 2, ///< Log1
    AkCurveInterpolation_InvSCurve		= 3, ///< Inversed S Curve
    AkCurveInterpolation_Linear			= 4, ///< Linear (Default)
    AkCurveInterpolation_SCurve			= 5, ///< S Curve
    AkCurveInterpolation_Exp1			= 6, ///< Exp1
    AkCurveInterpolation_SineRecip		= 7, ///< Reciprocal of sine curve
    AkCurveInterpolation_Exp3			= 8, ///< Exp3
	AkCurveInterpolation_LastFadeCurve  = 8, ///< Update this value to reflect last curve available for fades
	AkCurveInterpolation_Constant		= 9  ///< Constant ( not valid for fading values )
//DONT GO BEYOND 15! The value is stored on 5 bits,
//but we can use only 4 bits for the actual values, keeping
//the 5th bit at 0 to void problems when the value is
//expanded to 32 bits.
};
#define AKCURVEINTERPOLATION_NUM_STORAGE_BIT 5 ///< Internal storage restriction, for internal use only.

#ifndef AK_MAX_ENVIRONMENTS_PER_OBJ
	#define AK_MAX_ENVIRONMENTS_PER_OBJ			(4) ///< Maximum number of environments in which a single game object may be located at a given time.
#endif

#define AK_NUM_LISTENERS						(8)	///< Number of listeners that can be used.

/// Environmental information per game object per given environment.
struct AkEnvironmentValue
{
	AkEnvID EnvID;			///< Unique environment identifier
	AkReal32 fControlValue; ///< Value in the range [0.0f:1.0f], send level to environment	
};

// ---------------------------------------------------------------
// Languages
// ---------------------------------------------------------------
#define AK_MAX_LANGUAGE_NAME_SIZE	(32)

// ---------------------------------------------------------------
// File Type ID Definitions
// ---------------------------------------------------------------

// These correspond to IDs specified in the conversion plug-ins' XML
// files. Audio sources persist them to "remember" their format.
// DO NOT CHANGE THEM without talking to someone in charge of persistence!

// Vendor ID.
#define AKCOMPANYID_AUDIOKINETIC        (0)     ///< Audiokinetic inc.
#define AKCOMPANYID_AUDIOKINETIC_EXTERNAL (1)   ///< Audiokinetic inc.
#define AKCOMPANYID_MCDSP				(256)	///< McDSP
#define AKCOMPANYID_WAVEARTS			(257)	///< WaveArts
#define AKCOMPANYID_PHONETICARTS		(258)	///< Phonetic Arts
#define AKCOMPANYID_IZOTOPE				(259)	///< iZotope

// File/encoding types of Audiokinetic.
#define AKCODECID_BANK                  (0)		///< Bank encoding
#define AKCODECID_PCM                   (1)		///< PCM encoding
#define AKCODECID_ADPCM                 (2)		///< ADPCM encoding
#define AKCODECID_XMA                   (3)		///< XMA encoding
#define AKCODECID_VORBIS				(4)		///< Vorbis encoding
#define AKCODECID_WIIADPCM              (5)		///< ADPCM encoding on the Wii
#define AKCODECID_PCMEX					(7)		///< Standard PCM WAV file parser for Wwise Authoring
#define AKCODECID_EXTERNAL_SOURCE       (8)		///< External Source (unknown encoding)
#define AKCODECID_XWMA					(9)		///< xWMA encoding
#define AKCODECID_AAC					(10)	///< AAC encoding (only available on Apple devices) -- see AkAACFactory.h
#define AKCODECID_FILE_PACKAGE			(11)	///< File package files generated by the File Packager utility.
#define AKCODECID_ATRAC9				(12)	///< ATRAC-9 encoding
#define AKCODECID_VAG					(13)	///< VAG/HE-VAG encoding
#define AKCODECID_PROFILERCAPTURE		(14)	///< Profiler capture file (.prof) as written through AK::SoundEngine::StartProfilerCapture

class IAkSoftwareCodec;
/// Registered file source creation function prototype.
AK_CALLBACK( IAkSoftwareCodec*, AkCreateFileSourceCallback )( void* in_pCtx );
/// Registered bank source node creation function prototype.
AK_CALLBACK( IAkSoftwareCodec*, AkCreateBankSourceCallback )( void* in_pCtx );

//-----------------------------------------------------------------------------
// Positioning
//-----------------------------------------------------------------------------

/// 3D Positioning type.
enum AkPositioningType
{
	AkUndefined			= 0,	///< Not defined
	Ak2DPositioning		= 1,	///< 2D
	Ak3DUserDef			= 2,	///< 3D user-defined
	Ak3DGameDef			= 3		///< 3D game-defined
};

/// Headphone / speakers panning rules
enum AkPanningRule
{
	AkPanningRule_Speakers		= 0,	///< Left and right positioned 90 degrees apart
	AkPanningRule_Headphones 	= 1		///< Left and right positioned 180 degrees apart
};

#endif  //_AK_DATA_TYPES_H_
