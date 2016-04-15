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
	//注意，由于CreateWindows的创建窗口大小是包含这个窗口边框的，所以需要使用AdjustWindowRect去适应我们的clent大小。
	RECT rc = { 0, 0, Width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
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

	case WM_MOUSEMOVE:
		return 0;

	case WM_LBUTTONDOWN:
	{
		POINT Mouse_Move;

		Mouse_Move.x = LOWORD(lParam);
		Mouse_Move.y = HIWORD(lParam);
		printf("Left Button Down   X:%d  ， Y:%d \n", LOWORD(lParam), HIWORD(lParam));

		//pick
		DirectX::XMMATRIX P = View_Projection;

		DirectX::XMFLOAT4 p00;
		DirectX::XMStoreFloat4(&p00, P.r[0]);

		DirectX::XMFLOAT4 p11;
		DirectX::XMStoreFloat4(&p11, P.r[1]);

		float vx = (+2.0f*Mouse_Move.x / Width - 1.0f) / p11.x;
		float vy = (-2.0f*Mouse_Move.y / height + 1.0f) / p11.y;

		// 视空间中的射线定义
		DirectX::XMVECTOR rayOrigin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		DirectX::XMVECTOR rayDir = DirectX::XMVectorSet(vx, vy, 1.0f, 0.0f);

		//射线转换到世界空间

		rayOrigin = DirectX::XMVector3TransformCoord(rayOrigin, back_View__);
		rayDir = DirectX::XMVector3TransformNormal(rayDir, back_View__);
		//单位化射线向量
		rayDir = DirectX::XMVector3Normalize(rayDir);
		//////
		//注意，这里从理论上来说，需要将射线再次转换到物体空间去，但是这里只是一个demon暂时不考虑这个问题
		//
		return 0;
	}
	case WM_LBUTTONUP:
		printf("Left Button Up\n");
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
