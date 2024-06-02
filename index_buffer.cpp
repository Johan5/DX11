#include "index_buffer.h"

#include "assert.h"

CIndexBuffer::CIndexBuffer(ID3D11Device& Device, uint32_t SizeInBytes, ECpuAccessPolicy AccessPolicy)
{
	// Setup matrix constant buffer
	D3D11_BUFFER_DESC MatrixBufferDesc;
	MatrixBufferDesc.ByteWidth = SizeInBytes;
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.BindFlags = static_cast<uint32_t>(EBindFlag::IndexBuffer);
	MatrixBufferDesc.CPUAccessFlags = static_cast<uint32_t>(AccessPolicy);
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;
	HRESULT Result = Device.CreateBuffer(&MatrixBufferDesc, nullptr, _pBuffer.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create constant buffer");
	_BufferSizeInBytes = SizeInBytes;
	_AccessPolicy = AccessPolicy;
}