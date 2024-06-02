
#pragma pack_matrix( row_major )

#define MAX_DRAW_BATCH 100

cbuffer PerCameraCb : register(b1)
{
	// World To Camera space
	float4x4 _ViewMatrix;
	// World To Viewport
	float4x4 _ViewAndProjectionMatrix;
};

cbuffer PerLightCb : register(b2)
{
	float3 _W_LightPos;
	float _LightIntensity;
}

struct SMaterial
{
	float _DiffuseStrength;
	float _SpecularStrength;
	float _AmbientStrength;
	// Large specular power means small specular highlight
	int _SpecularPower; // "phong exponent"
    float4 _Color;
};

struct PerObjectCbData
{
	// Local To World
    float4x4 _WorldMatrix;
	// Local To World for normals
    float4x4 _NormalWorldMatrix;
    SMaterial _Material;
};

cbuffer PerObjectCb : register(b3) 
{
    PerObjectCbData _PerObjectData[MAX_DRAW_BATCH];

}

PerObjectCbData GetCbData(uint InstanceId)
{
    return _PerObjectData[InstanceId];
}

struct SVsInput
{
	float3 _L_Position : POSITION;
	float3 _L_Normal : NORMAL;
    uint _InstanceId : SV_InstanceID;
};

struct SPsInput
{
	float4 _S_Position : SV_POSITION;
	float3 _W_Position : WORLD_SPACE_POSITION;
	float3 _C_Position : CAMERA_SPACE_POSITION;
	float3 _C_Normal : NORMAL;
	float3 _L0_Position : LIGHT0_SPACE_POSITION;
    uint _InstanceId : SV_InstanceID;
};

SPsInput DefaultVertexShader( SVsInput Input ) 
{
	SPsInput Output;

    PerObjectCbData Cb = GetCbData(Input._InstanceId);
	
    float4 W_Position = mul(Cb._WorldMatrix, float4(Input._L_Position, 1.0f));
	Output._W_Position = W_Position.xyz / W_Position.w;

	Output._S_Position = mul(_ViewAndProjectionMatrix, W_Position);

	float4 C_Position = mul(_ViewMatrix, W_Position);
	Output._C_Position = C_Position.xyz / C_Position.w;
	
    float4 W_Normal = mul(Cb._NormalWorldMatrix, float4(Input._L_Normal, 0.0f));
	W_Normal.w = 0.0f;
	float4 C_Normal = mul( _ViewMatrix, W_Normal );
	Output._C_Normal = normalize(C_Normal.xyz);

    Output._InstanceId = Input._InstanceId;
	
	return Output;
}

////////////////////////////////////////////////////////////////////////////////

TextureCube ShadowMap : register(t0);

SamplerState ShadowMapSampler : register(s0);

struct SLightingData
{
	float3 _Diffuse;
	float3 _Specular;
};

SLightingData CalcLighting(float3 _C_Pos, float3 _C_Normal, PerObjectCbData Cb)
{	
	float4 C_LightPos4f = mul(_ViewMatrix, float4(_W_LightPos, 1.0f));
	float3 C_LightPos = C_LightPos4f.xyz / C_LightPos4f.w;
	float3 C_VecToLight = C_LightPos - _C_Pos;
	float3 C_VecToCam = -_C_Pos;
	
	float3 n = normalize(_C_Normal); //interpolation might have caused it to be not normal?
	float3 l = normalize(C_VecToLight);
	float3 c = normalize(C_VecToCam);
	
	float3 DiffuseColor = Cb._Material._DiffuseStrength * _LightIntensity * Cb._Material._Color.xyz * saturate(dot(n, l));
	
	float3 SpecularColor;
	{
		float3 HighlightColor = float3(1.0f, 1.0f, 1.0f); // useing white highlight instead of object's color
		// h is the (unit) vector between l and c
		float3 h = normalize(l + c);
		float SpecularCosAngle = saturate(dot(n, h));
        SpecularColor = Cb._Material._SpecularStrength * _LightIntensity * HighlightColor * pow(SpecularCosAngle, Cb._Material._SpecularPower);
    }
		
	float Attenuation;
	{
		float DistToLightSq = dot(C_VecToLight, C_VecToLight);
		float AttenuationFactor = 0.02f;
		Attenuation = 1.0f / (1.0f + AttenuationFactor * DistToLightSq);
	}
	
	SLightingData Output;
	Output._Diffuse = Attenuation * DiffuseColor;
	Output._Specular = Attenuation * SpecularColor;
	return Output;
}

// Blinn-Phong reflection
float4 DefaultPixelShader(SPsInput Input) : SV_TARGET
{
	bool IsInShadow = false;
	{
		float3 LightToPos = Input._W_Position - _W_LightPos;
		float3 LightToPosDirection = normalize(LightToPos);
		float ClosestObjectDist = ShadowMap.Sample(ShadowMapSampler, LightToPosDirection);
		float LightToPosLength = length(LightToPos);
		float DepthBias = 0.01f;
		if (LightToPosLength > ClosestObjectDist + DepthBias)
		{
			IsInShadow = true;
		}
	}
    PerObjectCbData Cb = GetCbData(Input._InstanceId);
	
	float3 LightSourceColor;
	if (IsInShadow)
	{
		LightSourceColor = float3(0.0, 0.0, 0.0);
	}
	else
	{
        SLightingData LightingData = CalcLighting(Input._C_Position, Input._C_Normal, Cb);
		LightSourceColor = LightingData._Diffuse + LightingData._Specular;
	}
	
	float3 AmbientColor = Cb._Material._AmbientStrength * Cb._Material._Color.xyz;	
	float3 CombinedColor = AmbientColor + LightSourceColor;
	
	float3 GammeExp = float3(1.0f, 1.0f, 1.0f) / 2.2f;
	float3 GammaCorrectedColor = pow(CombinedColor, GammeExp);
	
	return float4(saturate(GammaCorrectedColor), 1.0f);
}
