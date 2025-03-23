#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>  // Microsoft ComPtr
#include <string>

class CPixelShader {
 public:
  bool Initialize(ID3D11Device& Device, const std::string& FileName,
                  const std::string& ShaderMainFunction);
  bool IsInitialized() const;

  ID3D11PixelShader* AccessPixelShader() { return _pPixelShader.Get(); }

 private:
  Microsoft::WRL::ComPtr<ID3D11PixelShader> _pPixelShader;
};
