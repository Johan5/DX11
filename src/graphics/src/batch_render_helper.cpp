#include "graphics/batch_render_helper.h"

#include "graphics/graphics_enums.h"
#include "utils/assert.h"

#include <algorithm>

namespace {
void AppendConstantDataToVec(const SRenderPacket& RenderPacket,
                             CBatchRenderHelper::LocalCbData& DataInOut) {
  // Per-object is always required
  {
    const uint8_t* pData = static_cast<const uint8_t*>(
        RenderPacket._Cbs._PerObject._ConstantData);
    DataInOut._PerObject.insert(
        DataInOut._PerObject.end(), pData,
        pData + RenderPacket._Cbs._PerObject._ConstantDataByteSize);
  }
  // Per-material (optional)
  if (RenderPacket._Cbs._PerMaterial) {
    const auto& mat = *RenderPacket._Cbs._PerMaterial;
    const uint8_t* pData = static_cast<const uint8_t*>(mat._ConstantData);
    DataInOut._PerMaterial.insert(DataInOut._PerMaterial.end(), pData,
                                  pData + mat._ConstantDataByteSize);
  }
  // Per-skeleton (optional)
  if (RenderPacket._Cbs._PerSkeleton) {
    const auto& skn = *RenderPacket._Cbs._PerSkeleton;
    const uint8_t* pData = static_cast<const uint8_t*>(skn._ConstantData);
    DataInOut._PerSkeleton.insert(DataInOut._PerSkeleton.end(), pData,
                                  pData + skn._ConstantDataByteSize);
  }

  DataInOut._BatchSize++;
}

struct SRenderPacketSorter {
  // The purpose here is to group the packets to optimize performance. We don't care which (e.g.) material is rendered first.
  bool operator()(const SRenderPacket& a, const SRenderPacket& b) {
    if (a._Material != b._Material) {
      return a._Material.CalcRenderHash() < b._Material.CalcRenderHash();
    }
    return a._Mesh._MeshType < b._Mesh._MeshType;
  }
};
}  // namespace

void CBatchRenderHelper::Initialize(CGraphics& Graphics) {
  _PerObjectCB = Graphics.CreateConstantBuffer(_CbSize, ECpuAccessPolicy::CpuWrite);
  _PerMaterialCB = Graphics.CreateConstantBuffer(_CbSize, ECpuAccessPolicy::CpuWrite);
  _PerSkeletonCB = Graphics.CreateConstantBuffer(_CbSize, ECpuAccessPolicy::CpuWrite);
}

void CBatchRenderHelper::RenderInstanced(CRenderContext& RenderContext,
                                         CGraphics& Graphics,
                                         ERenderPass Pass) {
  std::vector<SRenderPacket>& RenderQue =
      (Pass == ERenderPass::Normal ? _NormalRenderQue : _ShadowRenderQue);
  if (RenderQue.empty()) {
    return;
  }

  std::sort(RenderQue.begin(), RenderQue.end(), SRenderPacketSorter{});

  LocalCbData CbData;
  SRenderPacket* pPrevPacket = nullptr;
  auto wouldOverflow = [&](const SRenderPacket& pkt) {
    bool obj = (CbData._PerObject.size() +
                pkt._Cbs._PerObject._ConstantDataByteSize) > _CbSize;
    bool mat = pkt._Cbs._PerMaterial &&
               (CbData._PerMaterial.size() +
                pkt._Cbs._PerMaterial-> _ConstantDataByteSize) > _CbSize;
    bool skn = pkt._Cbs._PerSkeleton &&
               (CbData._PerSkeleton.size() +
                pkt._Cbs._PerSkeleton-> _ConstantDataByteSize) > _CbSize;
    return obj || mat || skn;
  };

  for (SRenderPacket& CurrPacket : RenderQue) {
    bool cbIsFull = wouldOverflow(CurrPacket);
    bool ContextUpdateRequired = (pPrevPacket == nullptr) ||
                                 !CanUseSameDraw(*pPrevPacket, CurrPacket) ||
                                 cbIsFull;
    if (ContextUpdateRequired) {
      if (pPrevPacket) {
        // Render the previous batch, not including CurrPacket
        RenderBatch(RenderContext, CbData, *pPrevPacket, Pass);
        CbData.clear();
      }
      // Update Context for new batch, which starts with CurrPacket
      UpdateContext(RenderContext, Graphics, CurrPacket, pPrevPacket);
    }

    ASSERT(ContextUpdateRequired || !pPrevPacket ||
               pPrevPacket->_Cbs._PerObject._ConstantDataByteSize ==
                   CurrPacket._Cbs._PerObject._ConstantDataByteSize,
           "Unexpected per-object constant buffer size change in batch");

    AppendConstantDataToVec(CurrPacket, CbData);

    pPrevPacket = &CurrPacket;
  }
  if (pPrevPacket) {
    // Render the final batch
    RenderBatch(RenderContext, CbData, *pPrevPacket, Pass);
  }
  RenderQue.clear();
}

void CBatchRenderHelper::QueForInstancedRendering(const SRenderPacket& Packet,
                                                  ERenderPass Pass) {
  switch (Pass) {
    case ERenderPass::Normal:
      _NormalRenderQue.push_back(Packet);
      break;
    case ERenderPass::Shadow:
      _ShadowRenderQue.push_back(Packet);
      break;
    default:
      ASSERT(false, "This should never happen.");
      break;
  }
}

void CBatchRenderHelper::UpdateContext(CRenderContext& Context,
                                       CGraphics& Graphics,
                                       SRenderPacket& CurrPacket,
                                       SRenderPacket* pPrevPacket) const {
  bool UpdateVertexAndIndexBuffers =
      !pPrevPacket || pPrevPacket->_Mesh != CurrPacket._Mesh;
  bool UpdateVertexShader =
      !pPrevPacket || pPrevPacket->_Material._VS != CurrPacket._Material._VS;
  bool UpdateGeoShader =
      !pPrevPacket || pPrevPacket->_Material._GS != CurrPacket._Material._GS;
  bool UpdatePixelShader =
      !pPrevPacket || pPrevPacket->_Material._PS != CurrPacket._Material._PS;
  bool UpdatePixelShaderTexture =
      UpdatePixelShader ||
      pPrevPacket->_Material._Texture != CurrPacket._Material._Texture;
  bool UpdatePixelShaderSampler =
      UpdatePixelShader || pPrevPacket->_Material._TextureSampler !=
                               CurrPacket._Material._TextureSampler;

  if (UpdateVertexAndIndexBuffers) {
    Context.SetVertexBuffer(CurrPacket._Mesh._VertexBuffer);
    if (CurrPacket._Mesh._IndexBuffer.IsValid()) {
      Context.SetIndexBuffer(CurrPacket._Mesh._IndexBuffer);
    }
  }
  if (UpdateVertexShader) {
    CVertexShader* pVS = Graphics.AccessVertexShader(CurrPacket._Material._VS);
    ASSERT(pVS, "Failed to find VertexShader");
    if (pVS) {
      Context.SetVertexShader(*pVS);
    }
  }
  if (UpdateGeoShader) {
    CGeometryShader* pGS =
        Graphics.AccessGeometryShader(CurrPacket._Material._GS);
    if (pGS) {
      Context.SetGeometryShader(*pGS);
    }
  }
  if (UpdatePixelShader) {
    CPixelShader* pPS = Graphics.AccessPixelShader(CurrPacket._Material._PS);
    ASSERT(pPS, "Failed to find PixelShader");
    if (pPS) {
      Context.SetPixelShader(*pPS);
    }
  }
  if (UpdatePixelShaderTexture) {
    Context.SetPixelShaderTexture(CurrPacket._Material._Texture,
                                  NGraphicsDefines::ShadowMapTextureSlot + 1);
  }
  if (UpdatePixelShaderSampler) {
    Context.SetPixelShaderSampler(CurrPacket._Material._TextureSampler,
                                  NGraphicsDefines::ShadowMapSamplerSlot + 1);
  }
}

void CBatchRenderHelper::RenderBatch(CRenderContext& RenderContext,
                                     const LocalCbData& CbData,
                                     const SRenderPacket& LastPacket,
                                     ERenderPass Pass) {
  if (_PerObjectCB.AccessRawBuffer() == nullptr) {
    ASSERT(false, "Trying to render with uninitialized RenderManager");
    return;
  }
  // 1. Upload CB channels and bind to expected slots
  // Per-object is used by VS (always) and PS (normal pass shaders reference it)
  RenderContext.UpdateConstantBuffer(_PerObjectCB, CbData._PerObject.data(),
                                     CbData._PerObject.size());
  RenderContext.SetVertexShaderConstantBuffer(_PerObjectCB,
                                              EConstantBufferIdx::PerObject);
  RenderContext.SetPixelShaderConstantBuffer(_PerObjectCB,
                                             EConstantBufferIdx::PerObject);

  // Per-material is pixel-shader only, only in normal pass
  if (Pass == ERenderPass::Normal && !CbData._PerMaterial.empty()) {
    RenderContext.UpdateConstantBuffer(_PerMaterialCB, CbData._PerMaterial.data(),
                                       CbData._PerMaterial.size());
    RenderContext.SetPixelShaderConstantBuffer(_PerMaterialCB,
                                               EConstantBufferIdx::PerMaterial);
  }

  // Per-skeleton is VS-only
  if (!CbData._PerSkeleton.empty()) {
    RenderContext.UpdateConstantBuffer(_PerSkeletonCB, CbData._PerSkeleton.data(),
                                       CbData._PerSkeleton.size());
    RenderContext.SetVertexShaderConstantBuffer(_PerSkeletonCB,
                                                EConstantBufferIdx::PerSkeleton);
  }

  // 2. Draw
  if (LastPacket._Mesh._VertexBuffer.GetProperties()._SingleVertexSizeInBytes ==
      0) {
    ASSERT(false, "Vertex size of 0");
    return;
  }
  if (CbData._BatchSize == 1) {
    if (LastPacket._Mesh._IndexBuffer.IsValid()) {
      RenderContext.DrawIndexed(LastPacket._Mesh._IndexBuffer.GetIndexCount());
    } else {
      RenderContext.Draw(
          LastPacket._Mesh._VertexBuffer.GetProperties()._VertexCount);
    }
  } else {
    RenderContext.DrawInstanced(
        LastPacket._Mesh._VertexBuffer.GetProperties()._VertexCount,
        CbData._BatchSize);
  }
}
