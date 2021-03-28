#pragma once

#include "graphics.h"
#include "input_handler.h"
#include "world.h"
#include "frame_timer.h"

#include <memory>

class CGameApplication
{
public:
	CGameApplication( CInputHandler& InputHandler, CGraphics& Graphics );
	~CGameApplication();

	// This is the main update loop, called from the system class
	bool ProduceNewFrame();

private:
	CInputHandler& _InputHandler;
	CGraphics& _Graphics;	
	CWorld _World;

	CFrameTimer _FrameTimer;

	bool _ShouldExitApplication = false;
};
