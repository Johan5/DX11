#pragma once

#include "graphics_enums.h"

#include <stdint.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr

class CIndexBuffer
{
	friend class CGraphics; // Only allow CGraphics to create
public:
	CIndexBuffer() = default;

	bool IsValid() const { return _pBuffer != nullptr; }

	ID3D11Buffer* AccessRawBuffer() { return _pBuffer.Get(); }
	const ID3D11Buffer* GetRawBuffer() const { return _pBuffer.Get(); }
	uint32_t GetSizeInBytes() const { return _BufferSizeInBytes; }
	ECpuAccessPolicy GetAccessPolicy() const { return _AccessPolicy; }

private:
	CIndexBuffer(ID3D11Device& Device, uint32_t SizeInBytes, ECpuAccessPolicy AccessPolicy);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;
	uint32_t _BufferSizeInBytes = 0;
	ECpuAccessPolicy _AccessPolicy;
};