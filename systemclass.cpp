#include "systemclass.h"

#include "inputclass.h"
#include "graphics.h"
#include "input_enums.h"
#include "input_handler.h"


namespace
{
	EInputCode TranslateVirtualKeyToInputCode( WPARAM Param )
	{
		/* https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes */
		// Extend this table as needed
		constexpr int VirtualKeyLookupTableSize = 91;
		static EInputCode VirtualKeyToInputCode[VirtualKeyLookupTableSize] = {
			EInputCode::Unknown,
			EInputCode::LeftMouseButton,
			EInputCode::RightMouseButton,
			EInputCode::Unknown,
			EInputCode::MiddleMouseButton,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Backspace,
			EInputCode::Tab,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Enter,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Shift,
			EInputCode::Ctrl,
			EInputCode::Alt,
			EInputCode::Unknown,
			EInputCode::CapsLock,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Escape,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Space,
			EInputCode::PgUp,
			EInputCode::PgDown,
			EInputCode::End,
			EInputCode::Home,
			EInputCode::LeftArrow,
			EInputCode::UpArrow,
			EInputCode::RightArrow,
			EInputCode::DownArrow,
			EInputCode::Select,
			EInputCode::Print,
			EInputCode::Execute,
			EInputCode::PrintScreen,
			EInputCode::Ins,
			EInputCode::Del,
			EInputCode::Help,
			EInputCode::Key0,
			EInputCode::Key1,
			EInputCode::Key2,
			EInputCode::Key3,
			EInputCode::Key4,
			EInputCode::Key5,
			EInputCode::Key6,
			EInputCode::Key7,
			EInputCode::Key8,
			EInputCode::Key9,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::Unknown,
			EInputCode::KeyA,
			EInputCode::KeyB,
			EInputCode::KeyC,
			EInputCode::KeyD,
			EInputCode::KeyE,
			EInputCode::KeyF,
			EInputCode::KeyG,
			EInputCode::KeyH,
			EInputCode::KeyI,
			EInputCode::KeyJ,
			EInputCode::KeyK,
			EInputCode::KeyL,
			EInputCode::KeyM,
			EInputCode::KeyN,
			EInputCode::KeyO,
			EInputCode::KeyP,
			EInputCode::KeyQ,
			EInputCode::KeyR,
			EInputCode::KeyS,
			EInputCode::KeyT,
			EInputCode::KeyU,
			EInputCode::KeyV,
			EInputCode::KeyW,
			EInputCode::KeyX,
			EInputCode::KeyY,
			EInputCode::KeyZ,			
		};

		if ( Param < VirtualKeyLookupTableSize )
		{
			return VirtualKeyToInputCode[Param];
		}
		return EInputCode::Unknown;
	}
}


bool CSystem::Initialize()
{
	int ScreenWidth = 0;
	int ScreenHeight = 0;

	InitializeWindows(ScreenWidth, ScreenHeight);

	_Graphics = std::make_unique<CGraphics>();
	bool GfxOk = _Graphics->Initialize(ScreenWidth, ScreenHeight, _Wnd);
	if ( !GfxOk)
	{
		return false;
	}
	_InputHandler = std::make_unique<CInputHandler>();
	_GameApplication = std::make_unique<CGameApplication>( *_InputHandler, *_Graphics );
	
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
			// translates keystrokes into character codes, generating WM_CHAR messages
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
			_InputHandler->KeyInputFromOS( CInputHandler::EInputType::KeyDown, TranslateVirtualKeyToInputCode( Param ) );
			return 0;
		}
		case WM_KEYUP:
		{
			_InputHandler->KeyInputFromOS( CInputHandler::EInputType::KeyUp, TranslateVirtualKeyToInputCode( Param ) );
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
	if ( _InputHandler->IsKeyDown( EInputCode::Escape ) )
	{
		return false;
	}
	bool Result = _GameApplication->ProduceNewFrame();
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
