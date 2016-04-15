#include "FirstCamera.h"
/////////////
/////////////////////XNA_MATH_CODE////////////////////////////////////////////////////
DirectX::XMFLOAT4 operator+(DirectX::XMFLOAT4 &parameter1, DirectX::XMFLOAT4 &parameter2)
{
	return DirectX::XMFLOAT4(
		parameter1.x + parameter2.x,
		parameter1.y + parameter2.y,
		parameter1.z + parameter2.z,
		parameter1.w + parameter1.w
		);
}
/////////////////////////////////////////////////////////////////////////////////////////
////////////
//���죬����
FirstCamera::FirstCamera()
{
	//�������ʼ�������Ϣ
	//���������� ��������ӽ�����
	Position = DirectX::XMFLOAT4(0.0f, 1.0f, 4.0f, 0.0f);
	//Point = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	Pos_Up = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	Target = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f,0.0f);
	Pos_Right = DirectX::XMFLOAT4(-1.0f, 0.0, 0.0, 0.0);

	horizonMove = DirectX::XMFLOAT3(0, 0, 0);//���ó�ʼ��ת�Ƕ�

	View_World = DirectX::XMMatrixIdentity();//XMMatrixIdentity������λ����

	View_View = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&Position), DirectX::XMLoadFloat4(&(Target + Position)), DirectX::XMLoadFloat4(&Pos_Up));
	//XMMatrixLookAtLH�������ص�������->��ͼ�任����

	View_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, 800 / (FLOAT)600, 0.01f, 100.0f);
	//�Ƕ�Ϊpi/2��90�ȣ���ƥ���ݺ��Ϊ800/600����ƽ��Ϊ0.01��Զƽ��Ϊ100

}
FirstCamera::~FirstCamera()
{

}

//�趨λ��
void FirstCamera::SetCamera(DirectX::XMFLOAT4 Pos, DirectX::XMFLOAT4 Tg, DirectX::XMFLOAT4 Up, DirectX::XMFLOAT4 Right)
{
	Position = Pos;
	Target = Tg;
	Pos_Up = Up;
	Pos_Right = Right;
}

DirectX::XMFLOAT4 FirstCamera::GetPosition()//get Position
{
	return Position;
}
DirectX::XMFLOAT4 FirstCamera::Get_Pos_Right()//Right
{
	return Pos_Right;
}
DirectX::XMFLOAT4 FirstCamera::Get_Pos_Up()//get Pos_Up
{
	return Pos_Up;
}
//Move Camera ,first + last -
void FirstCamera::MoveCamera_LeftRight(float xx)
{
	//horizonMove.x = xx;
	float normaliz_Length = (Pos_Right.x)*(Pos_Right.x) + (Pos_Right.z)*(Pos_Right.z);
	float normaliz_X = Pos_Right.x / normaliz_Length;
	float normaliz_Z = Pos_Right.z / normaliz_Length;

	horizonMove.x += normaliz_X*xx;
	horizonMove.z += normaliz_Z*xx;

	IsCameraMove = TRUE;
}
void FirstCamera::MoveCamera_UpDown(float xx)
{
	//horizonMove.y = xx;
	IsCameraMove = TRUE;
}
void FirstCamera::MoveCamera_ForwadBack(float xx)
{
	
	//horizonMove.z = xx;
	float normaliz_Length = (Target.x)*(Target.x) + (Target.z)*(Target.z);
	float normaliz_X = Target.x / normaliz_Length;
	float normaliz_Z = Target.z / normaliz_Length;
	
	horizonMove.x += normaliz_X*xx;
	horizonMove.z += normaliz_Z*xx;

	IsCameraMove = TRUE;
}
//Rotate Cameras
void FirstCamera::RotateBy_Y(float xx)
{


	DirectX::XMMATRIX Angle_RotateY = DirectX::XMMatrixRotationY(xx);//����Ƕ�

	//�Ƕȱ��
	//�ܹ���Ҫ�޸�
	DirectX::XMVECTOR after = DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&Target), Angle_RotateY);

	DirectX::XMStoreFloat4(&Target, after);

	//right ҲҪ�仯
	after = DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&Pos_Right), Angle_RotateY);

	DirectX::XMStoreFloat4(&Pos_Right, after);

	IsCameraMove = TRUE;

}
void FirstCamera::RotateBy_X(float xx)
{
	DirectX::XMFLOAT4 Restrore = Target;

	//DirectX::XMFLOAT4 Restore = Target;//��һ����û�����취��

	DirectX::XMMATRIX Angle_RotateX = DirectX::XMMatrixRotationAxis(XMLoadFloat4(&Pos_Right), xx);//����Ƕ�a

	DirectX::XMVECTOR after = DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&Target), Angle_RotateX);
	//ʹ��
	//XMMatrixRotationAxis
	DirectX::XMStoreFloat4(&Target, after);

	//�ж�һ��,�������µ��ӽ�
	if (Target.y<-0.9 || Target.y>0.9)
	{
		Target = Restrore;
		return;
	}

	/*
	if (Target.z<1.0 && Target.z>-1.0)
	{
		Target = Restore;
		return;
	}*/

	DirectX::XMVECTOR after_up = DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&Pos_Up), Angle_RotateX);
	//ע������
	
	//DirectX::XMStoreFloat4(&Pos_Up, after_up);

	IsCameraMove = TRUE;
}
void FirstCamera::RotateBy_Z(float xx)
{

}
//apply
void FirstCamera::ApplyCameraChange()
{
	
	Position = DirectX::XMFLOAT4(
		Position.x + horizonMove.x,
		Position.y + horizonMove.y, 
		Position.z + horizonMove.z,
		Position.w );
			
	View_View = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&Position), DirectX::XMLoadFloat4(&(Target + Position)), DirectX::XMLoadFloat4(&Pos_Up));
	//update

	IsCameraMove = FALSE;
	horizonMove = DirectX::XMFLOAT3(0, 0, 0);

	//clean
}

