#include "texture_view.h"


CTextureView::CTextureView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView)
	: _pShaderResourceView(pShaderResourceView)
{
}

ID3D11ShaderResourceView* CTextureView::AccessTextureView()
{
	return _pShaderResourceView.Get();
}

ID3D11ShaderResourceView** CTextureView::AccessAddrOfTextureView()
{
	return _pShaderResourceView.GetAddressOf();
}