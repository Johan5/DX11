#pragma once

#include <cstdint>
#include <Windows.h>

#ifdef _MSC_VER
#define ASSERT( EXPR, MESSAGE ) \
	(void) ( !( EXPR ) \
	&& AssertDebug( MESSAGE, __FILE__, __LINE__ ) \
	&& ( __debugbreak(), false ) )


// Returns true if we should break into debugger
bool AssertDebug( const char* pMessage, const char* pFileName, uint32_t LineNr );
#endif
