#include "quaternion.h"

#include "misc_math.h"

CQuaternion::CQuaternion( float S, const CVector3f& Vec )
	: _S( S )
	, _Vec( Vec )
{
}
CQuaternion::CQuaternion( float A, float B, float C, float D )
	: _S( A )
	, _Vec( B, C, D )
{
}

float CQuaternion::GetScalar() const
{
	return _S;
}

const CVector3f& CQuaternion::GetVector() const
{
	return _Vec;
}

float CQuaternion::CalcLength() const
{
	return sqrtf( CalcLengthSquared() );
}

float CQuaternion::CalcLengthSquared() const
{
	return _S * _S + _Vec.CalcLengthSquared();
}

void CQuaternion::Normalize()
{
	*this = CalcNormalized();
}

CQuaternion CQuaternion::CalcNormalized() const
{
	float Len = CalcLength();
	assert( Len != 0.0f );
	return CQuaternion{ _S / Len, _Vec / Len };
}

CQuaternion CQuaternion::CalcConjugate() const
{
	return CQuaternion{ _S, -_Vec._X, -_Vec._Y, -_Vec._Z };
}

CQuaternion CQuaternion::CalcInverse() const
{
	return (1.0f / CalcLengthSquared()) * CalcConjugate();
}

/* static */
CQuaternion CQuaternion::CreateRotationQuaternion( float AngleInRadians, const CVector3f& UnitVector )
{
	assert( NMiscMath::AlmostEqual( UnitVector.CalcLengthSquared(), 1.0f ) );
	return CQuaternion{ cosf( AngleInRadians / 2.0f ), sinf( AngleInRadians / 2.0f ) * UnitVector };
}