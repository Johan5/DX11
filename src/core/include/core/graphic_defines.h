#pragma once

#include <cstdint>

namespace NGraphicsDefines
{
	const bool IsFullScreen = false;
	const bool IsVSyncEnabled = true;
	const float ScreenDepth = 1000.0f;
	const float ScreenNear = 1.0f;
	const float ScreenFar = 100.0f;

	// These slots should of course otimally not be hardcoded, but parsed from resource files
	const int32_t ShadowMapTextureSlot = 0;
	const int32_t ShadowMapSamplerSlot = 0;
}