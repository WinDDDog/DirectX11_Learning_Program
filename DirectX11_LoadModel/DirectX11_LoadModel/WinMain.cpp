#include "DrawFunction.h"

//windows程序入口m函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	static TCHAR szAppName[] = TEXT("DirectX11_FrameWork");
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
	RECT rc = { 0, 0, 800, 600 };
	hwnd = CreateWindow(szAppName,      // window class name
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

	ShowWindow(hwnd, iCmdshow);
	UpdateWindow(hwnd);

#ifdef _DEBUG
	FILE *stream;	//debug下需要一个控制台输出调试信息
	AllocConsole();
	freopen_s(&stream, "CONOUT$", "w", stdout);
#endif
	DrawFunction my_d3d_haha;
	my_d3d_haha.Init(hwnd);		//创建基本的D3D对象，设备，交换链，视窗等

	if (FAILED(my_d3d_haha.CompileTheShader())) return FALSE;
	else printf("CompileTheShader ----OK!\n");

	if (FAILED(my_d3d_haha.CreateGraph()))return FALSE;
	else printf("CreateGraph ----OK!\n");



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
			my_d3d_haha.RenderScene();
			Sleep(15);
		}
	}
	//Release;
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
