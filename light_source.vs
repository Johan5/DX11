
#pragma pack_matrix( row_major )

cbuffer PerCameraCb : register(b1)
{
	// World To Camera space
	float4x4 _ViewMatrix;
	// World To Viewport
	float4x4 _ViewAndProjectionMatrix;
};

cbuffer PerObjectCb : register(b2) 
{
	// Local To World
	float4x4 _WorldMatrix;
	// Local To World for normals
	float4x4 _NormalWorldMatrix;
}

struct SVsInput
{
	float3 _L_Position : POSITION;
	float3 _L_Normal : NORMAL;
};

struct SPsInput
{
	float4 _S_Position : SV_POSITION;
	float3 _L_Normal : NORMAL;
};


SPsInput LightSourceVertexShader( SVsInput Input ) 
{
	SPsInput Output;

	float4 W_Position = mul(_WorldMatrix, float4(Input._L_Position, 1.0f));

	Output._S_Position = mul(_ViewAndProjectionMatrix, W_Position);
	Output._L_Normal = Input._L_Normal;

	return Output;
}

