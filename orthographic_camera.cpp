#include "orthographic_camera.h"

#include "graphics.h"
#include "3d_math.h"

constexpr float DegToRad = 360.0f / ( 2.0f * 3.1415926535f );


// "World to Camera"
CMatrix4x4f COrthographicCamera::GetViewMatrix() const
{
	
	// Establish basis vectors for camera space
	return N3DMath::CreateCoordinateTransform( _Position, _Forward, _Up, N3DMath::ECoordinateTransformType::WorldToLocal );
}

CMatrix4x4f COrthographicCamera::GetProjectionMatrix() const
{
	float n = NGraphicsDefines::ScreenNear;
	float f = NGraphicsDefines::ScreenFar;
	float CameraDepth = f - n;
	// orthographic projection
	CMatrix4x4f Projection{ 2.0f / _ViewportWidth,	0.0f,					0.0f,				0.0f,
							0.0f,					2.0f / _ViewportHeight,	0.0f,				0.0f,
//							0.0f,					0.0f,					2.0f / ( n - f ),	- ( n + f ) / ( n - f ),
							0.0f,					0.0f,					1.0f / CameraDepth,	- n / CameraDepth,
							0.0f,					0.0f,					0.0f,				1.0f };
	return Projection;
}

CMatrix4x4f COrthographicCamera::GetViewAndProjection() const
{
	// 1. World to camera space.
	// 2. Camera space to [-1, 1] viewport
	// ( 3. Viewport to actual pixels and clipping, pipeline does this for me)
	assert( N3DMath::AreOrthogonal( _Forward, _Up ) );
	
	return GetViewMatrix() * GetProjectionMatrix() ;
}


