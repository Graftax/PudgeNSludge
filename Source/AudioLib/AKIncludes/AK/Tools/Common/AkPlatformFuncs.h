//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

// AkPlatformFuncs.h

/// \file 
/// Platform-dependent functions definition.

#ifndef _AK_TOOLS_COMMON_AKPLATFORMFUNCS_H
#define _AK_TOOLS_COMMON_AKPLATFORMFUNCS_H

#include "../../SoundEngine/Common/AkTypes.h"

#ifdef AK_WIN
#include "../Win32/AkPlatformFuncs.h"

#elif defined (AK_XBOX360)
#include <AK/Tools/XBox360/AkPlatformFuncs.h>

#elif defined (AK_PS3)
#include <AK/Tools/PS3/AkPlatformFuncs.h>

#elif defined (AK_WII_FAMILY)
#include <AK/Tools/Wii/AkPlatformFuncs.h>

#elif defined (AK_APPLE)
#include <AK/Tools/Mac/AkPlatformFuncs.h>
#include <AK/Tools/POSIX/AkPlatformFuncs.h>

#elif defined (AK_NGP)
#include <AK/Tools/NGP/AkPlatformFuncs.h>

#elif defined (AK_3DS)
#include <AK/Tools/3DS/AkPlatformFuncs.h>

#elif defined (AK_ANDROID)
#include <AK/Tools/Android/AkPlatformFuncs.h>
#include <AK/Tools/POSIX/AkPlatformFuncs.h>

#elif defined (AK_NACL)
#include <AK/Tools/nacl/AkPlatformFuncs.h>
#include <AK/Tools/POSIX/AkPlatformFuncs.h>

#else
#error AkPlatformFuncs.h: Undefined platform
#endif

#ifndef AK_PERF_RECORDING_RESET
#define AK_PERF_RECORDING_RESET()
#endif
#ifndef AK_PERF_RECORDING_START
#define AK_PERF_RECORDING_START( __StorageName__, __uExecutionCountStart__, __uExecutionCountStop__ )
#endif
#ifndef AK_PERF_RECORDING_STOP
#define AK_PERF_RECORDING_STOP( __StorageName__, __uExecutionCountStart__, __uExecutionCountStop__ )	
#endif

#endif // _AK_TOOLS_COMMON_AKPLATFORMFUNCS_H
