

cbuffer MatrixBuffer
{
	// To world space
	matrix _WorldMatrix;
	// To camera space and then to viewport
	matrix _ViewAndProjectionMatrix;
};

struct SVsInput
{
	float3 _Position : POSITION;
	float4 _Color : COLOR;
};

struct SPsInput
{
	float4 _Position : SV_POSITION;
	float4 _Color : COLOR;
};


SPsInput ColorVertexShader( SVsInput Input ) 
{
	SPsInput Output;

	Output._Position.xyz = Input._Position;
	Output._Position.w = 1.0;
	Output._Position = mul( Output._Position, _WorldMatrix );
	Output._Position = mul( Output._Position, _ViewAndProjectionMatrix );
	
	Output._Color = Input._Color;
	return Output;
}

