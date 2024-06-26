#pragma once

#include "graphics.h"
#include "render_target_view.h"
#include "light_source.h"
#include "constant_buffer.h"
#include "texture_view.h"
#include "sampler_state.h"
#include "texture_view.h"
#include "depth_stencil_view.h"
#include "texture.h"

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
	CShadowHandler() = default;
	void Initialize(CGraphics& Graphics, uint32_t Width, uint32_t Height);

	// Returns ptr to the shadow texture array (cubemap)
	SShadowData CreateShadowMap(CGraphics& Graphics, CBatchRenderHelper& BatchRenderHelper, const CLightSource& Light);

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

