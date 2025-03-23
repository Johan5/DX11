#include "input/perspective_camera.h"

#include "core/3d_math.h"
#include "core/graphic_defines.h"
#include "utils/assert.h"

CPerspectiveCamera::CPerspectiveCamera(float HorizontalFoV, float AspectRatio)
    : _HorizontalFoV(HorizontalFoV), _AspectRatio(AspectRatio) {}

void CPerspectiveCamera::SetFieldOfView(float Angle) {
  _HorizontalFoV = Angle;
}

void CPerspectiveCamera::SetAspectRatio(float AspectRatio) {
  _AspectRatio = AspectRatio;
}

CMatrix4x4f CPerspectiveCamera::GetProjectionMatrix() const {
  float n = NGraphicsDefines::ScreenNear;
  ASSERT(n > 0.0f, "Perspective camera needs a positive ScreenNear");
  float f = NGraphicsDefines::ScreenFar;
  float CameraDepth = f - n;
  CMatrix4x4f Perspective{n,    0.0f, 0.0f,  0.0f,   0.0f, n,    0.0f, 0.0f,
                          0.0f, 0.0f, n + f, -f * n, 0.0f, 0.0f, 1.0f, 0.0f};

  float CameraWidth = 2 * tanf(_HorizontalFoV / 2.0f);
  float CameraHeight = CameraWidth / _AspectRatio;
  // After the perspective map we just need to do an orthographic projection onto the viewport
  CMatrix4x4f Projection{2.0f / CameraWidth,
                         0.0f,
                         0.0f,
                         0.0f,
                         0.0f,
                         2.0f / CameraHeight,
                         0.0f,
                         0.0f,
                         0.0f,
                         0.0f,
                         1.0f / CameraDepth,
                         -n / CameraDepth,
                         0.0f,
                         0.0f,
                         0.0f,
                         1.0f};
  return Projection * Perspective;
}
