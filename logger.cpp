#include "logger.h"


#include <iostream>
#include <cstdio>

/* static */
char CLogger::_DebugStringBuffer[CLogger::_MaxDebugStringSize] = {};

/* static */
void CLogger::Log( const std::string& String )
{
	LogFormatInternal( String.c_str() );
}

/* static */
void CLogger::LogFormatInternal( const char* CString, ... )
{
	va_list args;
	va_start( args, CString );
	int32_t StringLen = vsnprintf( _DebugStringBuffer, CLogger::_MaxDebugStringSize, CString, args );
	std::cout << _DebugStringBuffer << std::endl;
	va_end( args );
}