#include "camera_base.h"

#include "3d_math.h"

void CCameraBase::StrafeRight()
{
	CVector3f Right = N3DMath::CalcCross( _Forward, _Up );
	_Position += _MovementSpeed * Right;
}

void CCameraBase::StrafeLeft()
{
	CVector3f Right = N3DMath::CalcCross( _Forward, _Up );
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
