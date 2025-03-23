#pragma once

#include "core/vector.h"
#include "game_object.h"
#include "graphics/constant_buffer.h"
#include "graphics/default_object_constant_buffer.h"
#include "graphics/graphics.h"
#include "graphics/pixel_shader.h"
#include "graphics/vertex_shader.h"

#include <array>
#include <vector>

/// Simple point light (emitting light in all directions)
class CLightSource : public CGameObject {
 public:
  struct SVertex {
    CVector3f _Position;
    CVector3f _Normal;
  };

  void Initialize(CGraphics& Graphics) override;
  void Shutdown() override;
  bool IsInitialized() const override;

  void Render(CBatchRenderHelper& BatchRenderHelper,
              const CCameraBase& Camera) override;

 private:
  std::vector<SVertex> _Vertices;

  SMaterial _Material;
  SMesh _Mesh;
  SDefaultObjectCb _CbData;
  SDefaultMaterialCb _MatCb;

  bool _IsInitialized = false;
};
