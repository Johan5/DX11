#pragma once

#include <DirectXMath.h>
#include "vector.h"
#include "matrix.h"

// Using left handed coordinate systems
// (viewing down +Z, +X right, +Y Up)
class COrthographicCamera
{
public:
	// The Width and Height is in "World space units"
	COrthographicCamera( float ViewportWidth, float ViewportHeight ) : _ViewportWidth(ViewportWidth), _ViewportHeight(ViewportHeight) {}

	CVector3f GetPosition() const;
	void SetPosition( const CVector3f& NewPosition );
	// World space -> Camera space
	CMatrix4x4f GetViewMatrix() const;
	// Camera space -> Screen projeciton, (-1, -1, 0) to (1, 1, 1)
	CMatrix4x4f GetProjectionMatrix() const;
	CMatrix4x4f GetViewAndProjection() const;

private:

	CVector3f _Position = CVector3f{ 0.0f, 0.0f, 0.0f };
	CVector3f _Forward = CVector3f{ 0.0f, 0.0f, 1.0f }.CalcNormalized();
	CVector3f _Up = CVector3f{ 0.0f, 1.0f, 0.0f }.CalcNormalized();

	// in "world space units"
	float _ViewportWidth;
	float _ViewportHeight;
};
