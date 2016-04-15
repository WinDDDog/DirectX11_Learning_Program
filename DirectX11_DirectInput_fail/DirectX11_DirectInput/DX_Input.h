#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#include <Windows.h>
#include <stdio.h>
#include <iostream>

// PRE-PROCESSING DIRECTIVES //
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
class DX_Input
{
public:
	DX_Input();
	DX_Input(const DX_Input&);
	virtual ~DX_Input();

	HRESULT Initialize(HINSTANCE, HWND, int, int);
	bool Frame();
	bool IsEscapePressed();
private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
	
	int m_mouseX, m_mouseY;

protected:
};
/*
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};
*/
#endif