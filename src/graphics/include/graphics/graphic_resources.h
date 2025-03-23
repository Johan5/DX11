#pragma once

#include <string>

namespace shader_names {
const inline std::string DefaultVertexShaderFileName = "default_object.fx";
const inline std::string DefaultPixelShaderFileName = "default_object.fx";
const inline std::string DefaultVertexShaderMainFunction =
    "DefaultVertexShader";
const inline std::string DefaultPixelShaderMainFunction = "DefaultPixelShader";

const inline std::string CubeShadowVertexShaderFileName = "shadow_map.fx";
const inline std::string CubeShadowGeometryShaderFileName = "shadow_map.fx";
const inline std::string CubeShadowPixelShaderFileName = "shadow_map.fx";
const inline std::string CubeShadowVertexShaderMainFunction = "VS";
const inline std::string CubeShadowGeometryShaderMainFunction = "GS";
const inline std::string CubeShadowPixelShaderMainFunction = "PS";
}  // namespace shader_names