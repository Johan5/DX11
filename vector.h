#pragma once

#include <math.h>
#include <cassert>

class CVector3f
{
public:
	CVector3f() = default;
	CVector3f( float X, float Y, float Z ) : _X{X} , _Y{Y}, _Z{Z} {}

	CVector3f operator+( const CVector3f& Other ) const;
	CVector3f operator-( const CVector3f& Other ) const;
	CVector3f operator/( float Denominator ) const;
	CVector3f& operator+=( const CVector3f& Other );
	CVector3f& operator-=( const CVector3f& Other );

	float Dot( const CVector3f& Other ) const;
	CVector3f CalcCross( const CVector3f& Other ) const;
	
	void Normalize();
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

inline CVector3f operator*( float lhs, const CVector3f& rhs );
inline CVector3f operator*( const CVector3f& lhs, float rhs );

class CVector4f
{
public:
	CVector4f() = default;
	CVector4f( float X, float Y, float Z, float W ) : _X{ X }, _Y{ Y }, _Z{ Z }, _W{ W } {}
	CVector4f( const CVector3f& XYZ, float W ) : _X{ XYZ._X }, _Y{ XYZ._Y }, _Z{ XYZ._Z }, _W{ W } {}

	CVector3f XYZ();

	float _X = 0.0f;
	float _Y = 0.0f;
	float _Z = 0.0f;
	float _W = 0.0f;
};

////////////////////////////////////////////////////////////////////////////////

inline CVector3f CVector3f::operator+( const CVector3f& Other ) const
{
	return CVector3f{ _X + Other._X, _Y + Other._Y, _Z + Other._Z };
}

inline CVector3f CVector3f::operator-( const CVector3f& Other ) const
{
	return CVector3f{ _X - Other._X, _Y - Other._Y, _Z - Other._Z };
}

inline CVector3f CVector3f::operator/( float Denominator ) const
{
	return CVector3f{ _X / Denominator, _Y / Denominator, _Z / Denominator };
}

inline CVector3f& CVector3f::operator+=( const CVector3f& Other )
{
	_X += Other._X;
	_Y += Other._Y;
	_Z += Other._Z;
	return *this;
}

inline CVector3f& CVector3f::operator-=( const CVector3f& Other )
{
	_X -= Other._X;
	_Y -= Other._Y;
	_Z -= Other._Z;
	return *this;
}

inline void CVector3f::Normalize()
{
	*this = this->CalcNormalized();
}

inline CVector3f CVector3f::CalcNormalized() const
{
	assert( CalcLength() != 0.0f );
	return *this / CalcLength();
}

inline float CVector3f::Dot( const CVector3f& Other ) const
{
	return _X * Other._X + _Y * Other._Y + _Z * Other._Z;
}

inline CVector3f CVector3f::CalcCross( const CVector3f& Other ) const
{
	// calculated using the "determinant formula"
	return CVector3f{ _Y * Other._Z - _Z * Other._Y, _Z * Other._X - _X * Other._Z, _X * Other._Y - _Y * Other._X };
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

inline CVector3f operator*( float lhs, const CVector3f& rhs )
{
	return CVector3f{ lhs * rhs._X, lhs * rhs._Y, lhs * rhs._Z };
}

inline CVector3f operator*( const CVector3f& lhs, float rhs )
{
	return operator*( rhs, lhs );
}

inline CVector3f CVector4f::XYZ()
{
	return CVector3f{ _X, _Y, _Z };
}
