#pragma once

#include "game_object.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "constant_buffer.h"
#include "graphics.h"
#include "vector.h"

#include <vector>


/// Simple light source emitting light in all directions
class CLightSource : public CGameObject
{
public:
	struct SConstantBuffer
	{
		CMatrix4x4f _ModelToWorld;
		// Transforms normals from model to world (it is the inverse transform of _ModelToWorld)
		CMatrix4x4f _NormalModelToWorld;
	};

	void Initialize(CGraphics& Graphics) override;
	void Shutdown() override;
	bool IsInitialized() const override;

	void Render(CRenderContext& RenderContext, const CCameraBase& Camera) override;

private:
	std::vector<CGameObject::STypicalVertex> _Vertices;
	CVertexBuffer _VertexBuffer;
	CConstantBuffer _ConstantBuffer;
	CVertexShader _VertexShader;
	CPixelShader _PixelShader;

	bool _IsInitialized = false;
};

