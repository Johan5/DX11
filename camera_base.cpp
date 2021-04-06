#include "camera_base.h"

#include "3d_math.h"
#include "misc_math.h"
#include "assert.h"


// "World to Camera"
CMatrix4x4f CCameraBase::GetViewMatrix() const
{
	// Establish basis vectors for camera space
	CMatrix4x4f Ret = N3DMath::CreateCoordinateTransform( _Position, _Forward, _Up, N3DMath::ECoordinateTransformType::WorldToLocal );
	return Ret;
}

CMatrix4x4f CCameraBase::GetViewAndProjection() const
{
	ASSERT( N3DMath::AreOrthogonal( GetForwardVec(), GetUpVec() ), "Camera Forward and Up vec are no longer orthogonal." );
	return GetProjectionMatrix() * GetViewMatrix();
}

const CVector3f& CCameraBase::GetPosition() const
{
	return _Position;
}
const CVector3f& CCameraBase::GetForwardVec() const 
{
	return _Forward;
}
const CVector3f& CCameraBase::GetUpVec() const 
{
	return _Up;
}

void CCameraBase::StrafeRight()
{
	CVector3f Right = N3DMath::CalcCross( _Up, _Forward );
	_Position += _MovementSpeed * Right;
}

void CCameraBase::StrafeLeft()
{
	CVector3f Right = N3DMath::CalcCross( _Up, _Forward );
	_Position -= _MovementSpeed * Right;
}

void CCameraBase::StrafeDown()
{
	_Position -= _MovementSpeed * _Up;
}

void CCameraBase::StrafeUp()
{
	_Position += _MovementSpeed * _Up;
}

void CCameraBase::RotateRight( float RotationStrength )
{
	_Forward = N3DMath::CalcVectorRotationAboutAxis( _Forward, _Up, RotationStrength * _RotationSpeed );
	ASSERT( NMiscMath::AlmostEqual( _Forward.CalcLengthSquared(), 1.0f ), "Camera _Forward is no longer unit length" );
}
void CCameraBase::RotateLeft( float RotationStrength )
{
	RotateRight( -RotationStrength );
}
void CCameraBase::RotateDown( float RotationStrength )
{
	CVector3f Right = N3DMath::CalcCross( _Up, _Forward );
	CVector3f NewUp = N3DMath::CalcVectorRotationAboutAxis( _Up, Right, RotationStrength * _RotationSpeed );
	// restrict movement so camera cant roll upside down
	if ( NewUp._Y < 0.0f )
	{
		NewUp._Y = 0.0f;
		NewUp.Normalize();
	}
	_Up = NewUp;
	_Forward = N3DMath::CalcCross( Right, NewUp );
	ASSERT( NMiscMath::AlmostEqual( _Up.CalcLengthSquared(), 1.0f ), "Camera _Up is no longer unit length" );
	ASSERT( NMiscMath::AlmostEqual( _Forward.CalcLengthSquared(), 1.0f ), "Camera _Forward is no longer unit length" );
}
void CCameraBase::RotateUp( float RotationStrength )
{
	return RotateDown( -RotationStrength );
}
