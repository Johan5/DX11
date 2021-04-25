#pragma once

#include <string>
#include <iostream>
#include <cstdio>

// This implementation is obv not threadsafe or reentrant
class CLogger
{
public:
	static void Log( const std::string& String );
	
	template <typename... T>
	static void LogFormat( const std::string& String, const T&... Args );
	
private:
	static constexpr const int32_t _MaxDebugStringSize = 4096;
	static char _DebugStringBuffer[_MaxDebugStringSize];
};


////////////////////////////////////////////////////////////////////////////////

template <typename... T>
void CLogger::LogFormat( const std::string& String, const T&... Args )
{
	memset(&_DebugStringBuffer, 0, _MaxDebugStringSize);
	int32_t StringLen = snprintf( _DebugStringBuffer, CLogger::_MaxDebugStringSize, String.c_str(), Args... );
	std::cout << _DebugStringBuffer << std::endl;
}
