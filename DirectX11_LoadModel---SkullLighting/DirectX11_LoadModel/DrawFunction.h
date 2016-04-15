#include "My_D3D_APP_Base.h"
#include "Basic_light.h"
class DrawFunction : public My_D3d_APP, public  Basic_Light
{
public:
	DrawFunction();
	~DrawFunction();
	virtual void RenderScene(void);
	HRESULT CompileTheShader(void);
	HRESULT CreateGraph(void);
protected:
	struct ConstantBuffer		//用于描述视角坐标
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
	};
	struct cbPerFrameLight		//用于描述视角坐标
	{
		DirectionalLight gDirLight;	//平行光
		PointLight gPointLight;		//点光
		//SpotLight gSpotLight;		//聚光灯
		DirectX::XMFLOAT4 gEyePosW;			//眼睛坐标必须为float4？为什么
	};
private:
	ID3D11VertexShader*     pVertexShader = nullptr;
	ID3D11PixelShader*      pPixelShader = nullptr;
	ID3D11InputLayout*      pVertexLayout = nullptr;
	ID3D11Buffer*           pVertexBuffer = nullptr;
	ID3D11Buffer*           pIndexBuffer = nullptr;
	ID3D11Buffer*           pConstantBuffer = nullptr;
	ID3D11RasterizerState*	mWireframeRS = nullptr;

	ID3D11Buffer*           pcbPerFrameLight = nullptr;//光照
	DirectX::XMMATRIX       View_World;
	DirectX::XMMATRIX       View_View;
	DirectX::XMMATRIX       View_Projection;

	UINT mSkullIndexCount;
};