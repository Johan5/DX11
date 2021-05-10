#pragma once

#include <vector>
#include <string>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr

class CGeometryShader
{
public:
	void Initialize(ID3D11Device& Device, const std::string& FileName, const std::string& ShaderMainFunction);
	bool IsInitialized() const;

	ID3D11GeometryShader* AccessGeometryShader() { return _pGeometryShader.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> _pGeometryShader;
};
