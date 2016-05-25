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
 
#ifndef _AKFXTAILHANDLER_H_
#define _AKFXTAILHANDLER_H_

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/AkCommonDefs.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

/// Default value when effect has not enterred tail mode yet.
#define AKFXTAILHANDLER_NOTINTAIL 0xFFFFFFFF

/// Effect tail handling utility class.
/// Handles varying number of tail frames from frame to frame (i.e. based on RTPC parameters).
/// Handles effect revived (quit tail) and reenters etc.
class AkFXTailHandler
{
public:
	/// Constructor
	inline AkFXTailHandler() 
		: uTailFramesRemaining( AKFXTAILHANDLER_NOTINTAIL )
		, uTotalTailFrames(0) {}

	/// Handle FX tail and zero pads AkAudioBuffer if necessary
	inline void HandleTail(	
		AkAudioBuffer * io_pBuffer, 
		AkUInt32 in_uTotalTailFrames )
	{
		bool bPreStop = io_pBuffer->eState == AK_NoMoreData;
		if ( bPreStop )
		{	
			// Tail not yet finished processing
			if ( uTailFramesRemaining > 0 )
			{
				// Not previously in tail, compute tail time
				if ( uTailFramesRemaining == AKFXTAILHANDLER_NOTINTAIL )
				{
					uTailFramesRemaining = in_uTotalTailFrames;
					uTotalTailFrames	 = in_uTotalTailFrames;
				}
				// Tail time changed, augment if necessary but preserve where we are so that effect will 
				// still finish when constanly changing this based on RTPC parameters
				else if ( in_uTotalTailFrames > uTotalTailFrames )
				{
					AkUInt32 uFramesElapsed = uTotalTailFrames - uTailFramesRemaining;
					uTailFramesRemaining = in_uTotalTailFrames - uFramesElapsed;
					uTotalTailFrames	 = in_uTotalTailFrames;
				}
				// Always full buffers while in tail
				AkUInt32 uNumTailFrames = (AkUInt32)(io_pBuffer->MaxFrames()-io_pBuffer->uValidFrames); 
				uTailFramesRemaining -= AkMin( uTailFramesRemaining, uNumTailFrames ); 
				io_pBuffer->ZeroPadToMaxFrames();
				if ( uTailFramesRemaining > 0 )
					io_pBuffer->eState = AK_DataReady;
			}
		}
		else
		{
			// Reset tail mode for next time if exits tail mode (on bus only)
			uTailFramesRemaining = AKFXTAILHANDLER_NOTINTAIL;
		}
	}

#ifdef __SPU__
	/// Interface for handling one channel at a time (neccessary when channels are processed sequentially on SPU)

	/// Recompute target fx tail length has necessary.
	AkForceInline void HandleTailChannelPre( AKRESULT in_eAudioBufferState, AkUInt32 in_uTotalTailFrames )
	{
		bool bPreStop = in_eAudioBufferState == AK_NoMoreData;
		if ( bPreStop )
		{	
			// Tail not yet finished processing
			if ( uTailFramesRemaining > 0 )
			{
				// Not previously in tail, compute tail time
				if ( uTailFramesRemaining == AKFXTAILHANDLER_NOTINTAIL )
				{
					uTailFramesRemaining = in_uTotalTailFrames;
					uTotalTailFrames	 = in_uTotalTailFrames;
				}
				// Tail time changed, augment if necessary but preserve where we are so that effect will 
				// still finish when constanly changing this based on RTPC parameters
				else if ( in_uTotalTailFrames > uTotalTailFrames )
				{
					AkUInt32 uFramesElapsed = uTotalTailFrames - uTailFramesRemaining;
					uTailFramesRemaining = in_uTotalTailFrames - uFramesElapsed;
					uTotalTailFrames	 = in_uTotalTailFrames;
				}
			}
		}
		else
		{
			// Reset tail mode for next time if exits tail mode (on bus only)
			uTailFramesRemaining = AKFXTAILHANDLER_NOTINTAIL;
		}
	}
	
	/// Zero pads given channel data (local storage address).
	AkForceInline void HandleTailChannel( AkAudioBuffer * in_pAudioBuffer, AkReal32 * io_pfChannelData )
	{
		bool bPreStop = in_pAudioBuffer->eState == AK_NoMoreData;
		if ( bPreStop && (uTailFramesRemaining > 0) )
		{	
			// Always full buffers while in tail
			AkUInt32 uNumTailFrames = (AkUInt32)(in_pAudioBuffer->MaxFrames()-in_pAudioBuffer->uValidFrames); 
			AKPLATFORM::AkMemSet( io_pfChannelData + in_pAudioBuffer->uValidFrames, 0, uNumTailFrames * sizeof(AkReal32) );
			in_pAudioBuffer->uValidFrames = in_pAudioBuffer->MaxFrames();
		}
	}

	/// Determine when done and adjust return code appropriately.
	AkForceInline void HandleTailChannelPost( AkAudioBuffer * io_pBuffer )
	{
		bool bPreStop = io_pBuffer->eState == AK_NoMoreData;
		if ( bPreStop && (uTailFramesRemaining > 0) )
		{		
			uTailFramesRemaining -= AkMin( uTailFramesRemaining, (AkUInt32)(io_pBuffer->MaxFrames()-io_pBuffer->uValidFrames) ); 
			if ( uTailFramesRemaining > 0 )
				io_pBuffer->eState = AK_DataReady;	
		}
	}
#endif

protected:

	AkUInt32	uTailFramesRemaining; // AKFXTAILHANDLER_NOTINTAIL, otherwise value represents number of frames remaining in tail
	AkUInt32	uTotalTailFrames;
	
} AK_ALIGN_DMA;





#endif // _AKFXTAILHANDLER_H_
