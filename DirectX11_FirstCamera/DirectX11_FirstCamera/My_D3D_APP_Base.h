#include "IncludeAll.h"

class My_D3d_APP
{
public:
	My_D3d_APP();
	My_D3d_APP(const My_D3d_APP&);
	virtual ~My_D3d_APP();

	virtual BOOL Init();
	virtual void RenderScene() = 0;

protected:
	//windows相关变量
	HWND	Main_Window_HWND;
	MSG		msg;
	//用于改变鼠标位置
	INPUT mouseInput;
	int Window_Center_X;
	int Window_Center_Y;
	BOOL SetMouseMoveCenter();

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