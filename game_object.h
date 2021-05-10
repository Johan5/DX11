#pragma once

#include "vector.h"
#include "matrix.h"
#include "graphics.h"

#include <cstdint>

class CGraphics;
class CRenderContext;
class CCameraBase;


class CGameObject
{
public:
	struct STypicalConstantBuffer
	{
		CMatrix4x4f _ModelToWorld;
		// Transforms normals from model to world (it is the inverse transform of _ModelToWorld)
		CMatrix4x4f _NormalModelToWorld;
		SMaterial _Material;
	};
	struct STypicalShadowConstantBuffer
	{
		CMatrix4x4f _ModelToWorld;
	};
	struct STypicalVertex
	{
		CVector3f _Position;
		CVector3f _Normal;
	};

	virtual ~CGameObject() = default;

	virtual void Initialize( CGraphics& Graphics ) {};
	virtual void Shutdown() {};
	virtual bool IsInitialized() const;

	virtual void Render( CRenderContext& RenderContext, const CCameraBase& Camera ) {};
	virtual void RenderShadows(CRenderContext& RenderContext) {};
	virtual bool ShouldRender() { return true; }
	virtual bool ShouldRenderShadows() { return true; }

	virtual void SetPosition(const CVector3f& NewPosition);
	virtual void SetScale(const CVector3f& NewScale);

	const CVector3f& GetPosition() const;

	CMatrix4x4f GetLocalToWorldTransform() const;
	CMatrix4x4f GetNormalLocalToWorldTransform() const;

private:
	friend class CWorld;
	void SetId( int64_t NewId );
	void UpdateTransforms() const;

private:
	int64_t _Id = -1;

	CVector3f _Position = CVector3f{ 0.0f, 0.0f, 0.0f };
	CVector3f _Scale = CVector3f{ 1.0f, 1.0f, 1.0f };
	// Normalized, in world coords
	//CVector3f _Forward = CVector3f::Forward();
	CVector3f _Forward = CVector3f{ 0.0f, 0.0f, 1.0f }.CalcNormalized();
	// Normalized, in world coords
	// CVector3f _Up = CVector3f::Up();
	CVector3f _Up = CVector3f{ 0.0f, 1.0f, 0.0f }.CalcNormalized();

	// these are just cached values
	mutable bool _TransformsAreStale = true;
	mutable CMatrix4x4f _LocalToWorldTransform;
	// transforms normal vectors to world space
	mutable CMatrix4x4f _NormalLocalToWorldTransform;
};

