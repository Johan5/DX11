#include "graphics/texture.h"

#include "utils/assert.h"

CTexture::CTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture)
    : _pTexture(pTexture) {}

const ID3D11Texture2D* CTexture::GetTexture() const {
  return _pTexture.Get();
}

ID3D11Texture2D* CTexture::AccessTexture() {
  return _pTexture.Get();
}