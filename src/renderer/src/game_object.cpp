#include "renderer/game_object.h"

#include "core/3d_math.h"
#include "core/matrix.h"

bool CGameObject::IsInitialized() const {
  return false;
}

void CGameObject::Render(CBatchRenderHelper& BatchRenderHelper,
                         const CCameraBase& Camera) {
  (void)BatchRenderHelper;
  (void)Camera;
  return;
}

void CGameObject::SetPosition(const CVector3f& NewPosition) {
  _Position = NewPosition;
  _TransformsAreStale = true;
}

void CGameObject::SetScale(const CVector3f& NewScale) {
  _Scale = NewScale;
  _TransformsAreStale = true;
}

const CVector3f& CGameObject::GetPosition() const {
  return _Position;
}

CMatrix4x4f CGameObject::GetLocalToWorldTransform() const {
  if (_TransformsAreStale) {
    UpdateTransforms();
  }
  return _LocalToWorldTransform;
}

CMatrix4x4f CGameObject::GetNormalLocalToWorldTransform() const {
  if (_TransformsAreStale) {
    UpdateTransforms();
  }
  return _NormalLocalToWorldTransform;
}

////////////////////////////////////////////////////////////////////////////////

void CGameObject::SetId(int64_t NewId) {
  _Id = NewId;
}

void CGameObject::UpdateTransforms() const {
  _LocalToWorldTransform = N3DMath::CreateCoordinateTransform(
      _Position, _Forward, _Up, _Scale,
      N3DMath::ECoordinateTransformType::LocalToWorld);
  _NormalLocalToWorldTransform =
      N3DMath::CreateNormalTransform(_LocalToWorldTransform)
          .value_or(CMatrix4x4f::Identity());
  _TransformsAreStale = false;
}
