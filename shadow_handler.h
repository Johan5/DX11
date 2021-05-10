#pragma once

#include "graphics.h"
#include "render_target.h"
#include "light_source.h"
#include "constant_buffer.h"
#include "texture_view.h"
#include "sampler_state.h"

#include <stdint.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr


namespace NShadowPass
{
	struct SPerLightCb 
	{
		// +X, -X, +Y, -Y, +Z, -Z
		CMatrix4x4f _LightViewMatrices[6];
		CMatrix4x4f _LightProjectionMatrix;
		float _LightMaxDist;
		CVector3f _Pad1;
	};
}

struct SShadowData
{
	// A cubemap
	CTextureView _ShadowMap;
	CSamplerState _ShadowMapSampler;
};

// TODO: make sure everything is cleaned up properly
class CShadowHandler
{
public:
	void Initialize(CGraphics& Graphics, uint32_t Width, uint32_t Height);

	// Returns ptr to the shadow texture array (cubemap)
	SShadowData CalculatePointLightShadows(CRenderContext& RenderContext, const CLightSource& Light, std::vector<std::unique_ptr<CGameObject>>& GameObjects);

	CVector2f GetShadowMapDimensions() const;

private:
	CConstantBuffer _PerLightConstantBuffer;
	// Pointer to array of 6 cubemap textures
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _pCubeMapTexture;
	// Viewing texture as render target
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pRenderTargetView;
	// Viewing texture as texture
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pShaderResourceView;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> _pCubeMapDepthStencilTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _pDepthStencilView;

	CSamplerState _SamplerState;
};

