#include "graphics/shadow_handler.h"

#include "core/misc_math.h"
#include "core/vector.h"
#include "graphics/graphics_enums.h"
#include "input/camera_base.h"
#include "utils/assert.h"

#include <limits>

void CShadowHandler::Initialize(CGraphics& Graphics, uint32_t Width,
                                uint32_t Height) {
  ASSERT(Width == Height, "Shadow map must be square.");
  {
    D3D11_TEXTURE2D_DESC TextureDesc;
    TextureDesc.Width = Width;
    TextureDesc.Height = Height;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 6;
    TextureDesc.Format =
        static_cast<DXGI_FORMAT>(EGfxResourceDataFormat::R32Float);
    TextureDesc.SampleDesc = DXGI_SAMPLE_DESC{1, 0};
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = static_cast<uint32_t>(EBindFlag::RenderTarget) |
                            static_cast<uint32_t>(EBindFlag::ShaderResource);
    TextureDesc.CPUAccessFlags =
        static_cast<uint32_t>(ECpuAccessPolicy::NoAccess);
    TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    _CubemapTexture = Graphics.CreateTexture(TextureDesc);
  }
  {
    D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc;
    RenderTargetViewDesc.Format =
        static_cast<DXGI_FORMAT>(EGfxResourceDataFormat::R32Float);
    RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    RenderTargetViewDesc.Texture2DArray.MipSlice = 0;
    RenderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
    RenderTargetViewDesc.Texture2DArray.ArraySize = 6;
    _RenderTargetView =
        Graphics.CreateRenderTargetView(_CubemapTexture, RenderTargetViewDesc);
  }
  {
    D3D11_SHADER_RESOURCE_VIEW_DESC ShaderViewDesc;
    ShaderViewDesc.Format =
        static_cast<DXGI_FORMAT>(EGfxResourceDataFormat::R32Float);
    ShaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    ShaderViewDesc.TextureCube.MostDetailedMip = 0;
    ShaderViewDesc.TextureCube.MipLevels = 1;
    _TextureView = Graphics.CreateTextureView(_CubemapTexture, ShaderViewDesc);
  }
  {
    D3D11_TEXTURE2D_DESC TextureDesc;
    TextureDesc.Width = Width;
    TextureDesc.Height = Height;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 6;
    TextureDesc.Format =
        static_cast<DXGI_FORMAT>(EGfxResourceDataFormat::D32Float);
    TextureDesc.SampleDesc = DXGI_SAMPLE_DESC{1, 0};
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = static_cast<uint32_t>(EBindFlag::DepthStencil);
    TextureDesc.CPUAccessFlags =
        static_cast<uint32_t>(ECpuAccessPolicy::NoAccess);
    TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    _pCubeMapDepthStencilTexture = Graphics.CreateTexture(TextureDesc);
  }
  {
    D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
    DepthStencilViewDesc.Format =
        static_cast<DXGI_FORMAT>(EGfxResourceDataFormat::D32Float);
    DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    DepthStencilViewDesc.Flags = 0;
    DepthStencilViewDesc.Texture2DArray.MipSlice = 0;
    DepthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
    DepthStencilViewDesc.Texture2DArray.ArraySize = 6;
    _DepthStencilView = Graphics.CreateDepthStencilView(
        _pCubeMapDepthStencilTexture, DepthStencilViewDesc);
  }

  _PerLightConstantBuffer = Graphics.CreateConstantBuffer(
      sizeof(NShadowPass::SPerLightCb), ECpuAccessPolicy::CpuWrite);

  {
    D3D11_SAMPLER_DESC SamplerDesc;
    ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
    SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.MipLODBias = 0.0f;
    SamplerDesc.MaxAnisotropy = 1U;
    SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    //SamplerDesc.BorderColor
    SamplerDesc.MinLOD = 0;
    SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    _SamplerState = Graphics.CreateSamplerState(SamplerDesc);
  }
}

SShadowData CShadowHandler::CreateShadowMap(
    CGraphics& Graphics, CBatchRenderHelper& BatchRenderHelper,
    const CVector3f& LightPos) {
  CRenderContext& RenderContext = Graphics.AccessRenderContest();
  ID3D11DeviceContext* pDeviceContext =
      RenderContext.Debug_AccessDxRaw().AccessDeviceContext();

  // extra parenthesis since Windows.h defines 'max()' as a macro
  float FloatMax = (std::numeric_limits<float>::max)();
  float ClearColor[4] = {FloatMax, FloatMax, FloatMax, 1.0f};
  pDeviceContext->ClearRenderTargetView(
      _RenderTargetView.AccessRenderTargetView(), ClearColor);
  pDeviceContext->ClearDepthStencilView(
      _DepthStencilView.AccessDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

  RenderContext.SetRenderTargets(
      1, _RenderTargetView.AccessAddrOfRenderTargetView(),
      _DepthStencilView.AccessDepthStencilView());
  RenderContext.SetViewport(GetShadowMapDimensions());
  RenderContext.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  NShadowPass::SPerLightCb LightCb;
  {  // +X, -X, +Y, -Y, +Z, -Z
    LightCb._LightMaxDist = NGraphicsDefines::ScreenFar;
    float FieldOfView = NMiscMath::Pi / 2.0f;
    float AspectRatio = 1.0f;
    CCameraBase DummyCamera{FieldOfView, AspectRatio};
    LightCb._LightProjectionMatrix = DummyCamera.GetProjectionMatrix();
    CVector3f Forward{1.0f, 0.0f, 0.0f};
    CVector3f Up{0.0f, 1.0f, 0.0f};
    DummyCamera.SetOrientation(LightPos, Forward, Up);
    LightCb._LightViewMatrices[0] = DummyCamera.GetViewMatrix();
    Forward = CVector3f{-1.0f, 0.0f, 0.0f};
    Up = CVector3f{0.0f, 1.0f, 0.0f};
    DummyCamera.SetOrientation(LightPos, Forward, Up);
    LightCb._LightViewMatrices[1] = DummyCamera.GetViewMatrix();
    Forward = CVector3f{0.0f, 1.0f, 0.0f};
    Up = CVector3f{0.0f, 0.0f, -1.0f};
    DummyCamera.SetOrientation(LightPos, Forward, Up);
    LightCb._LightViewMatrices[2] = DummyCamera.GetViewMatrix();
    Forward = CVector3f{0.0f, -1.0f, 0.0f};
    Up = CVector3f{0.0f, 0.0f, 1.0f};
    DummyCamera.SetOrientation(LightPos, Forward, Up);
    LightCb._LightViewMatrices[3] = DummyCamera.GetViewMatrix();
    Forward = CVector3f{0.0f, 0.0f, 1.0f};
    Up = CVector3f{0.0f, 1.0f, 0.0f};
    DummyCamera.SetOrientation(LightPos, Forward, Up);
    LightCb._LightViewMatrices[4] = DummyCamera.GetViewMatrix();
    Forward = CVector3f{0.0f, 0.0f, -1.0f};
    Up = CVector3f{0.0f, 1.0f, 0.0f};
    DummyCamera.SetOrientation(LightPos, Forward, Up);
    LightCb._LightViewMatrices[5] = DummyCamera.GetViewMatrix();
  }

  RenderContext.UpdateConstantBuffer(_PerLightConstantBuffer, &LightCb,
                                     sizeof(LightCb));
  RenderContext.SetGeometryShaderConstantBuffer(_PerLightConstantBuffer,
                                                EConstantBufferIdx::PerLight);
  RenderContext.SetPixelShaderConstantBuffer(_PerLightConstantBuffer,
                                             EConstantBufferIdx::PerLight);

  BatchRenderHelper.RenderInstanced(RenderContext, Graphics,
                                    ERenderPass::Shadow);

  RenderContext.ClearShaders();
  RenderContext.RestoreViewport();
  RenderContext.RestoreRenderTarget();

  return SShadowData{_TextureView, _SamplerState};
}

CVector2f CShadowHandler::GetShadowMapDimensions() {
  ASSERT(_CubemapTexture.GetTexture(),
         "Trying to fetch data from uninitialized texture");
  D3D11_TEXTURE2D_DESC CubeMapTextureDesc;
  _CubemapTexture.AccessTexture()->GetDesc(&CubeMapTextureDesc);
  return CVector2f{static_cast<float>(CubeMapTextureDesc.Width),
                   static_cast<float>(CubeMapTextureDesc.Height)};
}
