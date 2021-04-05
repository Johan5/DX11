#pragma once

#include <utility>

#include "vector.h"

// Row major matrix implementation
// Todo: inspect assembly, do I need to use SSE intrinsics?
class CMatrix4x4f
{
public:
	CMatrix4x4f() = default;
	CMatrix4x4f( float M11, float M12, float M13, float M14,
				 float M21, float M22, float M23, float M24,
				 float M31, float M32, float M33, float M34,
				 float M41, float M42, float M43, float M44 );

	CMatrix4x4f operator*( const CMatrix4x4f& Other ) const;
	CVector4f operator*( const CVector4f& Vector ) const;
	
	static CMatrix4x4f Identity() { return CMatrix4x4f( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f ); }
	static CMatrix4x4f CreateTransform( const CVector3f& Translate, const CVector3f& Scale, const CVector3f& Rotate );
	
	void Transpose();
	CMatrix4x4f CalcTranspose();
	
	float _M11 = 0.0f;
	float _M12 = 0.0f;
	float _M13 = 0.0f;
	float _M14 = 0.0f;
	float _M21 = 0.0f;
	float _M22 = 0.0f;
	float _M23 = 0.0f;
	float _M24 = 0.0f;
	float _M31 = 0.0f;
	float _M32 = 0.0f;
	float _M33 = 0.0f;
	float _M34 = 0.0f;
	float _M41 = 0.0f;
	float _M42 = 0.0f;
	float _M43 = 0.0f;
	float _M44 = 0.0f;
};


////////////////////////////////////////////////////////////////////////////////////

inline CMatrix4x4f::CMatrix4x4f(float M11, float M12, float M13, float M14, 
								float M21, float M22, float M23, float M24, 
								float M31, float M32, float M33, float M34, 
								float M41, float M42, float M43, float M44)
	: _M11( M11 ), _M12( M12 ), _M13( M13 ), _M14( M14 )
	, _M21( M21 ), _M22( M22 ), _M23( M23 ), _M24( M24 )
	, _M31( M31 ), _M32( M32 ), _M33( M33 ), _M34( M34 )
	, _M41( M41 ), _M42( M42 ), _M43( M43 ), _M44( M44 ) {}

inline CMatrix4x4f CMatrix4x4f::operator*(const CMatrix4x4f& Other) const
{
	float M11 = _M11 * Other._M11 + _M12 * Other._M21 + _M13 * Other._M31 + _M14 * Other._M41;
	float M12 = _M11 * Other._M12 + _M12 * Other._M22 + _M13 * Other._M32 + _M14 * Other._M42;
	float M13 = _M11 * Other._M13 + _M12 * Other._M23 + _M13 * Other._M33 + _M14 * Other._M43;
	float M14 = _M11 * Other._M14 + _M12 * Other._M24 + _M13 * Other._M34 + _M14 * Other._M44;
	
	float M21 = _M21 * Other._M11 + _M22 * Other._M21 + _M23 * Other._M31 + _M24 * Other._M41;
	float M22 = _M21 * Other._M12 + _M22 * Other._M22 + _M23 * Other._M32 + _M24 * Other._M42;
	float M23 = _M21 * Other._M13 + _M22 * Other._M23 + _M23 * Other._M33 + _M24 * Other._M43;
	float M24 = _M21 * Other._M14 + _M22 * Other._M24 + _M23 * Other._M34 + _M24 * Other._M44;
	
	float M31 = _M31 * Other._M11 + _M32 * Other._M21 + _M33 * Other._M31 + _M34 * Other._M41;
	float M32 = _M31 * Other._M12 + _M32 * Other._M22 + _M33 * Other._M32 + _M34 * Other._M42;
	float M33 = _M31 * Other._M13 + _M32 * Other._M23 + _M33 * Other._M33 + _M34 * Other._M43;
	float M34 = _M31 * Other._M14 + _M32 * Other._M24 + _M33 * Other._M34 + _M34 * Other._M44;
	
	float M41 = _M41 * Other._M11 + _M42 * Other._M21 + _M43 * Other._M31 + _M44 * Other._M41;
	float M42 = _M41 * Other._M12 + _M42 * Other._M22 + _M43 * Other._M32 + _M44 * Other._M42;
	float M43 = _M41 * Other._M13 + _M42 * Other._M23 + _M43 * Other._M33 + _M44 * Other._M43;
	float M44 = _M41 * Other._M14 + _M42 * Other._M24 + _M43 * Other._M34 + _M44 * Other._M44;

	return CMatrix4x4f{ M11, M12, M13, M14, M21, M22, M23, M24, M31, M32, M33, M34, M41, M42, M43, M44 };
}

inline CVector4f CMatrix4x4f::operator*(const CVector4f& Vector) const
{
	float x = _M11 * Vector._X + _M12 * Vector._Y + _M13 * Vector._Z + _M14 * Vector._W;
	float y = _M21 * Vector._X + _M22 * Vector._Y + _M23 * Vector._Z + _M24 * Vector._W;
	float z = _M31 * Vector._X + _M32 * Vector._Y + _M33 * Vector._Z + _M34 * Vector._W;
	float w = _M41 * Vector._X + _M42 * Vector._Y + _M43 * Vector._Z + _M44 * Vector._W;
	return CVector4f( x, y, z, w );
}

inline void CMatrix4x4f::Transpose()
{
	std::swap( _M12, _M21 );
	std::swap( _M13, _M31 );
	std::swap( _M14, _M41 );
	std::swap( _M23, _M32 );
	std::swap( _M24, _M42 );
	std::swap( _M34, _M43 );
}

inline CMatrix4x4f CMatrix4x4f::CalcTranspose()
{
	return CMatrix4x4f{
		_M11, _M21, _M31, _M41,
		_M12, _M22, _M32, _M42,
		_M13, _M23, _M33, _M43,
		_M14, _M24, _M34, _M44 };
}