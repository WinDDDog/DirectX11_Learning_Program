#include "My_D3D_APP_Base.h"

class DrawFunction : public My_D3d_APP
{
public:
	DrawFunction();
	~DrawFunction();
	virtual void RenderScene(void);
	HRESULT CompileTheShader(void);
	HRESULT CreateGraph(void);

	virtual HRESULT WinMain_InitWindow(HINSTANCE hInstance, int iCmdshow);
	virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Run();
	
protected:
	struct ConstantBuffer		//���������ӽ�����
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

	DirectX::XMMATRIX       View_World;
	DirectX::XMMATRIX       View_View;
	DirectX::XMMATRIX       View_Projection;

};