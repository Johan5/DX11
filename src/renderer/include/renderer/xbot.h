#pragma once

#include <d3d11.h>
#include <vector>

#include "core/matrix.h"
#include "core/vector.h"
#include "game_object.h"
#include "graphics/constant_buffer.h"
#include "graphics/default_object_constant_buffer.h"
#include "graphics/graphics.h"
#include "graphics/material.h"
#include "graphics/pixel_shader.h"
#include "graphics/render_packet.h"
#include "graphics/vertex_shader.h"

#include <mutex>


struct SCharacterConstantBuffer {
  // Local To World
  CMatrix4x4f _WorldMatrix;
  // Local To World for normals
  CMatrix4x4f _NormalWorldMatrix;

  CMatrix4x4f _SkinningMatrix[NGraphicsDefines::MaxBones];
};

class XBot : public CGameObject {
 public:
  ~XBot() override;

  void Initialize(CGraphics& Graphics) override;
  void Shutdown() override;
  bool IsInitialized() const override;

  void Render(CBatchRenderHelper& BatchRenderHelper,
              const CCameraBase& Camera) override;

  bool ShouldRenderShadows() const;
  void DisableShadowRendering();

  SMaterial& AccessMaterial();
  SCharacterConstantBuffer& AccessConstantBuffer();

 private:
  SMaterial _Material;
  SMesh _Mesh;
  SCharacterConstantBuffer _CbData;
  SDefaultObjectCb _ObjectCb;
  SDefaultMaterialCb _MatCb;

  bool _IsInitialized = false;
  bool _ShouldRenderShadow = false;
};
