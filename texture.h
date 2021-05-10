#pragma once

#include "graphics_enums.h"

#include <stdint.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr


class CTexture
{
public:
	CTexture(ID3D11Device& Device, uint32_t Width, uint32_t Height, EGfxResourceDataFormat Format, uint32_t BindFlags, ECpuAccessPolicy AccessPolicy);
	CTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture);

	const ID3D11Texture2D* GetTexture() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _pTexture;
};

