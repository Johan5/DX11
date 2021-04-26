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


class CCube : public CGameObject
{
public:
	struct SVertex
	{
		CVector3f _Position;
		CVector3f _Normal;
		CVector4f _Color;
	};
	~CCube() override;

	void Initialize( CGraphics& Graphics ) override;
	void Shutdown() override;
	bool IsInitialized() const override;

	void Render( CRenderContext& RenderContext, const CCameraBase& Camera ) override;

private:
	std::vector<SVertex> _Vertices;
	CVertexBuffer _VertexBuffer;
	CConstantBuffer _ConstantBuffer;
	CVertexShader _VertexShader;
	CPixelShader _PixelShader;

	bool _IsInitialized = false;
};


