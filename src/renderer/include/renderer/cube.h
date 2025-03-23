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

class CCube : public CGameObject {
 public:
  ~CCube() override;

  void Initialize(CGraphics& Graphics) override;
  void Shutdown() override;
  bool IsInitialized() const override;

  void Render(CBatchRenderHelper& BatchRenderHelper,
              const CCameraBase& Camera) override;

  bool ShouldRenderShadows() const;
  void DisableShadowRendering();

  SMaterial& AccessMaterial();
  SDefaultObjectCb& AccessObjectCb();
  SDefaultMaterialCb& AccessMaterialCb();


 private:
  SMaterial _Material;
  SMaterial _ShadowMaterial;
  SMesh _Mesh;
  std::string _Texture = "bricks";
  SDefaultObjectCb _objectCb;
  SDefaultMaterialCb _materialCb;

  bool _IsInitialized = false;
  bool _ShouldRenderShadow = true;
};
