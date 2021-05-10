#pragma once

#include "input_enums.h"
#include "input_handler_types.h"

#include <stdint.h>
#include <unordered_map>
#include <functional>


class CInputEvent
{
public:
	explicit CInputEvent( const SInputData& InputData );

	bool IsPressed( EInputCode InputCode ) const;
	
	// returns mouse movement from previous frame, ( bot-left origo with [0, 1] cooridnates )
	CVector2f GetMouseMovement() const;

private:
	SInputData _InputData;
};

////////////////////////////////////////////////////////////////////////////////

class CInputHandler
{
	friend class CSystem;
	using KeyCbkFuncT = std::function<void( const CInputEvent& InputCode )>;
public:
	CInputHandler( int32_t GameWindowWidth, int32_t GameWindowHeight );

	// Input events will be dispatched from the main thread at the start of each frame
	void DispatchInputEvents();

	// pCalleeId should probably be the caller ("self")
	void RegisterKeyInputEventCallback( const void* pCallerId, const KeyCbkFuncT& Callback );
	void DeRegisterKeyInputEventCallback( const void* pCallerId );

private:
	void KeyInputFromOS( EInputType InputType, EInputCode InputCode );
	void MouseMovementFromOs( int32_t NewX, int32_t NewY );

private:
	CDoubleBufferedInput _InputBuffers;
	std::unordered_map<const void*, KeyCbkFuncT> _KeyInputCallbackMap;
	float _GameWindowWidth = 0;
	float _GameWindowHeight = 0;
};
