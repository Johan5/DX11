#pragma once

#include "texture.h"

#include <d3d11.h>
#include <wrl/client.h>  // Microsoft ComPtr

class CGraphics;

class CDepthStencilView {
  friend CGraphics;

 public:
  CDepthStencilView() = default;

  ID3D11DepthStencilView* AccessDepthStencilView();
  ID3D11DepthStencilView** AccessAddrOfDepthStencilView();

 private:
  explicit CDepthStencilView(
      Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView);

  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _pDepthStencilView;
};
