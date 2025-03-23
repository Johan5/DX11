#include "graphics/pixel_shader.h"

#include "utils/logger.h"
#include "utils/string_util.h"
#include "utils/assert.h"


bool CPixelShader::Initialize( ID3D11Device& Device, const std::string& FileName, const std::string& ShaderMainFunction )
{
	using Microsoft::WRL::ComPtr;
	std::wstring FileNameW = NStringUtil::StringToWString( FileName );

	ComPtr<ID3D10Blob> pCompiledPixelShader;
	ComPtr<ID3D10Blob> pErrorMessage;
	HRESULT Result = D3DCompileFromFile( FileNameW.c_str(), nullptr, nullptr, ShaderMainFunction.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pCompiledPixelShader.GetAddressOf(), pErrorMessage.GetAddressOf() );
	if ( FAILED(Result) )
	{
		if ( pErrorMessage )
		{
			CLogger::Log(static_cast<char*>(pErrorMessage->GetBufferPointer()));
		}
		else
		{
			CLogger::LogFormat( "Failed to find shader file: %s", FileName.c_str() );
		}
		ASSERT(false, "Failed to compile pixel shader");
		return false;
	}

	Result = Device.CreatePixelShader( pCompiledPixelShader->GetBufferPointer(), pCompiledPixelShader->GetBufferSize(), nullptr, _pPixelShader.GetAddressOf() );
	ASSERT(SUCCEEDED(Result), "Failed to create pixel shader");

	return true;
}

bool CPixelShader::IsInitialized() const 
{
	return _pPixelShader;
}