#include "render_target.h"

#include "assert.h"


CRenderTarget::CRenderTarget(ID3D11Device& Device, uint32_t Width, uint32_t Height, EGfxResourceDataFormat Format)
{
	D3D11_TEXTURE2D_DESC TextureDesc;
	TextureDesc.Width = Width;
	TextureDesc.Height = Height;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = static_cast<DXGI_FORMAT>(Format);
	TextureDesc.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = static_cast<uint32_t>(EBindFlag::RenderTarget) | static_cast<uint32_t>(EBindFlag::ShaderResource);
	TextureDesc.CPUAccessFlags = static_cast<uint32_t>(ECpuAccessPolicy::NoAccess);
	TextureDesc.MiscFlags = 0;

	HRESULT Result = Device.CreateTexture2D(&TextureDesc, nullptr, _pCubeMapTexture.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create texture resource");

	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc;
	RenderTargetViewDesc.Format = static_cast<DXGI_FORMAT>(Format);
	RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetViewDesc.Texture2D.MipSlice = 0;
	Result = Device.CreateRenderTargetView(_pCubeMapTexture.Get(), &RenderTargetViewDesc, _pRenderTargetView.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create render target view");

	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format = static_cast<DXGI_FORMAT>(Format);
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;
	Result = Device.CreateShaderResourceView(_pCubeMapTexture.Get(), &ShaderResourceViewDesc, _pShaderResourceView.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create shader resource view");
}

ID3D11RenderTargetView* CRenderTarget::GetRenderTargetView()
{
	return _pRenderTargetView.Get();
}

ID3D11RenderTargetView** CRenderTarget::GetAddrOfRenderTargetView()
{
	return _pRenderTargetView.GetAddressOf();
}