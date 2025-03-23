#pragma once

#include "core/vector.h"
#include "core/matrix.h"
#include "graphics/graphics.h"
#include "graphics/render_packet.h"
#include "graphics/batch_render_helper.h"

#include <cstdint>

class CGraphics;
class CRenderContext;
class CCameraBase;


class CGameObject
{
public:
	virtual ~CGameObject() = default;

	virtual void Initialize( CGraphics& Graphics ) {};
	virtual void Shutdown() {};
	virtual bool IsInitialized() const;

	virtual void Render(CBatchRenderHelper& BatchRenderHelper, const CCameraBase& Camera);

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

