#include "input_handler.h"

#include <assert.h>
#include <utility>


std::vector<SKeyInput>& CDoubleBufferedInput::SwapBuffers()
{
	std::lock_guard<std::mutex> Lock{ _Mutex };
	std::swap( _FrontBufferIdx, _BackBufferIdx );
	_Buffers[_BackBufferIdx].clear();
	return _Buffers[_FrontBufferIdx];
}

void CDoubleBufferedInput::AddEventToBackBuffer( const  SKeyInput& InputEvent )
{
	std::lock_guard<std::mutex> Lock{ _Mutex };
	_Buffers[_BackBufferIdx].push_back( InputEvent );
}

////////////////////////////////////////////////////////////////////////////////

void CInputHandler::DispatchInputEvents()
{
	std::vector<SKeyInput>& Inputs = _InputBuffers.SwapBuffers();
	for ( const SKeyInput& Input : Inputs )
	{
		for ( auto& it : _KeyInputCallbackMap )
		{
			it.second( Input );
		}
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

	if ( InputType == EInputType::KeyDown || InputType == EInputType::KeyUp )
	{
		_InputBuffers.AddEventToBackBuffer( SKeyInput{ InputType, InputCode } );
	}
	else
	{
		assert( false ); // not implemented anything else than key input yet
	}
}

