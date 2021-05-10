#pragma once


#include <d3d11.h>

#include <stdint.h>


enum class ECpuAccessPolicy : uint32_t
{
	NoAccess = 0,
	CpuWrite = D3D11_CPU_ACCESS_WRITE,
	CpuRead = D3D11_CPU_ACCESS_READ,
};

enum class EGfxResourceDataFormat
{
	Unknown = DXGI_FORMAT_UNKNOWN,
	R32G32B32A32Float = DXGI_FORMAT_R32G32B32A32_FLOAT,
	R32G32B32Float = DXGI_FORMAT_R32G32B32_FLOAT,
	R32Float = DXGI_FORMAT_R32_FLOAT,
	D32Float = DXGI_FORMAT_D32_FLOAT,
};

enum class EBindFlag 
{
	VertexBuffer = D3D11_BIND_VERTEX_BUFFER,
	IndexBuffer = D3D11_BIND_INDEX_BUFFER,
	ConstantBuffer = D3D11_BIND_CONSTANT_BUFFER,
	ShaderResource = D3D11_BIND_SHADER_RESOURCE,
	StreamOutput = D3D11_BIND_STREAM_OUTPUT,
	RenderTarget = D3D11_BIND_RENDER_TARGET,
	DepthStencil = D3D11_BIND_DEPTH_STENCIL,
	UnorderedAccess = D3D11_BIND_UNORDERED_ACCESS,
	Decoder = D3D11_BIND_DECODER,
	VideoEncoder = D3D11_BIND_VIDEO_ENCODER
};