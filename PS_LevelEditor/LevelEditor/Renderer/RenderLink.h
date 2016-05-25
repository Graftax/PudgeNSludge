#pragma once

#include <Windows.h>
#include "EntryPoint.h"

extern "C"
{
	#pragma pack(push, 8)
	struct VEC3F
	{
		float x; float y; float z;
	};
	#pragma pack(pop)

	#pragma pack(push, 8)
	struct BASELIGHT
	{
		float AmbientColorX; float AmbientColorY; float AmbientColorZ; float AmbientColorW;
		float DiffuseColorX; float DiffuseColorY; float DiffuseColorZ; float DiffuseColorW;
		float SpecularColorX; float SpecularColorY; float SpecularColorZ; float SpecularColorW;
		float SpecularIntensity;
		float SpecularPower;
	};
	#pragma pack(pop)

	__declspec (dllexport) bool WINAPI _Init (HWND handle, int width, int height);
	__declspec (dllexport) void WINAPI _Render ();
	__declspec (dllexport) void WINAPI _Update (float fElapsedTime);
	__declspec (dllexport) void WINAPI _Shutdown ();
	__declspec (dllexport) void WINAPI _Up ();
	__declspec (dllexport) void WINAPI _Down ();
	__declspec (dllexport) void WINAPI _Left ();
	__declspec (dllexport) void WINAPI _Right ();
	__declspec (dllexport) void WINAPI _Forward ();
	__declspec (dllexport) void WINAPI _Backward ();
	__declspec (dllexport) VEC3F WINAPI _GetCamPos ();

	__declspec (dllexport) int WINAPI _GetNumHazards ();
	__declspec (dllexport) int WINAPI _GetNumObjects ();
	__declspec (dllexport) VEC3F WINAPI _GetHazard (int index);
	__declspec (dllexport) VEC3F WINAPI _GetObjectPos (int index);
	__declspec (dllexport) int WINAPI _GetHazardType (int index);
	__declspec (dllexport) int WINAPI _GetObjectType (int index);
	__declspec (dllexport) int WINAPI _GetNumLights ();
	__declspec (dllexport) int WINAPI _GetLightType (int index);

	__declspec (dllexport) void WINAPI _LoadLevel (char* szFileName, int length);
	__declspec (dllexport) void WINAPI _SwapLevel (char* szFileName, int length);
	__declspec (dllexport) void WINAPI _ReloadRenderer ();

	/// objects
	__declspec (dllexport) void WINAPI _AddObject (int _nType, VEC3F _vPosition);
	__declspec (dllexport) void WINAPI _RemoveObject (int _nArrayPos);
	__declspec (dllexport) void WINAPI _ChangeObjectType (int _nArrayPos, int _nType);
	__declspec (dllexport) void WINAPI _ChangeObjectPos (int _nArrayPos, VEC3F _vPosition);
	__declspec (dllexport) void WINAPI _RenderDebugSphere (VEC3F _Position);

	/// lights
	__declspec (dllexport) void WINAPI _AddDirectionalLight (BASELIGHT base, VEC3F Direction);
	__declspec (dllexport) void WINAPI _AddPointLight (BASELIGHT base, VEC3F Position, VEC3F Atten, float radius);
	__declspec (dllexport) void WINAPI _AddSpotLight (BASELIGHT base, VEC3F Position, VEC3F Direction, VEC3F Atten, float Radius, float Cutoff, float Expo);
	__declspec (dllexport) void WINAPI _RemoveLight(int _nArrayPos);

	__declspec (dllexport) void WINAPI _ChangeLightDiffuseColor (int _nArrayPos, float _fRed, float _fGreen, float _fBlue, float _fAlpha);
	__declspec (dllexport) void WINAPI _ChangeLightAmbientColor (int _nArrayPos, float _fRed, float _fGreen, float _fBlue, float _fAlpha);
	__declspec (dllexport) void WINAPI _ChangeLightSpecularColor (int _nArrayPos, float _fRed, float _fGreen, float _fBlue, float _fAlpha);

	__declspec (dllexport) void WINAPI _ChangeLightPosition (int _nArrayPos, VEC3F Position);
	__declspec (dllexport) void WINAPI _ChangeLightDirection (int _nArrayPos, VEC3F Direction);
	__declspec (dllexport) void WINAPI _ChangeLightAtten (int _nArrayPos, VEC3F Atten);

	__declspec (dllexport) void WINAPI _ChangeLightCutOff (int _nArrayPos, float _fCutoff);
	__declspec (dllexport) void WINAPI _ChangeLightExpo (int _nArrayPos, float _fExpo);
	__declspec (dllexport) void WINAPI _ChangeLightRadius (int _nArrayPos, float _fRadius);

	__declspec (dllexport) void WINAPI _ChangeLightSpecularIntensity (int _nArrayPos, float _fIntensity);
	__declspec (dllexport) void WINAPI _ChangeLightSpecularPower (int _nArrayPos, float _fPower);
	__declspec (dllexport) void WINAPI _ChangeLightType (int _nArrayPos, int _nType);


	__declspec (dllexport) float WINAPI		 _GetLightRadius (int _nArrayPos);
	__declspec (dllexport) VEC3F WINAPI		 _GetLightPos (int _nArrayPos);
	__declspec (dllexport) VEC3F WINAPI		 _GetLightDir (int _nArrayPos);
	__declspec (dllexport) VEC3F WINAPI		 _GetLightAtten (int _nArrayPos);
	__declspec (dllexport) float WINAPI		 _GetLightExpo (int _nArrayPos);
	__declspec (dllexport) float WINAPI		 _GetLightCutOff (int _nArrayPos);
	__declspec (dllexport) BASELIGHT WINAPI	 _GetLightBase (int _nArrayPos);

	__declspec (dllexport) void WINAPI _ChangeCameraPos (VEC3F pos);
}