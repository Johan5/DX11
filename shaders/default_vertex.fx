
#pragma pack_matrix( row_major )

#include "shared_definitions.fx"

struct SVsInput
{
	float3 _L_Position : POSITION;
	float3 _L_Normal : NORMAL;
    float2 _UV : TEXCOORD0;
    uint _InstanceId : SV_InstanceID;
};

SPsInput DefaultVertexShader( SVsInput Input ) 
{
    SPsInput Output;

    PerObjectCbData ObjectCb = _PerObjectData[Input._InstanceId];

    float4 W_Position = mul(float4(Input._L_Position, 1.0f), ObjectCb._WorldMatrix);
    Output._W_Position = W_Position.xyz;

    Output._S_Position = mul(W_Position, _ViewAndProjectionMatrix);

    float4 C_Position = mul(W_Position, _ViewMatrix);
    Output._C_Position = C_Position.xyz;

    float4 W_Normal = mul(float4(Input._L_Normal, 0.0f), ObjectCb._NormalWorldMatrix);
    float4 C_Normal = mul(W_Normal, _ViewMatrix);
    Output._C_Normal = normalize(C_Normal.xyz);

    Output._UV = Input._UV;
    Output._InstanceId = Input._InstanceId;

    return Output;
}
