#include "cube.h"

#include "camera_base.h"
#include "3d_math.h"
#include "pixel_shader.h"
#include "graphics_enums.h"
#include "assert.h"

#include <vector>


namespace
{
	const std::string VertexShaderFileName = "cube.vs";
	const std::string PixelShaderFileName = "cube.ps";
	const std::string VertexShaderMainFunction = "CubeVertexShader";
	const std::string PixelShaderMainFunction = "CubePixelShader";

	std::vector<CCube::SVertex> GenerateCubeModelVertices()
	{
		std::vector<CCube::SVertex> ModelData{ 36 };
		ModelData[0]._Position = CVector3f{ -1.0f, -1.0f, -1.0f }; // LEFT
		ModelData[0]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[0]._Color = CVector4f{ 1.0f, 0.0f, 0.0f, 1.0f };
		ModelData[1]._Position = CVector3f{ -1.0f, -1.0f, 1.0f };
		ModelData[1]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[1]._Color = CVector4f{ 1.0f, 0.0f, 0.0f, 1.0f };
		ModelData[2]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[2]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[2]._Color = CVector4f{ 1.0f, 0.0f, 0.0f, 1.0f };

		ModelData[3]._Position = CVector3f{ 1.0f, 1.0f, -1.0f }; // BEHIND
		ModelData[3]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[3]._Color = CVector4f{ 1.0f, 1.0f, 0.0f, 1.0f };
		ModelData[4]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[4]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[4]._Color = CVector4f{ 1.0f, 1.0f, 0.0f, 1.0f };
		ModelData[5]._Position = CVector3f{ -1.0f, 1.0f, -1.0f, };
		ModelData[5]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[5]._Color = CVector4f{ 1.0f, 1.0f, 0.0f, 1.0f };

		ModelData[6]._Position = CVector3f{ 1.0f, -1.0f, 1.0f }; // DOWN
		ModelData[6]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[6]._Color = CVector4f{ 0.0f, 0.0f, 1.0f, 1.0f };
		ModelData[7]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[7]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[7]._Color = CVector4f{ 0.0f, 0.0f, 1.0f, 1.0f };
		ModelData[8]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[8]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[8]._Color = CVector4f{ 0.0f, 0.0f, 1.0f, 1.0f };

		ModelData[9]._Position = CVector3f{ 1.0f, 1.0f, -1.0f }; // BEHIND
		ModelData[9]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[9]._Color = CVector4f{ 0.0f, 1.0f, 1.0f, 1.0f };
		ModelData[10]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[10]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[10]._Color = CVector4f{ 0.0f, 1.0f, 1.0f, 1.0f };
		ModelData[11]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[11]._Normal = CVector3f{ 0.0f, 0.0f, -1.0f };
		ModelData[11]._Color = CVector4f{ 0.0f, 1.0f, 1.0f, 1.0f };

		ModelData[12]._Position = CVector3f{ -1.0f, -1.0f, -1.0f }; // LEFT
		ModelData[12]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[12]._Color = CVector4f{ 1.0f, 0.0f, 1.0f, 1.0f };
		ModelData[13]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[13]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[13]._Color = CVector4f{ 1.0f, 0.0f, 1.0f, 1.0f };
		ModelData[14]._Position = CVector3f{ -1.0f, 1.0f, -1.0f };
		ModelData[14]._Normal = CVector3f{ -1.0f, 0.0f, 0.0f };
		ModelData[14]._Color = CVector4f{ 1.0f, 0.0f, 1.0f, 1.0f };

		ModelData[15]._Position = CVector3f{ 1.0f, -1.0f, 1.0f }; // DOWN
		ModelData[15]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[15]._Color = CVector4f{ 0.3f, 0.8f, 0.3f, 1.0f };
		ModelData[16]._Position = CVector3f{ -1.0f, -1.0f, 1.0f };
		ModelData[16]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[16]._Color = CVector4f{ 0.3f, 0.8f, 0.3f, 1.0f };
		ModelData[17]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[17]._Normal = CVector3f{ 0.0f, -1.0f, 0.0f };
		ModelData[17]._Color = CVector4f{ 0.3f, 0.8f, 0.3f, 1.0f };

		ModelData[18]._Position = CVector3f{ -1.0f, 1.0f, 1.0f }; // IN FRONT
		ModelData[18]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[18]._Color = CVector4f{ 0.8f, 0.3f, 0.3f, 1.0f };
		ModelData[19]._Position = CVector3f{ -1.0f, -1.0f, 1.0f };
		ModelData[19]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[19]._Color = CVector4f{ 0.8f, 0.3f, 0.3f, 1.0f };
		ModelData[20]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[20]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[20]._Color = CVector4f{ 0.8f, 0.3f, 0.3f, 1.0f };

		ModelData[21]._Position = CVector3f{ 1.0f, 1.0f, 1.0f }; // RIGHT
		ModelData[21]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[21]._Color = CVector4f{ 0.3f, 0.3f, 0.8f, 1.0f };
		ModelData[22]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[22]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[22]._Color = CVector4f{ 0.3f, 0.3f, 0.8f, 1.0f };
		ModelData[23]._Position = CVector3f{ 1.0f, 1.0f, -1.0f };
		ModelData[23]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[23]._Color = CVector4f{ 0.3f, 0.3f, 0.8f, 1.0f };

		ModelData[24]._Position = CVector3f{ 1.0f, -1.0f, -1.0f }; // RIGHT
		ModelData[24]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[24]._Color = CVector4f{ 0.7f, 0.7f, 0.4f, 1.0f };
		ModelData[25]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[25]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[25]._Color = CVector4f{ 0.7f, 0.7f, 0.4f, 1.0f };
		ModelData[26]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[26]._Normal = CVector3f{ 1.0f, 0.0f, 0.0f };
		ModelData[26]._Color = CVector4f{ 0.7f, 0.7f, 0.4f, 1.0f };

		ModelData[27]._Position = CVector3f{ 1.0f, 1.0f, 1.0f }; // UP
		ModelData[27]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[27]._Color = CVector4f{ 0.4f, 0.7f, 0.7f, 1.0f };
		ModelData[28]._Position = CVector3f{ 1.0f, 1.0f, -1.0f };
		ModelData[28]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[28]._Color = CVector4f{ 0.4f, 0.7f, 0.7f, 1.0f };
		ModelData[29]._Position = CVector3f{ -1.0f, 1.0f, -1.0f };
		ModelData[29]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[29]._Color = CVector4f{ 0.4f, 0.7f, 0.7f, 1.0f };

		ModelData[30]._Position = CVector3f{ 1.0f, 1.0f, 1.0f }; // UP
		ModelData[30]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[30]._Color = CVector4f{ 0.7f, 0.4f, 0.7f, 1.0f };
		ModelData[31]._Position = CVector3f{ -1.0f, 1.0f, -1.0f };
		ModelData[31]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[31]._Color = CVector4f{ 0.7f, 0.4f, 0.7f, 1.0f };
		ModelData[32]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[32]._Normal = CVector3f{ 0.0f, 1.0f, 0.0f };
		ModelData[32]._Color = CVector4f{ 0.7f, 0.4f, 0.7f, 1.0f };

		ModelData[33]._Position = CVector3f{ 1.0f, 1.0f, 1.0f }; // IN FRONT
		ModelData[33]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[33]._Color = CVector4f{ 0.9f, 0.9f, 0.9f, 1.0f };
		ModelData[34]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[34]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[34]._Color = CVector4f{ 0.9f, 0.9f, 0.9f, 1.0f };
		ModelData[35]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[35]._Normal = CVector3f{ 0.0f, 0.0f, 1.0f };
		ModelData[35]._Color = CVector4f{ 0.9f, 0.9f, 0.9f, 1.0f };

		return ModelData;
	}
}

CCube::~CCube()
{
	CCube::Shutdown();
}

void CCube::Initialize( CGraphics& Graphics )
{
	_Vertices = GenerateCubeModelVertices();
	
	uint32_t VertexBufferSize = static_cast<uint32_t>( _Vertices.size() * sizeof( SVertex ) );
	SVertexBufferProperties Properties;
	Properties._VertexDataSizeInBytes = VertexBufferSize;
	Properties._SingleVertexSizeInBytes = sizeof( SVertex );
	_VertexBuffer = Graphics.CreateVertexBuffer( _Vertices.data(), VertexBufferSize, Properties );
	
	_ConstantBuffer = Graphics.CreateConstantBuffer( sizeof(STypicalConstantBuffer), ECpuAccessPolicy::CpuWrite );

	std::vector<SShaderInputDescription> InputLayout;
	InputLayout.push_back( SShaderInputDescription{ "POSITION", EGfxResourceDataFormat::R32G32B32Float } );
	InputLayout.push_back( SShaderInputDescription{ "NORMAL", EGfxResourceDataFormat::R32G32B32Float } );
	InputLayout.push_back( SShaderInputDescription{ "COLOR", EGfxResourceDataFormat::R32G32B32A32Float } );
	_VertexShader = Graphics.CreateVertexShader( VertexShaderFileName, VertexShaderMainFunction, InputLayout );
	_PixelShader = Graphics.CreatePixelShader( PixelShaderFileName, PixelShaderMainFunction );

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
	ASSERT( _Vertices.size() > 0, "Trying to render 0 vertices?" );
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
