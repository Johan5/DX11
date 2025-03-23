#pragma once

#include "core/matrix.h"
#include "core/vector.h"

struct SDefaultColorData {
  float _DiffuseStrength = 1.0f;
  float _SpecularStrength = 1.0f;
  float _AmbientStrength = 0.15f;
  // Large specular power means small specular highlight
  int _SpecularPower = 256;                      // "phong exponent"
  CVector4f _Color = {1.0f, 0.08f, 0.6f, 1.0f};  // Pink
  int32_t _TextureSlot = -1;                     // -1 if no texture
  uint8_t _Pad[12];
};
struct SDefaultObjectConstantBuffer {
  // Local To World
  CMatrix4x4f _WorldMatrix;
  // Local To World for normals
  CMatrix4x4f _NormalWorldMatrix;
  SDefaultColorData _ColorData;
};