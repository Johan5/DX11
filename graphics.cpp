#include "graphics.h"

#include "vector.h"
#include "string_util.h"
#include "logger.h"
#include "assert.h"

struct STest
{
	CVector3f _Pos;
	CVector4f _Clr;
};


CVertexBuffer::CVertexBuffer( ID3D11Device& Device, const void* pVertexData, const SVertexBufferProperties& Properties )
{
	ASSERT(Properties._VertexDataSizeInBytes > 0, "Vertex data size needs to be positive" );
	ASSERT(Properties._SingleVertexSizeInBytes > 0, "Single vertex size needs to be positive" );
	D3D11_BUFFER_DESC VertexBufferDesc;
	ZeroMemory( &VertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );
	VertexBufferDesc.ByteWidth = Properties._VertexDataSizeInBytes;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA VertexData;
	ZeroMemory( &VertexData, sizeof( D3D11_SUBRESOURCE_DATA ) );
	VertexData.pSysMem = pVertexData;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;
	HRESULT VertexResult = Device.CreateBuffer( &VertexBufferDesc, &VertexData, _pVertexBuffer.GetAddressOf() );
	ASSERT( SUCCEEDED( VertexResult ), "Failed to create vertex buffer" );
	_Properties = Properties;
}

////////////////////////////////////////////////////////////////////////////////

CConstantBuffer::CConstantBuffer( ID3D11Device& Device, int32_t SizeInBytes, ECpuAccessPolicy AccessPolicy )
{
	// Setup matrix constant buffer
	D3D11_BUFFER_DESC MatrixBufferDesc;
	MatrixBufferDesc.ByteWidth = SizeInBytes;
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = static_cast<uint32_t>( AccessPolicy );
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;
	HRESULT Result = Device.CreateBuffer( &MatrixBufferDesc, nullptr, _pBuffer.GetAddressOf() );
	ASSERT( SUCCEEDED( Result ), "Failed to create constant buffer" );
	_BufferSizeInBytes = SizeInBytes;
	_AccessPolicy = AccessPolicy;
}

////////////////////////////////////////////////////////////////////////////////

void CRenderContext::Initialize( ID3D11DeviceContext* pDeviceContext )
{
	_pDeviceContext = pDeviceContext;
}

namespace
{
	struct DebugStruct
	{
		CVector3f _Position;
		CVector4f _Color;
	};
};

void CRenderContext::SetVertexBuffer( CVertexBuffer& VertexBuffer )
{
	ID3D11Buffer* pVertexBuffer = VertexBuffer.AccessVertexBuffer();
	ASSERT( pVertexBuffer, "Given vertex buffer is invalid" );
	uint32_t SingleVertexSize = VertexBuffer.GetProperties()._SingleVertexSizeInBytes;
	ASSERT(SingleVertexSize > 0, "Creating vertex buffer with vertices of size 0?" );
	uint32_t Stride = SingleVertexSize;
	uint32_t Offset = 0U;
	_pDeviceContext->IASetVertexBuffers( 0, 1, VertexBuffer.AccessVertexBufferAddr(), &Stride, &Offset );
}

void CRenderContext::SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology )
{
	_pDeviceContext->IASetPrimitiveTopology( PrimitiveTopology );
}

void CRenderContext::SetVertexShaderConstantBuffer( CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index )
{
	ID3D11Buffer* pConstantBuffer = ConstantBuffer.AccessRawBuffer();
	_pDeviceContext->VSSetConstantBuffers( static_cast<uint32_t>( Index ), 1, &pConstantBuffer );
}

void CRenderContext::SetVertexShader( CVertexShader& VertexShader )
{
	_pDeviceContext->IASetInputLayout( VertexShader.AccessInputLayout() );
	_pDeviceContext->VSSetShader( VertexShader.AccessVertexShader(), nullptr, 0 );
}

void CRenderContext::SetPixelShaderConstantBuffer( CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index )
{
	ID3D11Buffer* pConstantBuffer = ConstantBuffer.AccessRawBuffer();
	_pDeviceContext->PSSetConstantBuffers( static_cast<uint32_t>(Index), 1, &pConstantBuffer );
}

void CRenderContext::SetPixelShader( CPixelShader& PixelShader )
{
	_pDeviceContext->PSSetShader( PixelShader.AccessPixelShader(), nullptr, 0 );
}

void CRenderContext::UpdateConstantBuffer( CConstantBuffer& ConstantBuffer, const void* pNewData, int32_t NewDataSize  )
{
	ASSERT( ConstantBuffer.GetSizeInBytes() == NewDataSize, "Size mismatch, investigate." );
	ASSERT( (uint32_t)ConstantBuffer.GetAccessPolicy() | (uint32_t)ECpuAccessPolicy::CpuWrite, "Cannot update contents of constant buffer unless registered with CpuWrite" );
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	ZeroMemory( &MappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ));
	HRESULT Result = _pDeviceContext->Map( ConstantBuffer.AccessRawBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	ASSERT( SUCCEEDED( Result ), "Failed to update constant buffer" );
	memcpy( MappedResource.pData, pNewData, NewDataSize );
	_pDeviceContext->Unmap( ConstantBuffer.AccessRawBuffer(), 0 );
}

void CRenderContext::Draw( int32_t VertexCount )
{
	_pDeviceContext->Draw( VertexCount, 0 );
}

////////////////////////////////////////////////////////////////////////////////

CGraphics::~CGraphics()
{
	Shutdown();
}

bool CGraphics::Initialize(int ScreenWidth, int ScreenHeight, HWND Wnd)
{
	_Direct3D = std::make_unique<CDirectX3D>();
	_ScreenWidthInPix = ScreenWidth;
	_ScreenHeightInPix = ScreenHeight;
	bool Result = _Direct3D->Initialize( ScreenWidth, ScreenHeight, NGraphicsDefines::IsVSyncEnabled,
		Wnd, NGraphicsDefines::IsFullScreen, NGraphicsDefines::ScreenDepth, NGraphicsDefines::ScreenNear );
	if ( !Result )
	{
		MessageBoxW( Wnd, L"Could not initialize Direct3D", L"Error", MB_OK );
		return false;
	}

	_RenderContext.Initialize( _Direct3D->AccessDeviceContext() );

	return true;
}

void CGraphics::Shutdown()
{
	_Direct3D.release();
}

CVertexBuffer CGraphics::CreateVertexBuffer( const void* pVertexData, uint32_t VertexDataSizeInBytes, const SVertexBufferProperties& Settings )
{
	return CVertexBuffer{ AccessDevice(), pVertexData, Settings };
}

CVertexShader CGraphics::CreateVertexShader( const std::string& ShaderFileName, const std::string& ShaderMainFunction, std::vector<SShaderInputDescription>& ShaderInputLayout )
{
	_VertexShaderCache.emplace_back();
	CVertexShader& VertexShader = _VertexShaderCache.back();
	bool Success = VertexShader.Initialize( AccessDevice(), ShaderFileName, ShaderMainFunction, ShaderInputLayout );
	ASSERT( Success, "Faield to create vertex shader" );
	return VertexShader;
}

CPixelShader CGraphics::CreatePixelShader( const std::string& ShaderFileName, const std::string& ShaderMainFunction )
{
	_PixelShaderCache.emplace_back();
	CPixelShader& PixelShader = _PixelShaderCache.back();
	bool Success = PixelShader.Initialize( AccessDevice(), ShaderFileName, ShaderMainFunction );
	ASSERT( Success, "Failed to create pixel shader" );
	return PixelShader;
}

CConstantBuffer CGraphics::CreateConstantBuffer( int32_t SizeInBytes, ECpuAccessPolicy AccessPolicy )
{
	return CConstantBuffer{ AccessDevice(), SizeInBytes, AccessPolicy };
}

CRenderContext& CGraphics::StartRenderFrame( const CVector4f& BackgroundColor )
{
	_Direct3D->BeginScene( BackgroundColor._X, BackgroundColor._Y, BackgroundColor._Z, BackgroundColor._W );
	return _RenderContext;
}

void CGraphics::EndFrame( CRenderContext& /* RenderContext */ )
{
	_Direct3D->EndScene();
}
