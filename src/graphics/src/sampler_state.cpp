#include "graphics/sampler_state.h"

#include "utils/assert.h"

#include <utility>

CSamplerState::CSamplerState(
    Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState)
    : _SamplerState(std::move(SamplerState)) {}

bool CSamplerState::operator==(const CSamplerState& other) const {
  return _SamplerState.Get() == other._SamplerState.Get();
}

ID3D11SamplerState* CSamplerState::AccessSamplerState() {
  return _SamplerState.Get();
}

const ID3D11SamplerState* CSamplerState::GetSamplerState() const {
  return _SamplerState.Get();
}

ID3D11SamplerState** CSamplerState::AccessAddrOfSamplerState() {
  return _SamplerState.GetAddressOf();
}
