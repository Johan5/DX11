#pragma once

#include "vector.h"

class CQuaternion
{
public:
	CQuaternion() = default;
	CQuaternion( float S, const CVector3f& Vec );
	CQuaternion( float A, float B, float C, float D );

	CQuaternion operator+( const CQuaternion& Other ) const;
	CQuaternion operator*( const CQuaternion& Other ) const;

	float GetScalar() const;
	const CVector3f& GetVector() const;

	float CalcLength() const;
	float CalcLengthSquared() const;
	void Normalize();
	CQuaternion CalcNormalized() const;

	CQuaternion CalcConjugate() const;
	CQuaternion CalcInverse() const;
	
	static CQuaternion CreateRotationQuaternion( float AngleInRadians, const CVector3f& UnitVector );

private:
	float _S = 0.0f;
	CVector3f _Vec;
};

CQuaternion operator*( float Scalar, const CQuaternion& Quat );

////////////////////////////////////////////////////////////////////////////////

inline CQuaternion CQuaternion::operator+( const CQuaternion& Other ) const
{
	return CQuaternion{ _S + Other._S, _Vec + Other._Vec };
}

inline CQuaternion CQuaternion::operator*( const CQuaternion& Other ) const
{
	float NewS = _S * Other._S - _Vec.Dot( Other._Vec );
	CVector3f NewVec{ _S * Other._Vec + Other._S * _Vec + _Vec.CalcCross( Other._Vec ) };
	return CQuaternion{ NewS, NewVec };
}

inline CQuaternion operator*( float Scalar, const CQuaternion& Quat )
{
	return CQuaternion{ Scalar * Quat.GetScalar(), Scalar * Quat.GetVector() };
}
