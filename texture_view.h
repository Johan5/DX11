#pragma once


#include <stdint.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr


class CTextureView
{
public:
	CTextureView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView);

	ID3D11ShaderResourceView* AccessTextureView();
	ID3D11ShaderResourceView** AccessAddrOfTextureView();

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pShaderResourceView;
};

