#pragma once

#include "matrix.h"
#include "vector.h"

namespace N3DMath {

enum class ECoordinateTransformType {
  WorldToLocal,
  LocalToWorld,
};
// "Origin", "Forward" and "Up" must be given in world coordinates
CMatrix4x4f CreateCoordinateTransform(const CVector3f& Origin,
                                      const CVector3f& Forward,
                                      const CVector3f& Up,
                                      ECoordinateTransformType TransformType);

bool AreOrthogonal(const CVector3f& A, const CVector3f& B);
bool AlmostEqual(const CVector3f& A, const CVector3f& B);

CMatrix4x4f CreatePitchRotation(float AngleInRadians);
CMatrix4x4f CreateYawRotation(float AngleInRadians);
CMatrix4x4f CreateRollRotation(float AngleInRadians);

CVector3f CalcVectorRotationAboutAxis(const CVector3f& VectorToRotate,
                                      const CVector3f& UnitAxis,
                                      float AngleInRadians);
}  // namespace N3DMath
