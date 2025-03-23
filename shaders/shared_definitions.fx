
#ifndef SHARED_DEFINITIONS_HLSL
#define SHARED_DEFINITIONS_HLSL

#pragma pack_matrix( row_major )

#define MAX_BONES 200
// !! I edited these, check so c++ does correct thing !!
#define MAX_DRAW_BATCH_OBJECT 100
#define MAX_DRAW_BATCH_CHAR 1

struct SMaterial
{
	float _DiffuseStrength;
	float _SpecularStrength;
	float _AmbientStrength;
	// Large specular power means small specular highlight
	int _SpecularPower; // "phong exponent"
    float4 _Color; // Only used if no Texture
    int _TextureSlot; // -1 if no texture
    uint3 _Pad;
};

struct PerObjectCbData
{
	// Local To World
    float4x4 _WorldMatrix;
	// Local To World for normals
    float4x4 _NormalWorldMatrix;
};

struct PerMaterialCbData
{
    SMaterial _Material;
};

struct PerSkeletonCbData
{
    float4x4 _SkinningMatrix[MAX_BONES];
};

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

// Index using [InstanceId]
cbuffer PerObjectCb : register(b3)
{
    PerObjectCbData _PerObjectData[MAX_DRAW_BATCH_OBJECT];
}

PerObjectCbData GetObjectCb(uint InstanceId)
{
    return _PerObjectData[InstanceId];
}

// Index using [InstanceId]
cbuffer PerMaterialCb : register(b4)
{
    PerMaterialCbData _PerMaterialCbData[MAX_DRAW_BATCH_OBJECT];
}

PerMaterialCbData GetMaterialCb(uint InstanceId)
{
    return _PerMaterialCbData[InstanceId];
}

// Index using [InstanceId]
cbuffer PerSkeletonCbData : register(b5)
{
    PerSkeletonCbData _PerSkeletonCbData[MAX_DRAW_BATCH_CHAR];
}

PerSkeletonCbData GetSkeletonCb(uint InstanceId)
{
    return _PerSkeletonCbData[InstanceId];
}

struct SPsInput
{
    float4 _S_Position : SV_POSITION;
    float3 _W_Position : WORLD_SPACE_POSITION;
    float3 _C_Position : CAMERA_SPACE_POSITION;
    float3 _C_Normal : NORMAL;
    float2 _UV : TEXCOORD0;
    uint _InstanceId : SV_InstanceID;
};

#endif // COMMON_STRUCTS_HLSL
