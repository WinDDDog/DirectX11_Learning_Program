#include "My_D3D_APP_Base.h"

#include "FirstCamera.h"

class DrawFunction : public My_D3d_APP, public FirstCamera
{
public:
	DrawFunction();
	~DrawFunction();
	virtual void RenderScene(void);
	HRESULT CompileTheShader(void);
	HRESULT CreateGraph(void);
	HRESULT LoadTextureFromFile(void);

	virtual HRESULT WinMain_InitWindow(HINSTANCE hInstance, int iCmdshow);
	virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Run();
	//static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

	ID3D11ShaderResourceView*	pTextureRV_Grass = nullptr;	//LoadTexture
	ID3D11ShaderResourceView*   pTextureRV_ddog = nullptr;
	ID3D11SamplerState*         pSamplerLinear = nullptr;//TextDescription
};
