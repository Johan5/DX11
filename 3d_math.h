#pragma once
#include "matrix.h"
#include "vector.h"

namespace N3DMath
{

	enum class ECoordinateTransformType
	{
		WorldToLocal,
		LocalToWorld,
	};
	// "Origin", "Forward" and "Up" must be given in world coordinates
	CMatrix4x4f CreateCoordinateTransform( const CVector3f& Origin, const CVector3f& Forward, const CVector3f& Up, ECoordinateTransformType TransformType );

	CVector3f CalcCross( const CVector3f& A, const CVector3f& B );
	bool AreOrthogonal( const CVector3f& A, const CVector3f& B );
	bool AlmostEqual( const CVector3f& A, const CVector3f& B );
	
}
