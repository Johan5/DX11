#pragma once


#include "matrix.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h> // Microsoft ComPtr
#include <string>


// For now, the input format is hardcoded.
class CVertexShader
{
public:
	bool Initialize( ID3D11Device& Device, const std::string& FileName, const std::string& ShaderMainFunction );
	bool IsInitialized() const;

	ID3D11VertexShader* AccessVertexShader() { return _pVertexShader.Get(); }
	ID3D11InputLayout* AccessInputLayout() { return _pLayout.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _pLayout;
};


