#include "DrawFunction.h"
namespace
{
	// This is just used to forward Windows messages from a global window
	// procedure to our member function window procedure because we cannot
	// assign a member function to WNDCLASS::lpfnWndProc.
	DrawFunction* pWindow = NULL;
}
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT DrawFunction::WinMain_InitWindow(HINSTANCE hInstance, int iCmdshow)
{
	ShowCursor(FALSE);
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
	RECT rc = { 0, 0, 800, 600 };
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
			if (Mouse_Move_Forwad_Back != 0)
				MoveCamera_ForwadBack(Mouse_Move_Forwad_Back*((float)+0.04));

			if (Mouse_Move_Righr_Left != 0)
				MoveCamera_LeftRight(Mouse_Move_Righr_Left*((float)+0.04));
			RenderScene();
			Sleep(15);
		}
	}
	
	//Release;
	//return msg.wParam;
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
	case WM_MOUSEMOVE:
		POINT Mouse_Move;

	
		Mouse_Move.x = LOWORD(lParam);
		Mouse_Move.y = HIWORD(lParam);

		SendInput(1, &mouseInput, sizeof(INPUT));
		ClientToScreen(Main_Window_HWND, &Mouse_Move);
		//printf("Mouse Move :%ld, %ld\n", Mouse_Move.x - Window_Center_X, Mouse_Move.y - Window_Center_Y);
		Mouse_Move.x = Mouse_Move.x - Window_Center_X;//不能这样做，有误差！！
		Mouse_Move.y = Mouse_Move.y - Window_Center_Y;

		RotateBy_Y((float)(0.0015*Mouse_Move.x));
		RotateBy_X((float)(0.0015*Mouse_Move.y));
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 38://up
			if (Mouse_Move_Forwad_Back != 1)
				Mouse_Move_Forwad_Back = 1;
			//MoveCamera_ForwadBack((float)+0.2);
			//RotateBy_X((float)+0.05);

			break;
		case 40://down
			if (Mouse_Move_Forwad_Back != -1)
				Mouse_Move_Forwad_Back = -1;
			//MoveCamera_ForwadBack((float)-0.2);
			//RotateBy_X((float)-0.05);

			break;
		case 37://left
			if (Mouse_Move_Righr_Left != -1)
				Mouse_Move_Righr_Left = -1;
			//MoveCamera_LeftRight((float)-0.2);
			break;
		case 39://right
			if (Mouse_Move_Righr_Left != 1)
				Mouse_Move_Righr_Left = 1;

			//MoveCamera_LeftRight((float)+0.2);
			break;
		default:
			break;
		}
		return 0;

	case WM_KEYUP:
		switch (wParam)
		{
		case 38://up
			if (Mouse_Move_Forwad_Back != -1)
				Mouse_Move_Forwad_Back = 0;
			break;
		case 40://down
			if (Mouse_Move_Forwad_Back != 1)
				Mouse_Move_Forwad_Back = 0;
			break;
		case 37://left
			if (Mouse_Move_Righr_Left != 1)
			Mouse_Move_Righr_Left = 0;
			break;
		case 39://right
			if (Mouse_Move_Righr_Left != -1)
			Mouse_Move_Righr_Left = 0;
			break;
		default:
			break;
		}
		return 0;

	case WM_MOVE:
		if (FALSE == SetMouseMoveCenter())
			printf("Error in Set Mouse Move Center\n");

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
