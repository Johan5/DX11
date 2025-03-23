#pragma once


#include "graphics_enums.h"

#include <cstdint>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr

struct SVertexBufferProperties
{
	// Size of the entire buffer (all vertices)
	uint32_t _VertexDataSizeInBytes = 0;
	// Size of a single vertex
	uint32_t _SingleVertexSizeInBytes = 0;
	// Number of vertices
	uint32_t _VertexCount = 0;
};

class CVertexBuffer
{
	friend class CGraphics; // Only allow CGraphics to create
public:
	CVertexBuffer() = default;

	ID3D11Buffer* AccessVertexBuffer() { return _pVertexBuffer.Get(); }
	const ID3D11Buffer* GetVertexBuffer() const { return _pVertexBuffer.Get(); }
	ID3D11Buffer** AccessVertexBufferAddr() { return _pVertexBuffer.GetAddressOf(); }
	const SVertexBufferProperties& GetProperties() const { return _Properties; }

private:
	explicit CVertexBuffer(ID3D11Device& Device, const void* pVertexData, const SVertexBufferProperties& Properties);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pVertexBuffer;
	SVertexBufferProperties _Properties;
};