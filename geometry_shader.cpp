#include "geometry_shader.h"

#include "string_util.h"
#include "logger.h"
#include "assert.h"
#include <d3dcompiler.h>

void CGeometryShader::Initialize(ID3D11Device& Device, const std::string& FileName, const std::string& ShaderMainFunction)
{
	using Microsoft::WRL::ComPtr;
	HRESULT Result;
	std::wstring FileNameW = NStringUtil::StringToWString(FileName);

	ComPtr<ID3D10Blob> pCompiledGeometryShader;
	ComPtr<ID3D10Blob> pErrorMessage;
	Result = D3DCompileFromFile(FileNameW.c_str(), nullptr, nullptr, ShaderMainFunction.c_str(), "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pCompiledGeometryShader.GetAddressOf(), pErrorMessage.GetAddressOf());

	if (FAILED(Result))
	{
		if (pErrorMessage)
		{
			CLogger::LogFormat("Failed to compile shader file %s. Error: ", FileName.c_str());
		}
		else
		{
			CLogger::LogFormat("Failed to find shader file: %s", FileName.c_str());
		}
		return;
	}

	Result = Device.CreateGeometryShader(pCompiledGeometryShader->GetBufferPointer(), pCompiledGeometryShader->GetBufferSize(), nullptr, _pGeometryShader.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create geometry shader");
}

bool CGeometryShader::IsInitialized() const
{
	return _pGeometryShader;
}
