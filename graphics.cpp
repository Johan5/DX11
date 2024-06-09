#include "graphics.h"

#include "vector.h"
#include "string_util.h"
#include "logger.h"
#include "assert.h"
#include "stb_image_include.h"
#include "graphics_enums.h"
#include "graphic_resources.h"
#include "mesh_loader.h"

#include <filesystem>



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
	Result = initShaders();
	if (!Result)
	{
		MessageBoxW(Wnd, L"Failed to initialize Shaders", L"Error", MB_OK);
	}

	Result = initTextures();
	if (!Result)
	{
		MessageBoxW(Wnd, L"Failed to initialize Textures", L"Error", MB_OK);
	}

	Result = initMeshes();
	if (!Result)
	{
		MessageBoxW(Wnd, L"Failed to initialize Meshes", L"Error", MB_OK);
	}

	return true;
}

void CGraphics::Shutdown()
{
	_Direct3D.release();
}

CVertexBuffer CGraphics::CreateVertexBuffer(const void* pVertexData, uint32_t VertexDataSizeInBytes, const SVertexBufferProperties& Settings)
{
	return CVertexBuffer{ AccessDevice(), pVertexData, Settings };
}

CIndexBuffer CGraphics::CreateIndexBuffer(uint32_t SizeInBytes, ECpuAccessPolicy AccessPolicy)
{
	return CIndexBuffer(AccessDevice(), SizeInBytes, AccessPolicy);
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

CTexture CGraphics::CreateTexture(const D3D11_TEXTURE2D_DESC& TextureDesc, const D3D11_SUBRESOURCE_DATA& pInitialData)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	HRESULT Result = AccessDevice().CreateTexture2D(&TextureDesc, &pInitialData, pTexture.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create texture resource");
	return CTexture{ pTexture };
}

std::optional<CTexture> CGraphics::GetTextureByName(const std::string& Name)
{
	auto it = _Textures.find(Name);
	return (it == _Textures.end()) ? std::nullopt : std::optional<CTexture>{ it->second };
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

CSamplerState CGraphics::CreateSamplerState(const D3D11_SAMPLER_DESC& SamplerDesc)
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
	HRESULT Result = AccessDevice().CreateSamplerState(&SamplerDesc, pSamplerState.GetAddressOf());
	ASSERT(SUCCEEDED(Result), "Failed to create sampler state");
	return CSamplerState{ pSamplerState };
}

CVertexShader* CGraphics::AccessVertexShader(const std::string& fileName)
{
	auto it = _VertexShaders.find(fileName);
	return (it == _VertexShaders.end() ? nullptr : &it->second);
}

CPixelShader* CGraphics::AccessPixelShader(const std::string& fileName)
{
	auto it = _PixelShaders.find(fileName);
	return (it == _PixelShaders.end() ? nullptr : &it->second);
}

CGeometryShader* CGraphics::AccessGeometryShader(const std::string& fileName)
{
	auto it = _GeometryShaders.find(fileName);
	return (it == _GeometryShaders.end() ? nullptr : &it->second);
}

SMesh* CGraphics::AccessMesh(EMeshType MeshType)
{
	auto it = _Meshes.find(MeshType);
	return (it == _Meshes.end() ? nullptr : &it->second);
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

bool CGraphics::initShaders()
{
	std::filesystem::path shaderFolder = std::filesystem::current_path() / "shaders";
	std::vector<SShaderInputDescription> CubeInputLayout;
	CubeInputLayout.push_back(SShaderInputDescription{ "POSITION", EGfxResourceDataFormat::R32G32B32Float });
	CubeInputLayout.push_back(SShaderInputDescription{ "NORMAL", EGfxResourceDataFormat::R32G32B32Float });
	CubeInputLayout.push_back(SShaderInputDescription{ "TEXCOORD", EGfxResourceDataFormat::R32G32Float });
	CubeInputLayout.push_back(SShaderInputDescription{ "SV_InstanceID", EGfxResourceDataFormat::R32UInt });
	std::optional<CVertexShader> cubeVs = CreateVertexShader((shaderFolder / shader_names::DefaultVertexShaderFileName).string(), shader_names::DefaultVertexShaderMainFunction, CubeInputLayout);
	std::optional<CPixelShader> cubePs = CreatePixelShader((shaderFolder / shader_names::DefaultPixelShaderFileName).string(), shader_names::DefaultPixelShaderMainFunction);
	if (cubeVs && cubePs)
	{
		_VertexShaders.emplace(shader_names::DefaultVertexShaderFileName, cubeVs.value());
		_PixelShaders.emplace(shader_names::DefaultPixelShaderFileName, cubePs.value());
	}
	else
	{
		return false;
	}
	std::optional<CVertexShader> cubeShadowVs = CreateVertexShader((shaderFolder / shader_names::CubeShadowVertexShaderFileName).string(), shader_names::CubeShadowVertexShaderMainFunction, CubeInputLayout);
	std::optional<CPixelShader> cubeShadowPs = CreatePixelShader((shaderFolder / shader_names::CubeShadowPixelShaderFileName).string(), shader_names::CubeShadowPixelShaderMainFunction);
	std::optional<CGeometryShader> cubeShadowGs = CreateGeometryShader((shaderFolder / shader_names::CubeShadowGeometryShaderFileName).string(), shader_names::CubeShadowGeometryShaderMainFunction);
	if (cubeShadowVs && cubeShadowPs && cubeShadowGs)
	{
		_VertexShaders.emplace(shader_names::CubeShadowVertexShaderFileName, cubeShadowVs.value());
		_PixelShaders.emplace(shader_names::CubeShadowPixelShaderFileName, cubeShadowPs.value());
		_GeometryShaders.emplace(shader_names::CubeShadowGeometryShaderFileName, cubeShadowGs.value());
	}
	else
	{
		return false;
	}

	return true;
}

bool CGraphics::initTextures()
{
	
	std::filesystem::path path = std::filesystem::current_path() / "assets" / "bricks.jpg";
	int32_t width;
	int32_t height;
	int32_t channelCount;
	uint8_t* data = stbi_load(path.string().c_str(), &width, &height, &channelCount, STBI_rgb_alpha);
	if (data)
	{
		std::vector<float> fVec;
		for (int32_t i = 0; i < 4 * width * height; ++i)
		{
			fVec.push_back(data[i] / 255.0f);
		}
		D3D11_TEXTURE2D_DESC TextureDesc;
		TextureDesc.Width = width;
		TextureDesc.Height = height;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = static_cast<DXGI_FORMAT>(EGfxResourceDataFormat::R32G32B32A32Float);
		TextureDesc.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
		TextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		TextureDesc.BindFlags = static_cast<uint32_t>(EBindFlag::ShaderResource);
		TextureDesc.CPUAccessFlags = static_cast<uint32_t>(ECpuAccessPolicy::NoAccess);
		TextureDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA Data;
		Data.pSysMem = fVec.data();
		Data.SysMemPitch = 4 * 4 * width;
		Data.SysMemSlicePitch = 0;
		std::string name = path.stem().string();
		_Textures.emplace(name, CreateTexture(TextureDesc, Data));
		stbi_image_free(data);
		return true;
	}
	else
	{
		std::string error = stbi_failure_reason();
		CLogger::LogFormat("Failed to load texture. Path='{}', Error='{}'", path.string(), error);
		return false;
	}
}

bool CGraphics::initMeshes()
{
	_Meshes.emplace(EMeshType::Cube, mesh_loader::LoadMesh(*this, EMeshType::Cube));
	_Meshes.emplace(EMeshType::Sphere, mesh_loader::LoadMesh(*this, EMeshType::Sphere));
	return true;
}

std::optional<CVertexShader> CGraphics::CreateVertexShader(const std::string& ShaderFileName, const std::string& ShaderMainFunction, std::vector<SShaderInputDescription>& ShaderInputLayout)
{
	CVertexShader VertexShader;
	bool Success = VertexShader.Initialize(AccessDevice(), ShaderFileName, ShaderMainFunction, ShaderInputLayout);
	if (Success)
	{
		return VertexShader;
	}
	ASSERT(false, "Faield to create vertex shader");
	return std::nullopt;
}

std::optional<CGeometryShader> CGraphics::CreateGeometryShader(const std::string& ShaderFileName, const std::string& ShaderMainFunction)
{
	CGeometryShader GeometryShader;
	GeometryShader.Initialize(AccessDevice(), ShaderFileName, ShaderMainFunction);
	return GeometryShader;
}

std::optional<CPixelShader> CGraphics::CreatePixelShader(const std::string& ShaderFileName, const std::string& ShaderMainFunction)
{
	CPixelShader PixelShader;
	bool Success = PixelShader.Initialize(AccessDevice(), ShaderFileName, ShaderMainFunction);
	ASSERT(Success, "Failed to create pixel shader");
	return PixelShader;
}