#pragma once

#include <string>

namespace shader_names {
const inline std::string DefaultVertexShaderFileName = "default_vertex.fx";
const inline std::string DefaultPixelShaderFileName = "default_frag.fx";
const inline std::string DefaultVertexShaderMainFunction =
    "DefaultVertexShader";
const inline std::string DefaultPixelShaderMainFunction = "DefaultPixelShader";

const inline std::string CubeShadowVertexShaderFileName = "shadow_map.fx";
const inline std::string CubeShadowGeometryShaderFileName = "shadow_map.fx";
const inline std::string CubeShadowPixelShaderFileName = "shadow_map.fx";
const inline std::string CubeShadowVertexShaderMainFunction = "VS";
const inline std::string CubeShadowGeometryShaderMainFunction = "GS";
const inline std::string CubeShadowPixelShaderMainFunction = "PS";

const inline std::string CharacterVertexShaderFileName = "character_vertex.fx";
const inline std::string CharacterPixelShaderFileName = "default_frag.fx";
const inline std::string CharacterVertexShaderMainFunction =
    "CharacterVertexShader";
const inline std::string CharacterPixelShaderMainFunction =
    "DefaultPixelShader";
}  // namespace shader_names