#pragma once

#include "graphics_enums.h"

#include <stdint.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr


class CRenderTarget
{
public:
	CRenderTarget() = default;
	CRenderTarget(ID3D11Device& Device, uint32_t Width, uint32_t Height, EGfxResourceDataFormat Format);

	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11RenderTargetView** GetAddrOfRenderTargetView();

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _pCubeMapTexture;
	// Viewing texture as render target
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pRenderTargetView;
	// Viewing texture as shader resource
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pShaderResourceView;
};

