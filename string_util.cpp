#include "string_util.h"
#include <Windows.h>


std::wstring NStringUtil::StringToWString(const std::string& String)
{
	int InputStringLen = (int)String.length() + 1;
	int WCharLen = MultiByteToWideChar( CP_ACP, 0, String.c_str(), InputStringLen, 0, 0 );
	wchar_t* WStringBuffer = new wchar_t[WCharLen];
	MultiByteToWideChar( CP_ACP, 0, String.c_str(), InputStringLen, WStringBuffer, WCharLen );
	std::wstring Output{ WStringBuffer };
	delete[] WStringBuffer;
	return Output;
}
