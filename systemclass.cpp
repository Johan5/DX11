#include "systemclass.h"

#include "inputclass.h"
#include "graphicsclass.h"


bool CSystem::Initialize()
{
	int ScreenWidth = 0;
	int ScreenHeight = 0;

	InitializeWindows(ScreenWidth, ScreenHeight);

	_Input = std::make_unique<CInput>();
	if (!_Input)
	{
		return false;
	}
	_Input->Initialize();

	_Graphics = std::make_unique<CGraphics>();
	if (!_Graphics)
	{
		return false;
	}
	bool GfxOk = _Graphics->Initialize(ScreenWidth, ScreenHeight, _Wnd);
	if ( !GfxOk)
	{
		return false;
	}
	
	return true;
}

void CSystem::Shutdown()
{
	if (_Graphics)
	{
		_Graphics->Shutdown();
	}
	ShutdownWindows();
}

void CSystem::Run()
{
	MSG msg;
	ZeroMemory( &msg, sizeof( MSG ) );

	bool Done = false;
	while (!Done )
	{
		if ( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( msg.message == WM_QUIT )
		{
			Done = true;
		} else
		{
			bool Result = Frame();
			if ( !Result )
			{
				Done = true;
			}
		}
	}
}

LRESULT CALLBACK CSystem::MessageHandler(HWND Wnd, UINT Msg, WPARAM Param, LPARAM pParam)
{
	switch (Msg)
	{
	case WM_KEYDOWN:
		{
		_Input->KeyDown( (unsigned int)Param );
		return 0;
		}
	case WM_KEYUP:
		{
		_Input->KeyUp( (unsigned int)Param );
		return 0;
		}
		default:
		{
			return DefWindowProc( Wnd, Msg, Param, pParam );
		}
	}
}


bool CSystem::Frame()
{
	if ( _Input->IsKeyDown(VK_ESCAPE) )
	{
		return false;
	}

	bool Result = _Graphics->Frame();

	return Result;
}

void CSystem::InitializeWindows(int& ScreenWidth, int& ScreenHeight)
{
	// Set static ptr
	pApplicationHandle = this;

	_Instance = GetModuleHandle( nullptr );
	_ApplicationName = L"DX11Test";
	
	{
		WNDCLASSEX WC;
		WC.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		WC.lpfnWndProc = WndProc;
		WC.cbClsExtra = 0;
		WC.cbWndExtra = 0;
		WC.hInstance = _Instance;
		WC.hIcon = LoadIcon( nullptr, IDI_WINLOGO );
		WC.hIconSm = WC.hIcon;
		WC.hCursor = LoadCursor( nullptr, IDC_ARROW );
		WC.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
		WC.lpszMenuName = nullptr;
		WC.lpszClassName = _ApplicationName;
		WC.cbSize = sizeof( WNDCLASSEX );
		RegisterClassEx( &WC );
	}

	
	int PosX;
	int PosY;
	if ( NGraphicsDefines::IsFullScreen )
	{
		ScreenWidth = GetSystemMetrics( SM_CXSCREEN );
		ScreenHeight = GetSystemMetrics( SM_CYSCREEN );
		DEVMODE ScreenSettings;
		memset( &ScreenSettings, 0, sizeof( ScreenSettings ) );
		ScreenSettings.dmSize = sizeof( ScreenSettings );
		ScreenSettings.dmPelsWidth = (unsigned long)ScreenWidth;
		ScreenSettings.dmPelsHeight = (unsigned long)ScreenHeight;
		ScreenSettings.dmBitsPerPel = 32; // color depth
		ScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings( &ScreenSettings, CDS_FULLSCREEN );
		PosX = PosY = 0;
	}
	else
	{
		ScreenWidth = 800;
		ScreenHeight = 600;
		PosX = ( GetSystemMetrics( SM_CXSCREEN ) - ScreenWidth ) / 2;
		PosY = ( GetSystemMetrics( SM_CYSCREEN ) - ScreenHeight ) / 2;
	}

	DWORD Style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	_Wnd = CreateWindowEx( WS_EX_APPWINDOW, _ApplicationName, _ApplicationName, Style, PosX, PosY, ScreenWidth
		, ScreenHeight, nullptr, nullptr, _Instance, nullptr );
	
	// Bring up window and set as main focus
	ShowWindow( _Wnd, SW_SHOW );
	SetForegroundWindow( _Wnd );
	SetFocus( _Wnd );

	ShowCursor( false );	
}

void CSystem::ShutdownWindows()
{
	ShowCursor( true );
	if ( NGraphicsDefines::IsFullScreen )
	{
		ChangeDisplaySettings( nullptr, 0 );
	}

	DestroyWindow( _Wnd );
	_Wnd = nullptr;

	UnregisterClass( _ApplicationName, _Instance );
	_Instance = nullptr;

	pApplicationHandle = nullptr;
}

LRESULT WndProc(HWND Wnd, UINT Message, WPARAM Param, LPARAM pParam)
{
	switch ( Message )
	{
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
	default:
		return pApplicationHandle->MessageHandler( Wnd, Message, Param, pParam );
	}
	
}
