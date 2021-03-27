#include "cube.h"

#include <vector>

#include "camera_base.h"
#include "3d_math.h"

namespace
{
	const std::string VertexShaderFileName = "color.vs";
	const std::string PixelShaderFileName = "color.ps";
	const std::string VertexShaderMainFunction = "ColorVertexShader";
	const std::string PixelShaderMainFunction = "ColorPixelShader";

	std::vector<CCube::SVertex> GenerateCubeModelVertices()
	{
		std::vector<CCube::SVertex> ModelData{ 36 };
		ModelData[0]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[0]._Color = CVector4f{ 1.0f, 0.0f, 0.0f, 1.0f };
		ModelData[1]._Position = CVector3f{ -1.0f, -1.0f, 1.0f };
		ModelData[1]._Color = CVector4f{ 1.0f, 0.0f, 0.0f, 1.0f };
		ModelData[2]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[2]._Color = CVector4f{ 1.0f, 0.0f, 0.0f, 1.0f };

		ModelData[3]._Position = CVector3f{ 1.0f, 1.0f, -1.0f };
		ModelData[3]._Color = CVector4f{ 1.0f, 1.0f, 0.0f, 1.0f };
		ModelData[4]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[4]._Color = CVector4f{ 1.0f, 1.0f, 0.0f, 1.0f };
		ModelData[5]._Position = CVector3f{ -1.0f, 1.0f, -1.0f, };
		ModelData[5]._Color = CVector4f{ 1.0f, 1.0f, 0.0f, 1.0f };

		ModelData[6]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[6]._Color = CVector4f{ 0.0f, 0.0f, 1.0f, 1.0f };
		ModelData[7]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[7]._Color = CVector4f{ 0.0f, 0.0f, 1.0f, 1.0f };
		ModelData[8]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[8]._Color = CVector4f{ 0.0f, 0.0f, 1.0f, 1.0f };

		ModelData[9]._Position = CVector3f{ 1.0f, 1.0f, -1.0f };
		ModelData[9]._Color = CVector4f{ 0.0f, 1.0f, 1.0f, 1.0f };
		ModelData[10]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[10]._Color = CVector4f{ 0.0f, 1.0f, 1.0f, 1.0f };
		ModelData[11]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[11]._Color = CVector4f{ 0.0f, 1.0f, 1.0f, 1.0f };

		ModelData[12]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[12]._Color = CVector4f{ 1.0f, 0.0f, 1.0f, 1.0f };
		ModelData[13]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[13]._Color = CVector4f{ 1.0f, 0.0f, 1.0f, 1.0f };
		ModelData[14]._Position = CVector3f{ -1.0f, 1.0f, -1.0f };
		ModelData[14]._Color = CVector4f{ 1.0f, 0.0f, 1.0f, 1.0f };

		ModelData[15]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[15]._Color = CVector4f{ 0.3f, 0.8f, 0.3f, 1.0f };
		ModelData[16]._Position = CVector3f{ -1.0f, -1.0f, 1.0f };
		ModelData[16]._Color = CVector4f{ 0.3f, 0.8f, 0.3f, 1.0f };
		ModelData[17]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[17]._Color = CVector4f{ 0.3f, 0.8f, 0.3f, 1.0f };

		ModelData[18]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[18]._Color = CVector4f{ 0.8f, 0.3f, 0.3f, 1.0f };
		ModelData[19]._Position = CVector3f{ -1.0f, -1.0f, 1.0f };
		ModelData[19]._Color = CVector4f{ 0.8f, 0.3f, 0.3f, 1.0f };
		ModelData[20]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[20]._Color = CVector4f{ 0.8f, 0.3f, 0.3f, 1.0f };

		ModelData[21]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[21]._Color = CVector4f{ 0.3f, 0.3f, 0.8f, 1.0f };
		ModelData[22]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[22]._Color = CVector4f{ 0.3f, 0.3f, 0.8f, 1.0f };
		ModelData[23]._Position = CVector3f{ 1.0f, 1.0f, -1.0f };
		ModelData[23]._Color = CVector4f{ 0.3f, 0.3f, 0.8f, 1.0f };

		ModelData[24]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[24]._Color = CVector4f{ 0.7f, 0.7f, 0.4f, 1.0f };
		ModelData[25]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[25]._Color = CVector4f{ 0.7f, 0.7f, 0.4f, 1.0f };
		ModelData[26]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[26]._Color = CVector4f{ 0.7f, 0.7f, 0.4f, 1.0f };

		ModelData[27]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[27]._Color = CVector4f{ 0.4f, 0.7f, 0.7f, 1.0f };
		ModelData[28]._Position = CVector3f{ 1.0f, 1.0f, -1.0f };
		ModelData[28]._Color = CVector4f{ 0.4f, 0.7f, 0.7f, 1.0f };
		ModelData[29]._Position = CVector3f{ -1.0f, 1.0f, -1.0f };
		ModelData[29]._Color = CVector4f{ 0.4f, 0.7f, 0.7f, 1.0f };

		ModelData[30]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[30]._Color = CVector4f{ 0.7f, 0.4f, 0.7f, 1.0f };
		ModelData[31]._Position = CVector3f{ -1.0f, 1.0f, -1.0f };
		ModelData[31]._Color = CVector4f{ 0.7f, 0.4f, 0.7f, 1.0f };
		ModelData[32]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[32]._Color = CVector4f{ 0.7f, 0.4f, 0.7f, 1.0f };

		ModelData[33]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[33]._Color = CVector4f{ 0.9f, 0.9f, 0.9f, 1.0f };
		ModelData[34]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[34]._Color = CVector4f{ 0.9f, 0.9f, 0.9f, 1.0f };
		ModelData[35]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
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
	
	_ConstantBuffer = Graphics.CreateConstantBuffer( sizeof( SCubeConstantBuffer ), ECpuAccessPolicy::CpuWrite );

	_VertexShader = Graphics.CreateVertexShader( VertexShaderFileName, VertexShaderMainFunction );

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
	assert(_Vertices.size() > 0);
	RenderContext.SetVertexBuffer( _VertexBuffer );
	RenderContext.SetVertexShaderConstantBuffer( _ConstantBuffer, EConstantBufferIdx::PerObject );
	RenderContext.SetVertexShader( _VertexShader );
	// Set pixel shader
	RenderContext.Draw( static_cast<int32_t>( _Vertices.size() ) );
}

void CCube::SetPosition( const CVector3f& NewPosition )
{
	_Position = NewPosition;
	_LocalToWorldTransformIsStale = true;
}

void CCube::SetScale( const CVector3f& NewScale )
{
	_Scale = NewScale;
	_LocalToWorldTransformIsStale = true;
}

CMatrix4x4f CCube::GetLocalToWorldTransform() const
{
	if ( _LocalToWorldTransformIsStale )
	{
		// TODO: Apply scale
		_LocalToWorldTransform = N3DMath::CreateCoordinateTransform( _Position, _Forward, _Up, N3DMath::ECoordinateTransformType::LocalToWorld );
		_LocalToWorldTransformIsStale = false;
	}
	return _LocalToWorldTransform;
}