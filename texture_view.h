#pragma once

#include "texture.h"

#include <stdint.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr


class CGraphics;

class CTextureView
{
	friend CGraphics;
public:
	CTextureView() = default;
	
	ID3D11ShaderResourceView* AccessTextureView();
	ID3D11ShaderResourceView** AccessAddrOfTextureView();

private:
	CTextureView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pTextureView;
};

