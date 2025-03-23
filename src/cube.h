#pragma once

#include <d3d11.h>
#include <vector>

#include "game_object.h"
#include "vector.h"
#include "matrix.h"
#include "graphics.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "constant_buffer.h"
#include "material.h"
#include "render_packet.h"
#include "default_object_constant_buffer.h"

#include <mutex>


class CCube : public CGameObject
{
public:
	~CCube() override;

	void Initialize( CGraphics& Graphics ) override;
	void Shutdown() override;
	bool IsInitialized() const override;

	void Render(CBatchRenderHelper& BatchRenderHelper, const CCameraBase& Camera) override;

	bool ShouldRenderShadows() const;
	void DisableShadowRendering();

	SMaterial& AccessMaterial();
	SDefaultObjectConstantBuffer& AccessConstantBuffer();

private:
	SMaterial _Material;
	SMaterial _ShadowMaterial;
	SMesh _Mesh;
	std::string _Texture = "bricks";
	SDefaultObjectConstantBuffer _CbData;

	bool _IsInitialized = false;
	bool _ShouldRenderShadow = true;
};


