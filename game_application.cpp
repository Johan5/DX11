#include "game_application.h"


CGameApplication::CGameApplication( CInputHandler& InputHandler, CGraphics& Graphics )
	: _InputHandler( InputHandler ), _Graphics( Graphics )
{
	_World.Initialize( _Graphics );
}

bool CGameApplication::ProduceNewFrame()
{
	CVector4f BackgroundColor{ 0.2f, 0.2f, 0.2f, 1.0f };
	CRenderContext& RenderContext = _Graphics.StartRenderFrame( BackgroundColor );
	{
		_World.Render( RenderContext );
	}
	_Graphics.EndFrame( RenderContext );
	return true;
}
