#include "graphics/texture_view.h"

#include "utils/assert.h"


CTextureView::CTextureView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView)
	: _pTextureView(pShaderResourceView)
{
}

bool CTextureView::operator==(const CTextureView& other) const
{
	return _pTextureView.Get() == other._pTextureView.Get();
}

ID3D11ShaderResourceView* CTextureView::AccessTextureView()
{
	return _pTextureView.Get();
}

const ID3D11ShaderResourceView* CTextureView::GetTextureView() const
{
	return _pTextureView.Get();
}

ID3D11ShaderResourceView** CTextureView::AccessAddrOfTextureView()
{
	return _pTextureView.GetAddressOf();
}