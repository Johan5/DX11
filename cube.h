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

#include <mutex>


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
	void RenderShadows(CRenderContext& RenderContext) override;

	bool ShouldRenderShadows() override;
	void DisableShadowRendering();

	void SetColor(const CVector4f& NewColor);

private:
	std::vector<SVertex> _Vertices;
	CVertexBuffer _VertexBuffer;
	bool _NeedVertexBufferUpdate = false;
	CConstantBuffer _ConstantBuffer;
	CConstantBuffer _ShadowConstantBuffer;

	CVertexShader _VertexShader;
	CPixelShader _PixelShader;

	CVertexShader _ShadowVertexShader;
	CGeometryShader _ShadowGeometryShader;
	CPixelShader _ShadowPixelShader;

	std::mutex _RenderLock;

	bool _IsInitialized = false;
	bool _ShouldRenderShadow = true;
};


