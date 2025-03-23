#pragma once

#include "material.h"
#include "mesh.h"

#include <optional>
#include <span>
#include <variant>

struct SRawPtrConstantBufferData {
  const void* _ConstantData = nullptr;
  uint32_t _ConstantDataByteSize = 0;
};

// Per-instance constant buffers for a single draw packet.
// - _PerObject is required for all draws (shape varies by pass/shader).
// - _PerMaterial is used for normal (lit) passes.
// - _PerSkeleton is used for skinned meshes.
struct SInstanceCbs {
  SRawPtrConstantBufferData _PerObject{};
  std::optional<SRawPtrConstantBufferData> _PerMaterial{};
  std::optional<SRawPtrConstantBufferData> _PerSkeleton{};
};

struct SRenderPacket {
  friend bool CanUseSameDraw(const SRenderPacket& a, const SRenderPacket& b);

  SMesh _Mesh;
  SMaterial _Material;

  SInstanceCbs _Cbs;
};
