#ifndef _AK_ALLPLUGINSREGISTRATIONHELPERS_H_
#define _AK_ALLPLUGINSREGISTRATIONHELPERS_H_

#include <AK/Plugin/AllPluginsFactories.h>
#if defined(AK_PS3) || defined(AK_WII_FAMILY) || defined(AK_XBOX360) || defined(AK_WIN)
#include <AK/MotionEngine/Common/AkMotionEngine.h>
#endif


namespace AK
{
	namespace SoundEngine
	{

		#if !defined (AK_WII_FAMILY_HW) && !defined(AK_3DS)
			#define AK_SOFTWARE_EFFECT_PLATFORM
		#endif

		/// Early return if anything goes wrong
		#define AK_CHECK_ERROR( __FUNCCALL__ )	\
		{											\
			AKRESULT eResult = (__FUNCCALL__);		\
			if ( eResult != AK_Success )			\
				eFinal = eResult;					\
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllEffectPlugins()
		{
			AKRESULT eFinal = AK_Success;
#if defined(AK_SOFTWARE_EFFECT_PLATFORM)

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_COMPRESSOR,
				CreateCompressorFX,
				CreateCompressorFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_EXPANDER,
				CreateExpanderFX,
				CreateExpanderFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_PEAKLIMITER,
				CreatePeakLimiterFX,
				CreatePeakLimiterFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_MATRIXREVERB,
				CreateMatrixReverbFX,
				CreateMatrixReverbFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_ROOMVERB,
				CreateRoomVerbFX,
				CreateRoomVerbFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_DELAY,
				CreateDelayFX,
				CreateDelayFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_FLANGER,
				CreateFlangerFX,
				CreateFlangerFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_TREMOLO,
				CreateTremoloFX,
				CreateTremoloFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_GUITARDISTORTION,
				CreateGuitarDistortionFX,
				CreateGuitarDistortionFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_PARAMETRICEQ,
				CreateParametricEQFX,
				CreateParametricEQFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_METER,
				CreateMeterFX,
				CreateMeterFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_STEREODELAY,
				CreateStereoDelayFX,
				CreateStereoDelayFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_PITCHSHIFTER,
				CreatePitchShifterFX,
				CreatePitchShifterFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_TIMESTRETCH,
				CreateTimeStretchFX,
				CreateTimeStretchFXParams ) );
			
			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_HARMONIZER,
				CreateHarmonizerFX,
				CreateHarmonizerFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_GAIN,
				CreateGainFX,
				CreateGainFXParams ) );

#elif defined(AK_WII_FAMILY)

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_WII_STANDARD_REVERB,
				CreateWiiReverbStd,
				CreateWiiReverbStdFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_WII_HIGH_QUALITY_REVERB,
				CreateWiiReverbHi,
				CreateWiiReverbHiFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_WII_CHORUS,
				CreateWiiChorus,
				CreateWiiChorusFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_WII_DELAY,
				CreateWiiDelay,
				CreateWiiDelayFXParams ) );

#elif defined(AK_3DS)

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_3DS_REVERB,
				Create3DSReverb,
				Create3DSReverbFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_3DS_DELAY,
				Create3DSDelay,
				Create3DSDelayFXParams ) );

#endif

			return eFinal;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllSourcePlugins()
		{
			AKRESULT eFinal = AK_Success;
			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_SILENCE,
				CreateSilenceSource,
				CreateSilenceSourceParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_SINE,
				CreateSineSource,
				CreateSineSourceParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_TONE,
				CreateToneSource,
				CreateToneSourceParams ) );

#ifdef AK_WIN
			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_MP3,
				CreateMP3Source,
				CreateMP3SourceParams ) );
#endif

#if !defined( AK_NGP ) && !defined( AK_IOS ) && !defined( AK_3DS )
			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_AUDIOINPUT,
				CreateAudioInputSource,
				CreateAudioInputSourceParams ) );
#endif

			return eFinal;
		}


		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterConvolutionReverbPlugin()
		{
			AKRESULT eFinal = AK_Success;
#ifdef AK_SOFTWARE_EFFECT_PLATFORM
		AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_CONVOLUTIONREVERB,
				CreateConvolutionReverbFX,
				CreateConvolutionReverbFXParams ) );
#endif
			return eFinal;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterMcDSPPlugins()
		{
			AKRESULT eFinal = AK_Success;
#ifdef AK_SOFTWARE_EFFECT_PLATFORM
			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_MCDSP, 
				AKEFFECTID_MCDSPFUTZBOX,
				CreateMcDSPFutzBoxFX,
				CreateMcDSPFutzBoxFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_MCDSP, 
				AKEFFECTID_MCDSPML1,
				CreateMcDSPML1FX,
				CreateMcDSPML1FXParams ) );
#endif

			return eFinal;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisteriZotopePlugins()
		{
			AKRESULT eFinal = AK_Success;
#if defined( AK_WIN ) || defined( AK_XBOX360 )
			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_IZOTOPE, 
				IZEFFECTID_TRASHMULTIBANDDISTORTION,
				CreateTrashMultibandDistortionFX,
				CreateTrashMultibandDistortionFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_IZOTOPE, 
				IZEFFECTID_TRASHFILTERS,
				CreateTrashFiltersFX,
				CreateTrashFiltersFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_IZOTOPE, 
				IZEFFECTID_TRASHDYNAMICS,
                CreateTrashDynamicsFX,
                CreateTrashDynamicsFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_IZOTOPE, 
				IZEFFECTID_TRASHDELAY,
                CreateTrashDelayFX,
                CreateTrashDelayFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_IZOTOPE, 
				IZEFFECTID_TRASHBOXMODELER,
                CreateTrashBoxModelerFX,
                CreateTrashBoxModelerFXParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_IZOTOPE, 
				IZEFFECTID_HYBRIDREVERB,
				CreateHybridReverbFX,
				CreateHybridReverbFXParams ) );
#endif

			return eFinal;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterSoundSeedPlugins()
		{
			AKRESULT eFinal = AK_Success;
#ifdef AK_SOFTWARE_EFFECT_PLATFORM
			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_SOUNDSEEDIMPACT,
				CreateAkSoundSeedImpactFX,
				CreateAkSoundSeedImpactFXParams ) );
#endif

#ifndef AK_3DS
			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_SOUNDSEEDWIND,
				CreateSoundSeedWind,
				CreateSoundSeedWindParams ) );

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_SOUNDSEEDWOOSH,
				CreateSoundSeedWoosh,
				CreateSoundSeedWooshParams ) );
#endif

			return eFinal;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllCodecPlugins()
		{
			AKRESULT eFinal = AK_Success;
#ifndef AK_3DS
			AK::SoundEngine::RegisterCodec(
				AKCOMPANYID_AUDIOKINETIC,
				AKCODECID_VORBIS,
				CreateVorbisFilePlugin,
				CreateVorbisBankPlugin );
#endif

#ifdef AK_XBOX360
			AK::SoundEngine::RegisterCodec(
				AKCOMPANYID_AUDIOKINETIC,
				AKCODECID_XWMA,
				CreateXWMAFilePlugin,
				CreateXWMABankPlugin );
#endif

#ifdef AK_APPLE
			AK::SoundEngine::RegisterCodec(
				AKCOMPANYID_AUDIOKINETIC,
				AKCODECID_AAC,
				CreateAACFilePlugin,
				CreateAACBankPlugin );
#endif

#ifdef AK_NGP
			AK::SoundEngine::RegisterCodec(
				AKCOMPANYID_AUDIOKINETIC,
				AKCODECID_ATRAC9,
				CreateATRAC9FilePlugin,
				CreateATRAC9BankPlugin );
#endif

			return eFinal;

		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllRumblePlugins()
		{
			AKRESULT eFinal = AK_Success;
#if defined AK_PS3 || defined AK_WII_FAMILY || defined AK_XBOX360 || defined AK_WIN

			AK_CHECK_ERROR( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeMotionSource,
				AKCOMPANYID_AUDIOKINETIC,
				AKSOURCEID_MOTIONGENERATOR,
				AkCreateMotionGenerator,
				AkCreateMotionGeneratorParams ) );

			AK::MotionEngine::RegisterMotionDevice( 
				AKCOMPANYID_AUDIOKINETIC, 
				AKMOTIONDEVICEID_RUMBLE,
				AkCreateRumblePlugin );

#endif

			return eFinal;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllBuiltInPlugins()
		{
			AKRESULT eFinal = AK_Success;
			AK_CHECK_ERROR( RegisterAllCodecPlugins() );
			AK_CHECK_ERROR( RegisterAllEffectPlugins() );
			AK_CHECK_ERROR( RegisterAllSourcePlugins() );
			AK_CHECK_ERROR( RegisterAllRumblePlugins() );
			return eFinal;
		}	

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllPlugins()
		{
			AKRESULT eFinal = AK_Success;
			// Built-in products
			AK_CHECK_ERROR( RegisterAllBuiltInPlugins() );
			// Products which require additional license
			AK_CHECK_ERROR( RegisterConvolutionReverbPlugin() );
			AK_CHECK_ERROR( RegisterSoundSeedPlugins() );
			AK_CHECK_ERROR( RegisterMcDSPPlugins() );
			AK_CHECK_ERROR( RegisteriZotopePlugins() );
			return eFinal;
		}

	} // SoundEngine
} // AK

#endif // _AK_ALLPLUGINSREGISTRATIONHELPERS_H_
