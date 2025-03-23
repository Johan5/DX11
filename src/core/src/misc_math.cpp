#include "core/misc_math.h"

#include <cmath>

using namespace NMiscMath;

bool NMiscMath::AlmostEqual(float A, float B) {
  const float Tolerance = 0.001f;
  return abs(A - B) < Tolerance;
}
