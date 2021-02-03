#pragma once

#include "d3dclass.h"
#include "camera.h"
#include "model.h"
#include "color_shader.h"

#include <memory>

namespace NGraphicsDefines
{
	const bool IsFullScreen = false;
	const bool IsVSyncEnabled = true;
	const float ScreenDepth = 1000.0f;
	// Screen near?
	const float ScreenNear = 0.1f;
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

private:
	bool Render();
private:
	std::unique_ptr<CDirectX3D> _Direct3D;
	std::unique_ptr<CCamera> _Camera;
	std::unique_ptr<CModel> _Model;
	std::unique_ptr<CColorShader> _ColorShader;
};