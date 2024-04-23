#pragma once

#include "graphics_enums.h"

#include <stdint.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr

class CGraphics;

class CTexture
{
	friend CGraphics;
public:
	CTexture() = default;

	const ID3D11Texture2D* GetTexture() const;
	ID3D11Texture2D* AccessTexture();

private:
	CTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> _pTexture;
};

