#pragma once

#include "d3dclass.h"
#include "orthographic_camera.h"
#include "model.h"
#include "color_shader.h"
#include "frame_timer.h"

#include <memory>
#include <vector>

namespace NGraphicsDefines
{
	const bool IsFullScreen = false;
	const bool IsVSyncEnabled = true;
	const float ScreenDepth = 1000.0f;
	const float ScreenNear = 1.0f;
	const float ScreenFar = 10.0f;
}


class CGraphics
{
public:
	CGraphics() = default;
	CGraphics( const CGraphics& Other ) = delete;
	CGraphics( const CGraphics&& Other ) = delete;
	~CGraphics();
	CGraphics& operator=( const CGraphics& Other ) = delete;
	CGraphics& operator=( const CGraphics&& Other ) = delete;

	bool Initialize(int ScreenWidth, int ScreenHeight, HWND Wnd);
	void Shutdown();
	bool Frame();

	int GetScreenWidth() const { return _ScreenWidthInPix; }
	int GetScreenHeight() const { return _ScreenHeightInPix; }
	
private:
	bool Render( double TimeElapsedMs );
private:
	std::unique_ptr<CDirectX3D> _Direct3D;
	//std::unique_ptr<CCamera> _Camera;
	std::unique_ptr<COrthographicCamera> _Camera;
	std::vector< std::unique_ptr<CModelInterface> > _Models;
	std::unique_ptr<CColorShader> _ColorShader;

	int _ScreenWidthInPix;
	int _ScreenHeightInPix;

	CFrameTimer _FrameTimer;
};