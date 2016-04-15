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
	/*
	UINT msaa4xQuality(0);
	D3D11_TEXTURE2D_DESC desc = { 0 };
	*/
	/*
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
	*/
	/*
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = 1200;
	desc.Height = 600;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	ID3D11Texture2D *depthStencilBuffer(NULL);
	hr = dev->CreateTexture2D(&desc, NULL, &depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, 0, 0, 0);
		return FALSE;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = desc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = dev->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, &depthStencilView);
	if (FAILED(hr))
	{
		return FALSE;
	}

	devContext->OMSetRenderTargets(1, &backBuffer, depthStencilView);

	//有了视图要绑定到管线相应阶段
	devContext->OMSetRenderTargets(1, &backBuffer, NULL);
	depthStencilBuffer->Release();
	*/
	
	//日了狗
	ID3D11Texture2D *depthStencilBuffer(NULL);
	//测试多重采样抗锯齿等级
	UINT x4MsaaQuality;
	dev->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &x4MsaaQuality);
	printf("CheckMultisampleQualityLevels:%d\n", x4MsaaQuality);
	//
	D3D11_TEXTURE2D_DESC dsDesc;
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsDesc.Width = 1200;
	dsDesc.Height = 600;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.MipLevels = 1;
	dsDesc.ArraySize = 1;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.SampleDesc.Count = (x4MsaaQuality<1?1:4);///////日了狗，为什么呢，多重采样抗锯齿
	dsDesc.SampleDesc.Quality = (x4MsaaQuality<1 ? 0 : x4MsaaQuality - 1);
	dsDesc.MiscFlags = 0;
	dsDesc.Usage = D3D11_USAGE_DEFAULT;
	hr = dev->CreateTexture2D(&dsDesc, 0, &depthStencilBuffer);
	if (FAILED(hr))
	{
		//MessageBox(NULL, _T("Create depth stencil buffer failed!"), _T("ERROR"), MB_OK);
		return false;
	}
	hr = dev->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView);
	if (FAILED(hr))
	{
		//MessageBox(NULL, L("Create depth stencil view failed!"), _T("ERROR"), MB_OK);
		return false;
	}
	devContext->OMSetRenderTargets(1, &backBuffer, depthStencilView);
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