#pragma pack_matrix( row_major )

#include "shared_definitions.fx"

#define MAX_BONE_INFLUENCE 4

struct SVsInput
{
    float3 _L_Position  : POSITION;
    float3 _L_Normal    : NORMAL;
    float2 _UV          : TEXCOORD0;
    uint4  _BoneIndices : BONE_INDICES;
    float4 _BoneWeights : BONE_WEIGHTS;

    uint   _InstanceId  : SV_InstanceID;
};

SPsInput CharacterVertexShader(SVsInput Input)
{
    SPsInput Output;

    // Fetch per-instance CBs as defined in shared_definitions.fx
    PerObjectCbData   objectCb = GetObjectCb(Input._InstanceId);
PerSkeletonCbData skelCb   = GetSkeletonCb(Input._InstanceId);

    // Build blended skinning matrix
    float  weightSum      = dot(Input._BoneWeights, 1.0f.xxxx);
    float4x4 skinningMatrix;
    if (weightSum > 0.001f)
    {
        skinningMatrix =
              skelCb._SkinningMatrix[Input._BoneIndices.x] * Input._BoneWeights.x
            + skelCb._SkinningMatrix[Input._BoneIndices.y] * Input._BoneWeights.y
            + skelCb._SkinningMatrix[Input._BoneIndices.z] * Input._BoneWeights.z
            + skelCb._SkinningMatrix[Input._BoneIndices.w] * Input._BoneWeights.w;
    }
    else
    {
        // No weights -> identity skinning
        skinningMatrix = float4x4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    }

    // Apply skinning in local space
    float4 L_Pos = mul(float4(Input._L_Position, 1.0f), skinningMatrix);
    float3 L_Nrm = mul(float4(Input._L_Normal,   0.0f), skinningMatrix).xyz;

    // Local -> World
    float4 W_Position = mul(L_Pos, objectCb._WorldMatrix);
    float3 W_Normal   = normalize(mul(float4(L_Nrm, 0.0f), objectCb._NormalWorldMatrix).xyz);

    // World -> Camera and Clip
    float4 C_Position = mul(W_Position, _ViewMatrix);
    float3 C_Normal   = normalize(mul(float4(W_Normal, 0.0f), _ViewMatrix).xyz);
    float4 S_Position = mul(W_Position, _ViewAndProjectionMatrix);

    // Output for downstream pixel shader (default_frag.fx)
    Output._S_Position = S_Position;
    Output._W_Position = W_Position.xyz;
    Output._C_Position = C_Position.xyz;
    Output._C_Normal   = C_Normal;
    Output._UV         = Input._UV;
    Output._InstanceId = Input._InstanceId;

    return Output;
}
