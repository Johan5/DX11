#pragma once

#include "mesh.h"
#include "graphic_resources.h"
#include "texture_view.h"
#include "sampler_state.h"

#include <string>


struct SMaterial
{
	bool operator==(const SMaterial& other) const = default;
	size_t CalcRenderHash() const
	{
		std::hash<std::string> StringHasher;
		std::hash<const ID3D11ShaderResourceView*> TextureHasher;
		std::hash<const ID3D11SamplerState*> SamplerHasher;
		return  StringHasher(_VS) ^ 
				StringHasher(_GS) ^ 
				StringHasher(_PS) ^ 
				TextureHasher(_Texture.GetTextureView()) ^ 
				SamplerHasher(_TextureSampler.GetSamplerState());
	}

	std::string _VS = shader_names::DefaultVertexShaderFileName;
	std::string _GS;
	std::string _PS = shader_names::DefaultPixelShaderFileName;

	CTextureView _Texture;
	CSamplerState _TextureSampler;
};