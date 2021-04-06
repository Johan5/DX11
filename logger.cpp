#include "logger.h"


#include <iostream>
#include <cstdio>

/* static */
char CLogger::_DebugStringBuffer[CLogger::_MaxDebugStringSize] = {};

/* static */
void CLogger::Log( const std::string& String )
{
	LogFormat( String );
}
