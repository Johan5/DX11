#pragma once

#include "matrix.h"
#include "vector.h"

namespace N3DMath {

enum class ECoordinateTransformType {
  WorldToLocal,
  LocalToWorld,
};

// "Origin", "Forward", "Up", "Scale" must be given in world space
CMatrix4x4f CreateCoordinateTransform(const CVector3f& Origin,
                                      const CVector3f& Forward,
                                      const CVector3f& Up,
                                      const CVector3f& Scale,
                                      ECoordinateTransformType TransformType);

// Creates transform that takes normal from local to world space
std::optional<CMatrix4x4f> CreateNormalTransform(const CMatrix4x4f& localToWorldTransform);

bool AreOrthogonal(const CVector3f& A, const CVector3f& B);
bool AlmostEqual(const CVector3f& A, const CVector3f& B);

CMatrix4x4f CreatePitchRotation(float AngleInRadians);
CMatrix4x4f CreateYawRotation(float AngleInRadians);
CMatrix4x4f CreateRollRotation(float AngleInRadians);

CVector3f CalcVectorRotationAboutAxis(const CVector3f& VectorToRotate,
                                      const CVector3f& UnitAxis,
                                      float AngleInRadians);
}  // namespace N3DMath
