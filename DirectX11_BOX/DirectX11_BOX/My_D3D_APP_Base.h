#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>




class My_D3d_APP
{
public:
	My_D3d_APP();
	My_D3d_APP(const My_D3d_APP&);
	virtual ~My_D3d_APP();

	virtual BOOL Init(const HWND);
	virtual void RenderScene() = 0;

protected:
	//D3D相关变量
	ID3D11Device		*dev;			//设备
	ID3D11DeviceContext	*devContext;	//上下文
	IDXGISwapChain		*SwapChain;		//交换链
	ID3D11DepthStencilView	*depthStencilView;	//景深，模板对象
	ID3D11RenderTargetView	*backBuffer;		//后一帧缓冲区
	//D3D相关函数
	BOOL CreateD3DObject(const HWND);
private:

};