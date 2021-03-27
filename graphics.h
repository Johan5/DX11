#pragma once

#include "d3dclass.h"
#include "orthographic_camera.h"
#include "frame_timer.h"
#include "vertex_shader.h"

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
	const float ScreenFar = 10.0f;
}



enum class ECpuAccessPolicy : uint32_t
{
	NoAccess = 0,
	CpuWrite = D3D11_CPU_ACCESS_WRITE,
	CpuRead = D3D11_CPU_ACCESS_READ,
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

// This is the index that will be used when registering a buffer with the device context
enum class EConstantBufferIdx
{
	PerFrame,
	PerCamera,
	PerObject
};

class CConstantBuffer
{
	friend class CGraphics; // Only allow CGraphics to create
public:
	CConstantBuffer() = default;

	ID3D11Buffer* AccessRawBuffer() { return _pBuffer.Get(); }
	int32_t GetSizeInBytes() const { return _BufferSizeInBytes; }
	ECpuAccessPolicy GetAccessPolicy() const { return _AccessPolicy; }

private:
	CConstantBuffer( ID3D11Device& Device, int32_t SizeInBytes, ECpuAccessPolicy AccessPolicy );

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;
	int32_t _BufferSizeInBytes = 0;
	ECpuAccessPolicy _AccessPolicy;
};

////////////////////////////////////////////////////////////////////////////////

class CRenderContext
{
public:
	void Initialize( ID3D11DeviceContext* pDeviceContext );

	void SetVertexBuffer( CVertexBuffer& VertexBuffer );
	void SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology );
	void SetVertexShaderConstantBuffer( CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index );
	void SetVertexShader( CVertexShader& VertexShader );
	// optimally, ConstantBuffer data should be 16 byte aligned
	void UpdateConstantBuffer( CConstantBuffer& ConstantBuffer, const void* NewData, int32_t NewDataSize );
	void Draw( int32_t VertexCount );
private:
	ID3D11DeviceContext* _pDeviceContext;
};

////////////////////////////////////////////////////////////////////////////////

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
	CVertexShader CreateVertexShader( const std::string& ShaderFileName, const std::string& ShaderMainFunction );

	CConstantBuffer CreateConstantBuffer( int32_t SizeInBytes, ECpuAccessPolicy AccessPolicy );

	int GetScreenWidth() const { return _ScreenWidthInPix; }
	int GetScreenHeight() const { return _ScreenHeightInPix; }

	CRenderContext& StartRenderFrame( const CVector4f& BackgroundColor );
	void EndFrame( CRenderContext& RenderContext );
	
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
};
