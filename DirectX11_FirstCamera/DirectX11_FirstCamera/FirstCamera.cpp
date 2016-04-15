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
//构造，析构
FirstCamera::FirstCamera()
{
	//在这里初始化相机信息
	//建立，描述 摄像机与视角坐标
	Position = DirectX::XMFLOAT4(0.0f, 1.0f, 4.0f, 0.0f);
	//Point = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	Pos_Up = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	Target = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f,0.0f);
	Pos_Right = DirectX::XMFLOAT4(-1.0f, 0.0, 0.0, 0.0);

	horizonMove = DirectX::XMFLOAT3(0, 0, 0);//设置初始旋转角度

	View_World = DirectX::XMMatrixIdentity();//XMMatrixIdentity构建单位矩阵

	View_View = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&Position), DirectX::XMLoadFloat4(&(Target + Position)), DirectX::XMLoadFloat4(&Pos_Up));
	//XMMatrixLookAtLH函数返回的是世界->视图变换矩阵

	View_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, 800 / (FLOAT)600, 0.01f, 100.0f);
	//角度为pi/2（90度），匹配纵横比为800/600，近平面为0.01，远平面为100

}
FirstCamera::~FirstCamera()
{

}

//设定位置
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


	DirectX::XMMATRIX Angle_RotateY = DirectX::XMMatrixRotationY(xx);//计算角度

	//角度变幻
	//架构还要修改
	DirectX::XMVECTOR after = DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&Target), Angle_RotateY);

	DirectX::XMStoreFloat4(&Target, after);

	//right 也要变化
	after = DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&Pos_Right), Angle_RotateY);

	DirectX::XMStoreFloat4(&Pos_Right, after);

	IsCameraMove = TRUE;

}
void FirstCamera::RotateBy_X(float xx)
{
	DirectX::XMFLOAT4 Restrore = Target;

	//DirectX::XMFLOAT4 Restore = Target;//想一想又没其它办法？

	DirectX::XMMATRIX Angle_RotateX = DirectX::XMMatrixRotationAxis(XMLoadFloat4(&Pos_Right), xx);//计算角度a

	DirectX::XMVECTOR after = DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&Target), Angle_RotateX);
	//使用
	//XMMatrixRotationAxis
	DirectX::XMStoreFloat4(&Target, after);

	//判断一下,锁定上下的视角
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
	//注意这里
	
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

