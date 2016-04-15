#include "DrawFunction.h"
DrawFunction::DrawFunction()
{

}
DrawFunction::~DrawFunction()
{

}
void DrawFunction::RenderScene()
{
	devContext->ClearRenderTargetView(backBuffer, DirectX::Colors::MidnightBlue);
	devContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
	SwapChain->Present(0, 0);
}
