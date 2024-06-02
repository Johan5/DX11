#pragma once

#include "mesh.h"
#include "material.h"

#include <span>
#include <variant>

struct SRawPtrConstantBufferData
{
	const void* _ConstantData = nullptr;
	uint32_t _ConstantDataByteSize = 0;
};


struct SRenderPacket
{
	friend bool CanUseSameDraw(const SRenderPacket& a, const SRenderPacket& b)
	{
		return false; // TODO
	}

	SMesh _Mesh;
	SMaterial _Material;

	SRawPtrConstantBufferData _ConstantBufferData;
};

