#include "DrawFunction.h"

//windows程序入口m函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
#ifdef _DEBUG
	FILE *stream;	//debug下需要一个控制台输出调试信息
	AllocConsole();
	freopen_s(&stream, "CONOUT$", "w", stdout);
#endif
	DrawFunction my_d3d_haha;

	my_d3d_haha.WinMain_InitWindow(hInstance, iCmdshow);

	if (FALSE == my_d3d_haha.Init()) return FALSE;
	else printf("This is the template_3 for my DirectX_11 project \n");
		//创建基本的D3D对象，设备，交换链，视窗等

	if (FAILED(my_d3d_haha.CompileTheShader())) return FALSE;
	else printf("CompileTheShader ----OK!\n");

	if (FAILED(my_d3d_haha.CreateGraph()))return FALSE;
	else printf("CreateGraph ----OK!\n");

	my_d3d_haha.Run();

	return 0;
}



