#include "IncludeAll.h"

class FirstCamera
{
public:
	FirstCamera();
	virtual ~FirstCamera();
	FirstCamera(const FirstCamera&);

	void SetCamera(DirectX::XMFLOAT4 Position, DirectX::XMFLOAT4 Tg, DirectX::XMFLOAT4 Pos_Up, DirectX::XMFLOAT4 Right);//�趨λ��
	 
	DirectX::XMFLOAT4 GetPosition();//get Position
	DirectX::XMFLOAT4 Get_Pos_Right();//get Point
	DirectX::XMFLOAT4 Get_Pos_Up();//get Pos_Up

	void MoveCamera_LeftRight(float);
	void MoveCamera_UpDown(float);
	void MoveCamera_ForwadBack(float);
	void RotateBy_Y(float);
	void RotateBy_X(float);
	void RotateBy_Z(float);
	BOOL IsCameraMove;
	DirectX::XMFLOAT3 horizonMove;
	void ApplyCameraChange();


	DirectX::XMMATRIX       View_World;		//��������
	DirectX::XMMATRIX       View_View;		//����->��ͼ�任����
	DirectX::XMMATRIX       View_Projection;//�ӽ��ݺ��


private:

	//λ�ö�λ��
	//ָ�������������������λ��Position
	//ָ��������ϵ�����Pos_Up,��ǰTarget������ Pos_Right
	//�ֱ��Ӧ��		Y��   ��Z��		 �� X��	���̶�ס�����λ�ã�

	DirectX::XMFLOAT4		Position;	//λ��
	//DirectX::XMFLOAT4		Point;	//���ķ����ڵڶ��θİ��ʱ���Ƴ�
	DirectX::XMFLOAT4		Pos_Up;	//ָ�����ϵ�λ��
	DirectX::XMFLOAT4		Target; //���ķ���λ����
	DirectX::XMFLOAT4		Pos_Right;//ָ�����Ҳࡱ�ķ���
protected:
	int Mouse_Move_Forwad_Back;//0��ʾ������1��ʾ��ǰ��-1��ʾ���
	int Mouse_Move_Righr_Left;//0��ʾ������1��ʾ���ң�-1��ʾ����
};