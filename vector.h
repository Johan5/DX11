#pragma once



class CVector3f
{
public:
	CVector3f() = default;
	CVector3f( float X, float Y, float Z ) : _X{X} , _Y{Y}, _Z{Z} {}

	float _X;
	float _Y;
	float _Z;
};

class CVector4f
{
public:
	CVector4f() = default;
	CVector4f( float X, float Y, float Z, float W ) : _X{ X }, _Y{ Y }, _Z{ Z }, _W{ W } {}

	float _X;
	float _Y;
	float _Z;
	float _W;
};