#pragma once

#include "mesh.h"
#include "graphic_resources.h"

#include <string>


struct SMaterial
{
	std::string _VS = shader_names::DefaultVertexShaderFileName;
	std::string _GS;
	std::string _PS = shader_names::DefaultPixelShaderFileName;
};