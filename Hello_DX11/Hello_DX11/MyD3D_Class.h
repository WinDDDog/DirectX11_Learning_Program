#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>

class MyD3D_Class
{
private:
public:
	MyD3D_Class(void);
	MyD3D_Class(const MyD3D_Class&);
	virtual ~MyD3D_Class(void);

	BOOL CreateD3DObject(HWND);
};