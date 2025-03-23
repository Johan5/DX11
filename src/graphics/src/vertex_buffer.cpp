#include "graphics/vertex_buffer.h"

#include "utils/assert.h"

CVertexBuffer::CVertexBuffer(ID3D11Device& Device, const void* pVertexData, const SVertexBufferProperties& Properties)
{
	ASSERT(Properties._VertexDataSizeInBytes > 0, "Vertex data size needs to be positive");
	ASSERT(Properties._SingleVertexSizeInBytes > 0, "Single vertex size needs to be positive");
	D3D11_BUFFER_DESC VertexBufferDesc;
	ZeroMemory(&VertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	VertexBufferDesc.ByteWidth = Properties._VertexDataSizeInBytes;
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.BindFlags = static_cast<uint32_t>(EBindFlag::VertexBuffer);
	VertexBufferDesc.CPUAccessFlags = static_cast<uint32_t>(ECpuAccessPolicy::CpuWrite); // TODO: Expose this to caller instead?
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA VertexData;
	ZeroMemory(&VertexData, sizeof(D3D11_SUBRESOURCE_DATA));
	VertexData.pSysMem = pVertexData;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;
	HRESULT VertexResult = Device.CreateBuffer(&VertexBufferDesc, &VertexData, _pVertexBuffer.GetAddressOf());
	ASSERT(SUCCEEDED(VertexResult), "Failed to create vertex buffer");
	_Properties = Properties;
}