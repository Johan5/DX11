//#include "sphere.h"
//
//#include "3d_math.h"
//#include "graphics_enums.h"
//#include "assert.h"
//
//#include <vector>
//
//
//namespace
//{
//	const std::string VertexShaderFileName = "color.vs";
//	const std::string PixelShaderFileName = "color.ps";
//	const std::string VertexShaderMainFunction = "ColorVertexShader";
//	const std::string PixelShaderMainFunction = "ColorPixelShader";
//
//	
//}
//
//CSphere::~CSphere()
//{
//	CSphere::Shutdown();
//}
//
//void CSphere::Initialize( CGraphics& Graphics )
//{
//	uint32_t VertexBufferSize = static_cast<uint32_t>(_Vertices.size() * sizeof( SVertex ));
//	SVertexBufferProperties Properties;
//	Properties._VertexDataSizeInBytes = VertexBufferSize;
//	Properties._SingleVertexSizeInBytes = sizeof( SVertex );
//	_VertexBuffer = Graphics.CreateVertexBuffer( _Vertices.data(), VertexBufferSize, Properties );
//
//	_ConstantBuffer = Graphics.CreateConstantBuffer( sizeof( SCubeConstantBuffer ), ECpuAccessPolicy::CpuWrite );
//
//	std::vector<SShaderInputDescription> InputLayout;
//	InputLayout.push_back( SShaderInputDescription{ "POSITION", EGfxResourceDataFormat::R32G32B32Float } );
//	InputLayout.push_back( SShaderInputDescription{ "NORMAL", EGfxResourceDataFormat::R32G32B32Float } );
//	InputLayout.push_back( SShaderInputDescription{ "COLOR", EGfxResourceDataFormat::R32G32B32A32Float } );
//	_VertexShader = Graphics.CreateVertexShader( VertexShaderFileName, VertexShaderMainFunction, InputLayout );
//	_PixelShader = Graphics.CreatePixelShader( PixelShaderFileName, PixelShaderMainFunction );
//
//	_IsInitialized = true;
//}
//
//void CSphere::Shutdown()
//{
//	// TODO cleanup vertex buffer?
//	_IsInitialized = false;
//}
//
//bool CSphere::IsInitialized() const
//{
//	return _IsInitialized;
//}
//
//void CSphere::Render( CRenderContext& RenderContext, const CCameraBase& Camera )
//{
//	ASSERT( _Vertices.size() > 0, "Rendering 0 vertices?" );
//	RenderContext.SetVertexBuffer( _VertexBuffer );
//	CMatrix4x4f LocalToWorld = GetLocalToWorldTransform();
//	assert( sizeof( LocalToWorld ) == sizeof( SCubeConstantBuffer ) );
//	RenderContext.UpdateConstantBuffer( _ConstantBuffer, &LocalToWorld, sizeof( SCubeConstantBuffer ) );
//	RenderContext.SetVertexShaderConstantBuffer( _ConstantBuffer, EConstantBufferIdx::PerObject );
//	RenderContext.SetVertexShader( _VertexShader );
//	RenderContext.SetPixelShader( _PixelShader );
//	// Set pixel shader
//	RenderContext.Draw( static_cast<int32_t>(_Vertices.size()) );
//}
//
//void CSphere::SetPosition( const CVector3f& NewPosition )
//{
//	_Position = NewPosition;
//	_TransformsAreStale = true;
//}
//
//void CSphere::SetScale( const CVector3f& NewScale )
//{
//	_Scale = NewScale;
//	_TransformsAreStale = true;
//}
//
