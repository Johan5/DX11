

// Constant buffer
cbuffer MatrixBuffer
{
	// To world space
	matrix _WorldMatrix;
	// To camera space
	matrix _ViewMatrix;
	// To (2D) screen
	matrix _ProjectionMatrix;
};

struct SVsInput
{
	float3 _Position : POSITION;
	//float4 _Color : COLOR;
};

struct SPsInput
{
	float4 _Position : SV_POSITION;
	float4 _Color : COLOR;
};


// Main VS shader?
SPsInput ColorVertexShader( SVsInput Input ) 
{
	SPsInput Output;

	Output._Position.xyz = Input._Position;
	Output._Position.w = 1.0;
	Output._Color = float4( 0.6, 0.2, 0.6, 1.0);
	return Output;

	//Output._Position = mul( Input._Position, _WorldMatrix );
	//Output._Position = mul( Output._Position, _ViewMatrix );
	//Output._Position = mul( Output._Position, _ProjectionMatrix);
	
	//Output._Color = Input._Color;

	//return Output;
}

