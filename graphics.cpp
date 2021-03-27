#include "graphics.h"

#include "vector.h"
#include "string_util.h"
#include "logger.h"

struct STest
{
	CVector3f _Pos;
	CVector4f _Clr;
};


CVertexBuffer::CVertexBuffer( ID3D11Device& Device, const void* pVertexData, const SVertexBufferProperties& Properties )
{
	assert(Properties._VertexDataSizeInBytes > 0);
	assert(Properties._SingleVertexSizeInBytes > 0);
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
	assert( SUCCEEDED( VertexResult ) );
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
	if ( FAILED( Result ) )
	{
		assert( false );
	}
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
	assert( pVertexBuffer );
	uint32_t SingleVertexSize = VertexBuffer.GetProperties()._SingleVertexSizeInBytes;
	assert(SingleVertexSize > 0);
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

void CRenderContext::UpdateConstantBuffer( CConstantBuffer& ConstantBuffer, const void* pNewData, int32_t NewDataSize  )
{
	assert( ConstantBuffer.GetSizeInBytes() == NewDataSize );
	assert( (uint32_t)ConstantBuffer.GetAccessPolicy() | (uint32_t)ECpuAccessPolicy::CpuWrite );
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	ZeroMemory( &MappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ));
	ID3D11Buffer* RawBfr = ConstantBuffer.AccessRawBuffer();
	D3D11_BUFFER_DESC Desc;
	RawBfr->GetDesc( &Desc );
	HRESULT Result = _pDeviceContext->Map( ConstantBuffer.AccessRawBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( Result ) )
	{
		assert( false );
	}
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
		MessageBox( Wnd, L"Could not initialize Direct3D", L"Error", MB_OK );
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
	// Add caching here?
	return CVertexBuffer{ AccessDevice(), pVertexData, Settings };
}

CVertexShader CGraphics::CreateVertexShader( const std::string& ShaderFileName, const std::string& ShaderMainFunction )
{
	_VertexShaderCache.emplace_back();
	CVertexShader& VertexShader = _VertexShaderCache.back();
	VertexShader.Initialize( AccessDevice(), ShaderFileName, ShaderMainFunction );
	return VertexShader;
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

////bool CGraphics::Render( double TimeElapsedMs )
////{
//	// Set to black
//	_Direct3D->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );
//
//	{
//		// static float TotElapsedTimeSec = 0;
//		// TotElapsedTimeSec += static_cast<float>( TimeElapsedMs ) / 1000.0f;
//	//	//
//	//	// float CameraMovementSpeed = 0.5f;
//	//	// float XOffset = 0.5f * cosf( CameraMovementSpeed * TotElapsedTimeSec );
//	//	// float YOffset = 0.5f * sinf( CameraMovementSpeed * TotElapsedTimeSec );
//	//	// CVector3f NewPos = CVector3f{ XOffset, YOffset, 0.0f };
//	//	// _Camera->SetPosition( NewPos );
//	//}
//
//	
//	/*CMatrix4x4f ViewAndProjectionMatrix = _Camera->GetViewAndProjection();
//	for ( const std::unique_ptr<CGameObject>& pModel : _Models )
//	{
//		const CMatrix4x4f& LocalToWorld = pModel->GetLocalToWorldTransform();
//		pModel->PrepForRender( _Direct3D->AccessDeviceContext() );
//		_ColorShader->Render( _Direct3D->AccessDeviceContext(), pModel->GetNumVertices(), LocalToWorld, ViewAndProjectionMatrix );
//	}*/
//	
////	_Direct3D->EndScene();
////	return true;
////}
