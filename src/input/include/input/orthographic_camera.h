#pragma once

#include <DirectXMath.h>
#include "camera_base.h"
#include "core/matrix.h"
#include "core/vector.h"

// Using left handed coordinate systems
// (viewing down +Z, +X right, +Y Up)
class COrthographicCamera : public CCameraBase {
 public:
  // The Width and Height is in "World space units"
  COrthographicCamera(float ViewportWidth, float ViewportHeight)
      : _ViewportWidth(ViewportWidth), _ViewportHeight(ViewportHeight) {}

  // Camera space -> Screen projeciton, (-1, -1, 0) to (1, 1, 1)
  CMatrix4x4f GetProjectionMatrix() const override;

 private:
  // in "world space units"
  float _ViewportWidth;
  float _ViewportHeight;
};
