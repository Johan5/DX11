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
	bool IsInitialized();

	ID3D11VertexShader* AccessVertexShader() { return _pVertexShader.Get(); }
	ID3D11InputLayout* AccessInputLayout() { return _pLayout.Get(); }

private:
	bool SetShaderParameters( ID3D11DeviceContext* pDeviceContext, const CMatrix4x4f& WorldMatrix, const CMatrix4x4f& ViewAndProjectionMatrix );

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _pLayout;
	// Buffer used to set constant buffer data
	//ID3D11Buffer* _pMatrixBuffer = nullptr;
};


