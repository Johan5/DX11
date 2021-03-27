#pragma once

#include <string>
#include <cstdarg>

class CLogger
{
public:
	static void Log( const std::string& String );


	template <typename... T>
	static void LogFormat( const std::string& String, const T&... Args );
	//void LogFormat( const std::string& String, ... );
	
private:
	static void LogFormatInternal( const char* CString, ... );

	static constexpr const int32_t _MaxDebugStringSize = 4096;
	static char _DebugStringBuffer[_MaxDebugStringSize];
};

template <typename... T>
void CLogger::LogFormat( const std::string& String, const T&... Args )
//void CLogger::LogFormat( const std::string& String, ... )
{
	return LogFormatInternal( String.c_str(), std::forward<T>( Args )... );

}