//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

/// \file 
/// Audiokinetic platform checks. This is where we detect which platform
/// is being compiled, and where we define the corresponding AK-specific
/// symbols.

#ifndef __AKPLATFORMS_H__
#define __AKPLATFORMS_H__

#if defined( NN_PLATFORM_CTR )

	#define AK_3DS										///< Compiling for 3DS

#elif _XBOX_VER >= 200
 	// Check Xbox before WIN32 because WIN32 might also be defined in some cases in Xbox 360 projects
	
	#define AK_XBOX360									///< Compiling for Xbox 360

#elif defined( WIN32 ) || defined ( WIN64 )

	#define AK_WIN										///< Compiling for Windows

	#if defined (WIN32) && defined (_M_IX86)
		#define AK_CPU_X86								///< Compiling for 32-bit x86 CPU
	#elif defined (WIN64) && defined (_M_AMD64)
		#define AK_CPU_X86_64							///< Compiling for 64-bit x86 CPU
	#endif

#elif defined( __APPLE__ )

	#include <TargetConditionals.h>

	#define AK_APPLE									///< Compiling for an Apple platform

	#if TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR
		#define AK_IOS									///< Compiling for iOS (iPhone, iPad, iPod...)
		#define AK_CPU_ARM								///< Compiling for ARM CPU
		#if defined __ARM_NEON__
			#define AK_CPU_ARM_NEON						///< Compiling for ARM CPU with Neon
		#endif 
	#elif TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
		#define AK_IOS									///< Compiling for iOS (iPhone, iPad, iPod...)
		#define AK_CPU_X86								///< Compiling for 32-bit x86 CPU
	#elif !TARGET_OS_EMBEDDED
		#define AK_MAC_OS_X								///< Compiling for Mac OS X
		#if  TARGET_CPU_X86_64
			#define AK_CPU_X86_64						///< Compiling for 64-bit x86 CPU
		#elif TARGET_CPU_X86
			#define AK_CPU_X86							///< Compiling for 32-bit x86 CPU
		#endif
	#endif

#elif defined (__PPU__) || defined (__SPU__)

	#define AK_PS3		///< Compiling for PS3

#elif defined( CAFE )	//Must be before RVL_OS, because the WiiU also uses RVL_OS

	#define AK_WIIU										///< Compiling for WiiU
	#define AK_WII_FAMILY	

#elif defined( RVL_OS )

	#define AK_WII										///< Compiling for Wii
	#define AK_WII_FAMILY

#elif defined( __SCE__ ) && defined( __arm__ )

	#define AK_NGP										///< Compiling for NGP

#elif defined( __ANDROID__ )

	#define AK_ANDROID

#elif defined( __native_client__ )

	#define AK_NACL
	#define AK_CPU_X86									///< Compiling for 32-bit x86 CPU

#else

	#error Unsupported platform, or platform-specific symbols not defined

#endif

//Define AK_WIIU_SOFTWARE to run the WiiU in software mode
#if defined AK_WII || defined AK_WIIU && !defined AK_WIIU_SOFTWARE
#define AK_WII_FAMILY_HW 
#endif

#endif // __AKPLATFORMS_H__
