#pragma once

#include "graphics.h"
#include "render_packet.h"

#include <cstdint>

class CBatchRenderHelper {
 public:
  struct LocalCbData {
    void clear() {
      _BatchSize = 0;
      _PerObject.clear();
      _PerMaterial.clear();
      _PerSkeleton.clear();
    }

    uint32_t _BatchSize = 0;
    std::vector<uint8_t> _PerObject;
    std::vector<uint8_t> _PerMaterial;
    std::vector<uint8_t> _PerSkeleton;
  };

  void Initialize(CGraphics& Graphics);
  void RenderInstanced(CRenderContext& RenderContext, CGraphics& Graphics,
                       ERenderPass Pass);
  /// Does NOT assume ownership of the given data. Caller must ensure data persist until rendering.
  void QueForInstancedRendering(const SRenderPacket& Packet, ERenderPass Pass);

 private:
  void UpdateContext(CRenderContext& Context, CGraphics& Graphics,
                     SRenderPacket& CurrPacket,
                     SRenderPacket* pPrevPacket) const;
  void RenderBatch(CRenderContext& RenderContext, const LocalCbData& CbData,
                   const SRenderPacket& LastPacket, ERenderPass Pass);

  std::vector<SRenderPacket> _NormalRenderQue;
  std::vector<SRenderPacket> _ShadowRenderQue;

  // Separate constant buffers per channel
  CConstantBuffer _PerObjectCB;
  CConstantBuffer _PerMaterialCB;
  CConstantBuffer _PerSkeletonCB;

  static inline uint32_t _CbSize = 32768; // 32KB per channel per batch
};
