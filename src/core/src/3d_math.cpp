#include "core/3d_math.h"

#include "core/misc_math.h"
#include "core/quaternion.h"
#include "utils/assert.h"

CMatrix4x4f N3DMath::CreateCoordinateTransform(
    const CVector3f& Origin, const CVector3f& Forward, const CVector3f& Up,
    ECoordinateTransformType TransformType) {
  CVector3f Right = Up.CalcCross(Forward);
  if (TransformType == ECoordinateTransformType::LocalToWorld) {
    return CMatrix4x4f{Right._X, Up._X, Forward._X, Origin._X,
                       Right._Y, Up._Y, Forward._Y, Origin._Y,
                       Right._Z, Up._Z, Forward._Z, Origin._Z,
                       0.0f,     0.0f,  0.0f,       1.0f};
  } else {
    CMatrix4x4f AdjustBasis{
        Right._X,   Right._Y,   Right._Z,   0.0f, Up._X, Up._Y, Up._Z, 0.0f,
        Forward._X, Forward._Y, Forward._Z, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f};
    CMatrix4x4f Translate{1.0f, 0.0f,       0.0f, -Origin._X, 0.0f, 1.0f,
                          0.0f, -Origin._Y, 0.0f, 0.0f,       1.0f, -Origin._Z,
                          0.0f, 0.0f,       0.0f, 1.0f};
    return AdjustBasis * Translate;
  }
}

bool N3DMath::AreOrthogonal(const CVector3f& A, const CVector3f& B) {
  return NMiscMath::AlmostEqual(A.Dot(B), 0.0f);
}

bool N3DMath::AlmostEqual(const CVector3f& A, const CVector3f& B) {
  return NMiscMath::AlmostEqual(A._X, B._X) &&
         NMiscMath::AlmostEqual(A._Y, B._Y) &&
         NMiscMath::AlmostEqual(A._Z, B._Z);
}

// clang-format off
CMatrix4x4f N3DMath::CreatePitchRotation( float AngleInRadians )
{
	return CMatrix4x4f{ 1.0f, 0.0f,						0.0f,					0.0f,
						0.0f, cosf( AngleInRadians ),	sinf( AngleInRadians ),	0.0f,
						0.0f, -sinf( AngleInRadians ),	cosf( AngleInRadians ),	0.0f,
						0.0f, 0.0f,						0.0f,					1.0f };
}
CMatrix4x4f N3DMath::CreateYawRotation( float AngleInRadians )
{
	return CMatrix4x4f{ cosf( AngleInRadians ),	0.0f, -sinf( AngleInRadians ),	0.0f,
						0.0f,					1.0f, 0.0f,						0.0f,
						sinf( AngleInRadians ),	0.0f, cosf( AngleInRadians ),	0.0f,
						0.0f,					0.0f, 0.0f,						1.0f };
}
CMatrix4x4f N3DMath::CreateRollRotation( float AngleInRadians ) 
{
	return CMatrix4x4f{ cosf( AngleInRadians ),		sinf( AngleInRadians ),		0.0f, 0.0f,
						-sinf( AngleInRadians ),	cosf( AngleInRadians ),		0.0f, 0.0f,
						0.0f,						0.0f,						1.0f, 0.0f,
						0.0f,						0.0f,						0.0f, 1.0f };
}

CVector3f N3DMath::CalcVectorRotationAboutAxis( const CVector3f& VectorToRotate, const CVector3f& UnitAxis, float AngleInRadians )
{
	ASSERT( NMiscMath::AlmostEqual( UnitAxis.CalcLengthSquared(), 1.0f ), "The given axis must be unit length." );

	CQuaternion p{ 0, VectorToRotate };
	CQuaternion q = CQuaternion::CreateRotationQuaternion( AngleInRadians, UnitAxis );
	CQuaternion qInv = q.CalcInverse();

	CQuaternion RotatedQuat = q * p * qInv;
	ASSERT( NMiscMath::AlmostEqual( RotatedQuat.GetScalar(), 0.0f ), "Scalar part is supposed to be 0 after quat rotation..." );
	ASSERT( NMiscMath::AlmostEqual( RotatedQuat.GetVector().CalcLengthSquared(), VectorToRotate.CalcLengthSquared() ), "Rotating a vector changed it's norm. " );
	return RotatedQuat.GetVector();
}

// clang-format on
