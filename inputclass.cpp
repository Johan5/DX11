#include "inputclass.h"


void CInput::Initialize()
{
	for ( bool& Key : _Keys )
	{
		Key = false;
	}
}

void CInput::KeyDown(unsigned int InputIndex)
{
	_Keys[InputIndex] = true;
}

void CInput::KeyUp(unsigned int InputIndex)
{
	_Keys[InputIndex] = false;
}

bool CInput::IsKeyDown(unsigned int InputIndex)
{
	return _Keys[InputIndex];
}






