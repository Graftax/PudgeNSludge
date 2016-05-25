///////////////////////////////////////////////////
// File:    main.cpp
// Date:    3/30/2012
// Author:  Josh Fields
// 
// Purpose: This is the entry point of Pudge & Sludge.
//			App starts at WinMain.
//
///////////////////////////////////////////////////
#include "config.h"
#include "resource.h"
//#include <vld.h>
#include "CGame.h"
#include "../RenderEngine/MultiThreadRender.h"
#include "../RenderEngine/RenderEngine.h"
#include "States\CSplashScreenState.h"
#include "../Utilities/Logger/Logger.h"

//This is for Memory Leak debug
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif

#define ALLOC_NUM 0L
//Function declarations for window creation and WinProc

#ifdef _DEBUG
void ToggleFullScreen(HWND winHandl);
#endif

LONG WINAPI CatchAll(_EXCEPTION_POINTERS *pExceptionInfo);
HWND BuildWindow(HINSTANCE AppInstance);
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Game is Running
bool gIsRunning = true;

 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Function:		WinMain
 // Date:			3/30/2012
 // Author:			Josh Fields
 // 
 //					// Taken from MSDN: http://msdn.microsoft.com/en-us/library/ms633559(v=vs.85).aspx
 // Parameters:		HINSTANCE hInstance - A handle to the current instance of the application.
 //					HINSTANCE hPrevInstance - Always NULL
 //					LPSTR lpCmdLine - command line arguments passed
 //					int nShowCmd - Controls how the window is to be shown
 //
 // Return Value:	If the function succeeds, terminating when it receives a WM_QUIT message, 
 //					it should return the exit value contained in that message's wParam parameter. 
 //					If the function terminates before entering the message loop, it should return zero
 //
 // Purpose:		The entry point of the application
 //
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd)
{
	// When compiling in debug, enable memory leak detection, and start a console
	// attached to this process' stdio
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(ALLOC_NUM);
	SetUnhandledExceptionFilter(CatchAll);

#else
	SetUnhandledExceptionFilter(CatchAll);
#endif

	// Seed random
	srand((unsigned int)time(NULL));
	remove("./dumpfile.mdmp");

	//Create Window
	HWND WindowHandle = BuildWindow(hInstance);

	if (WindowHandle == NULL)
	{
		//LOG(LOG_ERROR, "WinMain", "Window could not be created. Exiting.");
		return -1;
	}
	RECT r;
	GetClientRect(WindowHandle, &r);

	// Initialize systems
	Logger::Initialize();

// Console Window for Logger
#ifdef _DEBUG
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	FILE* ptr = stdout;
	freopen_s(&ptr, "CON", "w", stdout);

	HANDLE outh = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize = {160, 51};
	SetConsoleScreenBufferSize(outh, bufferSize);
	SMALL_RECT consoleSize = { 0, 0, 159, 50 };
	SetConsoleWindowInfo(outh, TRUE, &consoleSize);
#endif
	//MultiThreadRender::GetInstance()->InitRenderEngine( WindowHandle, hInstance, CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), true, true );

	ShowWindow(WindowHandle, SW_SHOW);
	UpdateWindow(WindowHandle);


	CGame::GetInstance()->Init(WindowHandle, hInstance);	
	
	// Main Game Loop
	MSG msg = { 0 };
	while (gIsRunning)
	{
		//message proc stuff for window management
		while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{   
			// Peek Till you Get A Paint Last message in the stack (maybe?)
			if(msg.message == WM_PAINT)
				break; 

			// Pass Message Off
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		//CGame Stuff Goes Here
		if (!CGame::GetInstance()->Main ())
		{
			gIsRunning = false;
		}
	}

	Logger::Shutdown();

	// Shutdown running systems	
	CGame::GetInstance()->Shutdown();

	// Delete all instances
	CGame::DeleteInstance();

	PostQuitMessage(0);

	return 0;
}

HWND BuildWindow(HINSTANCE AppInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)WinProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= AppInstance;
	wcex.hIcon			= LoadIcon(NULL, MAKEINTRESOURCE( IDI_ICON1 ));
	wcex.hCursor		= (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL; //(HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= (LPCSTR)"Pudge & Sludge!";
	wcex.hIconSm		= NULL;

	if (RegisterClassEx(&wcex) == 0)
	{
		//LOG(LOG_ERROR, "::BuildWindow", "RegisterClassEx FAILED!");
		return NULL;
	}

#ifdef _DEBUG
	unsigned int uiStyle = WS_CAPTION |	WS_SYSMENU | WS_MAXIMIZEBOX, uiStyleX = NULL;
#else
	unsigned int uiStyle = WS_POPUP /*WS_POPUP | WS_VISIBLE /*| WS_MAXIMIZE*/, uiStyleX = WS_EX_TOPMOST | WS_EX_APPWINDOW;// | WS_EX_TOPMOST;
#endif
 
	RECT r;
	r.top = r.left = 100;
	r.right = CGame::GetInstance()->GetScreenWidth();
	r.bottom = CGame::GetInstance()->GetScreenHeight();
	r.right += r.left;
	r.bottom += r.top;

	AdjustWindowRectEx(&r, uiStyle, FALSE,uiStyleX);
	

	HWND WinHandl = CreateWindow(wcex.lpszClassName, (LPCSTR)"Pudge & Sludge", uiStyle,
		r.top, r.left, r.right, r.bottom,
		HWND_DESKTOP, NULL, AppInstance, NULL);

	if (!WinHandl)
	{
		//LOG(LOG_ERROR, "Renderer::BuildWindow", "CreateWindowEx FAILED!");
		return NULL;
	}

	return WinHandl;
}

#ifdef _DEBUG
void ToggleFullScreen(HWND winHandl)
{
	LONG_PTR retval = GetWindowLongPtr(winHandl, GWL_STYLE);
	if (retval == NULL)
	{
		//this means fail
//		Logger::Log(LOG_ERROR, "ToggleFullScreen", "Bad value returned from GetWindowLongPtr");
	}
	else
	{
		// windowed to full screen
		if (retval & WS_CAPTION)
		{
			DEVMODE dmScreenSettings;
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

			dmScreenSettings.dmSize         = sizeof(dmScreenSettings);     
			dmScreenSettings.dmPelsWidth    = CGame::GetInstance()->GetScreenWidth();
			dmScreenSettings.dmPelsHeight   = CGame::GetInstance()->GetScreenHeight();
			dmScreenSettings.dmBitsPerPel   = 16;
			dmScreenSettings.dmFields       = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			SetWindowLongPtr(winHandl, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
			SetWindowLongPtr(winHandl, GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE);
			//ShowWindow(winHandl, SW_HIDE);
			//ShowWindow(winHandl, SW_MAXIMIZE);
			UpdateWindow(winHandl);
			ShowWindow(winHandl, SW_SHOW);
		}
		// full screen to windowed
		else
		{
			SetWindowLongPtr(winHandl, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
			SetWindowLongPtr(winHandl, GWL_STYLE, WS_CAPTION |	WS_SYSMENU);
			ChangeDisplaySettings(0,0);
			UpdateWindow(winHandl);
			ShowWindow(winHandl, SW_SHOW | SW_NORMAL);
		}
	}
}
#endif

LONG WINAPI CatchAll(_EXCEPTION_POINTERS *pExceptionInfo)
{
	int index = 0;
	LPCTSTR filename = "dumpfile.mdmp";
	HANDLE hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL );
	
	char *report = NULL;
	if (hFile!=INVALID_HANDLE_VALUE)
	{
		_MINIDUMP_EXCEPTION_INFORMATION ExInfo;
	
		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionInfo;
		ExInfo.ClientPointers = NULL;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
		report = new char[1024];
		sprintf_s(report, 1024, "Look in dump file: dumpfile.mdmp\nThe error was:\n\n%d\n%s", ExInfo.ExceptionPointers->ExceptionRecord->ExceptionCode, ExInfo.ExceptionPointers->ExceptionRecord->ExceptionRecord->ExceptionInformation);
		MessageBox(NULL, report, "AHHH BREAK!",MB_OK);
		CloseHandle(hFile);
		delete[] report;
	}

	return 0;

}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!gIsRunning)
		return 0;

	switch(message)
	{
	case WM_PAINT:
		break;
	case WM_QUIT:
		gIsRunning = false;
		break;
	case WM_DESTROY:
		gIsRunning = false;
		FreeConsole();
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_INPUT:
		break;
	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:

		switch (wParam)
		{
		case WA_INACTIVE:
			break;
		default:
			break;
		};
		break;
	case WM_MOUSEACTIVATE:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}