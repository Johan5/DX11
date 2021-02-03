#include "systemclass.h"

#include <Windows.h>

#include <iostream>
#include <sstream>
#include <memory>

using namespace std;

namespace
{
    // This stream prints to VS output window. 
    class CVsOutputStreamBuffer
        : public stringbuf
    {
    public:
        ~CVsOutputStreamBuffer() { sync(); }
        int sync()
        {
            ::OutputDebugStringA(str().c_str());
            str(string()); // Clear the string buffer
            return 0;
        }
    };
    CVsOutputStreamBuffer VsOutputStreamBuffer;
}


//int main()
//{
//    // Redirect cout to OutputDebugString!
//    cout.rdbuf(&VsOutputStreamBuffer); 
//
// /*   Dx11Test();*/
//}

int WINAPI WinMain( HINSTANCE Instance, HINSTANCE PrevInstance, PSTR pScmdline, int Cmdshow )
{
    //Redirect cout to Visual Studio Output window
    cout.rdbuf(&VsOutputStreamBuffer);
    cout << "WinMain running... Here we go" << endl;

    unique_ptr<CSystem> pSystem = make_unique<CSystem>();
	if ( !pSystem )
	{
        return 0;
	}

    if (pSystem->Initialize() )
    {
        pSystem->Run();
    }

    pSystem->Shutdown();
	return 0;
}
