#pragma once

#include "input_enums.h"

#include <stdint.h>
#include <vector>
#include <functional>
#include <unordered_map>
#include <mutex>


struct SKeyInput
{
	EInputType _InputType = EInputType::Invalid;
	EInputCode _InputCode = EInputCode::Unknown;
};

// Input is stored in order it was received, with oldest event first.
// Is threadsafe
class CDoubleBufferedInput
{
public:
	// Swaps the front and backbuffers. Clears the new back buffer, returns the new front buffer.
	std::vector<SKeyInput>& SwapBuffers();
	void AddEventToBackBuffer( const SKeyInput& InputEvent );

private:
	std::vector<SKeyInput> _Buffers[2];
	int32_t _FrontBufferIdx = 0;
	int32_t _BackBufferIdx = 1;

	std::mutex _Mutex;
};

class CInputHandler
{
	friend class CSystem;
public:
	using KeyCbkFuncT = std::function<void( const SKeyInput& InputCode )>;

	// Input events will be dispatched from the main thread at the start of each frame
	void DispatchInputEvents();

	// pCalleeId should probably be the caller ("self")
	void RegisterKeyInputEventCallback( const void* pCallerId, const KeyCbkFuncT& Callback );
	void DeRegisterKeyInputEventCallback( const void* pCallerId );

private:
	void KeyInputFromOS( EInputType InputType, EInputCode InputCode );

private:
	CDoubleBufferedInput _InputBuffers;
	std::unordered_map<const void*, KeyCbkFuncT> _KeyInputCallbackMap;
};
