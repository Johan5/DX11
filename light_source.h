#pragma once

#include "game_object.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "constant_buffer.h"
#include "graphics.h"
#include "vector.h"
#include "default_object_constant_buffer.h"

#include <vector>
#include <array>


/// Simple point light (emitting light in all directions)
class CLightSource : public CGameObject
{
public:
	struct SVertex
	{
		CVector3f _Position;
		CVector3f _Normal;
	};

	void Initialize(CGraphics& Graphics) override;
	void Shutdown() override;
	bool IsInitialized() const override;

	void Render(CBatchRenderHelper& BatchRenderHelper, const CCameraBase& Camera) override;

private:
	std::vector<SVertex> _Vertices;

	SMaterial _Material;
	SMesh _Mesh;
	SDefaultObjectConstantBuffer _CbData;

	bool _IsInitialized = false;
};

