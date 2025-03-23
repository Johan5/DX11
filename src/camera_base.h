#pragma once

#include "vector.h"
#include "matrix.h"

class CCameraBase
{
public:
	// World space -> Camera space
	CMatrix4x4f GetViewMatrix() const;

	// Camera space -> Screen projection (x in [-1,1], y in [-1,1], z in [0,1])
	virtual CMatrix4x4f GetProjectionMatrix() const = 0;
	CMatrix4x4f GetViewAndProjection() const;

	const CVector3f& GetPosition() const;
	const CVector3f& GetForwardVec() const;
	const CVector3f& GetUpVec() const;

	void SetOrientation(const CVector3f& Position, const CVector3f& Forward, const CVector3f& Up);

	void StrafeRight();
	void StrafeLeft();
	void StrafeDown();
	void StrafeUp();
	void MoveForward();
	void MoveBackwards();

	// Negative values rotate counter-clockwise
	void Yaw( float RotationStrength );
	void Pitch( float RotationStrength );

	// Yaw+
	void RotateRight( float RotationStrength );
	// Yaw-
	void RotateLeft( float RotationStrength );
	// Pitch+
	void RotateDown( float RotationStrength );
	// Pitch-
	void RotateUp( float RotationStrength );
	
	void RotateAboutAxis( const CVector3f& UnitAxisInWorldCoordinates, float RotationStrength );

private: 
	CVector3f _Position = CVector3f{ 0.0f, 0.0f, 0.0f };
	CVector3f _Forward = CVector3f{ 0.0f, 0.0f, 1.0f }.CalcNormalized();
	CVector3f _Up = CVector3f{ 0.0f, 1.0f, 0.0f }.CalcNormalized();
	float _MovementSpeed = 0.1f;
	// given in radians
	float _RotationSpeed = 5.0f;


};
