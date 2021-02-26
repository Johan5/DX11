#include "vector.h"

float CVector3f::Dot(const CVector3f& Other) const
{
	return _X * Other._X + _Y * Other._Y + _Z * Other._Z;
}
