//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

// AkLock.h

/// \file 
/// Platform independent synchronization services for plug-ins.

#ifndef _AK_TOOLS_COMMON_AKLOCK_H
#define _AK_TOOLS_COMMON_AKLOCK_H

#include "../../AkPlatforms.h"

#ifdef AK_WIN
#include "../../Tools/Win32/AkLock.h"

#elif defined (AK_XBOX360)
#include "../../Tools/XBox360/AkLock.h"

#elif defined (AK_PS3)
#include "../../Tools/PS3/AkLock.h"

#elif defined (AK_WII_FAMILY)
#include "../../Tools/Wii/AkLock.h"

#elif defined (AK_APPLE) 
#include "../../Tools/POSIX/AkLock.h"

#elif defined (AK_NGP)
#include "../../Tools/NGP/AkLock.h"

#elif defined (AK_3DS)
#include "../../Tools/3DS/AkLock.h"

#elif defined (AK_ANDROID)
#include "../../Tools/POSIX/AkLock.h"

#elif defined (AK_NACL)
#include "../../Tools/POSIX/AkLock.h"

#else
#error AkLock.h: Undefined platform
#endif

#endif // _AK_TOOLS_COMMON_AKLOCK_H
