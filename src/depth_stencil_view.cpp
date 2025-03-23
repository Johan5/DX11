#include "depth_stencil_view.h"

#include "assert.h"


CDepthStencilView::CDepthStencilView(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView)
	: _pDepthStencilView(pDepthStencilView)
{
}

ID3D11DepthStencilView* CDepthStencilView::AccessDepthStencilView()
{
	return _pDepthStencilView.Get();
}

ID3D11DepthStencilView** CDepthStencilView::AccessAddrOfDepthStencilView()
{
	return _pDepthStencilView.GetAddressOf();
}