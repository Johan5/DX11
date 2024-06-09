#include "game_object.h"

#include "matrix.h"
#include "3d_math.h"


bool CGameObject::IsInitialized() const
{
	return false;
}

void CGameObject::Render(CBatchRenderHelper& BatchRenderHelper, const CCameraBase& Camera)
{
	(void)BatchRenderHelper;
	(void)Camera;
	return;
}

void CGameObject::SetPosition(const CVector3f& NewPosition)
{
	_Position = NewPosition;
	_TransformsAreStale = true;
}

void CGameObject::SetScale(const CVector3f& NewScale)
{
	_Scale = NewScale;
	_TransformsAreStale = true;
}

const CVector3f& CGameObject::GetPosition() const
{
	return _Position;
}

CMatrix4x4f CGameObject::GetLocalToWorldTransform() const
{
	if (_TransformsAreStale)
	{
		UpdateTransforms();
	}
	//return _LocalToWorldTransform;
	CMatrix4x4f ScaledMatrix = _LocalToWorldTransform * CMatrix4x4f::CalcScaleMatrix(_Scale); // DBG
	// dont forget to scale normal transform too
	// TODO: This is multiplying local coordinate orientation with world coordinate scale, it is not going to work for rotated objects
	return _LocalToWorldTransform * CMatrix4x4f::CalcScaleMatrix(_Scale);
}


CMatrix4x4f CGameObject::GetNormalLocalToWorldTransform() const
{
	if (_TransformsAreStale)
	{
		UpdateTransforms();
	}
	return _NormalLocalToWorldTransform ;
}

////////////////////////////////////////////////////////////////////////////////


void CGameObject::SetId(int64_t NewId)
{
	_Id = NewId;
}

void CGameObject::UpdateTransforms() const
{
	_LocalToWorldTransform = N3DMath::CreateCoordinateTransform(_Position, _Forward, _Up, N3DMath::ECoordinateTransformType::LocalToWorld);
	// The normal "local to world" cannot be used for normals since it might contain non-uniform scaling.
	// Normals require inverse scaling (i.e. if model scales 0.5 in x, normals need to be scaled 2 in x)
	_NormalLocalToWorldTransform = N3DMath::CreateCoordinateTransform(_Position, _Forward, _Up, N3DMath::ECoordinateTransformType::WorldToLocal);
	_NormalLocalToWorldTransform.Transpose();
	_TransformsAreStale = false;
}
