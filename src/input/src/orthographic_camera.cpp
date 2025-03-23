#include "input/orthographic_camera.h"

#include "core/graphic_defines.h"
#include "core/3d_math.h"

constexpr float DegToRad = 360.0f / ( 2.0f * 3.1415926535f );



CMatrix4x4f COrthographicCamera::GetProjectionMatrix() const
{
	float n = NGraphicsDefines::ScreenNear;
	float f = NGraphicsDefines::ScreenFar;
	float CameraDepth = f - n;
	// orthographic projection
	CMatrix4x4f Projection{ 2.0f / _ViewportWidth,	0.0f,					0.0f,				0.0f,
							0.0f,					2.0f / _ViewportHeight,	0.0f,				0.0f,
							0.0f,					0.0f,					1.0f / CameraDepth,	-n / CameraDepth,
							0.0f,					0.0f,					0.0f,				1.0f };
	return Projection;
}
