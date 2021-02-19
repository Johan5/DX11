#pragma once

#include <DirectXMath.h>


// Using left handed coordinate systems
class CCamera
{
public:
	void SetPosition( float X, float Y, float Z );

	DirectX::XMFLOAT3 GetPosition();

	void GetViewMatrix(DirectX::XMMATRIX& MatrixOut );
	void GetProjectionMatrix( DirectX::XMMATRIX& MatrixOut );
	void GetViewAndProjection(DirectX::XMMATRIX& MatrixOut );

private:
	float _PosX = 0.5f;
	float _PosY = 0.0f;
	float _PosZ = 0.0f;

	// in degrees
	float _FieldOfViewDegrees = 90.0f;
};
