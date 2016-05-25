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

// Length of delay line is mapped on 4 frames boundary (i.e. may not be suited for reverberation for example)
// This is not a delay line implementation, but rather just some services for memory managment related 
// to specific delay line execution needs as detailed by clients

#ifndef _AKDSP_DELAYLINEMEMORY_
#define _AKDSP_DELAYLINEMEMORY_

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/IAkPluginMemAlloc.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#ifdef __SPU__
#include <AK/Plugin/PluginServices/PS3/SPUServices.h>
#endif

#define AK_ALIGN_TO_NEXT_BOUNDARY( __num__, __boundary__ ) (((__num__) + ((__boundary__)-1)) & ~((__boundary__)-1))

namespace AK
{
	namespace DSP
	{
		template < class T_SAMPLETYPE, AkUInt32 T_MAXNUMCHANNELS > 
		class CAkDelayLineMemory
		{
		public:

#ifndef __SPU__

			CAkDelayLineMemory( )
				: m_uDelayLineLength( 0 )
				, m_uOffset( 0 )
				, m_uNumChannels( 0 )
			{
				AKPLATFORM::AkMemSet( m_pDelay, 0, T_MAXNUMCHANNELS*sizeof(T_SAMPLETYPE *) );
			}

			AKRESULT Init( 
				AK::IAkPluginMemAlloc * in_pAllocator, 
				AkUInt32 in_uDelayLineLength,  
				AkUInt32 in_uNumChannels )
			{
				m_uNumChannels = in_uNumChannels;
				// Align delay length on 4 frame boundary to simplify DMA and SIMD alignement
				m_uDelayLineLength = AK_ALIGN_TO_NEXT_BOUNDARY( in_uDelayLineLength, 4 ); 
				m_uOffset = 0;
				if ( m_uDelayLineLength )
				{
					for ( AkUInt32 i = 0; i < m_uNumChannels; i++ )
					{
						m_pDelay[i] = (T_SAMPLETYPE*)AK_PLUGIN_ALLOC( in_pAllocator, AK_ALIGN_SIZE_FOR_DMA( sizeof(T_SAMPLETYPE) * m_uDelayLineLength ) );
						if ( m_pDelay[i] == NULL )
							return AK_InsufficientMemory;
					}
				}
				return AK_Success;
			}

			void Term( AK::IAkPluginMemAlloc * in_pAllocator )
			{
				for ( AkUInt32 i = 0; i < m_uNumChannels; i++ )
				{
					if ( m_pDelay[i] )
					{
						AK_PLUGIN_FREE( in_pAllocator, m_pDelay[i] );
						m_pDelay[i] = NULL;
					}
				}
				m_uDelayLineLength = 0;
			}

			void Reset( )
			{
				if ( m_uDelayLineLength )
				{
					for ( AkUInt32 i = 0; i < m_uNumChannels; i++ )
					{
						if (m_pDelay[i])
							AKPLATFORM::AkMemSet( (void*) m_pDelay[i], 0, m_uDelayLineLength*sizeof(T_SAMPLETYPE) );
					}
				}
				m_uOffset = 0;
			}

#endif	// #ifndef __SPU__	

			AkForceInline AkUInt32 GetCurrentOffset()
			{
				return m_uOffset;
			}

			AkForceInline void SetCurrentOffset( AkUInt32 in_uOffset )
			{
				m_uOffset = in_uOffset;
			}

			AkForceInline AkUInt32 GetDelayLength()
			{
				return m_uDelayLineLength;
			}

			T_SAMPLETYPE * GetCurrentPointer( AkUInt32 in_uOffset, AkUInt32 in_uChannelIndex )
			{
				return m_pDelay[in_uChannelIndex] + in_uOffset;
			}

#ifdef __SPU__
			// Use internal offset to unwrap delay lines onto SPU local storage
			void GetChannel( T_SAMPLETYPE * in_pLSStorage, AkUInt32 in_uNumFrames, AkUInt32 in_uChannelIndex, AkUInt32 in_uDMATag )
			{
				if ( in_uChannelIndex < m_uNumChannels  )
				{
					if ( m_uDelayLineLength <= in_uNumFrames )
					{
						// Small delay may wrap multiple times per execution, send whole delay line over
						T_SAMPLETYPE * pAddressToGet = m_pDelay[in_uChannelIndex];
						AkUInt32 uDataSize = m_uDelayLineLength*sizeof(T_SAMPLETYPE);
						AkDmaGet( "DelayLineMemory::DelayMem", in_pLSStorage, (std::uint64_t)pAddressToGet, uDataSize, in_uDMATag, 0, 0);
					}
					else
					{
						// Large delay, only DMA necessary section for this execution
						T_SAMPLETYPE * pAddressToGet = m_pDelay[in_uChannelIndex] + m_uOffset;
						AkUInt32 uNumFramesFirstDMA = AkMin( m_uDelayLineLength - m_uOffset, in_uNumFrames );
						AkUInt32 uDataSize = uNumFramesFirstDMA*sizeof(T_SAMPLETYPE);
						AkDmaGet( "DelayLineMemory::DelayMem(1)", in_pLSStorage, (std::uint64_t)pAddressToGet, uDataSize, in_uDMATag, 0, 0);
						if ( uNumFramesFirstDMA < in_uNumFrames )
						{
							// Large DMA will wrap during execution, 2 DMAs are necessary
							pAddressToGet = m_pDelay[in_uChannelIndex];
							uDataSize = (in_uNumFrames-uNumFramesFirstDMA)*sizeof(T_SAMPLETYPE);
							AkDmaGet( "DelayLineMemory::DelayMem(2)", in_pLSStorage+uNumFramesFirstDMA, (std::uint64_t)pAddressToGet, uDataSize, in_uDMATag, 0, 0);
						}
					}
				}
			}

			// Move entire delay line to SPU local storage
			void GetFullDelayChannel( T_SAMPLETYPE * in_pLSStorage, AkUInt32 in_uChannelIndex, AkUInt32 in_uDMATag )
			{
				if ( in_uChannelIndex < m_uNumChannels  )
				{
					AkUInt32 uFramesStreamed = 0;
					do 
					{
						AkUInt32 uFramesDMA = AkMin( (m_uDelayLineLength-uFramesStreamed), 16*1024/sizeof(T_SAMPLETYPE) );
						AkUInt32 uSizeTransfer = uFramesDMA*sizeof(T_SAMPLETYPE);
						AKASSERT( (uSizeTransfer % 16) == 0 );
						T_SAMPLETYPE * pAddressToGet = &m_pDelay[in_uChannelIndex][uFramesStreamed];
						AkDmaGet( "DelayLineMemory::DelayMem(Full)", in_pLSStorage+uFramesStreamed, (std::uint64_t)pAddressToGet, uSizeTransfer, in_uDMATag, 0, 0);
						uFramesStreamed += uFramesDMA;
					}
					while ( uFramesStreamed < m_uDelayLineLength );
				}
			}

			// Put back unwrapped SPU local storage onto main memory
			// SPU local storage pointer refers to while delay line and uses internal offset for large buffers
			// Useful when there are multiple read taps associated with a single write tap
			void PutChannelWriteTap( T_SAMPLETYPE * in_pLSStorage, AkUInt32 in_uNumFrames, AkUInt32 in_uChannelIndex, AkUInt32 in_uDMATag )
			{	
				if ( m_uDelayLineLength <= in_uNumFrames )
				{
					// Small delay may wrap multiple times per execution, send whole delay line over
					T_SAMPLETYPE * pAddressToPut = m_pDelay[in_uChannelIndex];
					AkUInt32 uDataSize = m_uDelayLineLength*sizeof(T_SAMPLETYPE);
					AkDmaPut( "DelayLineMemory::DelayMem", in_pLSStorage, (std::uint64_t)pAddressToPut, uDataSize, in_uDMATag, 0, 0);
				}
				else
				{
					// Large delay, only DMA necessary section for this execution
					T_SAMPLETYPE * pAddressToPut = m_pDelay[in_uChannelIndex] + m_uOffset;
					AkUInt32 uNumFramesFirstDMA = AkMin( m_uDelayLineLength - m_uOffset, in_uNumFrames );
					AkUInt32 uDataSize = uNumFramesFirstDMA*sizeof(T_SAMPLETYPE);
					AkDmaPut( "DelayLineMemory::DelayMem(1)", in_pLSStorage+m_uOffset, (std::uint64_t)pAddressToPut, uDataSize, in_uDMATag, 0, 0);

					if ( uNumFramesFirstDMA < in_uNumFrames )
					{
						// Large DMA will wrap during execution, 2 DMAs are necessary
						pAddressToPut = m_pDelay[in_uChannelIndex];
						uDataSize = (in_uNumFrames-uNumFramesFirstDMA)*sizeof(T_SAMPLETYPE);
						AkDmaPut( "DelayLineMemory::DelayMem(2)", in_pLSStorage, (std::uint64_t)pAddressToPut, uDataSize, in_uDMATag, 0, 0);
					}
				}
			}

			void Wait( AkUInt32 in_uDMATag )
			{
				AkDmaWait(1<<in_uDMATag);  
			}

			// Put back unwrapped SPU local storage onto main memory (using internal offset on main memory only)
			// SPU local storage pointer refers to only part of the full delay line for large buffers
			void PutChannel( T_SAMPLETYPE * in_pLSStorage, AkUInt32 in_uNumFrames, AkUInt32 in_uChannelIndex, AkUInt32 in_uDMATag )
			{	
				if ( m_uDelayLineLength <= in_uNumFrames )
				{
					// Small delay may wrap multiple times per execution, send whole delay line over
					T_SAMPLETYPE * pAddressToPut = m_pDelay[in_uChannelIndex];
					AkUInt32 uDataSize = m_uDelayLineLength*sizeof(T_SAMPLETYPE);
					AkDmaPut( "DelayLineMemory::DelayMem", in_pLSStorage, (std::uint64_t)pAddressToPut, uDataSize, in_uDMATag, 0, 0);
				}
				else
				{
					// Large delay, only DMA necessary section for this execution
					T_SAMPLETYPE * pAddressToPut = m_pDelay[in_uChannelIndex] + m_uOffset;
					AkUInt32 uNumFramesFirstDMA = AkMin( m_uDelayLineLength - m_uOffset, in_uNumFrames );
					AkUInt32 uDataSize = uNumFramesFirstDMA*sizeof(T_SAMPLETYPE);
					AkDmaPut( "DelayLineMemory::DelayMem(1)", in_pLSStorage, (std::uint64_t)pAddressToPut, uDataSize, in_uDMATag, 0, 0);

					if ( uNumFramesFirstDMA < in_uNumFrames )
					{
						// Large DMA will wrap during execution, 2 DMAs are necessary
						pAddressToPut = m_pDelay[in_uChannelIndex];
						uDataSize = (in_uNumFrames-uNumFramesFirstDMA)*sizeof(T_SAMPLETYPE);
						AkDmaPut( "DelayLineMemory::DelayMem(2)", in_pLSStorage+uNumFramesFirstDMA, (std::uint64_t)pAddressToPut, uDataSize, in_uDMATag, 0, 0);
					}
				}
			}

#endif // #ifdef __SPU__

		public:

			T_SAMPLETYPE *	m_pDelay[T_MAXNUMCHANNELS];	// Delay lines for each channel
			AkUInt32		m_uDelayLineLength;			// Total delay line length    
			AkUInt32		m_uOffset;					// Current delay line write position
			AkUInt32		m_uNumChannels;				// Number of delayed channels
		};

	} // namespace DSP
} // namespace AK

#endif // _AKDSP_DELAYLINEMEMORY_