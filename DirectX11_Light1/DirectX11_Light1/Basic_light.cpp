#include "Basic_light.h"
Basic_Light::Basic_Light()
{
	//�������еĶ���ֻ������ɫ�йأ����Ƕ�������ɫ
	//DirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);//������
	//DirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);//�������
	DirLight.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);//������
	DirLight.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);//�������
	//DirLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);//�����
	DirLight.Direction = XMFLOAT3(-1.0f,-1.0f, 0.0f);//bug?

	pLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);//������
	pLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	//�������
	pLight.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);//�����
	pLight.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);	//λ������
	pLight.Range = 250;//��Χ
	pLight.Att = XMFLOAT3(0.0f, 0.1f, 0.0f);
}
Basic_Light::~Basic_Light()
{

}