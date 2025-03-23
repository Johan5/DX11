#include "input_handler.h"

#include "assert.h"
#include "logger.h"

#include <utility>
#include <algorithm>



CInputEvent::CInputEvent( const SInputData& InputData )
	: _InputData( InputData )
{
}

bool CInputEvent::IsPressed( EInputCode InputCode ) const
{
	return std::find( _InputData._CurrentlyHeldDown.begin(), _InputData._CurrentlyHeldDown.end(), InputCode ) != _InputData._CurrentlyHeldDown.end();
}

CVector2f CInputEvent::GetMouseMovement() const
{
	return _InputData._RawMousePos - _InputData._RawMousePosPrevFrame;
}

////////////////////////////////////////////////////////////////////////////////

CInputHandler::CInputHandler( int32_t GameWindowWidth, int32_t GameWindowHeight )
	: _GameWindowWidth( (float) GameWindowWidth)
	, _GameWindowHeight( (float) GameWindowHeight)
{
	ASSERT( _GameWindowWidth != 0.0f && _GameWindowHeight != 0.0f, "Incorrect window size, game will crash." );
}

void CInputHandler::DispatchInputEvents()
{
	const SInputData& InputData = _InputBuffers.SwapBuffers();
	CInputEvent InputEvent{ InputData };
	for ( auto& it : _KeyInputCallbackMap )
	{
		it.second( InputEvent );
	}
}
 
void CInputHandler::RegisterKeyInputEventCallback( const void* pCallerId, const KeyCbkFuncT& Callback )
{
	_KeyInputCallbackMap.emplace( pCallerId, Callback );
}

void CInputHandler::DeRegisterKeyInputEventCallback( const void* pCallerId )
{
	_KeyInputCallbackMap.erase( pCallerId );
}

void CInputHandler::KeyInputFromOS(EInputType InputType, EInputCode InputCode)
{
	//CLogger::LogFormat( "Input registered, Type %d, Code %d", (int32_t)InputType, (int32_t)InputCode );
	if ( InputType == EInputType::KeyDown || InputType == EInputType::KeyUp )
	{
		_InputBuffers.AddKeyInput( InputType, InputCode );
	}
	else
	{
		ASSERT( false, "Unrecognized input" ); // not implemented anything else than key input yet
	}
}

void CInputHandler::MouseMovementFromOs( int32_t NewX, int32_t NewY )
{
	float ScaledX = (float)NewX / _GameWindowWidth;
	// Flip Y-axis since OS has origo top-left but this engine uses bot-left
	float ScaledY = 1.0f - NewY / _GameWindowHeight;
	_InputBuffers.AddMousePos( CVector2f{ ScaledX, ScaledY } );
}
