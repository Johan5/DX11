#pragma once

#include "texture.h"

#include <d3d11.h>
#include <wrl/client.h>  // Microsoft ComPtr
#include <cstdint>

class CGraphics;

class CTextureView {
  friend CGraphics;

 public:
  CTextureView() = default;

  bool operator==(const CTextureView& other) const;

  ID3D11ShaderResourceView* AccessTextureView();
  const ID3D11ShaderResourceView* GetTextureView() const;
  ID3D11ShaderResourceView** AccessAddrOfTextureView();

 private:
  explicit CTextureView(
      Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView);

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pTextureView;
};
