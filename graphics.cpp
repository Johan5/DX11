#include "graphics.h"

#include "vector.h"
#include "string_util.h"
#include "logger.h"
#include "assert.h"
#include "graphics_enums.h"


namespace
{
	void UpdateDx11Buffer(ID3D11DeviceContext& DeviceContext, ID3D11Buffer* pBuffer, const void* pNewData, size_t NewDataSize)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		ZeroMemory(&MappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		HRESULT Result = DeviceContext.Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		ASSERT(SUCCEEDED(Result), "Failed to update buffer");
		memcpy(MappedResource.pData, pNewData, NewDataSize);
		DeviceContext.Unmap(pBuffer, 0);
	}
}


////////////////////////////////////////////////////////////////////////////////

CVertexBuffer::CVertexBuffer( ID3D11Device& Device, const void* pVertexData, const SVertexBufferProperties& Properties )
{
	ASSERT(Properties._VertexDataSizeInBytes > 0, "Vertex data size needs to be positive" );
	ASSERT(Properties._SingleVertexSizeInBytes > 0, "Single vertex size needs to be positive" );
	D3D11_BUFFER_DESC VertexBufferDesc;
	ZeroMemory( &VertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );
	VertexBufferDesc.ByteWidth = Properties._VertexDataSizeInBytes;
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.BindFlags = static_cast<uint32_t>(EBindFlag::VertexBuffer);
	VertexBufferDesc.CPUAccessFlags = static_cast<uint32_t>(ECpuAccessPolicy::CpuWrite); // TODO: Expose this to caller instead?
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

void CRenderContext::Initialize(CDirectX3D* pDirectX3D)
{
	_pDirectX3D = pDirectX3D;
}

namespace
{
	struct DebugStruct
	{
		CVector3f _Position;
		CVector4f _Color;
	};
};

void CRenderContext::SetVertexBuffer(CVertexBuffer& VertexBuffer)
{
	ID3D11Buffer* pVertexBuffer = VertexBuffer.AccessVertexBuffer();
	ASSERT(pVertexBuffer, "Given vertex buffer is invalid");
	uint32_t SingleVertexSize = VertexBuffer.GetProperties()._SingleVertexSizeInBytes;
	ASSERT(SingleVertexSize > 0, "Creating vertex buffer with vertices of size 0?");
	uint32_t Stride = SingleVertexSize;
	uint32_t Offset = 0U;
	_pDirectX3D->AccessDeviceContext()->IASetVertexBuffers(0, 1, VertexBuffer.AccessVertexBufferAddr(), &Stride, &Offset);
}

void CRenderContext::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology)
{
	_pDirectX3D->AccessDeviceContext()->IASetPrimitiveTopology(PrimitiveTopology);
}

void CRenderContext::SetVertexShaderConstantBuffer(CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index)
{
	ID3D11Buffer* pConstantBuffer = ConstantBuffer.AccessRawBuffer();
	_pDirectX3D->AccessDeviceContext()->VSSetConstantBuffers(static_cast<uint32_t>(Index), 1, &pConstantBuffer);
}

void CRenderContext::SetVertexShader(CVertexShader& VertexShader)
{
	_pDirectX3D->AccessDeviceContext()->IASetInputLayout(VertexShader.AccessInputLayout());
	_pDirectX3D->AccessDeviceContext()->VSSetShader(VertexShader.AccessVertexShader(), nullptr, 0);
}

void CRenderContext::SetPixelShaderConstantBuffer(CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index)
{
	ID3D11Buffer* pConstantBuffer = ConstantBuffer.AccessRawBuffer();
	_pDirectX3D->AccessDeviceContext()->PSSetConstantBuffers(static_cast<uint32_t>(Index), 1, &pConstantBuffer);
}

void CRenderContext::SetPixelShader(CPixelShader& PixelShader)
{
	_pDirectX3D->AccessDeviceContext()->PSSetShader(PixelShader.AccessPixelShader(), nullptr, 0);
}

void CRenderContext::SetGeometryShader(CGeometryShader& GeometryShader)
{
	_pDirectX3D->AccessDeviceContext()->GSSetShader(GeometryShader.AccessGeometryShader(), nullptr, 0);
}

void CRenderContext::SetGeometryShaderConstantBuffer(CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index)
{
	ID3D11Buffer* pConstantBuffer = ConstantBuffer.AccessRawBuffer();
	_pDirectX3D->AccessDeviceContext()->GSSetConstantBuffers(static_cast<uint32_t>(Index), 1, &pConstantBuffer);
}

void CRenderContext::SetPixelShaderTexture(CTextureView& TextureView, int32_t SlotIdx)
{
	ID3D11ShaderResourceView** ppTextureView = TextureView.AccessAddrOfTextureView();
	_pDirectX3D->AccessDeviceContext()->PSSetShaderResources(SlotIdx, 1, ppTextureView);
}

void CRenderContext::SetPixelShaderSampler(CSamplerState& SamplerState, int32_t SlotIdx)
{
	ID3D11SamplerState** ppSamplerState = SamplerState.AccessAddrOfSamplerState();
	_pDirectX3D->AccessDeviceContext()->PSSetSamplers(SlotIdx, 1, ppSamplerState);
}

void CRenderContext::SetRenderTarget(CRenderTargetView& RenderTarget)
{
	_pDirectX3D->AccessDeviceContext()->OMSetRenderTargets(1, RenderTarget.AccessAddrOfRenderTargetView(), _pDirectX3D->AccessDepthStencilView());
}

void CRenderContext::SetRenderTargets(int32_t NumTargets, ID3D11RenderTargetView** ppRTVs, ID3D11DepthStencilView* pDSVs)
{
	_pDirectX3D->AccessDeviceContext()->OMSetRenderTargets(NumTargets, ppRTVs, pDSVs);
}

void CRenderContext::SetViewport(CVector2f NewSize)
{
	D3D11_VIEWPORT Viewport;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;
	Viewport.Width = NewSize._X;
	Viewport.Height = NewSize._Y;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	_pDirectX3D->AccessDeviceContext()->RSSetViewports(1, &Viewport);
}

void CRenderContext::ClearShaders()
{
	_pDirectX3D->AccessDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	_pDirectX3D->AccessDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	_pDirectX3D->AccessDeviceContext()->PSSetShader(nullptr, nullptr, 0);
}

void CRenderContext::ClearTextureSlot(int32_t SlotIdx)
{
	ID3D11ShaderResourceView* nullView[] = { nullptr };
	_pDirectX3D->AccessDeviceContext()->PSSetShaderResources(SlotIdx, 1, nullView);
}

void CRenderContext::RestoreRenderTarget()
{
	_pDirectX3D->AccessDeviceContext()->OMSetRenderTargets(1, _pDirectX3D->AccessAddrOfRenderTargetView(), _pDirectX3D->AccessDepthStencilView());
}

void CRenderContext::RestoreViewport()
{
	D3D11_VIEWPORT Viewport = _pDirectX3D->GetViewport();
	_pDirectX3D->AccessDeviceContext()->RSSetViewports(1, &Viewport);
}

void CRenderContext::UpdateVertexBuffer(CVertexBuffer& VertexBuffer, const void* pNewData, size_t NewDataSize)
{
	ASSERT(VertexBuffer.GetProperties()._VertexDataSizeInBytes == NewDataSize, "Size mismatch");
	UpdateDx11Buffer(*_pDirectX3D->AccessDeviceContext(), VertexBuffer.AccessVertexBuffer(), pNewData, NewDataSize);
}

void CRenderContext::UpdateConstantBuffer(CConstantBuffer& ConstantBuffer, const void* pNewData, size_t NewDataSize)
{
	ASSERT(ConstantBuffer.GetSizeInBytes() == NewDataSize, "Size mismatch, investigate.");
	ASSERT((uint32_t)ConstantBuffer.GetAccessPolicy() | (uint32_t)ECpuAccessPolicy::CpuWrite, "Cannot update contents of constant buffer unless registered with CpuWrite");
	UpdateDx11Buffer(*_pDirectX3D->AccessDeviceContext(), ConstantBuffer.AccessRawBuffer(), pNewData, NewDataSize);
}

void CRenderContext::Draw(int32_t VertexCount)
{
	_pDirectX3D->AccessDeviceContext()->Draw( VertexCount, 0 );
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

	_RenderContext.Initialize( _Direct3D.get() );

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

CGeometryShader CGraphics::CreateGeometryShader(const std::string& ShaderFileName, const std::string& ShaderMainFunction)
{
	_GeometryShaderCache.emplace_back();
	CGeometryShader& GeometryShader = _GeometryShaderCache.back();
	GeometryShader.Initialize(AccessDevice(), ShaderFileName, ShaderMainFunction);
	return GeometryShader;
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

CTexture CGraphics::CreateTexture(uint32_t Width, uint32_t Height, EGfxResourceDataFormat Format, uint32_t BindFlags, ECpuAccessPolicy AccessPolicy)
{
	D3D11_TEXTURE2D_DESC TextureDesc;
	TextureDesc.Width = Width;
	TextureDesc.Height = Height;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = static_cast<DXGI_FORMAT>(Format);
	TextureDesc.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = BindFlags;
	TextureDesc.CPUAccessFlags = static_cast<uint32_t>(AccessPolicy);
	TextureDesc.MiscFlags = 0;
	return CreateTexture(TextureDesc);
}

CTexture CGraphics::CreateTexture(const D3D11_TEXTURE2D_DESC& TextureDesc)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	HRESULT Result = AccessDevice().CreateTexture2D(&TextureDesc, nullptr, pTexture.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create texture resource");
	return CTexture{ pTexture };
}

CTextureView CGraphics::CreateTextureView(CTexture& Texture, const D3D11_SHADER_RESOURCE_VIEW_DESC& Desc)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	HRESULT Result = AccessDevice().CreateShaderResourceView(Texture.AccessTexture(), &Desc, pTextureView.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create shader resource view");
	return CTextureView{ pTextureView };
}

CRenderTargetView CGraphics::CreateRenderTargetView(CTexture& Texture, const D3D11_RENDER_TARGET_VIEW_DESC& Desc)
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	HRESULT Result = AccessDevice().CreateRenderTargetView(Texture.AccessTexture(), &Desc, pRenderTargetView.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create render target view");
	return CRenderTargetView{ pRenderTargetView };
}

CDepthStencilView CGraphics::CreateDepthStencilView(CTexture& Texture, const D3D11_DEPTH_STENCIL_VIEW_DESC& Desc)
{
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	HRESULT Result = AccessDevice().CreateDepthStencilView(Texture.AccessTexture(), &Desc, pDepthStencilView.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create render target view");
	return CDepthStencilView{ pDepthStencilView };
}

CSamplerState CGraphics::CreateSamplerState()
{
	return CSamplerState{ AccessDevice() };
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
