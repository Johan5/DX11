#include "render_target_view.h"

#include "assert.h"


//CRenderTargetView::CRenderTargetView(ID3D11Device& Device, CTexture& Texture, uint32_t Width, uint32_t Height, EGfxResourceDataFormat Format)
//{
//	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc;
//	RenderTargetViewDesc.Format = static_cast<DXGI_FORMAT>(Format);
//	RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
//	RenderTargetViewDesc.Texture2D.MipSlice = 0;
//	HRESULT Result = Device.CreateRenderTargetView(Texture.AccessTexture(), &RenderTargetViewDesc, _pRenderTargetView.GetAddressOf());
//	ASSERT(SUCCEEDED(Result), "Failed to create render target view");
//}

CRenderTargetView::CRenderTargetView(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenterTargetView)
	: _pRenderTargetView(pRenterTargetView)
{
}

ID3D11RenderTargetView* CRenderTargetView::AccessRenderTargetView()
{
	return _pRenderTargetView.Get();
}

ID3D11RenderTargetView** CRenderTargetView::AccessAddrOfRenderTargetView()
{
	return _pRenderTargetView.GetAddressOf();
}