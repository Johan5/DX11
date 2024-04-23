#pragma once

#include "graphics_enums.h"
#include "texture.h"

#include <stdint.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr

class CGraphics;

class CRenderTargetView
{
	friend CGraphics;
public:
	CRenderTargetView() = default;

	ID3D11RenderTargetView* AccessRenderTargetView();
	ID3D11RenderTargetView** AccessAddrOfRenderTargetView();

private:
	CRenderTargetView(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenterTargetView);
	// Viewing texture as render target
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pRenderTargetView;
};

