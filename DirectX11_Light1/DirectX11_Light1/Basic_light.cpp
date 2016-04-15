#include "Basic_light.h"
Basic_Light::Basic_Light()
{
	//这里所有的定义只与光的颜色有关，就是定义光的颜色
	//DirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);//环境光
	//DirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);//漫反射光
	DirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);//环境光
	DirLight.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);//漫反射光
	//DirLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);//镜面光
	DirLight.Direction = XMFLOAT3(-1.0f,-1.0f, 0.0f);//bug?

	pLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);//环境光
	pLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	//漫反射光
	pLight.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);//镜面光
	pLight.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);	//位置坐标
	pLight.Range = 250;//范围
	pLight.Att = XMFLOAT3(0.0f, 0.1f, 0.0f);
}
Basic_Light::~Basic_Light()
{

}