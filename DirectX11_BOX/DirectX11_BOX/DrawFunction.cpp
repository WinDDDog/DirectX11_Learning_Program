#include "DrawFunction.h"
extern float ChangeLevel;
extern float ChangeVertical;
DrawFunction::DrawFunction()
{

}
DrawFunction::~DrawFunction()
{
	pVertexShader->Release();
	pPixelShader->Release();
	pVertexLayout->Release();
	pVertexBuffer->Release();
	pIndexBuffer->Release();
	pConstantBuffer->Release();
}
void DrawFunction::RenderScene()
{
	//通过时间改变world坐标

	// Update our time
	static float t = 0.0f;

	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;

	//

	// Animate the cube
	//
	//View_World = DirectX::XMMatrixRotationY(t);//按y转换,左右
	DirectX::XMMATRIX mSpin = DirectX::XMMatrixRotationY(ChangeLevel / 100);//按y转换,左右
	DirectX::XMMATRIX mOrbit = DirectX::XMMatrixRotationX(ChangeVertical / 100);//按x转换，上下
	View_World = mSpin * mOrbit;

	//clear back Buffer
	devContext->ClearRenderTargetView(backBuffer, DirectX::Colors::MidnightBlue);
	//Update variables
	ConstantBuffer cb;
	cb.mWorld = DirectX::XMMatrixTranspose(View_World);	//XMMatrixTranspose 计算转置矩阵（行变列，列变行）？？为啥
	cb.mView = DirectX::XMMatrixTranspose(View_View);
	cb.mProjection = DirectX::XMMatrixTranspose(View_Projection);
	devContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);
	// Renders a square
	//
	devContext->VSSetShader(pVertexShader, nullptr, 0);
	devContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	devContext->PSSetShader(pPixelShader, nullptr, 0);
	devContext->DrawIndexed(36, 0, 0);        // 6 vertices needed for 2 triangles in a triangle list


	//devContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
	SwapChain->Present(0, 0);
}
HRESULT DrawFunction::CompileTheShader(void)
{
	HRESULT hr = S_OK;//ret
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	//Compiler vertex shader
	//组建加载顶点着色器
	hr = D3DCompileFromFile(L"Box.fx", nullptr, nullptr, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &pErrorBlob);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		printf("Can not find FX File .\nPlease run this executable from the directory that contains the FX file\n Error HRESULT:%ld\n\n", hr);
		return hr;
#else
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
#endif
	}
	//Create VERTEX shader
	hr = dev->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}
	// Define the input layout
	//定义输入布局
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = dev->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pVertexLayout);
	if (FAILED(hr))
		return hr;

	// Set the input layout
	devContext->IASetInputLayout(pVertexLayout);
	// Compile the pixel shader
	//组建加载像素着色器
	ID3DBlob* pPSBlob = nullptr;
	hr = D3DCompileFromFile(L"Box.fx", nullptr, nullptr, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &pErrorBlob);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		printf("Can not find FX File .\nPlease run this executable from the directory that contains the FX file\n Error HRESULT:%ld\n\n", hr);
		return hr;
#else
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
#endif
	}
	// Create the pixel shader
	hr = dev->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader);
	if (FAILED(hr))
		return hr;
	//release resource

	pVSBlob->Release();
	pPSBlob->Release();

	return hr;
}
HRESULT DrawFunction::CreateGraph(void)
{

	HRESULT hr = S_OK;//ret
	// Create vertex buffer
	//创建，定义顶点坐标
	struct SimpleVertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
	};

	SimpleVertex vertices[] =
	{	//正面
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },//红
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },//黄
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f) },//蓝
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },//绿
		//反面
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC bd;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex)* 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = vertices;
	hr = dev->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		printf("Can not create vertex buffer\n Error HRESULT:%ld\n\n", hr);
#endif
		return hr;
	}
	// Set vertex buffer
	//设置顶点缓冲区,绑定到设备的输入槽上
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	devContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Create index buffer
	//创建，这叫啥来着，顶点描述表？
	WORD indices[] =
	{
		0, 2, 1,
		0, 3, 2,

		4, 5, 6,
		4, 6, 7,

		0, 1, 4,
		1, 5, 4,

		2, 3, 6,
		3, 7, 6,

		0, 4, 3,
		3, 4, 7,

		1, 2, 5,
		5, 2, 6
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD)* 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = dev->CreateBuffer(&bd, &InitData, &pIndexBuffer);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		printf("Can not create index buffer\n Error HRESULT:%ld\n\n", hr);
#endif
		return hr;
	}
	// Set index buffer
	devContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// Set primitive topology
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	//建立常量缓冲共享内存
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = dev->CreateBuffer(&bd, nullptr, &pConstantBuffer);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		printf("Can not create the constant buffer\n Error HRESULT:%ld\n\n", hr);
#endif
		return hr;
	}
	// Create the constant buffer
	//建立，描述 摄像机与视角坐标
	// Initialize the world matrix
	View_World = DirectX::XMMatrixIdentity();//XMMatrixIdentity构建单位矩阵
	/*// Initialize the view matrix
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(3.0f, -3.0f, 4.0f, 0.0f);;	//眼睛坐标
	DirectX::XMVECTOR Point = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //看向的位置
	DirectX::XMVECTOR Eye_Up = DirectX::XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f);//定义眼睛"向上"的方向
	*/
	// Initialize the view matrix
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 0.0f, 6.0f, 0.0f);;	//眼睛坐标
	DirectX::XMVECTOR Point = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); //看向的位置
	DirectX::XMVECTOR Eye_Up = DirectX::XMVectorSet(0.0f, 1.0f, 100.0f, 0.0f);//定义眼睛"向上"的方向
	View_View = DirectX::XMMatrixLookAtLH(Eye, Point, Eye_Up);		//XMMatrixLookAtLH函数返回的是世界->视图变换矩阵

	View_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, 800 / (FLOAT)600, 10.0f, 100.0f);
	//角度为pi/2（90度），匹配纵横比为800/600，近平面为0.01，远平面为100
	return hr;
}