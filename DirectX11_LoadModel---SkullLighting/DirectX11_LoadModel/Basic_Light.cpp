#include "Basic_light.h"
Basic_Light::Basic_Light()
{
	//�������еĶ���ֻ������ɫ�йأ����Ƕ�������ɫ
	DirLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);//������ 
	DirLight.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);//�������
	//DirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);//������
	//DirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);//�������
	DirLight.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);//�����
	DirLight.Direction = XMFLOAT3(0.0f, -1.0f, 1.0f);//bug?

	pLight.Ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);//������
	pLight.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);	//�������
	pLight.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);//�����
	pLight.Position = XMFLOAT3(0.0f, 0.0f, -3.0f);	//λ������
	pLight.Range = 1000.0;//��Χ
	pLight.Att = XMFLOAT3(0.0, 0.0, 0.0);

}
Basic_Light::~Basic_Light()
{

}