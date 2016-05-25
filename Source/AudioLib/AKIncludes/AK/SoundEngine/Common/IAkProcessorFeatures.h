/***********************************************************************
The content of this file includes source code for the sound engine
portion of the AUDIOKINETIC Wwise Technology and constitutes "Level
Two Source Code" as defined in the Source Code Addendum attached
with this file.  Any use of the Level Two Source Code shall be
subject to the terms and conditions outlined in the Source Code
Addendum and the End User License Agreement for Wwise(R).

Version: <VERSION>  Build: <BUILDNUMBER>
Copyright (c) <COPYRIGHTYEAR> Audiokinetic Inc.
***********************************************************************/

/// \file 
/// Runtime processor supported features detection interface.

#ifndef _IAK_PROCESSORFEATURES_H_
#define _IAK_PROCESSORFEATURES_H_

#include "AkTypes.h"

#if (defined(AK_CPU_X86_64) || defined(AK_CPU_X86)) && !defined AK_IOS

namespace AK
{   
	/// SIMD instruction sets.
	enum AkSIMDProcessorSupport
	{
		AK_SIMD_SSE = 1<<0,		///< SSE support.	
		AK_SIMD_SSE2 = 1<<1,	///< SSE2 support.
		AK_SIMD_SSE3 = 1<<2,	///< SSE3 support.
		AK_SIMD_SSSE3 = 1<<3	///< SSSE3 support.
	};

	/// Runtime processor supported features detection interface. Allows to query specific processor features
	/// to chose optimal implementation.
	/// \warning The functions in this interface are not thread-safe, unless stated otherwise.
	class IAkProcessorFeatures
	{
	protected:
		/// Virtual destructor on interface to avoid warnings.
		virtual ~IAkProcessorFeatures(){}

	public:
		/// Query for specific SIMD instruction set support. See AkSIMDProcessorSupport for options.
		virtual bool GetSIMDSupport(AkSIMDProcessorSupport in_eSIMD) = 0;
		/// Query L2 cache size to optimize prefetching techniques where necessary.
		virtual AkUInt32 GetCacheSize() = 0;
		/// Query cache line size to optimize prefetching techniques where necessary.
		virtual AkUInt32 GetCacheLineSize() = 0;
	};
}

#endif // (defined(AK_CPU_X86_64) || defined(AK_CPU_X86)) && !defined AK_IOS
#endif //_IAK_PROCESSORFEATURES_H_

