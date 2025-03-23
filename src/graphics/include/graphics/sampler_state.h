#pragma once



#include <cstdint>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr


class CSamplerState
{
	friend class CGraphics; // Only allow CGraphics to create
public:
	CSamplerState() = default;

	bool operator==(const CSamplerState& other) const;

	ID3D11SamplerState* AccessSamplerState();
	const ID3D11SamplerState* GetSamplerState() const;
	ID3D11SamplerState** AccessAddrOfSamplerState();

private:
	explicit CSamplerState(Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState);

	Microsoft::WRL::ComPtr<ID3D11SamplerState> _SamplerState;
};

