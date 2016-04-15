#include "DX_Input.h"

DX_Input::DX_Input()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}

DX_Input::~DX_Input()
{
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

}

HRESULT DX_Input::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT hr = S_OK;	//ret

	m_mouseX = 0;
	m_mouseY = 0;

	hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr)) return hr;

	hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(hr)) return hr;

	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) return hr;

	// Set the cooperative level of the keyboard to not share with other programs.
	hr = m_keyboard->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);//！！这里设为DISCL_FOREGROUND前台就会出现错误!!
	if (FAILED(hr)) return hr;

	// Now acquire the keyboard.
	hr = m_keyboard->Acquire();
	if (FAILED(hr))	return hr;

	hr = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(hr))	return hr;

	// Set the data format for the mouse using the pre-defined mouse data format.
	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))	return hr;

	// Set the cooperative level of the mouse to share with other programs.
	hr = m_mouse->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))	return hr;

	// Acquire the mouse.
	hr = m_mouse->Acquire();
	if (FAILED(hr))	return hr;

	return hr;
}
bool DX_Input::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	
	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}
	

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}
bool DX_Input::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool DX_Input::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}
void DX_Input::ProcessInput()
{
	if (m_mouseState.lX != 0 || m_mouseState.lY != 0)
	{
		m_mouseX += m_mouseState.lX;
		m_mouseY += m_mouseState.lY;

		printf("Mouse:%d,%d   location:%d,%d\n", m_mouseState.lX, m_mouseState.lY, m_mouseX, m_mouseY);
	}
	/*
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0)  { m_mouseX = 0; }
	if (m_mouseY < 0)  { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth)  { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
	*/
	return;
}
bool DX_Input::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		printf("KeyBoard:%d\n", m_keyboardState[DIK_ESCAPE]);
		return true;
	}
	return false;
}