#ifndef CONFIG_H
#define CONFIG_H
#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x0800
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <stack>
#include <process.h>
#include "DXIncludes/dinput.h"
#include "DXIncludes/d3d9.h"
#include "DXIncludes/d3dx9.h"
#include "DXIncludes/dxdiag.h"
#include "DXIncludes/XInput.h"
#include "DXIncludes/dinput.h"

#include "../Utilities/Logger/Logger.h"
//using namespace Cioffi;
using namespace std;

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// This gives us a break line
#include <cstdio>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

//Logger Defines
#define DEFAULT_CONSOLE_LOG_LEVEL LOG_DEBUG
#define DEFAULT_FILE_LOG_LEVEL LOG_WARN
#define	DEFAULT_LOG_DIR ""

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define WIDESCREEN_WIDTH 1280
#define WIDESCREEN_HEIGHT 720

//// Audio Events Enum
//enum {SFX_MAX};
//
//// Structure for Triggering Audio Events
//struct AudioEvent
//{
//	void* data;
//	void* sender;
//	//vec3f position;
//};
//
////Audio Event Init
//static AudioEvent tAudioEvent;
//static AudioEvent tPlayerEvent;
//static AudioEvent tEnemyEvent;
//static AudioEvent tDialogueEvent;
//static AudioEvent tMusicEvent;

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


// this will show memory leaks but because Wise overwrites the new operator we have to do this for the memory leak debug.
#ifndef _BROKEN_AUDIO
#define _BROKEN_AUDIO
	#ifdef _DEBUG
	   #ifndef DBG_NEW
		  #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		  #define new DBG_NEW
	   #endif
	#endif  // _DEBUG
#endif

#define _ANIMATIONS 1
#define _PARTICLES 1

#define GAME_UNIT 122
#endif