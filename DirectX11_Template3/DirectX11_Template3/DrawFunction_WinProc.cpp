#include "DrawFunction.h"
namespace
{
//用于保存winproc指针
	DrawFunction* pWindow = NULL;
}
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT DrawFunction::WinMain_InitWindow(HINSTANCE hInstance, int iCmdshow)
{
	static TCHAR szAppName[] = TEXT("DirectX11_FrameWork");

	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = MainWndProc;
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
	RECT rc = { 0, 0, Width, height };
	Main_Window_HWND = CreateWindow(szAppName,      // window class name
		TEXT("The DirectX11 Program"),		// window caption
		WS_OVERLAPPEDWINDOW,			// window style
		CW_USEDEFAULT,	// initial x position
		CW_USEDEFAULT,	// initial y position
		rc.right - rc.left,	// initial x size
		rc.bottom - rc.top,	// initial y size
		NULL,           // parent window handle
		NULL,            // window menu handle
		hInstance,		// program instance handle
		NULL);			// creation parameters

	ShowWindow(Main_Window_HWND, iCmdshow);
	UpdateWindow(Main_Window_HWND);

	pWindow = this;
	return 0;
}
void DrawFunction::Run()
{
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
			RenderScene();
			Sleep(15);
		}
	}
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


	bool bProcessed = false;

	switch (msg)
	{
	case WM_NCCREATE:
		break;

	default:
		if (NULL != pWindow)
		{
			bProcessed = true;
			return pWindow->WndProc(hwnd, msg, wParam, lParam);
		}
		break;
	}

	if (false == bProcessed)
	{
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);

}

LRESULT CALLBACK DrawFunction::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC             hdc;
	PAINTSTRUCT		ps;

	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_PAINT:		//WM_PAINT这个消息必须存在，BeginPaint，EndPaint必须存在，否则无法绘制
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
