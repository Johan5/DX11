#include "vertex_shader.h"

#include "string_util.h"
#include "logger.h"

#include <cassert>


bool CVertexShader::Initialize( ID3D11Device& Device, const std::string& FileName, const std::string& ShaderMainFunction )
{
	using Microsoft::WRL::ComPtr;
	HRESULT Result;
	std::wstring FileNameW = NStringUtil::StringToWString( FileName );

	ComPtr<ID3D10Blob> pCompiledVertexShader;
	ComPtr<ID3D10Blob> pErrorMessage;
	Result = D3DCompileFromFile( FileNameW.c_str(), nullptr, nullptr, ShaderMainFunction.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pCompiledVertexShader.GetAddressOf(), pErrorMessage.GetAddressOf() );

	if ( FAILED( Result ) )
	{
		if ( pErrorMessage )
		{
			//CLogger::LogFormat( "Failed to compile shader file %s. Error: ", FileName );
			CLogger::Log( static_cast<char*>( pErrorMessage->GetBufferPointer() ) );
		}
		else
		{
			//CLogger::LogFormat( "Failed to find shader file: %s", FileName );
		}
		return false;
	}

	Result = Device.CreateVertexShader( pCompiledVertexShader->GetBufferPointer(), pCompiledVertexShader->GetBufferSize(), nullptr, _pVertexShader.GetAddressOf() );
	assert( SUCCEEDED( Result ) );

	// Create input layout
	D3D11_INPUT_ELEMENT_DESC pPolygonLayout[2];
	pPolygonLayout[0].SemanticName = "POSITION";
	pPolygonLayout[0].SemanticIndex = 0;
	pPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pPolygonLayout[0].InputSlot = 0;
	pPolygonLayout[0].AlignedByteOffset = 0;
	pPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pPolygonLayout[0].InstanceDataStepRate = 0;

	pPolygonLayout[1].SemanticName = "COLOR";
	pPolygonLayout[1].SemanticIndex = 0;
	pPolygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pPolygonLayout[1].InputSlot = 0;
	pPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pPolygonLayout[1].InstanceDataStepRate = 0;

	int32_t NumElementsInVsInputLayout = sizeof( pPolygonLayout ) / sizeof( pPolygonLayout[0] );
	Result = Device.CreateInputLayout( pPolygonLayout, NumElementsInVsInputLayout, pCompiledVertexShader->GetBufferPointer(),
		pCompiledVertexShader->GetBufferSize(), _pLayout.GetAddressOf() );
	assert( SUCCEEDED( Result ) );

	return true;
	//Result = D3DCompileFromFile( PsFilename.c_str(), nullptr, nullptr, "ColorPixelShader", 
	//                             "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pCompiledPixelShader, &pErrorMessage );
}

bool CVertexShader::IsInitialized()
{
	return _pVertexShader && _pLayout;
}

bool CVertexShader::SetShaderParameters( ID3D11DeviceContext* pDeviceContext, const CMatrix4x4f& WorldMatrix, const CMatrix4x4f& ViewAndProjectionMatrix )
{
//	HRESULT Result;
//	// Apparently, DX11 "requires" matrices to be transposed?
//	CMatrix4x4f TransposedWorldMatrix = WorldMatrix;
//	TransposedWorldMatrix.Transpose();
//	CMatrix4x4f TransposedViewAndProjectionMatrix = ViewAndProjectionMatrix;
//	TransposedViewAndProjectionMatrix.Transpose();
//
//	// Set SMatrixCb
//	D3D11_MAPPED_SUBRESOURCE MappedResource;
//	Result = pDeviceContext->Map( _pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
//	if ( FAILED( Result ) )
//	{
//		return false;
//	}
//
//	SMatrixCb* pConstantBufferData = static_cast<SMatrixCb*>( MappedResource.pData );
//	pConstantBufferData->_World = WorldMatrix;
//	pConstantBufferData->_ViewAndProjection = ViewAndProjectionMatrix;
//
//	pDeviceContext->Unmap( _pMatrixBuffer, 0 );
//
//	unsigned int BufferNumber = 0U;
//	pDeviceContext->VSSetConstantBuffers( BufferNumber, 1, &_pMatrixBuffer );
//
	return true;
}
