#include "core/3d_math.h"

#include "core/misc_math.h"
#include "core/quaternion.h"
#include "utils/assert.h"

CMatrix4x4f N3DMath::CreateCoordinateTransform(
    const CVector3f& Origin, const CVector3f& Forward, const CVector3f& Up,
    const CVector3f& Scale, ECoordinateTransformType TransformType) {

  CVector3f z = Forward.CalcNormalized();
  CVector3f x = Up.CalcCross(z).CalcNormalized();
  CVector3f y = z.CalcCross(x).CalcNormalized();

  x = x * Scale._X;
  y = y * Scale._Y;
  z = z * Scale._Z;

  if (TransformType == ECoordinateTransformType::LocalToWorld) {
    return CMatrix4x4f{x._X,      x._Y,      x._Z,      0.0f, y._X, y._Y,
                       y._Z,      0.0f,      z._X,      z._Y, z._Z, 0.0f,
                       Origin._X, Origin._Y, Origin._Z, 1.0f};
  } else {
    CVector3f InvScale;
    InvScale._X = (Scale._X != 0.0f) ? 1.0f / Scale._X : 0.0f;
    InvScale._Y = (Scale._Y != 0.0f) ? 1.0f / Scale._Y : 0.0f;
    InvScale._Z = (Scale._Z != 0.0f) ? 1.0f / Scale._Z : 0.0f;

    // The inverse of an orthonormal rotation matrix is its transpose.
    // For a row-major matrix, this means the basis vectors become the COLUMNS.
    // The inverse translation is the negative dot product of the origin with
    // each of the original basis vectors.
    return CMatrix4x4f{x._X,
                       y._X,
                       z._X,
                       0.0f,
                       x._Y,
                       y._Y,
                       z._Y,
                       0.0f,
                       x._Z,
                       y._Z,
                       z._Z,
                       0.0f,
                       -Origin.Dot(x) * InvScale._X,
                       -Origin.Dot(y) * InvScale._Y,
                       -Origin.Dot(z) * InvScale._Z,
                       1.0f};
  }
}

std::optional<CMatrix4x4f> N3DMath::CreateNormalTransform(
    const CMatrix4x4f& localToWorldTransform) {
  // Invert and transpose the 4x4 matrix
  // (this is obviously more costly than inverting the 3x3, but at the time of
  // writing I don't have a 3x3 matrix class in this project)
  std::optional<CMatrix4x4f> inverse = localToWorldTransform.CalcInvert();
  ASSERT(inverse, "Failed to calc inverse. Is localToWorld corrupt?");
  if (!inverse) {
    return std::nullopt;
  }

  CMatrix4x4f normalMat = inverse->CalcTranspose();

  // Remove all translation (we are dealing with normal vectors)
  normalMat._M14 = 0.0f;
  normalMat._M24 = 0.0f;
  normalMat._M34 = 0.0f;
  normalMat._M41 = 0.0f;
  normalMat._M42 = 0.0f;
  normalMat._M43 = 0.0f;

  return normalMat;
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
