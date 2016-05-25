//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

/// \file 
/// The main communication interface (between the in-game sound engine and
/// authoring tool).
/// \sa
/// - \ref initialization_comm
/// - \ref termination_comm

#ifndef _AK_COMMUNICATION_H
#define _AK_COMMUNICATION_H

#include "../SoundEngine/Common/AkTypes.h"
#include "../SoundEngine/Common/AkMemoryMgr.h"
#include "../Tools/Common/AkPlatformFuncs.h"

/// Platform-independent initialization settings of communication module between the Wwise sound engine
/// and authoring tool.
/// \sa 
/// - AK::Comm::Init()
struct AkCommSettings
{
	AkUInt32	uPoolSize;		///< Size of the communication pool, in bytes. 
	AkThreadProperties threadProperties; ///< Communication & Connection threading properties (its default priority is AK_THREAD_PRIORITY_ABOVENORMAL)

	/// Ports used for communication between the Wwise authoring application and your game.
	/// All of these ports are opened in the game when Wwise communication is enabled.
	/// When using HIO type communication, the ports are in fact channels and they must be 4 
	/// consecutives channels in the order they are defined in the Port structure.
	///
	/// \sa
	/// - \ref initialization_comm_ports
	/// - AK::Comm::GetDefaultInitSettings()
	/// - AK::Comm::Init()
	struct Ports
	{
		/// Constructor
		Ports()
		{
#if defined(AK_3DS)
			//Default port is arbitrarily set to 8, letting the first ports for game.

			uDiscoveryBroadcast	= 8;

			uCommand			= uDiscoveryBroadcast+1;
			uNotification		= uDiscoveryBroadcast+2;
			uControl			= uDiscoveryBroadcast+3;
#else
			uDiscoveryBroadcast	= 42042;

	#if defined( AK_WII ) || defined ( AK_WIIU )
				// Dynamic/ephemeral ports are not supported on the Wii
				uCommand		= 42043;
				uNotification	= 42044;
				uControl		= 42045;
	#else // defined( AK_WII )
				// All other platforms: 0 = Dynamic/Ephemeral ports
				uCommand		= 0;
				uNotification	= 0;
				uControl		= 0;
	#endif // defined( AK_WII )

#endif
		}

		/// This is where the authoring application broadcasts "Game Discovery" requests
		/// to discover games running on the network. Default value: 42042.
		///
		/// \warning Unlike the other ports in this structure, this port cannot be dynamic
		///          (cannot be set to 0). Refer to \ref initialization_comm_ports_discovery_broadcast
		///          for more details.
		AkUInt16 uDiscoveryBroadcast;

		/// Used by the "command" channel. Default value: 0 (dynamic) on most platforms; 42043 on the Wii.
		///
		/// \remark Set to 0 to request a dynamic/ephemeral port.
		AkUInt16 uCommand;

		/// Used by the "notification" channel. Default value: 0 (dynamic) on most platforms; 42044 on the Wii.
		///
		/// \remark Set to 0 to request a dynamic/ephemeral port.
		AkUInt16 uNotification;

		/// Used by the "control" channel. Default value: 0 (dynamic) on most platforms; 42045 on the Wii.
		///
		/// \remark Set to 0 to request a dynamic/ephemeral port.
		AkUInt16 uControl;
	};

	/// Ports used for communication between the Wwise authoring application and your game.
	/// \sa
	/// - \ref initialization_comm
	/// - AkCommSettings::Ports
	/// - AK::Comm::Init()
	Ports ports;	

	///< Tells if the base console communication library should be initialized.  
	///< If set to false, the game should load/initialize the console's communication library prior to calling this function.
	///< Set to false only if your game already use sockets before the sound engine initialization.
	///< Some consoles have critical requirements for initialization, see \ref initialization_comm_console_lib
	bool bInitSystemLib;		
};

namespace AK
{
	namespace Comm
	{
		///////////////////////////////////////////////////////////////////////
		/// @name Initialization
		//@{

		/// Initializes the communication module. When this is called, and AK::SoundEngine::RenderAudio()
		/// is called periodically, you may use the authoring tool to connect to the sound engine.
		///
		/// \warning This function must be called after the sound engine and memory manager have
		///          been properly initialized.
		///
		///
		/// \remark The AkCommSettings structure should be initialized with
		///         AK::Comm::GetDefaultInitSettings(). You can then change some of the parameters
		///			before calling this function.
		///
		/// \return
		///      - AK_Success if initialization was successful.
		///      - AK_InvalidParameter if one of the settings is invalid.
		///      - AK_InsufficientMemory if the specified pool size is too small for initialization.
		///      - AK_Fail for other errors.
		///		
		/// \sa
		/// - \ref initialization_comm
		/// - AK::Comm::GetDefaultInitSettings()
		/// - AkCommSettings::Ports
        AK_EXTERNAPIFUNC( AKRESULT, Init )(
			const AkCommSettings &	in_settings///< Initialization settings.			
			);

		/// Gets the communication module's default initialization settings values.
		/// \sa
		/// - \ref initialization_comm 
		/// - AK::Comm::Init()
		AK_EXTERNAPIFUNC( void, GetDefaultInitSettings )(
            AkCommSettings &	out_settings	///< Returned default initialization settings.
		    );
		
		/// Terminates the communication module.
		/// \warning This function must be called before the memory manager is terminated.		
		/// \sa
		/// - \ref termination_comm 
        AK_EXTERNAPIFUNC( void, Term )();

        //@}
	}
}

#endif // _AK_COMMUNICATION_H
