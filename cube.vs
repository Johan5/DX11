
#pragma pack_matrix( row_major )

cbuffer PerCameraCb : register(b1)
{
	// World To Camera space
	float4x4 _ViewMatrix;
	// World To Viewport
	float4x4 _ViewAndProjectionMatrix;
	float3 _Light1Pos;
	float _HasLight1; //used as bool
};

struct SMaterial
{
	float _DiffuseStrength;
	float _SpecularStrength;
	float _AmbientStrength;
	// Large specular power means small specular highlight
	int _SpecularPower; // "phong exponent"
};

cbuffer PerObjectCb : register(b2) 
{
	// Local To World
	float4x4 _WorldMatrix;
	// Local To World for normals
	float4x4 _NormalWorldMatrix;
	SMaterial _Material;
}

struct SVsInput
{
	float3 _L_Position : POSITION;
	float3 _L_Normal : NORMAL;
	float4 _Color : COLOR;
};

struct SPsInput
{
	float4 _S_Position : SV_POSITION;
	float3 _C_Position : POSITION;
	float3 _C_Normal : NORMAL;
	float4 _Color : COLOR;
};


SPsInput CubeVertexShader( SVsInput Input ) 
{
	SPsInput Output;

	float4 W_Position = mul(_WorldMatrix, float4(Input._L_Position, 1.0f));

	Output._S_Position = mul(_ViewAndProjectionMatrix, W_Position);

	float4 C_Position = mul(_ViewMatrix, W_Position);
	C_Position /= C_Position.w;
	Output._C_Position = C_Position;
	
	float4 W_Normal = mul( _NormalWorldMatrix, float4(Input._L_Normal, 0.0f));
	W_Normal.w = 0.0f;
	float4 C_Normal = mul( _ViewMatrix, W_Normal );
	Output._C_Normal = normalize(C_Normal.xyz);
	
	Output._Color = Input._Color;

	return Output;
}

