#include "game_application.h"


CGameApplication::CGameApplication( CInputHandler& InputHandler, CGraphics& Graphics )
	: _InputHandler( InputHandler ), _Graphics( Graphics )
{
	_World.Initialize( _Graphics, _InputHandler );
	_InputHandler.RegisterKeyInputEventCallback( this, [this]( const SKeyInput& KeyInput )
	{
		if ( KeyInput._InputCode == EInputCode::Escape )
		{
			_ShouldExitApplication = true;
		}
	} );
}

CGameApplication::~CGameApplication()
{
	_InputHandler.DeRegisterKeyInputEventCallback( this );
}

bool CGameApplication::ProduceNewFrame()
{
	_InputHandler.DispatchInputEvents();

	CVector4f BackgroundColor{ 0.2f, 0.2f, 0.2f, 1.0f };
	CRenderContext& RenderContext = _Graphics.StartRenderFrame( BackgroundColor );
	{
		_World.Render( RenderContext );
	}
	_Graphics.EndFrame( RenderContext );
	return !_ShouldExitApplication;
}
