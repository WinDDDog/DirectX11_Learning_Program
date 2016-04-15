#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

//define the Height and width to the Project

const int Width = 800;
const int height = 600;

//class
class My_D3d_APP
{
public:
	My_D3d_APP();
	My_D3d_APP(const My_D3d_APP&);
	virtual ~My_D3d_APP();

	virtual BOOL Init();
	virtual void RenderScene() = 0;

protected:
	//windows������ر���
	HWND	Main_Window_HWND;
	MSG		msg;

	//D3D��ر���
	ID3D11Device		*dev;			//�豸
	ID3D11DeviceContext	*devContext;	//������
	IDXGISwapChain		*SwapChain;		//������
	ID3D11DepthStencilView	*depthStencilView;	//���ģ�����
	ID3D11RenderTargetView	*backBuffer;		//��һ֡������
	//D3D��غ���
	BOOL CreateD3DObject(const HWND);
private:

};