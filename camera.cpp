#include "camera.h"


constexpr float DegToRad = 360.0f / ( 2.0f * 3.1415926535f );


void CCamera::SetPosition(float X, float Y, float Z)
{
	_PosX = X;
	_PosY = Y;
	_PosZ = Z;
}

DirectX::XMFLOAT3 CCamera::GetPosition()
{
	return DirectX::XMFLOAT3( _PosX, _PosY, _PosZ );
}

void CCamera::GetViewMatrix(DirectX::XMMATRIX& MatrixOut)
{
	// Currently only supports translate
	MatrixOut = DirectX::XMMATRIX( 1.0f, 0.0f, 0.0f, 0.0f,
	                               0.0f, 1.0f, 0.0f, 0.0f,
	                               0.0f, 0.0f, 1.0f, 0.0f,
	                               -_PosX, -_PosY, -_PosZ, 1.0f );
}

void CCamera::GetProjectionMatrix(DirectX::XMMATRIX& MatrixOut)
{
	// perspective projection 
}

void CCamera::GetViewAndProjection(DirectX::XMMATRIX& MatrixOut)
{
	// Projection not yet implemented

	
	return GetViewMatrix( MatrixOut );
}


