// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>

#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>


using namespace DirectX;

// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader
ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer

// a struct to define a single vertex
struct VERTEX{ FLOAT X, Y, Z; XMFLOAT4 Color; };

// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void RenderFrame(void);     // renders a single frame
void CleanD3D(void);        // closes Direct3D and releases memory
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;
	
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"Wow a Triangle",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	InitD3D(hWnd);

	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		RenderFrame();
	}

	// clean up DirectX and COM
	CleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
					   PostQuitMessage(0);
					   return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = hWnd;                               // the window to be used
	scd.SampleDesc.Count = 4;                              // how many multisamples
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);


	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	devcon->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphics();
}


// this is the function used to render a single frame
void RenderFrame(void)
{
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	// clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(backbuffer, reinterpret_cast<float*>(color));

	// select which vertex buffer to display
	/*

	void IASetVertexBuffers(
	[in]                 UINT                StartSlot,	//顶点缓冲区所要绑定的起始输入槽。
	[in]                 UINT                NumBuffers,//顶点缓冲区所要绑定的输入槽的数量
	[in, optional]       ID3D11Buffer *const *ppVertexBuffers,//指向顶点缓冲区数组的第一个元素的指针
	[in, optional] const UINT                *pStrides,	//指向步长数组的第一个元素的指针,这个步长是指顶点缓冲区中的单元素的字节长度
	[in, optional] const UINT                *pOffsets	//指向偏移数组的第一个元素的指针
	);

	*/

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// select which primtive type we are using
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	/*
	void Draw(
  [in] UINT VertexCount,	//需要绘制的顶点数量
  [in] UINT StartVertexLocation//顶点起始绘制的偏移位置，这里从第一个开始，偏移为0
	);
*/
	devcon->Draw(3, 0);

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}


// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
	swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

	// close and release all existing COM objects
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	pVBuffer->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}


// this is the function that creates the shape to render
void InitGraphics()
{
	// create a triangle using the VERTEX struct
	VERTEX OurVertices[] =
	{
		{ 0.0f, 0.5f, 0.0f, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ 0.0f, -0.5f, 0.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ -0.45f, -0.0f, 0.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
	};


	// create the vertex buffer
	//创建顶点缓冲区
	/*
	typedef struct D3D11_BUFFER_DESC{
		UINT ByteWidth;		//我们将要创建的顶点缓冲区的大小，单位为字节
		D3D11_USAGE Usage;  //一个用于指定缓冲区用途的D3D11_USAGE枚举类型成员。有4个可选值
		UINT BindFlags;		//对于顶点缓冲区，该参数应设为D3D11_BIND_VERTEX_BUFFER
		UINT CPUAccessFlags;//指定CPU对资源的访问权限。设置为0则表示CPU无需读写缓冲。如果CPU需要向资源写入数据，则应指定D3D11_CPU_ACCESS_WRITE
		UINT MiscFlags;		//我们不需要为顶点缓冲区指定任何杂项（miscellaneous）标志值，所以该参数设为0
		UINT StructureByteStride;	//存储在结构化缓冲中的一个元素的大小，以字节为单位。这个属性只用于结构化缓冲，其他缓冲可以设置为0。所谓结构化缓冲，是指存储其中的元素大小都相等的缓冲
	} D3D11_BUFFER_DESC;
	*/
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX)* 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	bd.StructureByteStride = 0;
	bd.MiscFlags = 0;

	/*
	HRESULT CreateBuffer(
	[in]            const D3D11_BUFFER_DESC      *pDesc,		//上面的D3D11_BUFFER_DESC
	[in, optional]  const D3D11_SUBRESOURCE_DATA *pInitialData,	//表示初始化时的数据，这里由于只分配空间，就填NULL
	[out, optional]       ID3D11Buffer           **ppBuffer		//返回创建的Buffer地址的z指针
	);
	*/
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = OurVertices;
	dev->CreateBuffer(&bd, &vinitData, &pVBuffer);       // create the buffer
	


	// copy the vertices into the buffer
	//为缓冲区指定初始化数据
	/*
	typedef struct D3D11_SUBRESOURCE_DATA { 
    const void *pSysMem;	//pSysMem：包含初始化数据的系统内存数组的指针。当缓冲区可以存储n个顶点时，对应的初始化数组也应至少包含n个顶点
    UINT SysMemPitch;		//顶点缓冲区不使用该参数。
    UINT SysMemSlicePitch;	//顶点缓冲区不使用该参数。
	} D3D11_SUBRESOURCE_DATA;
	*/
	/*
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
	*/
}


// this function loads and prepares the shaders
void InitPipeline()
{
	ID3D10Blob* vertexShaderBuffer = NULL;
	ID3D10Blob* errorMessage =	NULL;
	ID3D10Blob* pixelShaderBuffer = NULL;
	// load and compile the two shaders
	//ID3D10Blob *VS, *PS;
	D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);

	// encapsulate both shaders into shader objects
	dev->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// create the input layout object
	/*
	typedef struct D3D11_INPUT_ELEMENT_DESC {
	LPCSTR SemanticName;	//一个与元素相关的字符串。它可以是任何有效的语义名，用于将顶点结构体中的元素映射为顶点着色器参数
	UINT SemanticIndex;		//附加在语义上的索引值，比如一个顶点结构体需要多组纹理坐标，我们不需要一个新的语义名，而是在语义名的后面加上一个索引值（参见第3，4项）。
	DXGI_FORMAT Format;		//一个用于指定元素格式的DXGI_FORMAT枚举类型成员，固定的格式。
	UINT InputSlot;			//指定当前元素来自于哪个输入槽(0~15),现在就一个，暂时为0
	UINT AlignedByteOffset  //（1个float4字节）比较难解释，需要研究一下原理
	D3D11_INPUT_CLASSIFICATION InputSlotClass;	//现在暂时指定为D3D11_INPUT_PER_VERTEX_DATA
	UINT InstanceDataStepRate;	//现在暂时指定为0
	} D3D11_INPUT_ELEMENT_DESC;
	例如：
	D3D11_INPUT_ELEMENT_DESC desc2[]=
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 12,
	D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
	D3D11_INPUT_PER_VERTEX_DATA, 0}
	{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 32,
	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	*/
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	/*
	HRESULT ID3D11Device::CreateInputLayout
	(
	const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,	//一个用于描述顶点结构体的D3D11_INPUT_ELEMENT_DESC数组（上面的结构体）
	UINT NumElements,									//数组的元素数量（从1开始）
	const void *pShaderBytecodeWithInputSignature,		//指向顶点着色器参数的字节码的指针
	SIZE_T BytecodeLength,								//顶点着色器参数的字节码长度，单位为字节。
	ID3D11InputLayout **ppInputLayout					//返回创建后的ID3D11InputLayout指针
	);
	*/
	dev->CreateInputLayout(ied, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &pLayout);

	devcon->IASetInputLayout(pLayout);
}