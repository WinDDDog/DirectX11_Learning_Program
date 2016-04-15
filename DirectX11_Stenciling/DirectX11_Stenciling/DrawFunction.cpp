#include "DrawFunction.h"
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
	//ͨ��ʱ��ı�world����

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

	View_World = DirectX::XMMatrixRotationY(t);//��yת��
	

	//clear back Buffer
	devContext->ClearRenderTargetView(backBuffer, DirectX::Colors::MidnightBlue);
	devContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
	
	devContext->OMSetDepthStencilState(pStencilReplace, 1);

	//Update variables
	ConstantBuffer cb;
	cb.mWorld = DirectX::XMMatrixTranspose(View_World);	//XMMatrixTranspose ����ת�þ����б��У��б��У�����Ϊɶ
	cb.mView = DirectX::XMMatrixTranspose(View_View);
	cb.mProjection = DirectX::XMMatrixTranspose(View_Projection);
	devContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);
	// Renders a square
	//world1
	devContext->VSSetShader(pVertexShader, nullptr, 0);
	devContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	devContext->PSSetShader(pPixelShader, nullptr, 0);
	//draw index
	devContext->DrawIndexed(12, 0, 0);        // 6 vertices needed for 2 triangles in a triangle list


	devContext->OMSetDepthStencilState(pStencilTest, 1);

	//world2
	ConstantBuffer cb1;
	View_World2 = DirectX::XMMatrixTranslation(1.0f, 1.0f, 0.0f);
	cb1.mWorld = DirectX::XMMatrixTranspose(View_World2);
	cb1.mView = DirectX::XMMatrixTranspose(View_View);
	cb1.mProjection = DirectX::XMMatrixTranspose(View_Projection);
	devContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb1, 0, 0);
	//draw index
	devContext->DrawIndexed(12, 0, 0);
	
	SwapChain->Present(0, 0);
}
HRESULT DrawFunction::CompileTheShader(void)
{
	HRESULT hr = S_OK;//ret
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	//Compiler vertex shader
	//�齨���ض�����ɫ��
	hr = D3DCompileFromFile(L"Squance.fx", nullptr, nullptr, "VS", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pVSBlob, &pErrorBlob);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		printf("Fail in FX_VS File \n Error HRESULT:%ld\n Error:%s\n", hr, pErrorBlob->GetBufferPointer());
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
	//�������벼��
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
	//�齨����������ɫ��
	ID3DBlob* pPSBlob = nullptr;
	hr = D3DCompileFromFile(L"Squance.fx", nullptr, nullptr, "PS", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &pErrorBlob);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		printf("Fail in FX_FS File \n Error HRESULT:%ld\n Error:%s\n", hr, pErrorBlob->GetBufferPointer());
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
	//���������嶥������
	struct SimpleVertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
	};

	SimpleVertex vertices[] =
	{	//����
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, +0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },//��
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, +0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },//��
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, +0.0f), DirectX::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f) },//��
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, +0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },//��
		//����
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, -0.1f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, -0.1f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -0.1f), DirectX::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, -0.1f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }
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
	//���ö��㻺����,�󶨵��豸���������
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	devContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Create index buffer
	//���������ɶ���ţ�������������
	WORD indices[] =
	{
		0, 2, 1,
		0, 3, 2,

		4, 5, 6,
		4, 6, 7
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD)* 12;        // 36 vertices needed for 12 triangles in a triangle list
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



	//�����������干���ڴ�
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
	//���������� ��������ӽ�����
	// Initialize the world matrix
	View_World = DirectX::XMMatrixIdentity();//XMMatrixIdentity������λ����
	View_World2 = DirectX::XMMatrixIdentity();
	/*// Initialize the view matrix
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(3.0f, -3.0f, 4.0f, 0.0f);;	//�۾�����
	DirectX::XMVECTOR Point = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //�����λ��
	DirectX::XMVECTOR Eye_Up = DirectX::XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f);//�����۾�"����"�ķ���
	*/
	// Initialize the view matrix
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 0.0f, 4.0f, 0.0f);;	//�۾�����
	DirectX::XMVECTOR Point = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); //�����λ��
	DirectX::XMVECTOR Eye_Up = DirectX::XMVectorSet(0.0f, 1.0f, 100.0f, 0.0f);//�����۾�"����"�ķ���
	View_View = DirectX::XMMatrixLookAtLH(Eye, Point, Eye_Up);		//XMMatrixLookAtLH�������ص�������->��ͼ�任����

	View_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, 800 / (FLOAT)600, 0.01f, 100.0f);
	//�Ƕ�Ϊpi/2��90�ȣ���ƥ���ݺ��Ϊ800/600����ƽ��Ϊ0.01��Զƽ��Ϊ100
	return hr;
}
HRESULT DrawFunction::CreateStencilDesc(void)
{
	HRESULT hr = S_OK;//ret
	D3D11_DEPTH_STENCIL_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));
	stencilDesc.DepthEnable = FALSE;
	stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	stencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	stencilDesc.StencilEnable = TRUE;
	stencilDesc.StencilReadMask = 0xff;
	stencilDesc.StencilWriteMask = 0xff;
	stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	stencilDesc.BackFace = stencilDesc.FrontFace;

	hr = dev->CreateDepthStencilState(&stencilDesc, &pStencilReplace);
	stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	stencilDesc.BackFace = stencilDesc.FrontFace;
	hr = dev->CreateDepthStencilState(&stencilDesc, &pStencilTest);

	return hr;
}