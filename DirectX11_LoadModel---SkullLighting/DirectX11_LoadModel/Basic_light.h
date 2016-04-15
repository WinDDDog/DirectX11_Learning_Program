#include "IncludeALL.h"
using namespace DirectX;
/*
1��Ambient���ɹ�Դ����Ļ������������
2��Diffuse���ɹ�Դ�������������������
3��Specular���ɹ�Դ����ĸ߹��������
4��Direction���ƹⷽ��
5��Position���ƹ�λ�á�
6��Range�����շ�Χ���뿪��Դ�ľ���������ֵ�ĵ㲻�ᱻ��������
7��Attenuation�����գ�a0��a1��a2����˳��洢3��˥��������˥������ֻ���ڵ�;۹�ƣ����ڿ��ƹ�ǿ�����˥���ĳ̶ȡ�
8��Spot����ָ�����ڿ��ƾ۹�Ƶ�Բ׶�������С�����ֵֻ���ھ۹�ơ�
*/
struct DirectionalLight	////ƽ�й�
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//������
	XMFLOAT4 Diffuse;	//�������
	XMFLOAT4 Specular;	//�߹�
	XMFLOAT3 Direction;	//�ƹⷽ��
	float Pad; // ռλ���һ��float���������ǾͿ������ù�Դ�����ˡ�
};

struct PointLight	////���
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//�����������
	XMFLOAT4 Diffuse;	//�������
	XMFLOAT4 Specular;	//�߹�

	// �����4Dʸ��: (Position, Range)
	XMFLOAT3 Position;	//�ƹ�λ��
	float Range;		//�йⷶΧ

	// �����4Dʸ��: (A0, A1, A2, Pad)
	XMFLOAT3 Att;	//��ǿ�����˥���ĳ̶�
	float Pad; // ռλ���һ��float�����������ǾͿ������ù�Դ�����ˡ�
};

struct SpotLight	////�۹��
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//�����������
	XMFLOAT4 Diffuse;	//�������
	XMFLOAT4 Specular;	//�߹�

	// �����4Dʸ��: (Position, Range)
	XMFLOAT3 Position;	//�ƹ�λ��
	float Range;		//�йⷶΧ

	// �����4Dʸ��: (Direction, Spot)
	XMFLOAT3 Direction;	//�ƹⷽ��
	float Spot;			//�۹�Ƶ�Բ׶�������С

	// �����4Dʸ��: (Att, Pad)
	XMFLOAT3 Att;	//��ǿ�����˥���ĳ̶�
	float Pad; // ռλ���һ��float�����������ǾͿ������ù�Դ�����ˡ�
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