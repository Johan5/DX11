#pragma once

#include <DirectXMath.h>

using namespace DirectX;


class CCamera
{
public:
	void SetPosition( float X, float Y, float Z );
	void SetRotation( float PitchInDegrees, float YawInDegrees, float RollInDegrees );

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix( XMMATRIX& MatrixOut );

private:
	float _PosX = 0.0f;
	float _PosY = 0.0f;
	float _PosZ = 0.0f;
	// "X rotation"
	float _PitchInDeg = 0.0f;
	// "Y rotation"
	float _YawInDeg = 0.0f;
	// "Z rotation"
	float _RollInDeg = 0.0f;
	XMMATRIX _ViewMatrix;
};
