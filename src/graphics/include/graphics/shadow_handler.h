#pragma once

#include "batch_render_helper.h"
#include "constant_buffer.h"
#include "depth_stencil_view.h"
#include "graphics.h"
#include "render_target_view.h"
#include "sampler_state.h"
#include "texture.h"
#include "texture_view.h"

#include <d3d11.h>
#include <wrl/client.h>  // Microsoft ComPtr
#include <cstdint>

namespace NShadowPass {
struct SPerLightCb {
  // +X, -X, +Y, -Y, +Z, -Z
  CMatrix4x4f _LightViewMatrices[6];
  CMatrix4x4f _LightProjectionMatrix;
  float _LightMaxDist;
  CVector3f _Pad1;
};
}  // namespace NShadowPass

struct SShadowData {
  // A cubemap
  CTextureView _ShadowMap;
  CSamplerState _ShadowMapSampler;
};

// TODO: make sure everything is cleaned up properly
class CShadowHandler {
 public:
  CShadowHandler() = default;
  void Initialize(CGraphics& Graphics, uint32_t Width, uint32_t Height);

  // Returns ptr to the shadow texture array (cubemap)
  SShadowData CreateShadowMap(CGraphics& Graphics,
                              CBatchRenderHelper& BatchRenderHelper,
                              const CVector3f& LightPos);

  CVector2f GetShadowMapDimensions();

 private:
  CConstantBuffer _PerLightConstantBuffer;
  // Pointer to array of 6 cubemap textures
  CTexture _CubemapTexture;
  // Viewing texture as render target
  CRenderTargetView _RenderTargetView;
  // Viewing texture as texture
  CTextureView _TextureView;

  CTexture _pCubeMapDepthStencilTexture;
  CDepthStencilView _DepthStencilView;

  CSamplerState _SamplerState;
};
