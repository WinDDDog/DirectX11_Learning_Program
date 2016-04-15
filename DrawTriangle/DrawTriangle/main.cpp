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
	[in]                 UINT                StartSlot,	//���㻺������Ҫ�󶨵���ʼ����ۡ�
	[in]                 UINT                NumBuffers,//���㻺������Ҫ�󶨵�����۵�����
	[in, optional]       ID3D11Buffer *const *ppVertexBuffers,//ָ�򶥵㻺��������ĵ�һ��Ԫ�ص�ָ��
	[in, optional] const UINT                *pStrides,	//ָ�򲽳�����ĵ�һ��Ԫ�ص�ָ��,���������ָ���㻺�����еĵ�Ԫ�ص��ֽڳ���
	[in, optional] const UINT                *pOffsets	//ָ��ƫ������ĵ�һ��Ԫ�ص�ָ��
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
  [in] UINT VertexCount,	//��Ҫ���ƵĶ�������
  [in] UINT StartVertexLocation//������ʼ���Ƶ�ƫ��λ�ã�����ӵ�һ����ʼ��ƫ��Ϊ0
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
	//�������㻺����
	/*
	typedef struct D3D11_BUFFER_DESC{
		UINT ByteWidth;		//���ǽ�Ҫ�����Ķ��㻺�����Ĵ�С����λΪ�ֽ�
		D3D11_USAGE Usage;  //һ������ָ����������;��D3D11_USAGEö�����ͳ�Ա����4����ѡֵ
		UINT BindFlags;		//���ڶ��㻺�������ò���Ӧ��ΪD3D11_BIND_VERTEX_BUFFER
		UINT CPUAccessFlags;//ָ��CPU����Դ�ķ���Ȩ�ޡ�����Ϊ0���ʾCPU�����д���塣���CPU��Ҫ����Դд�����ݣ���Ӧָ��D3D11_CPU_ACCESS_WRITE
		UINT MiscFlags;		//���ǲ���ҪΪ���㻺����ָ���κ����miscellaneous����־ֵ�����Ըò�����Ϊ0
		UINT StructureByteStride;	//�洢�ڽṹ�������е�һ��Ԫ�صĴ�С�����ֽ�Ϊ��λ���������ֻ���ڽṹ�����壬���������������Ϊ0����ν�ṹ�����壬��ָ�洢���е�Ԫ�ش�С����ȵĻ���
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
	[in]            const D3D11_BUFFER_DESC      *pDesc,		//�����D3D11_BUFFER_DESC
	[in, optional]  const D3D11_SUBRESOURCE_DATA *pInitialData,	//��ʾ��ʼ��ʱ�����ݣ���������ֻ����ռ䣬����NULL
	[out, optional]       ID3D11Buffer           **ppBuffer		//���ش�����Buffer��ַ��zָ��
	);
	*/
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = OurVertices;
	dev->CreateBuffer(&bd, &vinitData, &pVBuffer);       // create the buffer
	


	// copy the vertices into the buffer
	//Ϊ������ָ����ʼ������
	/*
	typedef struct D3D11_SUBRESOURCE_DATA { 
    const void *pSysMem;	//pSysMem��������ʼ�����ݵ�ϵͳ�ڴ������ָ�롣�����������Դ洢n������ʱ����Ӧ�ĳ�ʼ������ҲӦ���ٰ���n������
    UINT SysMemPitch;		//���㻺������ʹ�øò�����
    UINT SysMemSlicePitch;	//���㻺������ʹ�øò�����
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
	LPCSTR SemanticName;	//һ����Ԫ����ص��ַ��������������κ���Ч�������������ڽ�����ṹ���е�Ԫ��ӳ��Ϊ������ɫ������
	UINT SemanticIndex;		//�����������ϵ�����ֵ������һ������ṹ����Ҫ�����������꣬���ǲ���Ҫһ���µ����������������������ĺ������һ������ֵ���μ���3��4���
	DXGI_FORMAT Format;		//һ������ָ��Ԫ�ظ�ʽ��DXGI_FORMATö�����ͳ�Ա���̶��ĸ�ʽ��
	UINT InputSlot;			//ָ����ǰԪ���������ĸ������(0~15),���ھ�һ������ʱΪ0
	UINT AlignedByteOffset  //��1��float4�ֽڣ��Ƚ��ѽ��ͣ���Ҫ�о�һ��ԭ��
	D3D11_INPUT_CLASSIFICATION InputSlotClass;	//������ʱָ��ΪD3D11_INPUT_PER_VERTEX_DATA
	UINT InstanceDataStepRate;	//������ʱָ��Ϊ0
	} D3D11_INPUT_ELEMENT_DESC;
	���磺
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
	const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,	//һ��������������ṹ���D3D11_INPUT_ELEMENT_DESC���飨����Ľṹ�壩
	UINT NumElements,									//�����Ԫ����������1��ʼ��
	const void *pShaderBytecodeWithInputSignature,		//ָ�򶥵���ɫ���������ֽ����ָ��
	SIZE_T BytecodeLength,								//������ɫ���������ֽ��볤�ȣ���λΪ�ֽڡ�
	ID3D11InputLayout **ppInputLayout					//���ش������ID3D11InputLayoutָ��
	);
	*/
	dev->CreateInputLayout(ied, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &pLayout);

	devcon->IASetInputLayout(pLayout);
}