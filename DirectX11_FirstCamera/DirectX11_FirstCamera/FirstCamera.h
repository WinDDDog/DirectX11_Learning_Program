#include "IncludeAll.h"

class FirstCamera
{
public:
	FirstCamera();
	virtual ~FirstCamera();
	FirstCamera(const FirstCamera&);

	void SetCamera(DirectX::XMFLOAT4 Position, DirectX::XMFLOAT4 Tg, DirectX::XMFLOAT4 Pos_Up, DirectX::XMFLOAT4 Right);//设定位置
	 
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


	DirectX::XMMATRIX       View_World;		//世界坐标
	DirectX::XMMATRIX       View_View;		//世界->视图变换矩阵
	DirectX::XMMATRIX       View_Projection;//视角纵横比


private:

	//位置定位：
	//指定相机相对于世界坐标的位置Position
	//指定相机向上的坐标Pos_Up,向前Target，向右 Pos_Right
	//分别对应于		Y轴   ，Z轴		 ， X轴	来固定住相机的位置；

	DirectX::XMFLOAT4		Position;	//位置
	//DirectX::XMFLOAT4		Point;	//看的方向，在第二次改版的时候被移除
	DirectX::XMFLOAT4		Pos_Up;	//指定向上的位置
	DirectX::XMFLOAT4		Target; //看的方向单位向量
	DirectX::XMFLOAT4		Pos_Right;//指定“右侧”的方向
protected:
	int Mouse_Move_Forwad_Back;//0表示不动，1表示向前，-1表示向后
	int Mouse_Move_Righr_Left;//0表示不动，1表示向右，-1表示向左
};