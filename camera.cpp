#include "camera.h"


constexpr float DegToRad = 360.0f / ( 2.0f * 3.1415926535f );


void CCamera::SetPosition(float X, float Y, float Z)
{
	_PosX = X;
	_PosY = Y;
	_PosZ = Z;
}

void CCamera::SetRotation(float PitchInDegrees, float YawInDegrees, float RollInDegrees)
{
	_PitchInDeg = PitchInDegrees;
	_YawInDeg = YawInDegrees;
	_RollInDeg = RollInDegrees;
}

XMFLOAT3 CCamera::GetPosition()
{
	return XMFLOAT3( _PosX, _PosY, _PosZ );
}

XMFLOAT3 CCamera::GetRotation()
{
	return XMFLOAT3( _PitchInDeg, _YawInDeg, _RollInDeg );
}

void CCamera::Render()
{
	XMFLOAT3 Up{ 0.0f, 1.0f, 0.0f };
	XMVECTOR UpVector = XMLoadFloat3( &Up );
	XMFLOAT3 Position{ _PosX, _PosY, _PosZ };
	XMVECTOR PositionVector = XMLoadFloat3( &Position );
	XMFLOAT3 LookAt{ 0.0f, 0.0f, 1.0f };
	XMVECTOR LookAtVector = XMLoadFloat3( &LookAt );
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw( DegToRad * _PitchInDeg, DegToRad * _YawInDeg, DegToRad * _RollInDeg );

	LookAtVector = XMVector3TransformCoord( LookAtVector, RotationMatrix );
	UpVector = XMVector3TransformCoord( UpVector, RotationMatrix );
	
	LookAtVector = XMVectorAdd( PositionVector, LookAtVector );
	

	_ViewMatrix = XMMatrixLookAtLH( PositionVector, LookAtVector, UpVector );
}

void CCamera::GetViewMatrix(XMMATRIX& MatrixOut)
{
	MatrixOut = _ViewMatrix;
}


