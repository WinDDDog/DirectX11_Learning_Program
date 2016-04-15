#include "My_D3D_APP_Base.h"

class DrawFunction : public My_D3d_APP
{
public:
	DrawFunction();
	~DrawFunction();
	virtual void RenderScene(void);
	HRESULT CompileTheShader(void);
	HRESULT CreateGraph(void);
	HRESULT CreateStencilDesc(void);
protected:
	struct ConstantBuffer		//用于描述视角坐标
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
	};
private:
	ID3D11VertexShader*     pVertexShader = nullptr;
	ID3D11PixelShader*      pPixelShader = nullptr;
	ID3D11InputLayout*      pVertexLayout = nullptr;
	ID3D11Buffer*           pVertexBuffer = nullptr;
	ID3D11Buffer*           pIndexBuffer = nullptr;
	ID3D11Buffer*           pConstantBuffer = nullptr;

	ID3D11DepthStencilState* pStencilReplace = nullptr;
	ID3D11DepthStencilState* pStencilTest = nullptr;

	DirectX::XMMATRIX       View_World;
	DirectX::XMMATRIX       View_World2;
	DirectX::XMMATRIX       View_View;
	DirectX::XMMATRIX       View_Projection;

};