#include "My_D3D_APP_Base.h"

My_D3d_APP::My_D3d_APP()
{

}
My_D3d_APP::~My_D3d_APP()
{
	printf("Clean D3d-----End\n");
}
BOOL My_D3d_APP::Init(const HWND hwnd)
{
	dev = NULL;
	devContext = NULL;
	SwapChain = NULL;
	depthStencilView = NULL;
	backBuffer = NULL;

	if (CreateD3DObject(hwnd)) printf("Init D3d ------OK\n");
	else printf("Error in CreateD3DObject");

	return TRUE;
}

BOOL My_D3d_APP::CreateD3DObject(const HWND hwnd)
{

	//初始化交换链

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = 1200;
	scd.BufferDesc.Height = 600;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//创建设备与缓冲区
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &SwapChain, &dev, NULL, &devContext);
	if (FAILED(hr))
	{
		return FALSE;
	}

	ID3D11Texture2D *t2D;
	//获取后缓冲区地址
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&t2D);

	//创建视图  
	dev->CreateRenderTargetView(t2D, NULL, &backBuffer);
	//释放后缓冲区引用  
	t2D->Release();
	//创建深度缓冲区视图
	//要先创建对应缓冲区
	//创建缓冲区Texture2D，要先给出描述
	UINT msaa4xQuality(0);
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = 1200;
	desc.Height = 600;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 4;
	desc.SampleDesc.Quality = msaa4xQuality - 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	ID3D11Texture2D *depthStencilBuffer(NULL);
	hr = dev->CreateTexture2D(&desc, NULL, &depthStencilBuffer);
	if (FAILED(hr))
	{
		return FALSE;
	}
	hr = dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	if (FAILED(hr))
	{
		return FALSE;
	}
	//有了视图要绑定到管线相应阶段
	devContext->OMSetRenderTargets(1, &backBuffer, NULL);
	depthStencilBuffer->Release();

	//最后设置portview
	D3D11_VIEWPORT viewPort = { 0 };
	viewPort.Width = static_cast<FLOAT>(1200);
	viewPort.Height = static_cast<FLOAT>(600);
	viewPort.MinDepth = 0.f;
	viewPort.MaxDepth = 1.f;
	viewPort.TopLeftX = 0.f;
	viewPort.TopLeftY = 0.f;
	devContext->RSSetViewports(1, &viewPort);

	return TRUE;
}