#include "InputManager.h"
#include "../PS/CGame.h"

//CInputManager* CInputManager::m_pInstance = NULL;

CInputManager::CInputManager(void)
{
	m_pDI = NULL;
	m_pKeyboard = NULL;
	m_pMouse = NULL;

	m_dwBufferElements = NULL;
	m_keyLayout = NULL;
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_nMouseX = 0; 
	m_nMouseY = 0;
	m_pGame = NULL;
	m_fSensitivity = 1.0f;
}

CInputManager::~CInputManager(void)
{

}

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
bool CInputManager::Initialize(	HWND hWnd, 
								HINSTANCE hInstance)
{
	HRESULT hr;

	//init the main direct input interface
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&m_pDI, NULL);
	if(FAILED(hr))
	{
		LOG(LOG_ERROR, "CInputManager::Initialize", "Failed to create DirectInput8 device.");
		return false;
	}
	
	ZeroMemory(m_chKeyboardState, 256);
	ZeroMemory(m_chPrevKeyboardState, 256);

	//init the direct input interface for the keyboard
	hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);

	if(FAILED(hr))
	{
		LOG(LOG_ERROR, "CInputManager::Initialize", "Failed to init interface for the keyboard.");
		return false;
	}

	//set the data format
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		LOG(LOG_ERROR, "CInputManager::Initialize", "Failed to Set data for the Keyboard.");
		return false;
	}

	//set the cooperatice level of the keyboard to not share with other programs
	hr = m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND|DISCL_EXCLUSIVE);
	if(FAILED(hr))
	{
		LOG(LOG_ERROR, "CInputManager::Initialize", "Failed to Set the cooperative level of the Keyboard.");
		return false;
	}

	

	DIPROPDWORD dipdw;
	memset(&dipdw, 0, sizeof(DIPROPDWORD));
	dipdw.diph.dwSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	//init the direct input interface for the mouse
	hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if(FAILED(hr))
	{
		LOG(LOG_ERROR, "CInputManager::Initialize", "Failed to init mouse.");
		return false;
	}

	//set the data for the mouse
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(hr))
	{
		LOG(LOG_ERROR, "CInputManager::Initialize", "Failed to set data for the mouse.");
		return false;
	}

	//set the cooperative level of the mouse to share with other programs
	hr = m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND|DISCL_EXCLUSIVE);
	if(FAILED(hr))
	{
		LOG(LOG_ERROR, "CInputManager::Initialize", "Failed to set the cooperative level of the mouse.");
		return false;
	}

	//aquire the mouse
	hr = m_pMouse->Acquire();
	if(FAILED(hr))
	{
		LOG(LOG_ERROR, "CInputManager::Initialize", "Failed to acquire mouse.");
		//return false;
	}
	
	m_pGame = CGame::GetInstance();

	m_nMouseX = CGame::GetInstance()->GetScreenWidth()/2; 
	m_nMouseY = CGame::GetInstance()->GetScreenHeight()/2;
	
	//m_ptMousePos.x = m_nMouseX;
	//m_ptMousePos.y = m_nMouseY;

	SetCursorPos(m_nMouseX, m_nMouseY);
	//ScreenToClient(m_pGame->GetHWND(), &m_ptMousePos);

	
	//POINT ptCursor;
	//GetCursorPos(&ptCursor);
	ShowCursor(false);
	//ScreenToClient(m_pGame->GetHWND(), &ptCursor);
	//SetMousePosX(ptCursor.x);
	//SetMousePosY(ptCursor.y);

	return true;
}

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
void CInputManager::Update(void)
{
	//TODO: Run through the update for the input components.
	//read the current state of the keyboard
	ReadKeyboard();

	//read the current state of the mouse and set result to the return
	bool result;
	result = ReadMouse();

	if(result)
	{
		//process the changes in the mouse and keyboard.
		ProcessInput();
	}
}

/////////////////////////////////////////////////////////////////////
//ReadMouse():		Gets the current state of the mouse.
//
//Returns:			bool - true if the mouse was able to be read by
//							direct input.
//
//Mod Date:			5/23/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
void CInputManager::ClearInput(void)
{
	if(m_pKeyboard)
	{
		//	clear the currently pressed keys
		memset(m_chKeyboardState, 0, 256 * sizeof(unsigned char)); 
		//	clear the previously pressed keys
		memset(m_chPrevKeyboardState, 0, 256 * sizeof(unsigned char));
	}

	if(m_pMouse)
	{
		//	clear the currently pressed keys
		memset(m_diMouseState.rgbButtons, 0, 8*sizeof(BYTE)); 
		//	clear the previously pressed keys
		memset(m_diPrevMouseState.rgbButtons, 0, 8*sizeof(BYTE));
	}
}

/////////////////////////////////////////////////////////////////////
//ReadKeyboard():	Gets the current state of the keyboard.
//
//Returns:			bool - true if keyboard was able to be read by
//							direct input.
//
//Mod Date:			5/21/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::ReadKeyboard(void)
{
	HRESULT hr;

	//fill in the previouse
	memcpy(m_chPrevKeyboardState, m_chKeyboardState, sizeof(m_chPrevKeyboardState));

	//read the keyboard device
	hr = m_pKeyboard->GetDeviceState(sizeof(m_chKeyboardState), 
		(LPVOID)&m_chKeyboardState);

	if(FAILED(hr))
	{
		//if the keyboard lost focus or was not acquired
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			//try to get get control again.
			hr = m_pKeyboard->Acquire();
			if(FAILED(hr))
			{
				//LOG(LOG_ERROR, "CInputManager::ReadKeyboard", "Failed to acquire keyboard.");
				return false;
			}
		}
		else
		{
			//else it just failed
			return false;
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//ReadMouse():		Gets the current state of the mouse.
//
//Returns:			bool - true if the mouse was able to be read by
//							direct input.
//
//Mod Date:			5/23/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::ReadMouse(void)
{
	HRESULT hr;

	//fill in the previous
	memcpy(&m_diPrevMouseState, &m_diMouseState, sizeof(m_diPrevMouseState));

	//read the mouse
	if(m_pMouse == NULL)
	{
		return false;
	}
	
	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_diMouseState);
	if(FAILED(hr))
	{
		//if the mouse lost focus or was not acquired
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			//try to get control back
			hr = m_pMouse->Acquire();
			if(FAILED(hr))
			{
				//LOG(LOG_ERROR, "CInputManager::ReadMouse", "Failed to acquire Mouse.");
				return false;
			}
		}
		else
		{
			//else it just failed
			return false;
		}
	}

	//char message[50];
	//sprintf(message, "Mouse Movement: X:%d    Y:%d", m_diMouseState.lX, m_diMouseState.lY);
	//LOG(LOG_DEBUG, "CInputManager::ReadMouse", message);
	return true;
}

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
void CInputManager::ProcessInput(void)
{
	//update the location of the mouse cursor
	LONG lMovement = m_diMouseState.lX;
	if(lMovement > CGame::GetInstance()->GetScreenWidth())
	{
		lMovement = 0;
	}

	if(lMovement < -CGame::GetInstance()->GetScreenWidth())
	{
		lMovement = 0;
	}
	
	m_nMouseX += (int)(lMovement * m_fSensitivity);
	//m_ptMousePos.x = m_nMouseX;

	lMovement = m_diMouseState.lY;
	if(lMovement > CGame::GetInstance()->GetScreenHeight())
	{
		lMovement = 0;
	}

	if(lMovement < -CGame::GetInstance()->GetScreenHeight())
	{
		lMovement = 0;
	}

	m_nMouseY += (int)(lMovement * m_fSensitivity);
	
	m_ptMousePos.x = (LONG)(m_nMouseX);
	m_ptMousePos.y = (LONG)(m_nMouseY);

	ClientToScreen(m_pGame->GetHWND(), &m_ptMousePos);
	//SetCursorPos((int)(m_ptMousePos.x), (int)(m_ptMousePos.x));
	//m_ptMousePos.y = m_nMouseY;
	
	//
	//sprintf(message, "Mouse Movement: X:%d    Y:%d", m_diMouseState.lX, m_diMouseState.lY);
	//LOG(LOG_DEBUG, "CInputManager::ReadMouse", message);
	//
	//Logger::ClearConsole();
	//ensure the mouse location doesn't exceed the screen width or height
	if(m_nMouseX < 0)
	{
		m_nMouseX = 0;
	}
	
	if(m_nMouseY < 0)
	{
		m_nMouseY = 0;
	}
	
	if(m_nMouseX > CGame::GetInstance()->GetScreenWidth())
	{
		m_nMouseX = CGame::GetInstance()->GetScreenWidth();
	}
	
	if(m_nMouseY > CGame::GetInstance()->GetScreenHeight())
	{
		m_nMouseY = CGame::GetInstance()->GetScreenHeight();
	}

	

	//char message[50];
	//sprintf(message, "Mouse Pos: X:%d    Y:%d", m_nMouseX, m_nMouseY);
	//LOG(LOG_DEBUG, "CInputManager::ProcessInput", message);
}

/////////////////////////////////////////////////////////////////////
//KeyDown():		Determines whether the key is currently down.
//
//Returns:			bool - true if the key is down.
//
//Mod Date:			5/23/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::KeyDown(unsigned char chDIKCode)
{
	return (m_chKeyboardState[chDIKCode] & 0x80) ? true : false;
}

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
bool CInputManager::KeyPressed(unsigned char chDIKCode)
{
	return KeyDown(chDIKCode) && !(m_chPrevKeyboardState[chDIKCode] & 0x80);
}

/////////////////////////////////////////////////////////////////////
//KeyPressedEx():	The buffered version of KeyPressed()
//
//Returns:			bool - true if any of the keys pressed this frame
//							were the key that we were looking for.
//
//Mod Date:			5/24/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::KeyPressedEx(unsigned char chDIKCode)
{
	for(DWORD i = 0; i < m_dwBufferElements; i++)
	{
		if((m_didod[i].dwOfs == chDIKCode) && (m_didod[i].dwData & 0x80))
		{
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////
//KeyUp():			Gets if the specified key is currently up
//
//Returns:			bool - true if the key is up
//
//Mod Date:			5/24/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::KeyUp(unsigned char chDIKCode)
{
	return !KeyDown(chDIKCode);
}

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
bool CInputManager::KeyReleased(unsigned char chDIKCode)
{
	return KeyUp(chDIKCode) && (m_chPrevKeyboardState[chDIKCode] & 0x80);
}

/////////////////////////////////////////////////////////////////////
//KeyReleasedEx():	The buffered version of KeyReleased()
//
//Returns:			bool - true if keyboard was able to be read by
//							direct input.
//
//Mod Date:			5/24/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::KeyReleasedEx(unsigned char chDIKCode)
{
	for(DWORD i = 0; i < m_dwBufferElements; i++)
	{
		if((m_didod[i].dwOfs == chDIKCode) && !(m_didod[i].dwData & 0x80))
		{
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////
//CheckKeys():		Checks the current state and returns when it hits
//					the first key that is down
//
//Returns:			char - the first key that is down.
//
//Mod Date:			5/21/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
char CInputManager::CheckKeys(void)
{
	if(!GetKeyboardState(m_chAsciiKeys))
	{
		return NULL;
	}

	unsigned short sNum = 0;
	unsigned int nVKCode = 0;

	for(unsigned int i = 0; i < 256; i++)
	{
		if(KeyDown((unsigned char)i))
		{
			nVKCode = MapVirtualKeyEx(i, 1, m_keyLayout);

			ToAsciiEx(nVKCode, i, m_chAsciiKeys, &sNum, 0, m_keyLayout);
		}
	}

	return (char)sNum;
}

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
char CInputManager::CheckBufferedKeysEx(void)
{
	if(!(GetKeyboardState(m_chAsciiKeys)))
	{
		return NULL;
	}

	unsigned short sNum = 0;
	unsigned int nVKCode = 0;

	for(DWORD i = 0; i < m_dwBufferElements; i++)
	{
		for(unsigned int j = 0; j < 256; j++)
		{
			if((m_didod[i].dwOfs == j) && (m_didod[i].dwData & 0x80))
			{
				nVKCode = MapVirtualKeyEx(m_didod[i].dwOfs, 1, m_keyLayout);

				ToAsciiEx(nVKCode, m_didod[i].dwOfs, m_chAsciiKeys, &sNum, 0, m_keyLayout);
			}
		}
	}

	return (char)sNum;
}

/////////////////////////////////////////////////////////////////////
//GetDIKCode():		returns the DIK Code of the key
//
//Returns:			bool - true if keyboard was able to be read by
//							direct input.
//
//Mod Date:			5/21/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
unsigned char CInputManager::GetDIKCode(void)
{
	unsigned char chNum = 0;

	for(unsigned int i = 0; i < 256; i++)
	{
		if(KeyDown((unsigned char)i))
		{
			chNum = (unsigned char)i;
		}
	}

	return chNum;
}

/////////////////////////////////////////////////////////////////////
//GetBufferedDIKCodeEx():	buffered version of the GetDIKCode() function.
//
//Returns:					bool - true if keyboard was able to be 
//									read by direct input.
//
//Mod Date:					5/21/12
//Mod Initials:				JM
/////////////////////////////////////////////////////////////////////
unsigned char CInputManager::GetBufferedDIKCodeEx(void)
{
	unsigned char chNum = 0;

	for(DWORD i = 0; i < m_dwBufferElements; i++)
	{
		for(unsigned int j = 0; j < 256; j++)
		{
			if((m_didod[i].dwOfs == j) && (m_didod[i].dwData & 0x80))
			{
				chNum = (unsigned char)j;
				break;
			}
		}
	}

	return chNum;
}

/////////////////////////////////////////////////////////////////////
//MouseButtonUp():	Checks to see if the mouse button is not down
//
//Returns:			bool - true if mouse button is not down.
//
//Mod Date:			5/21/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::MouseButtonUp(unsigned char chButton)
{
	return !MouseButtonDown(chButton);
}

/////////////////////////////////////////////////////////////////////
//MouseButtonDown():Checks to see if the mouse button, chButton, is
//					currently down.
//
//Returns:			bool - true if the mouse button is down.
//							
//Mod Date:			5/21/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::MouseButtonDown(unsigned char chButton)
{
	return (m_diMouseState.rgbButtons[chButton] & 0x80) ? true : false;
}

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
bool CInputManager::MouseButtonPressed(unsigned char chButton)
{
	return (m_diMouseState.rgbButtons[chButton] & 0x80) && 
		!(m_diPrevMouseState.rgbButtons[chButton] & 0x80);
}

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
bool CInputManager::MouseButtonPressedEx(unsigned char chButton)
{
	for(DWORD i = 0; i < m_dwBufferElements; i++)
	{
		if((m_didod[i].dwOfs == (DWORD)(DIMOFS_BUTTON0 + chButton)) &&
			(m_didod[i].dwData & 0x80))
		{
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////
//MouseButtonReleased():checks if the mouse button was released
//
//Returns:			bool - true if the button was down in the previous
//							state, but is up in the current one.
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::MouseButtonReleased(unsigned char chButton)
{
	return MouseButtonUp(chButton) && 
		(m_diPrevMouseState.rgbButtons[chButton] & 0x80);
}

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
bool CInputManager::MouseButtonReleasedEx(unsigned char chButton)
{
	for(DWORD i = 0; i < m_dwBufferElements; i++)
	{
		if((m_didod[i].dwOfs == (DWORD)(DIMOFS_BUTTON0)) && 
			!(m_didod[i].dwData & 0x80))
		{
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////
//LeftMouseButtonDown():checks to see if the left mouse button is down.
//
//Returns:			bool - true if the left mouse button is down.
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::LeftMouseButtonDown(void)
{
	return (m_diMouseState.rgbButtons[0] & 0x80) ? true : false;
}

/////////////////////////////////////////////////////////////////////
//RightMouseButtonDown():Checks to see if the right mouse button is down.
//
//Returns:			bool - True if the right mouse button is down.
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
bool CInputManager::RightMouseButtonDown(void)
{
	return (m_diMouseState.rgbButtons[1] & 0x80) ? true : false;
}

/////////////////////////////////////////////////////////////////////
//LeftMouseButtonPressed():Buffered version of LeftMouseButtonDown()
//					function.
//
//Returns:			bool - True if the right mouse button is down.
//
//Mod Date:			5/25/12
//Mod Initials:		JM
/////////////////////////////////////////////////////////////////////
int CInputManager::LeftMouseButtonPressed(void)
{
	return ((m_diMouseState.rgbButtons[0] & 0x80) && 
		!(m_diPrevMouseState.rgbButtons[0] & 0x80));
}

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
int CInputManager::RightMouseButtonPressed(void)
{
	return ((m_diMouseState.rgbButtons[1] & 0x80) &&
		!(m_diPrevMouseState.rgbButtons[1] & 0x80));
}

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
void CInputManager::Shutdown(void)
{
	//release the mouse
	if(m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = NULL;
	}

	//release the keyboard
	if(m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
		m_pKeyboard = NULL;
	}

	//release the main interface to direct input
	if(m_pDI)
	{
		m_pDI->Release();
		m_pDI = NULL;
	}
}

long CInputManager::GetMouseMovementX(void)
{
	return (long)(m_diMouseState.lX * m_fSensitivity);
}

long CInputManager::GetMouseMovementY(void)
{
	return (long)(m_diMouseState.lY * m_fSensitivity);
}

void CInputManager::SetSensitivity(float fSensitivity)
{
	if(fSensitivity < 0.1f)
	{
		fSensitivity = 0.1f;
	}
	
	if(fSensitivity > 2.0f)
	{
		fSensitivity = 2.0f;
	}
	
	m_fSensitivity = fSensitivity;
}

D3DXVECTOR3 CInputManager::GetCursorPosition(void)
{
	POINT ptCursor;
	//GetCursorPos(&ptCursor);
	ptCursor.x = (LONG)m_nMouseX;
	ptCursor.y = (LONG)m_nMouseY;
	//ScreenToClient(m_pGame->GetHWND(), &ptCursor);

	//Caps the cursor to the screen
	if(ptCursor.x <= (LONG)0.0f)
		ptCursor.x = (LONG)0.0f;
	else if(ptCursor.x >= (LONG)CGame::GetInstance()->GetScreenWidth())
		ptCursor.x = (LONG)CGame::GetInstance()->GetScreenWidth();
	if(ptCursor.y <= (LONG)0.0f)
		ptCursor.y = (LONG)0.0f;
	else if(ptCursor.y >= (LONG)CGame::GetInstance()->GetScreenHeight())
		ptCursor.y = (LONG)CGame::GetInstance()->GetScreenHeight();

	//Sets the cursor to the center of the screen
	ptCursor.x -= (LONG)(CGame::GetInstance()->GetScreenWidth() / 2.0f);
	ptCursor.y += (LONG)(CGame::GetInstance()->GetScreenHeight() / 2.0f);

	//Sets the cursor to the new cursor 
	D3DXVECTOR3 d3dCursorPos;
	d3dCursorPos.x = (float)ptCursor.x;
	d3dCursorPos.y = (float)ptCursor.y;
	d3dCursorPos.z = 1.0f;

	return d3dCursorPos;
}

void CInputManager::OffSetMouseForStart(void)
{
	m_nMouseX = (int)(CGame::GetInstance()->GetScreenWidth() / 2.0f);// + (int)(CGame::GetInstance()->GetScreenWidth() / 35.0f);
	m_nMouseY = (int)(CGame::GetInstance()->GetScreenHeight() / 2.0f);// - (int)(CGame::GetInstance()->GetScreenHeight() / 35.0f);
}

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
void CInputManager::IgnoreInputForSetAmountOfTime( float _fDT, float _Timer )
{
	// If the timer is less than timer alloted increase timer.
	if( m_fIgnoreTimer < _Timer )
		m_fIgnoreTimer += _fDT;
	// If timer is greater than time alloted break out of function.
	if( m_fIgnoreTimer >= _Timer )
		return;

	// If we reached this point Clear out Input.
	ClearInput();
}