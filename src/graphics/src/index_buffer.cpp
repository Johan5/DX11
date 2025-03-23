#include "graphics/index_buffer.h"

#include "utils/assert.h"

CIndexBuffer::CIndexBuffer(ID3D11Device& Device, const void* pIndexData,
                           uint32_t SizeInBytes, uint32_t IndexCount,
                           ECpuAccessPolicy AccessPolicy)
    : _BufferSizeInBytes(SizeInBytes),
      _IndexCount(IndexCount),
      _AccessPolicy(AccessPolicy) {
  // Setup matrix constant buffer
  D3D11_BUFFER_DESC MatrixBufferDesc;
  MatrixBufferDesc.ByteWidth = SizeInBytes;
  MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  MatrixBufferDesc.BindFlags = static_cast<uint32_t>(EBindFlag::IndexBuffer);
  MatrixBufferDesc.CPUAccessFlags = static_cast<uint32_t>(AccessPolicy);
  MatrixBufferDesc.MiscFlags = 0;
  MatrixBufferDesc.StructureByteStride = 0;
  D3D11_SUBRESOURCE_DATA IndexData;
  ZeroMemory(&IndexData, sizeof(D3D11_SUBRESOURCE_DATA));
  IndexData.pSysMem = pIndexData;
  IndexData.SysMemPitch = 0;
  IndexData.SysMemSlicePitch = 0;
  HRESULT Result = Device.CreateBuffer(&MatrixBufferDesc, &IndexData,
                                       _pBuffer.GetAddressOf());
  ASSERT(SUCCEEDED(Result), "Failed to create constant buffer");
}