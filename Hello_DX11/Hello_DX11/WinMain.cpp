#include <Windows.h>
#include <stdio.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <iostream>
using namespace std;
//windows程序入口main函数
ostream& operator<<(ostream& os, DirectX::FXMVECTOR v)
{
	DirectX::XMFLOAT4 dest;
	DirectX::XMStoreFloat4(&dest, v);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
	return os;
}

ostream& operator<<(ostream& os, DirectX::CXMMATRIX m)
{
	for (int i = 0; i < 4; ++i)
	{
		os << m.r[i]<< "\t";
		os << endl;
	}
	return os;
}
void Render();
void Release();
BOOL CreateD3DObject(HWND hwnd);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	static TCHAR szAppName[] = TEXT("HelloWin");
	HWND   hwnd;
	MSG    msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,      // window class name
		TEXT("The Hello Program"),		// window caption
		WS_OVERLAPPEDWINDOW,			// window style
		CW_USEDEFAULT,	// initial x position
		CW_USEDEFAULT,	// initial y position
		CW_USEDEFAULT,	// initial x size
		CW_USEDEFAULT,	// initial y size
		NULL,           // parent window handle
		NULL,            // window menu handle
		hInstance,		// program instance handle
		NULL);			// creation parameters

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
#ifdef _DEBUG
	FILE *stream;	//debug下需要一个控制台输出调试信息
	AllocConsole();
	freopen_s(&stream, "CONOUT$", "w", stdout);
	DirectX::XMMATRIX v 
		(1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 2.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 4.0f, 0.0f,
		 1.0f, 2.0f, 3.0f, 1.0f);

	std::cout << "1" << std::endl << v << std::endl;
#endif
	CreateD3DObject(hwnd);
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// z
			Render();
		}
	}
	Release();
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC             hdc;
	PAINTSTRUCT		ps;
	RECT			rect;

	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("Hello"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		return 0;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
