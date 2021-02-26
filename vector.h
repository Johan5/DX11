#pragma once

#include <math.h>


class CVector3f
{
public:
	CVector3f() = default;
	CVector3f( float X, float Y, float Z ) : _X{X} , _Y{Y}, _Z{Z} {}

	CVector3f operator/( float Denominator ) const;

	float Dot( const CVector3f& Other ) const;
	
	CVector3f CalcNormalized() const;

	float CalcLength() const;
	float CalcLengthSquared() const;

	static CVector3f Forward();
	static CVector3f Up();

public:
	float _X = 0.0f;
	float _Y = 0.0f;
	float _Z = 0.0f;
};

class CVector4f
{
public:
	CVector4f() = default;
	CVector4f( float X, float Y, float Z, float W ) : _X{ X }, _Y{ Y }, _Z{ Z }, _W{ W } {}
	CVector4f( const CVector3f& XYZ, float W ) : _X{ XYZ._X }, _Y{ XYZ._Y }, _Z{ XYZ._Z }, _W{ W } {}

	float _X = 0.0f;
	float _Y = 0.0f;
	float _Z = 0.0f;
	float _W = 0.0f;
};

inline CVector3f CVector3f::operator/( float Denominator ) const
{
	return CVector3f{ _X / Denominator, _Y / Denominator, _Z / Denominator };
}

inline CVector3f CVector3f::CalcNormalized() const
{
	return *this / CalcLength();
}

inline float CVector3f::CalcLength() const
{
	return sqrtf( _X * _X + _Y * _Y + _Z * _Z );
}

inline float CVector3f::CalcLengthSquared() const
{
	return _X * _X + _Y * _Y + _Z * _Z;
}

inline CVector3f CVector3f::Forward()
{
	return CVector3f{ 0.0f, 1.0f, 0.0f };
}

inline CVector3f CVector3f::Up()
{
	return CVector3f{ 0.0f, 0.0f, 1.0f };
}


