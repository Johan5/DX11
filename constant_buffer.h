#pragma once

#include "graphics_enums.h"

#include <stdint.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr

// This is the index that will be used when registering a buffer with the device context
enum class EConstantBufferIdx
{
	PerFrame,
	PerCamera,
	PerObject
};

class CConstantBuffer
{
	friend class CGraphics; // Only allow CGraphics to create
public:
	CConstantBuffer() = default;

	ID3D11Buffer* AccessRawBuffer() { return _pBuffer.Get(); }
	int32_t GetSizeInBytes() const { return _BufferSizeInBytes; }
	ECpuAccessPolicy GetAccessPolicy() const { return _AccessPolicy; }

private:
	CConstantBuffer(ID3D11Device& Device, int32_t SizeInBytes, ECpuAccessPolicy AccessPolicy);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;
	int32_t _BufferSizeInBytes = 0;
	ECpuAccessPolicy _AccessPolicy;
};