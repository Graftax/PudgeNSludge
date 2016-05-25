//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

// AkSimd.h

/// \file 
/// AKSIMD - Windows implementation

#ifndef _AK_SIMD_PLATFORM_H_
#define _AK_SIMD_PLATFORM_H_

#include <AK/SoundEngine/Platforms/SSE/AkSimd.h>

/// Get the element at index __num__ in vector __vName
#define AKSIMD_GETELEMENT_V4F32( __vName, __num__ )			(__vName).m128_f32[(__num__)]				///< Retrieve scalar element from vector.

#endif //_AK_SIMD_PLATFORM_H_

