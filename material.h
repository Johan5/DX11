#pragma once

#include "mesh.h"
#include "graphic_resources.h"

#include <string>


struct SMaterial
{
	bool operator==(const SMaterial& other) const = default;

	std::string _VS = shader_names::DefaultVertexShaderFileName;
	std::string _GS;
	std::string _PS = shader_names::DefaultPixelShaderFileName;
};