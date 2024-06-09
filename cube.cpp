#include "cube.h"

#include "camera_base.h"
#include "3d_math.h"
#include "pixel_shader.h"
#include "graphics_enums.h"
#include "assert.h"
#include "logger.h"
#include "graphic_resources.h"

#include <random>
#include <vector>


namespace
{
	// Lazy rgb implementation
	CVector4f GenRandomColor()
	{
		static std::mt19937 Rng{ 0 };
		static std::uniform_real_distribution<float> Distribution(0.0f, 1.0f);
		return CVector4f{ Distribution(Rng), Distribution(Rng), Distribution(Rng), 1.0f };
	}

	std::pair<CTextureView, CSamplerState> CreateBrickTexture(CGraphics& Graphics)
	{
		std::optional<CTexture> BrickTexture = Graphics.GetTextureByName("bricks");
		ASSERT(BrickTexture, "Failed to retrieve brick texture");
		if (!BrickTexture)
		{
			return { CTextureView{}, CSamplerState{} };
		}
		D3D11_SHADER_RESOURCE_VIEW_DESC TextureViewDesc;
		ZeroMemory(&TextureViewDesc, sizeof(TextureViewDesc));
		TextureViewDesc.Format = static_cast<DXGI_FORMAT>(EGfxResourceDataFormat::R32G32B32A32Float);
		TextureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		TextureViewDesc.Texture2D.MipLevels = 1;
		CTextureView TextureView = Graphics.CreateTextureView(BrickTexture.value(), TextureViewDesc);

		D3D11_SAMPLER_DESC SamplerDesc;
		ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
		SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.MipLODBias = 0.0f;
		SamplerDesc.MaxAnisotropy = 1;
		SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		SamplerDesc.MinLOD = 0;
		SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		CSamplerState SamplerState = Graphics.CreateSamplerState(SamplerDesc);

		return { TextureView, SamplerState };
	}
}

CCube::~CCube()
{
	CCube::Shutdown();
}

void CCube::Initialize( CGraphics& Graphics )
{
	_Material._VS = shader_names::DefaultVertexShaderFileName;
	_Material._PS = shader_names::DefaultPixelShaderFileName;
	std::pair<CTextureView, CSamplerState> BrickTexture = CreateBrickTexture(Graphics);
	_Material._Texture = BrickTexture.first;
	_Material._TextureSampler = BrickTexture.second;

	_ShadowMaterial._VS = shader_names::CubeShadowVertexShaderFileName;
	_ShadowMaterial._GS = shader_names::CubeShadowGeometryShaderFileName;
	_ShadowMaterial._PS = shader_names::CubeShadowPixelShaderFileName;
	SMesh* pMesh = Graphics.AccessMesh(EMeshType::Cube);
	if (pMesh)
	{
		_Mesh = *pMesh;
	}
	else
	{
		ASSERT(false, "Failed to retrieve cube mesh");
		return;
	}
	
	_CbData._ColorData = SDefaultColorData{ ._DiffuseStrength = 1.0f, ._SpecularStrength = 1.0f, ._AmbientStrength = 0.15f, ._SpecularPower = 256, ._Color = GenRandomColor() };

	_IsInitialized = true;
}

void CCube::Shutdown()
{
	// TODO cleanup vertex buffer?
	_IsInitialized = false;
}

bool CCube::IsInitialized() const
{
	return _IsInitialized;
}

void CCube::Render(CBatchRenderHelper& BatchRenderHelper, const CCameraBase& Camera)
{
	if (!IsInitialized())
	{
		return;
	}

	_CbData._WorldMatrix = GetLocalToWorldTransform();
	_CbData._NormalWorldMatrix = GetNormalLocalToWorldTransform();
	SRenderPacket NormalRenderPacket{ 
		._Mesh = _Mesh, 
		._Material = _Material, 
		._ConstantBufferData = SRawPtrConstantBufferData{._ConstantData = static_cast<void*>(&_CbData), ._ConstantDataByteSize = sizeof(_CbData)}
	};
	BatchRenderHelper.QueForInstancedRendering(NormalRenderPacket, ERenderPass::Normal);
	
	if (ShouldRenderShadows())
	{
		SRenderPacket ShadowRenderPacket{
			._Mesh = _Mesh,
			._Material = _ShadowMaterial,
			._ConstantBufferData = SRawPtrConstantBufferData{._ConstantData = static_cast<void*>(&_CbData._WorldMatrix), ._ConstantDataByteSize = sizeof(_CbData._WorldMatrix)}
		};
		BatchRenderHelper.QueForInstancedRendering(ShadowRenderPacket, ERenderPass::Shadow);
	}
}

bool CCube::ShouldRenderShadows() const
{
	return _ShouldRenderShadow;
}

void CCube::DisableShadowRendering()
{
	_ShouldRenderShadow = false;
}

SMaterial& CCube::AccessMaterial()
{
	return _Material;
}

SDefaultObjectConstantBuffer& CCube::AccessConstantBuffer()
{
	return _CbData;
}