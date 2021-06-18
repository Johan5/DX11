#pragma once

#include "d3dclass.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "constant_buffer.h"
#include "texture.h"
#include "render_target.h"
#include "geometry_shader.h"
#include "texture_view.h"
#include "sampler_state.h"

#include <string>
#include <memory>
#include <vector>
#include <wrl/client.h> // Microsoft ComPtr

class CVertexShader;

namespace NGraphicsDefines
{
	const bool IsFullScreen = false;
	const bool IsVSyncEnabled = true;
	const float ScreenDepth = 1000.0f;
	const float ScreenNear = 1.0f;
	const float ScreenFar = 100.0f;
}

////////////////////////////////////////////////////////////////////////////////

// Colors might be replaced if object has a per-vertex coloring or texture
struct SMaterial
{
	float _DiffuseStrength = 1.0f;
	float _SpecularStrength = 1.0f;
	float _AmbientStrength = 0.15f;
	// Large specular power means small specular highlight
	int32_t _SpecularPower = 256;
};

////////////////////////////////////////////////////////////////////////////////

struct SVertexBufferProperties
{
	uint32_t _VertexDataSizeInBytes = 0;
	uint32_t _SingleVertexSizeInBytes = 0;
};

class CVertexBuffer
{
	friend class CGraphics; // Only allow CGraphics to create
public:
	CVertexBuffer() = default;

	ID3D11Buffer* AccessVertexBuffer() { return _pVertexBuffer.Get(); }
	ID3D11Buffer** AccessVertexBufferAddr() { return _pVertexBuffer.GetAddressOf(); }
	const SVertexBufferProperties& GetProperties() const { return _Properties; }

private:
	CVertexBuffer( ID3D11Device& Device, const void* pVertexData, const SVertexBufferProperties& Properties );

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pVertexBuffer;
	SVertexBufferProperties _Properties;
};

////////////////////////////////////////////////////////////////////////////////

// The render context is the basic rendering API used by gameplay classes
class CRenderContext
{
public:
	void Initialize(CDirectX3D* pDirectX3D);

	void SetVertexBuffer( CVertexBuffer& VertexBuffer );
	void SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology );
	void SetVertexShaderConstantBuffer( CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index );
	void SetVertexShader( CVertexShader& VertexShader );
	void SetPixelShaderConstantBuffer( CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index );
	void SetPixelShader( CPixelShader& PixelShader );
	void SetGeometryShader(CGeometryShader& GeometryShader);
	void SetGeometryShaderConstantBuffer(CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index);

	void SetPixelShaderTexture(CTextureView& TextureView, int32_t SlotIdx);

	void SetPixelShaderSampler(CSamplerState& SamplerState, int32_t SlotIdx);

	void SetRenderTarget(CRenderTarget& RenderTarget);
	void SetRenderTargets(int32_t NumTargets, ID3D11RenderTargetView** ppRTVs, ID3D11DepthStencilView* pDSVs);
	void SetViewport(CVector2f NewSize);

	// Clears registered shaders
	void ClearShaders();
	void ClearTextureSlot(int32_t SlotIdx);
	// Restores render target to the initial backbuffer
	void RestoreRenderTarget();
	void RestoreViewport();
	void UpdateVertexBuffer(CVertexBuffer& VertexBuffer, const void* pNewData, int32_t NewDataSize);
	// optimally, ConstantBuffer data should be 16 byte aligned
	void UpdateConstantBuffer( CConstantBuffer& ConstantBuffer, const void* NewData, int32_t NewDataSize );
	void Draw( int32_t VertexCount );

	// TODO: Removes this
	CDirectX3D& Debug_AccessDxRaw() { return *_pDirectX3D; }
private:
	CDirectX3D* _pDirectX3D;
};

////////////////////////////////////////////////////////////////////////////////

// The CGraphics class' primary usage is creating (and owning) various graphics related resources,
// such as buffers and shaders.
class CGraphics
{
public:
	CGraphics() = default;
	CGraphics( const CGraphics& Other ) = delete;
	CGraphics( const CGraphics&& Other ) = delete;
	~CGraphics();
	CGraphics& operator=( const CGraphics& Other ) = delete;
	CGraphics& operator=( const CGraphics&& Other ) = delete;

	bool Initialize( int ScreenWidth, int ScreenHeight, HWND Wnd );
	void Shutdown();

	CVertexBuffer CreateVertexBuffer( const void* pVertexData, uint32_t VertexDataSizeInBytes, const SVertexBufferProperties& Settings );
	CVertexShader CreateVertexShader( const std::string& ShaderFileName, const std::string& ShaderMainFunction, std::vector<SShaderInputDescription>& ShaderInputLayout );
	CGeometryShader CreateGeometryShader(const std::string& ShaderFileName, const std::string& ShaderMainFunction);
	CPixelShader CreatePixelShader( const std::string& ShaderFileName, const std::string& ShaderMainFunction );

	CConstantBuffer CreateConstantBuffer( int32_t SizeInBytes, ECpuAccessPolicy AccessPolicy );
	CTexture CreateTextureResource(uint32_t Width, uint32_t Height, EGfxResourceDataFormat Format, uint32_t BindFlags, ECpuAccessPolicy AccessPolicy);
	CRenderTarget CreateRenderTarget(uint32_t Width, uint32_t Height, EGfxResourceDataFormat Format);

	CSamplerState CreateSamplerState();

	int GetScreenWidth() const { return _ScreenWidthInPix; }
	int GetScreenHeight() const { return _ScreenHeightInPix; }

	CRenderContext& StartRenderFrame( const CVector4f& BackgroundColor );
	void EndFrame( CRenderContext& RenderContext );
	CRenderContext& AccessRenderContest() { return _RenderContext; }
	
	ID3D11Device& AccessDevice() { return *_Direct3D->AccessDevice(); }

private:
	Microsoft::WRL::ComPtr<ID3D10Blob> CompileVertexShader( const std::string& FileName, const std::string& ShaderMainFunction ) const;

private:
	std::unique_ptr<CDirectX3D> _Direct3D;
	//std::unique_ptr<CColorShader> _ColorShader;

	CRenderContext _RenderContext;

	int32_t _ScreenWidthInPix;
	int32_t _ScreenHeightInPix;

	std::vector<CVertexShader> _VertexShaderCache;
	std::vector<CGeometryShader> _GeometryShaderCache;
	std::vector<CPixelShader> _PixelShaderCache;
};
