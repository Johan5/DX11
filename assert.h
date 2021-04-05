#pragma once

#include <stdint.h>
#include <windows.h>
#include <string>

#define ASSERT( EXPR, MESSAGE ) \
	(void) ( !( EXPR ) \
	&& AssertDebug( MESSAGE, __FILE__, __LINE__ ) \
	&& ( __debugbreak(), false ) )


// Returns true if we should break into debugger
bool AssertDebug( const char* pMessage, const char* pFileName, uint32_t LineNr )
{
	const std::string MsgTitle{ "Assertion failed" };
	const std::string MsgString = std::string{ pMessage } + "\n\n" + std::string{ "Break into debugger?" };
	MSGBOXPARAMSA MsgParams;
	ZeroMemory( &MsgParams, sizeof( MsgParams ) );
	MsgParams.cbSize = sizeof( MsgParams );
	MsgParams.lpszText = MsgString.c_str();
	MsgParams.lpszCaption = MsgTitle.c_str();
	MsgParams.dwStyle = MB_YESNO | MB_SETFOREGROUND;

	switch ( MessageBoxIndirectA( &MsgParams ) )
	{
	case IDYES:
	{
		return true;
	}
	case IDNO:
	{
		return false;
	}
	default:
	{
		return true;
	}
	}

	return true;
}