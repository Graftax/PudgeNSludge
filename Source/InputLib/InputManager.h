//////////////////////////////////////////////////////////////////////////
// Title: InputManager.h
// Author: Josh Morgan
// Date: 5/21/12
// Purpose:	This class will handle the initialization of Direct Input and
//			handle the input from the devices. It will be initialized when
//			the game starts and shutdown when the game ends.
//////////////////////////////////////////////////////////////////////////

#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include "../PS/config.h"
#include "../Utilities/MathLib/vec3.h"
//
//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "../dxguid.lib")

#define KEYBOARD_BUFFER_SIZE 1

enum PlayerActions {MAX_ACTIONS};

class CGame;

__declspec(align(32))
class CInputManager
{
private:
	//LPDIRECTINPUT8 m_lpDI;
	CGame* m_pGame;

	IDirectInput8* m_pDI;
	IDirectInputDevice8* m_pKeyboard;
	IDirectInputDevice8* m_pMouse;

	BYTE m_chPrevKeyboardState[256];
	BYTE m_chKeyboardState[256];
	char buffer[256];
	DIDEVICEOBJECTDATA m_didod[10];
	DWORD m_dwBufferElements;
	BYTE m_chAsciiKeys[256];
	HKL m_keyLayout;

	DIMOUSESTATE m_diMouseState;
	DIMOUSESTATE m_diPrevMouseState;
	int m_nScreenWidth, m_nScreenHeight;
	int m_nMouseX, m_nMouseY;
	POINT m_ptMousePos;
	float m_fSensitivity;

	float m_fIgnoreTimer;		// Timer that will be used to ignore input.

	static CInputManager* m_pInstance;

public:
	CInputManager(void);
	~CInputManager(void);

	CInputManager(const CInputManager&);
	CInputManager& operator=(const CInputManager&);

	/////////////////////////////////////////////////////////////////////
	//Initialize():		This function is used to initialized and setup
	//					the different devices, like the keyboard and
	//					mouse.
	//
	//Returns:			bool - true if the devices were set up correctly
	//
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool Initialize(HWND hWnd, 
					HINSTANCE hInstance);

	/////////////////////////////////////////////////////////////////////
	//Update():			This Function is used to get the new state of
	//					the keyboard and the mouse, and then update the
	//					mouse position.
	//
	//Returns:			void
	//
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void Update(void);

	/////////////////////////////////////////////////////////////////////
	//ClearInput():		Clears the input of the current states.
	//
	//Returns:			void
	//
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void ClearInput(void);

	/////////////////////////////////////////////////////////////////////
	//ReadKeyboard():	Gets the current state of the keyboard.
	//
	//Returns:			bool - true if keyboard was able to be read by
	//							direct input.
	//
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool ReadKeyboard(void);

	/////////////////////////////////////////////////////////////////////
	//ReadMouse():		Gets the current state of the mouse.
	//
	//Returns:			bool - true if the mouse was able to be read by
	//							direct input.
	//
	//Mod Date:			5/23/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool ReadMouse(void);

	/////////////////////////////////////////////////////////////////////
	//ProcessInput():	Processes the movement of the mouse and
	//					updates the position of the mouse.
	//
	//Returns:			void
	//							
	//
	//Mod Date:			5/23/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void ProcessInput(void);

	/////////////////////////////////////////////////////////////////////
	//KeyDown():		Determines whether the key is currently down.
	//
	//Returns:			bool - true if the key is down.
	//
	//Mod Date:			5/23/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool KeyDown(unsigned char);

	/////////////////////////////////////////////////////////////////////
	//KeyPressed():		Determines if a key is down, and if it were down
	//					last frame.
	//
	//Returns:			bool - true if the button is down, but wasn't
	//							down last frame.
	//
	//Mod Date:			5/23/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool KeyPressed(unsigned char);

	/////////////////////////////////////////////////////////////////////
	//KeyPressedEx():	The buffered version of KeyPressed()
	//
	//Returns:			bool - true if any of the keys pressed this frame
	//							were the key that we were looking for.
	//
	//Mod Date:			5/24/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool KeyPressedEx(unsigned char);

	/////////////////////////////////////////////////////////////////////
	//KeyUp():			Gets if the specified key is currently up
	//
	//Returns:			bool - true if the key is up
	//
	//Mod Date:			5/24/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool KeyUp(unsigned char);

	/////////////////////////////////////////////////////////////////////
	//KeyReleased():	Used to tell if the key was down for the previous
	//					state, but is up this frame.
	//
	//Returns:			bool - true if the key is up in the current state
	//							of the keyboard, but down in the previous
	//							state.
	//
	//Mod Date:			5/24/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool KeyReleased(unsigned char);

	/////////////////////////////////////////////////////////////////////
	//KeyReleasedEx():	The buffered version of KeyReleased()
	//
	//Returns:			bool - true if keyboard was able to be read by
	//							direct input.
	//
	//Mod Date:			5/24/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool KeyReleasedEx(unsigned char);

	/////////////////////////////////////////////////////////////////////
	//CheckKeys():		Checks the current state and returns when it hits
	//					the first key that is down
	//
	//Returns:			char - the first key that is down.
	//
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	char CheckKeys(void);

	/////////////////////////////////////////////////////////////////////
	//CheckBufferedKeysEx():	buffered version of the CheckKeys()
	//							function.
	//
	//Returns:					char - the first key in the list of keys
	//									that are down.
	//
	//Mod Date:					5/24/12
	//Mod Initials:				JM
	/////////////////////////////////////////////////////////////////////
	char CheckBufferedKeysEx(void);

	/////////////////////////////////////////////////////////////////////
	//GetDIKCode():		returns the DIK Code of the key
	//
	//Returns:			bool - true if keyboard was able to be read by
	//							direct input.
	//
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	unsigned char GetDIKCode(void);

	/////////////////////////////////////////////////////////////////////
	//GetBufferedDIKCodeEx():	buffered version of the GetDIKCode() function.
	//
	//Returns:					bool - true if keyboard was able to be 
	//									read by direct input.
	//
	//Mod Date:					5/21/12
	//Mod Initials:				JM
	/////////////////////////////////////////////////////////////////////
	unsigned char GetBufferedDIKCodeEx(void);

	/////////////////////////////////////////////////////////////////////
	//MouseButtonUp():	Checks to see if the mouse button is not down
	//
	//Returns:			bool - true if mouse button is not down.
	//
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool MouseButtonUp(unsigned char chButton);

	/////////////////////////////////////////////////////////////////////
	//MouseButtonDown():Checks to see if the mouse button, chButton, is
	//					currently down.
	//
	//Returns:			bool - true if the mouse button is down.
	//							
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool MouseButtonDown(unsigned char chButton);

	/////////////////////////////////////////////////////////////////////
	//MouseButtonPressed():	Checks if the mouse button, chButton, was
	//					previously up but is now down.
	//
	//Returns:			bool - true if the button was up in the previous
	//							state but is down this state.
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool MouseButtonPressed(unsigned char chButton);

	/////////////////////////////////////////////////////////////////////
	//MouseButtonPressedEx():The buffered version of the MouseButtonPressed()
	//							function.
	//
	//Returns:			bool - true if the mouse button was up but is now
	//							down.
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool MouseButtonPressedEx(unsigned char chButton);

	/////////////////////////////////////////////////////////////////////
	//MouseButtonReleased():checks if the mouse button was released
	//
	//Returns:			bool - true if the button was down in the previous
	//							state, but is up in the current one.
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool MouseButtonReleased(unsigned char chButton);

	/////////////////////////////////////////////////////////////////////
	//MouseButtonReleasedEx():the buffered version of the MouseButtonReleased()
	//					function.
	//
	//Returns:			bool - true if the button was down last state but
	//							is up this frame.
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool MouseButtonReleasedEx(unsigned char chButton);

	/////////////////////////////////////////////////////////////////////
	//LeftMouseButtonDown():checks to see if the left mouse button is down.
	//
	//Returns:			bool - true if the left mouse button is down.
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool LeftMouseButtonDown(void);

	/////////////////////////////////////////////////////////////////////
	//RightMouseButtonDown():Checks to see if the right mouse button is down.
	//
	//Returns:			bool - True if the right mouse button is down.
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	bool RightMouseButtonDown(void);

	/////////////////////////////////////////////////////////////////////
	//LeftMouseButtonPressed():Buffered version of LeftMouseButtonDown()
	//					function.
	//
	//Returns:			bool - True if the right mouse button is down.
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	int LeftMouseButtonPressed(void);

	/////////////////////////////////////////////////////////////////////
	//RightMouseButtonPressed():Buffered version of RightMouseButtonDown()
	//					function.
	//
	//Returns:			bool - true if keyboard was able to be read by
	//							direct input.
	//
	//Mod Date:			5/25/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	int RightMouseButtonPressed(void);
	
	/////////////////////////////////////////////////////////////////////
	//Shutdown():		Shuts down the input manager, freeing up any
	//					memory that was allocated, and shutting down
	//					Direct Input.
	//
	//Returns:			void
	//
	//Mod Date:			5/21/12
	//Mod Initials:		JM
	/////////////////////////////////////////////////////////////////////
	void Shutdown(void);

	/////////////////////////////////////////////////////////////////////
	//Shutdown():		Shuts down the input manager, freeing up any
	//					memory that was allocated, and shutting down
	//					Direct Input.
	//
	//Returns:			void
	//
	//Mod Date:			6/14/12
	//Mod Initials:		BS
	/////////////////////////////////////////////////////////////////////
	void OffSetMouseForStart(void);

	/////////////////////////////////////////////////////////////////////
	//Shutdown():		Clears all Input and ignores it until the alloted
	//					timer has been reached. FDT is the elapsed time
	//					and the _Timer is how much time it should wait.
	//
	//Returns:			void
	//
	//Mod Date:			7/30/12
	//Mod Initials:		
	/////////////////////////////////////////////////////////////////////
	void IgnoreInputForSetAmountOfTime( float _fDT, float _Timer );

	/////////////////////////
	//		Accessors	   //
	/////////////////////////
	long GetMouseMovementX(void);
	long GetMouseMovementY(void);
	int GetMousePosX(void) { return m_nMouseX; }
	int GetMousePosY(void) { return m_nMouseY; }
	float GetSensitivity(void) { return m_fSensitivity; }
	D3DXVECTOR3 GetCursorPosition(void);

	////////////////////////
	//		Mutators	  //
	////////////////////////
	void SetMousePosX(int nMousePosX) { m_nMouseX = nMousePosX; }
	void SetMousePosY(int nMousePosY) { m_nMouseY = nMousePosY; }
	void SetSensitivity(float fSensitivity);

	void ResetIgnoreTimer(void) { m_fIgnoreTimer = 0.0f; }
};

#endif //INPUTMANAGER_H_