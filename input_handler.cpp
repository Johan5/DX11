#include "input_handler.h"

#include <assert.h>


void CInputHandler::KeyInputFromOS(EInputType InputType, EInputCode InputCode)
{
	if ( InputType == EInputType::KeyDown )
	{
		if ( !_KeyDown[static_cast<int>( InputCode )] )
		{
			// Should dispatch KeyDown event
			
			_KeyDown[static_cast<int>( InputCode )] = true;
		}
		
	} else if ( InputType == EInputType::KeyUp )
	{
		if ( _KeyDown[static_cast<int>( InputCode )] )
		{
			// Should dispatch KeyUp event
			_KeyDown[static_cast<int>( InputCode )] = false;
		}
	} else
	{
		assert( false );
	}
}

