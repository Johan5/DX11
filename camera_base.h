#pragma once

#include "vector.h"
#include "matrix.h"

class CCameraBase
{
public:
	// World space -> Camera space
	virtual CMatrix4x4f GetViewMatrix() const = 0;
	// Camera space -> Screen projeciton, (-1, -1, 0) to (1, 1, 1)
	virtual CMatrix4x4f GetProjectionMatrix() const = 0;
	virtual CMatrix4x4f GetViewAndProjection() const = 0;

	const CVector3f& GetPosition() const;
	const CVector3f& GetForwardVec() const;
	const CVector3f& GetUpVec() const;

	void StrafeRight();
	void StrafeLeft();
	void StrafeDown();
	void StrafeUp();

	// Yaw+
	void RotateRight( float RotationStrength );
	// Yaw-
	void RotateLeft( float RotationStrength );
	// Pitch+
	void RotateDown( float RotationStrength );
	// Pitch-
	void RotateUp( float RotationStrength );
	
private: 
	CVector3f _Position = CVector3f{ 0.0f, 0.0f, 0.0f };
	CVector3f _Forward = CVector3f{ 0.0f, 0.0f, 1.0f }.CalcNormalized();
	CVector3f _Up = CVector3f{ 0.0f, 1.0f, 0.0f }.CalcNormalized();
	float _MovementSpeed = 0.01f;
	// given in radians
	float _RotationSpeed = 0.01f;


};
