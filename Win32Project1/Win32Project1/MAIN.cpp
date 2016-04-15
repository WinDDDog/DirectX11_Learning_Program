#include <windows.h>
#include <windowsx.h>

#include <d3d11.h>

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 786

// COM pointers
ID3D11Device *dev;
ID3D11DeviceContext *devCon;
IDXGISwapChain *sc;
ID3D11RenderTargetView *backBuffer;

float color[4] = { 0.0, 0.2, 0.4, 1.0 };

void renderFrame()
{
	devCon->ClearRenderTargetView(backBuffer, color);

	if (color[1] < 1 && color[2] < 1 && color[0] < 1)
	{
		color[0] += (rand() % 100 + 1)*0.001;
		color[1] += (rand() % 100 + 1)*0.001;
		color[2] += (rand() % 100 + 1)*0.001;
	}
	else
	{
		color[0] -= (rand() % 100 + 1)*0.001;
		color[1] -= (rand() % 100 + 1)*0.001;
		color[2] -= (rand() % 100 + 1)*0.001;
	}

	// swap buffers
	sc->Present(0, 0);
}

void initD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &sc, &dev, NULL, &devCon);

	ID3D11Texture2D *t2D;
	sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&t2D);
	dev->CreateRenderTargetView(t2D, NULL, &backBuffer);
	t2D->Release();
	devCon->OMSetRenderTargets(1, &backBuffer, NULL);

}

void cleanD3D()
{
	sc->SetFullscreenState(FALSE, NULL);

	dev->Release();
	devCon->Release();
	backBuffer->Release();
	sc->Release();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lCmdLIne, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass1";

	RegisterClassEx(&wc);

	RECT wRect = { 0, 0, 600, 600 };
	AdjustWindowRect(&wRect, WS_GROUP, FALSE);
	HWND hWnd = CreateWindowEx(NULL, L"WindowClass1", L"BUCH MEGA WINDOW", WS_GROUP, 0, 0, wRect.right - wRect.left, wRect.bottom - wRect.top, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, SW_SHOW);

	initD3D(hWnd);

	MSG msg = { 0 };

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) break;
		}
		else
		{
			renderFrame();
			// 50 FPS  (1000/20)
			Sleep(20);
		}
	}

	cleanD3D();

	return msg.wParam;
}