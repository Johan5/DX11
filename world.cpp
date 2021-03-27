#include "world.h"

#include "graphics.h"
#include "cube.h"
#include "matrix.h"


void CWorld::Initialize( CGraphics& Graphics )
{
	_pGraphics = &Graphics;
	float ViewportHeightInWorldUnits = 10.0f;
	float AspectRatio = (float)Graphics.GetScreenWidth() / (float)Graphics.GetScreenHeight();
	float ViewportWidthInWorldUnits = ViewportHeightInWorldUnits * AspectRatio;
	_Camera = std::make_unique<COrthographicCamera>( ViewportWidthInWorldUnits, ViewportHeightInWorldUnits );
	_CameraConstantBuffer = Graphics.CreateConstantBuffer( sizeof( SCameraConstantBuffer ), ECpuAccessPolicy::CpuWrite );

	ID3D11Buffer* RawBfr = _CameraConstantBuffer.AccessRawBuffer();
	D3D11_BUFFER_DESC Desc;
	RawBfr->GetDesc( &Desc );

	SpawnDefaultObjects();
}

void CWorld::SpawnDefaultObjects()
{
	CCube* Cube = SpawnGameObject<CCube>();
	Cube->SetPosition( CVector3f{ 0.0f, 0.0f, 5.0f } );
}

void CWorld::Update()
{

}

void CWorld::Render( CRenderContext& RenderContext )
{

	// Set view and projection matrix
	CMatrix4x4f ViewandProjection = _Camera->GetViewAndProjection();
	ViewandProjection.Transpose();
	RenderContext.UpdateConstantBuffer( _CameraConstantBuffer, &ViewandProjection, sizeof( ViewandProjection ) );
	RenderContext.SetVertexShaderConstantBuffer( _CameraConstantBuffer, EConstantBufferIdx::PerCamera );
	RenderContext.SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	for ( auto& pGameObject : _GameObjects )
	{
		pGameObject->Render( RenderContext, *_Camera );
	}
}
