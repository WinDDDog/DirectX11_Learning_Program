//-------------------------------------------------------------------------------------
//定义光照结构
//----------------------------------------------------------------------------------------
struct DirectionalLight	////平行光
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight		////点光
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight	////聚光灯
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};
//--------------------------------------------------------------------------------------
// 常量缓冲定义
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)	//世界，投影坐标转换
{
	matrix World;
	matrix View;
	matrix Projection;
}
cbuffer cbPerFrameLight						//光线结构打包,暂时就一个平行光
{
	DirectionalLight gDirLight;	//平行光
	PointLight gPointLight;		//点光
	//SpotLight gSpotLight;		//聚光灯
	float4 gEyePosW;			//眼睛坐标
};
//--------------------------------------------------------------------------------------
// 输入输出结构
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Normal : NORMAL;
};
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Normal : NORMAL;
};

//--------------------------------------------------------------------------------------
//其它结构定义
//--------------------------------------------------------------------------------------
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};
//=======================================================================================
//--------------------------------------------------------------------------------------
// 功能函数实现
//--------------------------------------------------------------------------------------
//实现平行光ComputeDirectionalLight（材质，平行光结构，反射点法线,指向眼睛的单位向量,返回1，返回2，返回3）
void ComputeDirectionalLight(
	Material mat, DirectionalLight L,
	float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// 初始化输出的变量
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 光照矢量与光线的传播方向相反
	float3 lightVec = -L.Direction;

		// 添加环境光
		ambient = mat.Ambient * L.Ambient;

	// 添加漫反射和镜面光，注意，保证normal，lightVec是单位向量（归一化）
	float diffuseFactor = dot(lightVec, normal);//计算cosA

	// Flatten避免动态分支
	[branch]
	if (diffuseFactor > 0.0f)	//如果角度的余弦值大于0，
	{
		float3 v = reflect(-lightVec, normal);//reflect通过入射光与法线计算反射向量
			float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);//pow用于计算材质光的线反射度

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
//实现点光源的函数ComputePointLight（材质，点光源结构，顶点位置，反射法线，指向眼睛的单位向量，返回1，返回2，返回3）
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;

		// The distance from surface to light.
		float d = length(lightVec);

	// Range test.
	if (d > L.Range)
		return;

	// Normalize the light vector.
	lightVec /= d;

	// Ambient term.
	ambient = mat.Ambient * L.Ambient;
	
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);
	
	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
			float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
	
	// Attenuate
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));
	/*
	diffuse *= att;
	spec *= att;
	*/
}
///////////////////////////////////////////////////////////////////////////////////////////////
//实现聚光灯的函数ComputeSpotLight（材料，聚光灯结构，顶点位置，顶点法线，指向眼睛向量，返回1，返回2，返回3）
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	//获得由顶点位置指向光源位置的向量
	float3 lightVec = L.Position - pos;

		// The distance from surface to light.
		//获得距离
		float d = length(lightVec);

	// Range test.
	//是否超出
	if (d > L.Range)
		return;

	// Normalize the light vector.
	//单位化
	lightVec /= d;

	// Ambient term.
	//计算环境光
	ambient = mat.Ambient * L.Ambient;
	
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	//计算方向与法线的余弦值
	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
			float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);//计算衰减值

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(L.Att, float3(1.0f, d, d*d));
	
	ambient *= spot;
	diffuse *= att;
	spec *= att;
	
}

//=======================================================================================
//--------------------------------------------------------------------------------------
// 顶点着色器
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT pos)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(pos.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Normal = pos.Normal;
	return output;
}

//--------------------------------------------------------------------------------------
// 像素着色器
//----------------------------------------------------------------------------------- ---
float4 PS(VS_OUTPUT input) : SV_Target
{

	float3 toEye = normalize(float3(0.0f, 1.0f, 1.0f));
	//初始化3个float4存放环境光，漫反射光，高光
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	Material goodluck;
	goodluck.Ambient = float4(0.7f, 0.85f, 0.7f, 1.0f);
	goodluck.Diffuse = float4(0.7f, 0.85f, 0.7f, 1.0f);
	goodluck.Specular = float4(0.8f, 0.85f, 0.8f, 1.0f);
	goodluck.Reflect = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 A, D, S;

	float3 normal = normalize(input.Normal);

	ComputeDirectionalLight(goodluck, gDirLight, normal, toEye, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;
	
	ComputePointLight(goodluck, gPointLight, input.Pos, normal, toEye, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;
	
	float4 litColor = ambient + diffuse + spec;

		//return gPointLight.Ambient;
	return litColor;

}
