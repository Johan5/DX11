#pragma once

#include "camera_base.h"
#include "core/misc_math.h"

// Using left handed coordinate systems
// (viewing down +Z, +X right, +Y Up)
class CPerspectiveCamera : public CCameraBase
{
public:
	CPerspectiveCamera( float HorizontalFoV, float AspectRatio );
	void SetFieldOfView( float HorizontalFoV );
	void SetAspectRatio( float AspectRatio );

	// Camera space -> Screen projeciton, (-1, -1, 0) to (1, 1, 1)
	CMatrix4x4f GetProjectionMatrix() const override;

private:
	float _HorizontalFoV = 0.0f;
	float _AspectRatio = 0.0f;
};

