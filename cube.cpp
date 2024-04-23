#include "cube.h"

#include "camera_base.h"
#include "3d_math.h"
#include "pixel_shader.h"
#include "graphics_enums.h"
#include "assert.h"

#include <random>
#include <vector>


namespace
{
	namespace NWorldRender
	{
		const std::string VertexShaderFileName = "cube.fx";
		const std::string PixelShaderFileName = "cube.fx";
		const std::string VertexShaderMainFunction = "CubeVertexShader";
		const std::string PixelShaderMainFunction = "CubePixelShader";
	}

	namespace NShadowMap
	{
		const std::string VertexShaderFileName = "shadow_map.fx";
		const std::string GeometryShaderFileName = "shadow_map.fx";
		const std::string PixelShaderFileName = "shadow_map.fx";
		const std::string VertexShaderMainFunction = "VS";
		const std::string GeometryShaderMainFunction = "GS";
		const std::string PixelShaderMainFunction = "PS";
	}

	// Lazy rgb implementation
	CVector4f GenRandomColor()
	{
		static std::mt19937 Rng{ 0 };
		static std::uniform_real_distribution<float> Distribution(0.0f, 1.0f);
		return CVector4f{ Distribution(Rng), Distribution(Rng), Distribution(Rng), 1.0f };
	}

	std::vector<CCube::SVertex> GenerateCubeModelVertices()
	{
		std::vector<CCube::SVertex> ModelData{ 36 };
		ModelData[0]._Position = CVector3f{ -0.5f, -0.5f, -0.5f }; // LEFT
		ModelData[0]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[0]._Color = GenRandomColor();
		ModelData[1]._Position = CVector3f{ -0.5f, -0.5f, 0.5f };
		ModelData[1]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[1]._Color = ModelData[0]._Color;
		ModelData[2]._Position = CVector3f{ -0.5f, 0.5f, 0.5f };
		ModelData[2]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[2]._Color = ModelData[0]._Color;

		ModelData[3]._Position = CVector3f{ 0.5f, 0.5f, -0.5f }; // BEHIND
		ModelData[3]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[3]._Color = GenRandomColor();
		ModelData[4]._Position = CVector3f{ -0.5f, -0.5f, -0.5f };
		ModelData[4]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[4]._Color = ModelData[3]._Color;
		ModelData[5]._Position = CVector3f{ -0.5f, 0.5f, -0.5f, };
		ModelData[5]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[5]._Color = ModelData[3]._Color;

		ModelData[6]._Position = CVector3f{ 0.5f, -0.5f, 0.5f }; // DOWN
		ModelData[6]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[6]._Color = GenRandomColor();
		ModelData[7]._Position = CVector3f{ -0.5f, -0.5f, -0.5f };
		ModelData[7]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[7]._Color = ModelData[6]._Color;
		ModelData[8]._Position = CVector3f{ 0.5f, -0.5f, -0.5f };
		ModelData[8]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[8]._Color = ModelData[6]._Color;

		ModelData[9]._Position = CVector3f{ 0.5f, 0.5f, -0.5f }; // BEHIND
		ModelData[9]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[9]._Color = GenRandomColor();
		ModelData[10]._Position = CVector3f{ 0.5f, -0.5f, -0.5f };
		ModelData[10]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[10]._Color = ModelData[9]._Color;
		ModelData[11]._Position = CVector3f{ -0.5f, -0.5f, -0.5f };
		ModelData[11]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[11]._Color = ModelData[9]._Color;

		ModelData[12]._Position = CVector3f{ -0.5f, -0.5f, -0.5f }; // LEFT
		ModelData[12]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[12]._Color = GenRandomColor();
		ModelData[13]._Position = CVector3f{ -0.5f, 0.5f, 0.5f };
		ModelData[13]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[13]._Color = ModelData[12]._Color;
		ModelData[14]._Position = CVector3f{ -0.5f, 0.5f, -0.5f };
		ModelData[14]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[14]._Color = ModelData[12]._Color;

		ModelData[15]._Position = CVector3f{ 0.5f, -0.5f, 0.5f }; // DOWN
		ModelData[15]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[15]._Color = GenRandomColor();
		ModelData[16]._Position = CVector3f{ -0.5f, -0.5f, 0.5f };
		ModelData[16]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[16]._Color = ModelData[15]._Color;
		ModelData[17]._Position = CVector3f{ -0.5f, -0.5f, -0.5f };
		ModelData[17]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[17]._Color = ModelData[15]._Color;

		ModelData[18]._Position = CVector3f{ -0.5f, 0.5f, 0.5f }; // IN FRONT
		ModelData[18]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[18]._Color = GenRandomColor();
		ModelData[19]._Position = CVector3f{ -0.5f, -0.5f, 0.5f };
		ModelData[19]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[19]._Color = ModelData[18]._Color;
		ModelData[20]._Position = CVector3f{ 0.5f, -0.5f, 0.5f };
		ModelData[20]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[20]._Color = ModelData[18]._Color;

		ModelData[21]._Position = CVector3f{ 0.5f, 0.5f, 0.5f }; // RIGHT
		ModelData[21]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[21]._Color = GenRandomColor();
		ModelData[22]._Position = CVector3f{ 0.5f, -0.5f, -0.5f };
		ModelData[22]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[22]._Color = ModelData[21]._Color;
		ModelData[23]._Position = CVector3f{ 0.5f, 0.5f, -0.5f };
		ModelData[23]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[23]._Color = ModelData[21]._Color;

		ModelData[24]._Position = CVector3f{ 0.5f, -0.5f, -0.5f }; // RIGHT
		ModelData[24]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[24]._Color = GenRandomColor();
		ModelData[25]._Position = CVector3f{ 0.5f, 0.5f, 0.5f };
		ModelData[25]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[25]._Color = ModelData[24]._Color;
		ModelData[26]._Position = CVector3f{ 0.5f, -0.5f, 0.5f };
		ModelData[26]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[26]._Color = ModelData[24]._Color;

		ModelData[27]._Position = CVector3f{ 0.5f, 0.5f, 0.5f }; // UP
		ModelData[27]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[27]._Color = GenRandomColor();
		ModelData[28]._Position = CVector3f{ 0.5f, 0.5f, -0.5f };
		ModelData[28]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[28]._Color = ModelData[27]._Color;
		ModelData[29]._Position = CVector3f{ -0.5f, 0.5f, -0.5f };
		ModelData[29]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[29]._Color = ModelData[27]._Color;

		ModelData[30]._Position = CVector3f{ 0.5f, 0.5f, 0.5f }; // UP
		ModelData[30]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[30]._Color = GenRandomColor();
		ModelData[31]._Position = CVector3f{ -0.5f, 0.5f, -0.5f };
		ModelData[31]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[31]._Color = ModelData[30]._Color;
		ModelData[32]._Position = CVector3f{ -0.5f, 0.5f, 0.5f };
		ModelData[32]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[32]._Color = ModelData[30]._Color;

		ModelData[33]._Position = CVector3f{ 0.5f, 0.5f, 0.5f }; // IN FRONT
		ModelData[33]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[33]._Color = GenRandomColor();
		ModelData[34]._Position = CVector3f{ -0.5f, 0.5f, 0.5f };
		ModelData[34]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[34]._Color = ModelData[33]._Color;
		ModelData[35]._Position = CVector3f{ 0.5f, -0.5f, 0.5f };
		ModelData[35]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[35]._Color = ModelData[33]._Color;

		return ModelData;
	}
}

CCube::~CCube()
{
	CCube::Shutdown();
}

void CCube::Initialize( CGraphics& Graphics )
{
	std::lock_guard<std::mutex> RenderLock(_RenderLock);

	_Vertices = GenerateCubeModelVertices();
	
	uint32_t VertexBufferSize = static_cast<uint32_t>( _Vertices.size() * sizeof( SVertex ) );
	SVertexBufferProperties Properties;
	Properties._VertexDataSizeInBytes = VertexBufferSize;
	Properties._SingleVertexSizeInBytes = sizeof( SVertex );
	_VertexBuffer = Graphics.CreateVertexBuffer( _Vertices.data(), VertexBufferSize, Properties );
	
	_ConstantBuffer = Graphics.CreateConstantBuffer( sizeof(STypicalConstantBuffer), ECpuAccessPolicy::CpuWrite );
	_ShadowConstantBuffer = Graphics.CreateConstantBuffer(sizeof(STypicalShadowConstantBuffer), ECpuAccessPolicy::CpuWrite);

	std::vector<SShaderInputDescription> InputLayout;
	InputLayout.push_back( SShaderInputDescription{ "POSITION", EGfxResourceDataFormat::R32G32B32Float } );
	InputLayout.push_back( SShaderInputDescription{ "NORMAL", EGfxResourceDataFormat::R32G32B32Float } );
	InputLayout.push_back( SShaderInputDescription{ "COLOR", EGfxResourceDataFormat::R32G32B32A32Float } );
	_VertexShader = Graphics.CreateVertexShader(NWorldRender::VertexShaderFileName, NWorldRender::VertexShaderMainFunction, InputLayout );
	_PixelShader = Graphics.CreatePixelShader(NWorldRender::PixelShaderFileName, NWorldRender::PixelShaderMainFunction );

	_ShadowVertexShader = Graphics.CreateVertexShader(NShadowMap::VertexShaderFileName, NShadowMap::VertexShaderMainFunction, InputLayout);
	_ShadowGeometryShader = Graphics.CreateGeometryShader(NShadowMap::GeometryShaderFileName, NShadowMap::GeometryShaderMainFunction);
	_ShadowPixelShader = Graphics.CreatePixelShader(NShadowMap::PixelShaderFileName, NShadowMap::PixelShaderMainFunction);

	_IsInitialized = true;
}

void CCube::Shutdown()
{
	// TODO cleanup vertex buffer?
	_IsInitialized = false;
}

bool CCube::IsInitialized() const
{
	return _IsInitialized;
}

void CCube::Render( CRenderContext& RenderContext, const CCameraBase& Camera )
{
	if (!ShouldRender() || !IsInitialized())
	{
		return;
	}
	std::lock_guard<std::mutex> RenderLock(_RenderLock);
	ASSERT( _Vertices.size() > 0, "Trying to render 0 vertices?" );
	if (_NeedVertexBufferUpdate)
	{
		RenderContext.UpdateVertexBuffer(_VertexBuffer, _Vertices.data(),_Vertices.size() * sizeof(SVertex));
		_NeedVertexBufferUpdate = false;
	}
	RenderContext.SetVertexBuffer( _VertexBuffer );
	{
		STypicalConstantBuffer CB;
		CB._ModelToWorld = GetLocalToWorldTransform();
		CB._NormalModelToWorld = GetNormalLocalToWorldTransform();
		RenderContext.UpdateConstantBuffer(_ConstantBuffer, &CB, sizeof(STypicalConstantBuffer) );
	}
	RenderContext.SetVertexShaderConstantBuffer( _ConstantBuffer, EConstantBufferIdx::PerObject );
	RenderContext.SetPixelShaderConstantBuffer(_ConstantBuffer, EConstantBufferIdx::PerObject);
	RenderContext.SetVertexShader( _VertexShader );
	RenderContext.SetPixelShader( _PixelShader );
	RenderContext.Draw( static_cast<int32_t>( _Vertices.size() ) );
}

void CCube::RenderShadows(CRenderContext& RenderContext)
{
	if (!ShouldRenderShadows() || !IsInitialized())
	{
		return;
	}
	std::lock_guard<std::mutex> RenderLock(_RenderLock);
	ASSERT(_Vertices.size() > 0, "Trying to render 0 vertices?");
	if (_NeedVertexBufferUpdate)
	{
		RenderContext.UpdateVertexBuffer(_VertexBuffer, _Vertices.data(), _Vertices.size() * sizeof(SVertex));
		_NeedVertexBufferUpdate = false;
	}
	RenderContext.SetVertexBuffer(_VertexBuffer);
	{
		STypicalShadowConstantBuffer CB;
		CB._ModelToWorld = GetLocalToWorldTransform();
		RenderContext.UpdateConstantBuffer(_ShadowConstantBuffer, &CB, sizeof(STypicalShadowConstantBuffer));
	}
	RenderContext.SetVertexShaderConstantBuffer(_ShadowConstantBuffer, EConstantBufferIdx::PerObject);
	RenderContext.SetPixelShaderConstantBuffer(_ShadowConstantBuffer, EConstantBufferIdx::PerObject);
	RenderContext.SetVertexShader(_ShadowVertexShader);
	RenderContext.SetGeometryShader(_ShadowGeometryShader);
	RenderContext.SetPixelShader(_ShadowPixelShader);
	RenderContext.Draw(static_cast<int32_t>(_Vertices.size()));
}

bool CCube::ShouldRenderShadows()
{
	return _ShouldRenderShadow;
}

void CCube::DisableShadowRendering()
{
	_ShouldRenderShadow = false;
}

void CCube::SetColor(const CVector4f& NewColor)
{
	std::lock_guard<std::mutex> RenderLock(_RenderLock);
	for (SVertex& Vertex : _Vertices)
	{
		Vertex._Color = NewColor;
	}
	_NeedVertexBufferUpdate = true;

}