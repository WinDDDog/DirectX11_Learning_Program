#include "IncludeALL.h"
using namespace DirectX;
/*
1．Ambient：由光源发射的环境光的数量。
2．Diffuse：由光源发射的漫反射光的数量。
3．Specular：由光源发射的高光的数量。
4．Direction：灯光方向。
5．Position：灯光位置。
6．Range：光照范围（离开光源的距离大于这个值的点不会被照亮）。
7．Attenuation：按照（a0、a1和a2）的顺序存储3个衰减常量。衰减常量只用于点和聚光灯，用于控制光强随距离衰减的程度。
8．Spot：该指数用于控制聚光灯的圆锥体区域大小；这个值只用于聚光灯。
*/
struct DirectionalLight	////平行光
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//环境光
	XMFLOAT4 Diffuse;	//漫反射光
	XMFLOAT4 Specular;	//高光
	XMFLOAT3 Direction;	//灯光方向
	float Pad; // 占位最后一个float，这样我们就可以设置光源数组了。
};

struct PointLight	////点光
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//环境光的数量
	XMFLOAT4 Diffuse;	//漫反射光
	XMFLOAT4 Specular;	//高光

	// 打包到4D矢量: (Position, Range)
	XMFLOAT3 Position;	//灯光位置
	float Range;		//有光范围

	// 打包到4D矢量: (A0, A1, A2, Pad)
	XMFLOAT3 Att;	//光强随距离衰减的程度
	float Pad; // 占位最后一个float，，这样我们就可以设置光源数组了。
};

struct SpotLight	////聚光灯
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//环境光的数量
	XMFLOAT4 Diffuse;	//漫反射光
	XMFLOAT4 Specular;	//高光

	// 打包到4D矢量: (Position, Range)
	XMFLOAT3 Position;	//灯光位置
	float Range;		//有光范围

	// 打包到4D矢量: (Direction, Spot)
	XMFLOAT3 Direction;	//灯光方向
	float Spot;			//聚光灯的圆锥体区域大小

	// 打包到4D矢量: (Att, Pad)
	XMFLOAT3 Att;	//光强随距离衰减的程度
	float Pad; // 占位最后一个float，，这样我们就可以设置光源数组了。
};

class Basic_Light
{
public:
	Basic_Light();
	virtual ~Basic_Light();
	Basic_Light(const Basic_Light&);

private:

protected:
	DirectionalLight DirLight;
	PointLight pLight;
};