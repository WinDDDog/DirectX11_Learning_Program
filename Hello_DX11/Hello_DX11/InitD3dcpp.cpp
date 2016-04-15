#include "MyD3D_Class.h"

//D3D相关全局变量
ID3D11Device		*dev = NULL;
ID3D11DeviceContext	*devContext = NULL;
IDXGISwapChain		*SwapChain = NULL;

ID3D11DepthStencilView	*depthStencilView = NULL;
ID3D11RenderTargetView	*backBuffer = NULL;
float color[4] = { 0.0, 1.0, 0.0, 1.6 };
void Release()
{
	SwapChain->SetFullscreenState(FALSE, NULL);

	dev->Release();
	devContext->Release();
	backBuffer->Release();
	SwapChain->Release();
}
void Render()
{
	devContext->ClearRenderTargetView(backBuffer, reinterpret_cast<float*>(&color));
	devContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
	SwapChain->Present(0, 0);
}
BOOL CreateD3DObject(HWND hwnd)
{
	//初始化交换链
	/*
typedef struct DXGI_SWAP_CHAIN_DESC {  
  DXGI_MODE_DESC   BufferDesc;		//特指缓冲区的特性
  DXGI_SAMPLE_DESC SampleDesc;		//多重采样？抗锯齿？
  DXGI_USAGE       BufferUsage;		//对于交换链，为DXGI_USAGE_RENDER_TARGET_OUTPUT
  UINT             BufferCount;		//我们只创建一个后缓冲区（双缓冲），因此为1
  HWND             OutputWindow;	//指定窗口句柄，使用Windows窗体程序初始化完创建的主窗口句柄
  BOOL             Windowed;		//FASE 全屏显示，TRUE窗口显示
  DXGI_SWAP_EFFECT SwapEffect;		//通常为DXGI_SWAP_EFFECT_DISCARD
  UINT             Flags;			//可选，我们设为0
} DXGI_SWAP_CHAIN_DESC;  

typedef struct DXGI_MODE_DESC {
UINT                     Width;			//缓冲区宽，与分辨率有关？
UINT                     Height;		//缓冲区高，与分辨率有关？
DXGI_RATIONAL            RefreshRate;	//   其他参数一般为固定的，参见代码。
DXGI_FORMAT              Format;
DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
DXGI_MODE_SCALING        Scaling;
} DXGI_MODE_DESC, *LPDXGI_MODE_DESC;

*/
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
/*HRESULT  D3D11CreateDeviceAndSwapChain(  
 __in   IDXGIAdapter *pAdapter,			//选择图形适配器
 __in   D3D_DRIVER_TYPE DriverType,		//驱动类型，一般为D3D_DRIVER_TYPE_HARDWARE
 __in   HMODULE Software,				//NULL即可
 __in   UINT Flags,						// Flags为可选参数，一般为NULL
 __in   const D3D_FEATURE_LEVEL *pFeatureLevels,  // pFeatureLevels为我们提供给程序的特征等级的一个数组，下一个参数为数组中元素个数
 __in   UINT FeatureLevels,  
 __in   UINT SDKVersion,				// SDKVersion恒定为D3D11_SDK_VERSION；
 __in   const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,	//DXGI_SWAP_CHAIN_DESC初始化的交换链
 __out  IDXGISwapChain       **ppSwapChain,	//返回一个IDXGISwapChain对象的指针地址，这个交换链接用于渲染（二维指针）
 __out  ID3D11Device **ppDevice,		//  ppDevice为设备指针的地址，注意设备是指针类型，这里传递的是指针的地址（二维指针，d3d程序中所有的接口都声明为指针类型！）
 __out  D3D_FEATURE_LEVEL *pFeatureLevel, //    pFeatureLevel为最后程序选中的特征等级，我们定义相应的变量，传递它的地址进来； 
 __out  ID3D11DeviceContext **ppImmediateContext  //    pFeatureLevel为最后程序选中的特征等级，我们定义相应的变量，传递它的地址进来
;  
返回值
HRESULT
这个函数返回Direct3D 11返回值。
*/
	//创建设备与缓冲区
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &SwapChain, &dev, NULL, &devContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"创建d3d11设备失败！", L"错误", MB_OK);
		return FALSE;
	}

	ID3D11Texture2D *t2D;
	//获取后缓冲区地址
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&t2D);
	/*
	HRESULT CreateRenderTargetView
	(
	ID3D11Resource *pResource,             //视图对应资源
	const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,        //视图描述
	ID3D11RenderTargetView **ppRTView          //要创建的视图（指针的地址）
	);  */
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
		MessageBox(NULL, L"Create d3d11 depthStencil Fail！", L"Fail", MB_OK);
		return FALSE;
	}
	hr = dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create DepthStencilView Fai！", L"Fail", MB_OK);
		return FALSE;
	}
	//有了视图要绑定到管线相应阶段
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
