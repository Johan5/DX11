#pragma once

#include "vector.h"
#include "matrix.h"

struct SDefaultColorData
{
	float _DiffuseStrength = 1.0f;
	float _SpecularStrength = 1.0f;
	float _AmbientStrength = 0.15f;
	// Large specular power means small specular highlight
	int _SpecularPower = 256; // "phong exponent"
	// Pink
	CVector4f _Color = { 1.0f, 0.08f, 0.6f, 1.0f };
};
struct SDefaultObjectConstantBuffer
{
	// Local To World
	CMatrix4x4f _WorldMatrix;
	// Local To World for normals
	CMatrix4x4f _NormalWorldMatrix;
	SDefaultColorData _ColorData;
};