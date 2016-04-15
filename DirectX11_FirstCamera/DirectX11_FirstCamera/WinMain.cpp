#include "DrawFunction.h"

//windows�������m����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
#ifdef _DEBUG
	FILE *stream;	//debug����Ҫһ������̨���������Ϣ
	AllocConsole();
	freopen_s(&stream, "CONOUT$", "w", stdout);
#endif

	DrawFunction my_d3d_haha;
	my_d3d_haha.WinMain_InitWindow(hInstance, iCmdshow);

	my_d3d_haha.Init();		//����������D3D�����豸�����������Ӵ���

	if (FAILED(my_d3d_haha.CompileTheShader())) return FALSE;
	else printf("CompileTheShader ----OK!\n");

	if (FAILED(my_d3d_haha.CreateGraph()))return FALSE;
	else printf("CreateGraph ----OK!\n");

	if (FAILED(my_d3d_haha.LoadTextureFromFile())) return FALSE;
	else printf("LoadTexture ----OK!\n");

	my_d3d_haha.Run();

	return 0;
}



