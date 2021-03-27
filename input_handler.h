#pragma once

#include "input_enums.h"

#include <stdint.h>

class CInputHandler
{
	friend class CSystem;
public:
	
	enum class EInputType
	{
		KeyDown,
		KeyUp,
	};
	
	bool IsKeyDown( EInputCode InputCode ) const { return _KeyDown[(int32_t)InputCode]; }

private:
	void KeyInputFromOS( EInputType InputType, EInputCode InputCode );

private:
	bool _KeyDown[static_cast<int>( EInputCode::NumEnumOptions )] = {};
};
