#include "camera_base.h"

#include "3d_math.h"
#include "misc_math.h"

#include <cassert>

namespace
{
	// Rotate in ZX plane
	void RotateZX( CVector3f& VecToRotate, float Radians )
	{
		float NewX = VecToRotate._X * cos( Radians ) - VecToRotate._Z * sin( Radians );
		float NewZ = VecToRotate._X * sin( Radians ) + VecToRotate._Z * cos( Radians );
		VecToRotate._X = NewX;
		VecToRotate._Z = NewZ;
	}
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

void CCameraBase::RotateRight()
{
	RotateZX( _Forward, _RotationSpeed );
	assert( NMiscMath::AlmostEqual( 1.0f, _Forward.CalcLength() ) );
}
void CCameraBase::RotateLeft()
{
	RotateZX( _Forward, -_RotationSpeed );
	assert( NMiscMath::AlmostEqual( 1.0f, _Forward.CalcLength() ) );
}
