#pragma once

#include "vector.h"
#include "matrix.h"

class CCameraBase
{
public:
	// CVector3f GetPosition() const = 0;
	//virtual void SetPosition( const CVector3f& NewPosition ) = 0;
	// World space -> Camera space
	virtual CMatrix4x4f GetViewMatrix() const = 0;
	// Camera space -> Screen projeciton, (-1, -1, 0) to (1, 1, 1)
	virtual CMatrix4x4f GetProjectionMatrix() const = 0;
	virtual CMatrix4x4f GetViewAndProjection() const = 0;

	void StrafeRight();
	void StrafeLeft();
	void StrafeDown();
	void StrafeUp();

protected:
	CVector3f _Position = CVector3f{ 0.0f, 0.0f, 0.0f };
	CVector3f _Forward = CVector3f{ 0.0f, 0.0f, 1.0f }.CalcNormalized();
	CVector3f _Up = CVector3f{ 0.0f, 1.0f, 0.0f }.CalcNormalized();
	float _MovementSpeed = 0.01f;
};
