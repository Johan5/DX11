#pragma once

#include <d3d11.h>
#include <vector>

#include "game_object.h"
#include "vector.h"
#include "matrix.h"
#include "graphics.h"
#include "vertex_shader.h"
#include "pixel_shader.h"


class CCube : public CGameObject
{
public:
	struct SCubeConstantBuffer
	{
		CMatrix4x4f _ModelToWorld;
	};
	struct SVertex
	{
		CVector3f _Position;
		CVector4f _Color;
	};
	~CCube() override;

	void Initialize( CGraphics& Graphics ) override;
	void Shutdown() override;
	bool IsInitialized() const override;

	void Render( CRenderContext& RenderContext, const CCameraBase& Camera ) override;

	void SetPosition( const CVector3f& NewPosition );
	void SetScale( const CVector3f& NewScale );

//private:
	CMatrix4x4f GetLocalToWorldTransform() const;

private:
	CVector3f _Position;
	CVector3f _Scale = CVector3f{ 1.0f, 1.0f, 1.0f };
	// Normalized, in world coords
	//CVector3f _Forward = CVector3f::Forward();
	CVector3f _Forward = CVector3f{ 1.0f, 0.0f, 0.0f }.CalcNormalized();
	// Normalized, in world coords
	// CVector3f _Up = CVector3f::Up();
	CVector3f _Up = CVector3f{ 0.0f, 1.0f, 0.0f }.CalcNormalized();

	// these are just cached values
	mutable bool _LocalToWorldTransformIsStale = true;
	mutable CMatrix4x4f _LocalToWorldTransform;

	std::vector<SVertex> _Vertices;
	CVertexBuffer _VertexBuffer;
	CConstantBuffer _ConstantBuffer;
	CVertexShader _VertexShader;
	CPixelShader _PixelShader;

	bool _IsInitialized = false;
};


