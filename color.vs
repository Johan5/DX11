

cbuffer PerCameraCb : register(b1)
{
	// World To Viewport
	float4x4 _ViewAndProjectionMatrix;
};

cbuffer PerObjectCb : register(b2) 
{
	// Local To World
	float4x4 _WorldMatrix;
}

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
	//Output._Position = mul( Output._Position, _WorldMatrix );
	Output._Position = mul( Output._Position, _ViewAndProjectionMatrix );
	
	Output._Color = Input._Color;

	////
	if ( Input._Position.x < -0.99f && Input._Position.y < -0.99f && Input._Position.z < -0.99f ) {
		Output._Color = float4( 0.2f, 0.2f, 0.2f, 1.0f );
 	}

	////

	return Output;
}

