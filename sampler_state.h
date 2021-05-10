#pragma once



#include <stdint.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr


class CSamplerState
{
public:
	CSamplerState() = default;
	explicit CSamplerState(ID3D11Device& Device);

	ID3D11SamplerState* AccessSamplerState();
	ID3D11SamplerState** AccessAddrOfSamplerState();

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> _SamplerState;
};

