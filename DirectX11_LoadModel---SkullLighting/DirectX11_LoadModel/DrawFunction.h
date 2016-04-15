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
	struct ConstantBuffer		//���������ӽ�����
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
	};
	struct cbPerFrameLight		//���������ӽ�����
	{
		DirectionalLight gDirLight;	//ƽ�й�
		PointLight gPointLight;		//���
		//SpotLight gSpotLight;		//�۹��
		DirectX::XMFLOAT4 gEyePosW;			//�۾��������Ϊfloat4��Ϊʲô
	};
private:
	ID3D11VertexShader*     pVertexShader = nullptr;
	ID3D11PixelShader*      pPixelShader = nullptr;
	ID3D11InputLayout*      pVertexLayout = nullptr;
	ID3D11Buffer*           pVertexBuffer = nullptr;
	ID3D11Buffer*           pIndexBuffer = nullptr;
	ID3D11Buffer*           pConstantBuffer = nullptr;
	ID3D11RasterizerState*	mWireframeRS = nullptr;

	ID3D11Buffer*           pcbPerFrameLight = nullptr;//����
	DirectX::XMMATRIX       View_World;
	DirectX::XMMATRIX       View_View;
	DirectX::XMMATRIX       View_Projection;

	UINT mSkullIndexCount;
};