
#pragma pack_matrix( row_major )

#include "shared_definitions.fx"

TextureCube ShadowMap : register(t0);
Texture2D Texture1 : register(t1);

SamplerState ShadowMapSampler : register(s0);
SamplerState Sampler1 : register(s1);

struct SLightingData
{
	float3 _Diffuse;
	float3 _Specular;
};

float4 GetMaterialColor(SMaterial Material, float2 UV)
{
    if (Material._TextureSlot == -1)
    {
        return Material._Color;
    }
    if (Material._TextureSlot == 1)
    {
        return Texture1.Sample(Sampler1, UV);
    }
    return float4(1.0f, 0.0f, 1.0f, 1.0);
}

SLightingData CalcLighting(float3 _C_Pos, float3 _C_Normal, float2 UV, PerObjectCbData objectCb,
                           PerMaterialCbData materialCb)
{
	float4 C_LightPos4f = mul(_ViewMatrix, float4(_W_LightPos, 1.0f));
	float3 C_LightPos = C_LightPos4f.xyz / C_LightPos4f.w;
	float3 C_VecToLight = C_LightPos - _C_Pos;
	float3 C_VecToCam = -_C_Pos;

	float3 n = normalize(_C_Normal); //interpolation might have caused it to be not normal?
	float3 l = normalize(C_VecToLight);
	float3 c = normalize(C_VecToCam);

    float3 DiffuseColor = materialCb._Material._DiffuseStrength * _LightIntensity *
                            GetMaterialColor(materialCb._Material, UV).xyz * saturate(dot(n, l));

	float3 SpecularColor;
	{
		float3 HighlightColor = float3(1.0f, 1.0f, 1.0f); // using white highlight instead of object's color
		// h is the (unit) vector between l and c
		float3 h = normalize(l + c);
		float SpecularCosAngle = saturate(dot(n, h));
        SpecularColor = materialCb._Material._SpecularStrength * _LightIntensity * HighlightColor *
                        pow(SpecularCosAngle, materialCb._Material._SpecularPower);
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
    PerObjectCbData objectCb = GetObjectCb(Input._InstanceId);
    PerMaterialCbData materialCb = GetMaterialCb(Input._InstanceId);

	float3 LightSourceColor;
	if (IsInShadow)
	{
		LightSourceColor = float3(0.0, 0.0, 0.0);
	}
	else
	{
        SLightingData LightingData = CalcLighting(Input._C_Position, Input._C_Normal, Input._UV, objectCb, materialCb);
		LightSourceColor = LightingData._Diffuse + LightingData._Specular;
	}

    float3 AmbientColor = materialCb._Material._AmbientStrength * GetMaterialColor(materialCb._Material, Input._UV).xyz;
	float3 CombinedColor = AmbientColor + LightSourceColor;

	float3 GammaExp = float3(1.0f, 1.0f, 1.0f) / 2.2f;
	float3 GammaCorrectedColor = pow(CombinedColor, GammaExp);

	return float4(saturate(GammaCorrectedColor), 1.0f);
}
