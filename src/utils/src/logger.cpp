#include "utils/logger.h"

#include <iostream>

/* static */
char CLogger::_DebugStringBuffer[CLogger::_MaxDebugStringSize] = {};

/* static */
void CLogger::Log(const std::string& String) {
  LogFormat(String);
}
