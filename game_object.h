#pragma once

#include "vector.h"

#include <cstdint>

class CGraphics;
class CRenderContext;
class CCameraBase;
class CMatrix4x4f;


class CGameObject
{
public:
	virtual ~CGameObject() = default;

	virtual void Initialize( CGraphics& Graphics ) {};
	virtual void Shutdown() {};
	virtual bool IsInitialized() const;

	virtual void Render( CRenderContext& RenderContext, const CCameraBase& Camera ) {};

private:
	friend class CWorld;
	void SetId( int64_t NewId );

private:
	int64_t _Id = -1;
};

