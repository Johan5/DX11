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

void CCameraBase::SetOrientation(const CVector3f& Position, const CVector3f& Forward, const CVector3f& Up)
{
	_Position = Position;
	_Forward = Forward;
	_Up = Up;
	ASSERT(NMiscMath::AlmostEqual(_Forward.CalcLengthSquared(), 1.0f), "Camera _Forward is no longer unit length");
	ASSERT(NMiscMath::AlmostEqual(_Up.CalcLengthSquared(), 1.0f), "Camera _Up is no longer unit length");
	ASSERT(NMiscMath::AlmostEqual(_Forward.Dot(_Up), 0.0f), "Camera _Forward and _Up are no longer orthogonal");
}

void CCameraBase::StrafeRight()
{
	CVector3f Right = _Up.CalcCross( _Forward );
	_Position += _MovementSpeed * Right;
}

void CCameraBase::StrafeLeft()
{
	CVector3f Right = _Up.CalcCross( _Forward );
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

void CCameraBase::MoveForward()
{
	_Position += _MovementSpeed * _Forward;
}

void CCameraBase::MoveBackwards()
{
	_Position -= _MovementSpeed * _Forward;
}

void CCameraBase::Yaw( float RotationStrength )
{
	_Forward = N3DMath::CalcVectorRotationAboutAxis( _Forward, _Up, RotationStrength * _RotationSpeed );
	_Forward.Normalize(); // For numerical stability reasons
	ASSERT( NMiscMath::AlmostEqual( _Forward.CalcLengthSquared(), 1.0f ), "Camera _Forward is no longer unit length" );
}

void CCameraBase::Pitch( float RotationStrength )
{
	CVector3f Right = _Up.CalcCross( _Forward );
	CVector3f NewUp = N3DMath::CalcVectorRotationAboutAxis( _Up, Right, RotationStrength * _RotationSpeed );
	// restrict movement so camera cant roll upside down
	if ( NewUp._Y < 0.0f )
	{
		NewUp._Y = 0.0f;
	}
	NewUp.Normalize();
	_Up = NewUp; // For numerical stability reasons
	_Forward = Right.CalcCross( NewUp ).CalcNormalized();
	ASSERT( NMiscMath::AlmostEqual( _Up.CalcLengthSquared(), 1.0f ), "Camera _Up is no longer unit length" );
	ASSERT( NMiscMath::AlmostEqual( _Forward.CalcLengthSquared(), 1.0f ), "Camera _Forward is no longer unit length" );
	ASSERT( NMiscMath::AlmostEqual( _Forward.Dot( _Up ), 0.0f ), "Camera _Forward and _Up are no longer orthogonal" );
}

void CCameraBase::RotateRight( float RotationStrength )
{
	Yaw( RotationStrength );
}
void CCameraBase::RotateLeft( float RotationStrength )
{
	Yaw( -RotationStrength );
}
void CCameraBase::RotateDown( float RotationStrength )
{
	Pitch( RotationStrength );
}
void CCameraBase::RotateUp( float RotationStrength )
{
	Pitch( -RotationStrength );
}

void CCameraBase::RotateAboutAxis( const CVector3f& UnitAxisInWorldCoordinates, float RotationStrength )
{

	_Forward = N3DMath::CalcVectorRotationAboutAxis( _Forward, _Up, RotationStrength * _RotationSpeed );
}
