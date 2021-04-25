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
	R32G32B32Float = DXGI_FORMAT_R32G32B32_FLOAT,
	R32G32B32A32Float = DXGI_FORMAT_R32G32B32A32_FLOAT,
};