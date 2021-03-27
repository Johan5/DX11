#pragma once

#include "vector.h"
#include "matrix.h"

class CCameraBase
{
public:
	virtual CVector3f GetPosition() const = 0;
	virtual void SetPosition( const CVector3f& NewPosition ) = 0;
	// World space -> Camera space
	virtual CMatrix4x4f GetViewMatrix() const = 0;
	// Camera space -> Screen projeciton, (-1, -1, 0) to (1, 1, 1)
	virtual CMatrix4x4f GetProjectionMatrix() const = 0;
	virtual CMatrix4x4f GetViewAndProjection() const = 0;
};
