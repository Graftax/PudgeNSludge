//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

// AkTypes.h

/// \file 
/// Data type definitions.

#ifndef _AK_DATA_TYPES_PLATFORM_H_
#define _AK_DATA_TYPES_PLATFORM_H_

#define AK_MOTION								///< Internal use

#define AK_LFECENTER							///< Internal use
#define AK_REARCHANNELS							///< Internal use

#define AK_SUPPORT_WCHAR						///< Can support wchar
#define AK_OS_WCHAR								///< Use wchar natively

#define AK_RESTRICT		__restrict				///< Refers to the __restrict compilation flag available on some platforms
#define AK_EXPECT_FALSE( _x )	(_x)
#define AkForceInline	__forceinline			///< Force inlining
#define AkNoInline		__declspec(noinline)	///< Disable inlining

#define AK_SIMD_ALIGNMENT	16					///< Platform-specific alignment requirement for SIMD data
#define AK_ALIGN_SIMD( __Declaration__ ) __declspec(align(AK_SIMD_ALIGNMENT)) __Declaration__ ///< Platform-specific alignment requirement for SIMD data
#define AK_ALIGN_DMA							///< Platform-specific data alignment for DMA transfers
#define AK_ALIGN_FASTDMA 						///< Platform-specific data alignment for faster DMA transfers
#define AK_ALIGN_SIZE_FOR_DMA( __Size__ ) (__Size__) ///< Used to align sizes to next 16 byte boundary on platfroms that require it

typedef unsigned char		AkUInt8;			///< Unsigned 8-bit integer
typedef unsigned short		AkUInt16;			///< Unsigned 16-bit integer
typedef unsigned long		AkUInt32;			///< Unsigned 32-bit integer
typedef unsigned __int64	AkUInt64;			///< Unsigned 64-bit integer

#if defined(_WIN64)
typedef __int64 AkIntPtr;						///< Integer type for pointers
typedef unsigned __int64 AkUIntPtr;				///< Integer (unsigned) type for pointers
#else
typedef __w64 int AkIntPtr;						///< Integer type for pointers
typedef __w64 unsigned int AkUIntPtr;			///< Integer (unsigned) type for pointers
#endif


typedef char			AkInt8;					///< Signed 8-bit integer
typedef short			AkInt16;				///< Signed 16-bit integer
typedef long   			AkInt32;				///< Signed 32-bit integer
typedef int 			AkInt;					///< Signed integer
typedef __int64			AkInt64;				///< Signed 64-bit integer

typedef wchar_t			AkOSChar;				///< Generic character string

typedef float			AkReal32;				///< 32-bit floating point
typedef double          AkReal64;				///< 64-bit floating point

typedef void *					AkThread;		///< Thread handle
typedef AkUInt32				AkThreadID;		///< Thread ID
typedef AkUInt32 (__stdcall *AkThreadRoutine)(	void* lpThreadParameter	); ///< Thread routine
typedef void *					AkEvent;		///< Event handle
typedef void *					AkFileHandle;	///< File handle
typedef wchar_t			AkUtf16;				///< Type for 2 byte chars. Used for communication
												///< with the authoring tool.
#define AK_UINT_MAX		UINT_MAX

// For strings.
#define AK_MAX_PATH     260						///< Maximum path length.

typedef AkUInt32			AkFourcc;			///< Riff chunk

/// Create Riff chunk
#define AkmmioFOURCC( ch0, ch1, ch2, ch3 )									    \
		( (AkFourcc)(AkUInt8)(ch0) | ( (AkFourcc)(AkUInt8)(ch1) << 8 ) |		\
		( (AkFourcc)(AkUInt8)(ch2) << 16 ) | ( (AkFourcc)(AkUInt8)(ch3) << 24 ) )

#define AK_BANK_PLATFORM_DATA_ALIGNMENT	(16)	///< Required memory alignment for bank loading by memory address (see LoadBank())
#define AK_BANK_PLATFORM_ALLOC_TYPE		AkMalloc

/// Macro that takes a string litteral and changes it to an AkOSChar string at compile time
/// \remark This is similar to the TEXT() and _T() macros that can be used to turn string litterals into wchar_t strings
/// \remark Usage: AKTEXT( "Some Text" )
#define AKTEXT(x) L ## x

#endif //_AK_DATA_TYPES_PLATFORM_H_

