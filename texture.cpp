#include "texture.h"

#include "assert.h"

CTexture::CTexture(ID3D11Device& Device, uint32_t Width, uint32_t Height, EGfxResourceDataFormat Format, uint32_t BindFlags, ECpuAccessPolicy AccessPolicy)
{
	D3D11_TEXTURE2D_DESC TextureDesc;
	TextureDesc.Width = Width;
	TextureDesc.Height = Height;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = static_cast<DXGI_FORMAT>( Format );
	TextureDesc.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = BindFlags;
	TextureDesc.CPUAccessFlags = static_cast<uint32_t>(AccessPolicy);
	TextureDesc.MiscFlags = 0;

	HRESULT Result = Device.CreateTexture2D(&TextureDesc, nullptr, _pTexture.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create texture resource");
}

CTexture::CTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture)
	: _pTexture(pTexture)
{
}

const ID3D11Texture2D* CTexture::GetTexture() const
{
	return _pTexture.Get();
}