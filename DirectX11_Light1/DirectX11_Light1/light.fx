//-------------------------------------------------------------------------------------
//������սṹ
//----------------------------------------------------------------------------------------
struct DirectionalLight	////ƽ�й�
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight		////���
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight	////�۹��
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
// �������嶨��
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)	//���磬ͶӰ����ת��
{
	matrix World;
	matrix View;
	matrix Projection;
}
cbuffer cbPerFrameLight						//���߽ṹ���,��ʱ��һ��ƽ�й�
{
	DirectionalLight gDirLight;	//ƽ�й�
	PointLight gPointLight;		//���
	//SpotLight gSpotLight;		//�۹��
	float4 gEyePosW;			//�۾�����
};
//--------------------------------------------------------------------------------------
// ��������ṹ
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
//�����ṹ����
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
// ���ܺ���ʵ��
//--------------------------------------------------------------------------------------
//ʵ��ƽ�й�ComputeDirectionalLight�����ʣ�ƽ�й�ṹ������㷨��,ָ���۾��ĵ�λ����,����1������2������3��
void ComputeDirectionalLight(
	Material mat, DirectionalLight L,
	float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// ��ʼ������ı���
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// ����ʸ������ߵĴ��������෴
	float3 lightVec = -L.Direction;

		// ��ӻ�����
		ambient = mat.Ambient * L.Ambient;

	// ���������;���⣬ע�⣬��֤normal��lightVec�ǵ�λ��������һ����
	float diffuseFactor = dot(lightVec, normal);//����cosA

	// Flatten���⶯̬��֧
	[branch]
	if (diffuseFactor > 0.0f)	//����Ƕȵ�����ֵ����0��
	{
		float3 v = reflect(-lightVec, normal);//reflectͨ��������뷨�߼��㷴������
			float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);//pow���ڼ�����ʹ���߷����

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////
//ʵ�ֵ��Դ�ĺ���ComputePointLight�����ʣ����Դ�ṹ������λ�ã����䷨�ߣ�ָ���۾��ĵ�λ����������1������2������3��
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
//ʵ�־۹�Ƶĺ���ComputeSpotLight�����ϣ��۹�ƽṹ������λ�ã����㷨�ߣ�ָ���۾�����������1������2������3��
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	//����ɶ���λ��ָ���Դλ�õ�����
	float3 lightVec = L.Position - pos;

		// The distance from surface to light.
		//��þ���
		float d = length(lightVec);

	// Range test.
	//�Ƿ񳬳�
	if (d > L.Range)
		return;

	// Normalize the light vector.
	//��λ��
	lightVec /= d;

	// Ambient term.
	//���㻷����
	ambient = mat.Ambient * L.Ambient;
	
	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	//���㷽���뷨�ߵ�����ֵ
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
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);//����˥��ֵ

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(L.Att, float3(1.0f, d, d*d));
	
	ambient *= spot;
	diffuse *= att;
	spec *= att;
	
}

//=======================================================================================
//--------------------------------------------------------------------------------------
// ������ɫ��
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
// ������ɫ��
//----------------------------------------------------------------------------------- ---
float4 PS(VS_OUTPUT input) : SV_Target
{

	float3 toEye = normalize(float3(0.0f, 1.0f, 1.0f));
	//��ʼ��3��float4��Ż����⣬������⣬�߹�
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
