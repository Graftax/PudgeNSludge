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

#ifndef _AKFXPARAMETERCHANGEHANDLER_H_
#define _AKFXPARAMETERCHANGEHANDLER_H_

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkAssert.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

/// Can be used to track individual parameter changes to avoid costly computations when they remain constant
/// This class is designed to use only the lower bit information of the parameter IDs
namespace AK
{

	template <AkUInt32 T_MAXNUMPARAMS> 
	class AkFXParameterChangeHandler
	{
	public:

		inline AkFXParameterChangeHandler()
		{
			ResetAllParamChanges( );
		}

		inline void SetParamChange( AkPluginParamID in_ID )
		{
			AKASSERT( in_ID <= T_MAXNUMPARAMS );
			const AkUInt32 uByteIndex = in_ID/8;
			const AkUInt32 uBitMask = 1<<(in_ID-uByteIndex*8);
			m_uParamBitArray[uByteIndex] |= uBitMask;
		}

		inline bool HasChanged( AkPluginParamID in_ID )
		{
			AKASSERT( in_ID <= T_MAXNUMPARAMS );
			const AkUInt32 uByteIndex = in_ID/8;
			const AkUInt32 uBitMask = 1<<(in_ID-uByteIndex*8);
			return (m_uParamBitArray[uByteIndex] & uBitMask) > 0;
		}

		inline bool HasAnyChanged()
		{
			AkUInt32 uByteIndex = 0;
			do
			{
				if ( m_uParamBitArray[uByteIndex] > 0 )
					return true;
				++uByteIndex;
			} while (uByteIndex < (((T_MAXNUMPARAMS) + ((8)-1)) & ~((8)-1))/8 );
			return false;
		}

		inline void ResetParamChange( AkPluginParamID in_ID )
		{
			AKASSERT( in_ID <= T_MAXNUMPARAMS );
			const AkUInt32 uByteIndex = in_ID/8;
			const AkUInt32 uBitMask = 1<<(in_ID-uByteIndex*8);
			m_uParamBitArray[uByteIndex] &= ~uBitMask;
		}

		inline void ResetAllParamChanges( )
		{
			AKPLATFORM::AkMemSet( m_uParamBitArray, 0, sizeof(m_uParamBitArray) );
		}

		inline void SetAllParamChanges( )
		{
			AKPLATFORM::AkMemSet( m_uParamBitArray, 0xFF, sizeof(m_uParamBitArray) );
		}	

	protected:

		// Minimum storage in bits aligned to next byte boundary
		AkUInt8 m_uParamBitArray[(((T_MAXNUMPARAMS) + ((8)-1)) & ~((8)-1))/8]; 

	};

} // namespace AK

#endif // _AKFXPARAMETERCHANGEHANDLER_H_
