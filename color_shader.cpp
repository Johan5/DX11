#include "color_shader.h"

#include "graphicsclass.h"
#include "string_util.h"

#include <string>

CColorShader::~CColorShader()
{
	Shutdown();
}


bool CColorShader::Initialize(ID3D11Device* pDevice, HWND Wnd)
{
	std::wstring VsFilename = NStringUtil::StringToWString( "color.vs" );
	std::wstring PsFilename = NStringUtil::StringToWString( "color.ps" );
	bool Result = InitializeShader( pDevice, Wnd, VsFilename, PsFilename );
	return Result;
}

void CColorShader::Shutdown()
{
	ShutdownShader();
}

bool CColorShader::Render(ID3D11DeviceContext* pDeviceContext, int IndexCount, const CMatrix4x4f& WorldMatrix, const CMatrix4x4f& ViewAndProjectionMatrix )
{
	bool Result = SetShaderParameters( pDeviceContext, WorldMatrix, ViewAndProjectionMatrix );
	if ( !Result )
	{
		return false;
	}

	RenderShader( pDeviceContext, IndexCount );
	return true;
}


bool CColorShader::InitializeShader(ID3D11Device* pDevice, HWND Wnd, const std::wstring& VsFilename, const std::wstring& PsFilename)
{
	HRESULT Result;

	ID3D10Blob* pCompiledVertexShader = nullptr;
	ID3D10Blob* pErrorMessage = nullptr;
	Result = D3DCompileFromFile( VsFilename.c_str(), nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pCompiledVertexShader, &pErrorMessage );
	if ( FAILED(Result) )
	{
		if ( pErrorMessage )
		{ // Shader failed to compile
			OutputShaderErrorMessage( pErrorMessage, Wnd, VsFilename );
		}
		else
		{ // indicates failed to find shader file
			MessageBox( Wnd, VsFilename.c_str(), L"Failed to find shader file", MB_OK );
		}
		return false;
	}

	ID3D10Blob* pCompiledPixelShader = nullptr;
	Result = D3DCompileFromFile( PsFilename.c_str(), nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pCompiledPixelShader, &pErrorMessage );
	if ( FAILED( Result ) )
	{
		if ( pErrorMessage )
		{ // Shader failed to compile
			OutputShaderErrorMessage( pErrorMessage, Wnd, VsFilename );
		}
		else
		{ // indicates failed to find shader file
			MessageBox( Wnd, VsFilename.c_str(), L"Failed to find shader file", MB_OK );
		}
		return false;
	}

	Result = pDevice->CreateVertexShader( pCompiledVertexShader->GetBufferPointer(), pCompiledVertexShader->GetBufferSize(), nullptr, &_pVertexShader );
	if ( FAILED( Result ) )
	{
		return false;
	}
	Result = pDevice->CreatePixelShader( pCompiledPixelShader->GetBufferPointer(), pCompiledPixelShader->GetBufferSize(), nullptr, &_pPixelShader );
	if ( FAILED( Result ) )
	{
		return false;
	}

	// Create vertex input layout description
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

	int NumElementsInVsInputLayout = sizeof( pPolygonLayout ) / sizeof( pPolygonLayout[0] );
	Result = pDevice->CreateInputLayout( pPolygonLayout, NumElementsInVsInputLayout, pCompiledVertexShader->GetBufferPointer(),
		pCompiledVertexShader->GetBufferSize(), &_pLayout );
	if ( FAILED( Result ) )
	{
		return false;
	}

	pCompiledVertexShader->Release();
	pCompiledVertexShader = nullptr;
	pCompiledPixelShader->Release();
	pCompiledPixelShader = nullptr;

	// Setup matrix constant buffer
	D3D11_BUFFER_DESC MatrixBufferDesc;
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof( SMatrixCb );
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;
	Result = pDevice->CreateBuffer( &MatrixBufferDesc, nullptr, &_pMatrixBuffer );
	if ( FAILED( Result ) )
	{
		return false;
	}
	
	return true;
}

void CColorShader::ShutdownShader()
{
	if ( _pMatrixBuffer )
	{
		_pMatrixBuffer->Release();
		_pMatrixBuffer = nullptr;
	}
	if ( _pLayout )
	{
		_pLayout->Release();
		_pLayout = nullptr;
	}
	if ( _pPixelShader )
	{
		_pPixelShader->Release();
		_pPixelShader = nullptr;
	}
	if ( _pVertexShader )
	{
		_pVertexShader->Release();
		_pVertexShader = nullptr;
	}
}

void CColorShader::OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, HWND Wnd, const std::wstring& ShaderFilename)
{
	char* pCompileErrors = (char*)pErrorMessage->GetBufferPointer();
	int BufferSize = (int)pErrorMessage->GetBufferSize();
	std::ofstream FOut;
	FOut.open( "shader-error.txt" );
	for ( int i = 0; i < BufferSize; ++i )
	{
		FOut << pCompileErrors[i];
	}
	FOut.close();
	pErrorMessage->Release();
	pErrorMessage = nullptr;
	MessageBox( Wnd, L"Error compiling shader. Check shader-error.txt for message.", ShaderFilename.c_str(), MB_OK );
}

bool CColorShader::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, const CMatrix4x4f& WorldMatrix, const CMatrix4x4f& ViewAndProjectionMatrix )
{
	HRESULT Result;
	// Apparently, DX11 "requires" matrices to be transposed?
	CMatrix4x4f TransposedWorldMatrix = WorldMatrix;
	TransposedWorldMatrix.Transpose();
	CMatrix4x4f TransposedViewAndProjectionMatrix = ViewAndProjectionMatrix;
	TransposedViewAndProjectionMatrix.Transpose();

	// Set SMatrixCb
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	Result = pDeviceContext->Map( _pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( Result ) )
	{
		return false;
	}

	SMatrixCb* pConstantBufferData = static_cast<SMatrixCb*>( MappedResource.pData );
	pConstantBufferData->_World = WorldMatrix;
	pConstantBufferData->_ViewAndProjection = ViewAndProjectionMatrix;

	pDeviceContext->Unmap( _pMatrixBuffer, 0 );

	unsigned int BufferNumber = 0U;
	pDeviceContext->VSSetConstantBuffers( BufferNumber, 1, &_pMatrixBuffer );
	
	return true;
}	

void CColorShader::RenderShader(ID3D11DeviceContext* pDeviceContext, int IndexCount)
{
	// Set vertex input layout
	pDeviceContext->IASetInputLayout( _pLayout );
	// Set which shaders will be used in render pipeline
	pDeviceContext->VSSetShader( _pVertexShader, nullptr, 0 );
	pDeviceContext->PSSetShader( _pPixelShader, nullptr, 0 );
	// Run pipeline
	pDeviceContext->DrawIndexed( IndexCount, 0, 0 );
}
