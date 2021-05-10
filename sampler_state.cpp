#include "sampler_state.h"

#include "assert.h"


CSamplerState::CSamplerState(ID3D11Device& Device)
{
	D3D11_SAMPLER_DESC SamplerDesc;
	ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 1U;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//SamplerDesc.BorderColor
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT Result = Device.CreateSamplerState(&SamplerDesc, _SamplerState.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create sampler state");
}

ID3D11SamplerState* CSamplerState::AccessSamplerState()
{
	return _SamplerState.Get();
}

ID3D11SamplerState** CSamplerState::AccessAddrOfSamplerState()
{
	return _SamplerState.GetAddressOf();
}

