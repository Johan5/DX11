#include "renderer/xbot.h"

#include "core/3d_math.h"
#include "graphics/graphic_resources.h"
#include "graphics/graphics_enums.h"
#include "graphics/pixel_shader.h"
#include "input/camera_base.h"
#include "utils/assert.h"
#include "utils/logger.h"

#include <random>
#include <vector>

namespace {
// Lazy rgb implementation
CVector4f GenRandomColor() {
  static std::mt19937 Rng{0};
  static std::uniform_real_distribution<float> Distribution(0.0f, 1.0f);
  return CVector4f{Distribution(Rng), Distribution(Rng), Distribution(Rng),
                   1.0f};
}

std::pair<CTextureView, CSamplerState> CreateBrickTextureView(
    CGraphics& Graphics) {
  std::optional<CTextureView> BrickTextureView =
      Graphics.GetTextureViewByName("bricks");
  ASSERT(BrickTextureView, "Failed to retrieve brick texture");
  if (!BrickTextureView) {
    return {CTextureView{}, CSamplerState{}};
  }

  std::optional<CSamplerState> SamplerState = Graphics.GetDefaultSamplerState();
  ASSERT(SamplerState, "Failed to retrieve brickdefault sampler");
  if (!SamplerState) {
    return {CTextureView{}, CSamplerState{}};
  }

  return {BrickTextureView.value(), SamplerState.value()};
}
}  // namespace

XBot::~XBot() {
  XBot::Shutdown();
}

void XBot::Initialize(CGraphics& Graphics) {
  _Material._VS = shader_names::CharacterVertexShaderFileName;
  _Material._PS = shader_names::CharacterPixelShaderFileName;

  SMesh* pMesh = Graphics.AccessMesh(EMeshType::XBot);
  if (pMesh) {
    _Mesh = *pMesh;
    // The mesh is quite large (should probably be fixed in the mesh itself)
    SetScale(CVector3f{0.01f, 0.01f, 0.01f});
  } else {
    ASSERT(false, "Failed to retrieve cube mesh");
    return;
  }

  // Default material parameters for character
  _MatCb._Material._DiffuseStrength = 1.0f;
  _MatCb._Material._SpecularStrength = 0.5f;
  _MatCb._Material._AmbientStrength = 0.15f;
  _MatCb._Material._SpecularPower = 128;
  _MatCb._Material._Color = GenRandomColor();
  _MatCb._Material._TextureSlot = -1; // no texture by default

  _IsInitialized = true;
}

void XBot::Shutdown() {
  // TODO cleanup vertex buffer?
  _IsInitialized = false;
}

bool XBot::IsInitialized() const {
  return _IsInitialized;
}

void XBot::Render(CBatchRenderHelper& BatchRenderHelper,
                  const CCameraBase& Camera) {
  if (!IsInitialized()) {
    return;
  }
  ASSERT(_Mesh._OffsetMatrices, "Corrupt mesh");
  ASSERT(_Mesh._GlobalBindPoseTransforms, "Corrupt mesh");

  // Update per-object transforms
  _ObjectCb._WorldMatrix = GetLocalToWorldTransform();
  _ObjectCb._NormalWorldMatrix = GetNormalLocalToWorldTransform();

  // Update skinning matrices
  for (size_t boneId = 0; boneId < _Mesh._OffsetMatrices->size(); boneId++) {
    _CbData._SkinningMatrix[boneId] =
        _Mesh._OffsetMatrices.value()[boneId] *
        _Mesh._GlobalBindPoseTransforms.value()[boneId];
  }

  // Queue render with split constant buffers
  SRenderPacket NormalRenderPacket{
      ._Mesh = _Mesh,
      ._Material = _Material,
      ._Cbs = SInstanceCbs{._PerObject = SRawPtrConstantBufferData{
                                 ._ConstantData = static_cast<void*>(&_ObjectCb),
                                 ._ConstantDataByteSize = sizeof(_ObjectCb)},
                             ._PerMaterial = SRawPtrConstantBufferData{
                                 ._ConstantData = static_cast<void*>(&_MatCb),
                                 ._ConstantDataByteSize = sizeof(_MatCb)},
                             ._PerSkeleton = SRawPtrConstantBufferData{
                                 ._ConstantData = static_cast<void*>(_CbData._SkinningMatrix),
                                 ._ConstantDataByteSize = sizeof(_CbData._SkinningMatrix)}}};
  BatchRenderHelper.QueForInstancedRendering(NormalRenderPacket,
                                             ERenderPass::Normal);
}

bool XBot::ShouldRenderShadows() const {
  return _ShouldRenderShadow;
}

void XBot::DisableShadowRendering() {
  _ShouldRenderShadow = false;
}

SMaterial& XBot::AccessMaterial() {
  return _Material;
}

SCharacterConstantBuffer& XBot::AccessConstantBuffer() {
  return _CbData;
}