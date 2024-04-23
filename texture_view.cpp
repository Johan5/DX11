#include "texture_view.h"

#include "assert.h"


//CTextureView::CTextureView(CGraphics& Graphics, CTexture& Texture)
//{
//	ID3D11Device& Device = Graphics.AccessDevice();
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderViewDesc;
//	ShaderViewDesc.Format = static_cast<DXGI_FORMAT>(EGfxResourceDataFormat::R32Float);
//	ShaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
//	ShaderViewDesc.TextureCube.MostDetailedMip = 0;
//	ShaderViewDesc.TextureCube.MipLevels = 1;
//	HRESULT Result = Device.CreateShaderResourceView(Texture.AccessTexture(), &ShaderViewDesc, _pTextureView.GetAddressOf());
//	ASSERT(SUCCEEDED(Result), "Failed to create shader resource view");
//}

CTextureView::CTextureView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView)
	: _pTextureView(pShaderResourceView)
{
}

ID3D11ShaderResourceView* CTextureView::AccessTextureView()
{
	return _pTextureView.Get();
}

ID3D11ShaderResourceView** CTextureView::AccessAddrOfTextureView()
{
	return _pTextureView.GetAddressOf();
}