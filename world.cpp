#include "world.h"

#include "graphics.h"
#include "cube.h"
#include "matrix.h"

#include <functional>

void CWorld::Initialize( CGraphics& Graphics, CInputHandler& InputHandler )
{
	using namespace std::placeholders;

	_pGraphics = &Graphics;
	_pInputHandler = &InputHandler;
	float ViewportHeightInWorldUnits = 10.0f;
	float AspectRatio = (float)Graphics.GetScreenWidth() / (float)Graphics.GetScreenHeight();
	float ViewportWidthInWorldUnits = ViewportHeightInWorldUnits * AspectRatio;
	_Camera = std::make_unique<COrthographicCamera>( ViewportWidthInWorldUnits, ViewportHeightInWorldUnits );
	InputHandler.RegisterKeyInputEventCallback( this, std::bind( &CWorld::HandleUserInput, this, _1 ) );
	_CameraConstantBuffer = Graphics.CreateConstantBuffer( sizeof( SCameraConstantBuffer ), ECpuAccessPolicy::CpuWrite );

	ID3D11Buffer* RawBfr = _CameraConstantBuffer.AccessRawBuffer();

	SpawnDefaultObjects();
}

void CWorld::ShutDown()
{
	_pGraphics = nullptr;
	_pInputHandler->DeRegisterKeyInputEventCallback( this );
	_pInputHandler = nullptr;
}

void CWorld::Update()
{

}

void CWorld::Render( CRenderContext& RenderContext )
{
	// Set view and projection matrix
	CMatrix4x4f ViewandProjection = _Camera->GetViewAndProjection();
	RenderContext.UpdateConstantBuffer( _CameraConstantBuffer, &ViewandProjection, sizeof( ViewandProjection ) );
	RenderContext.SetVertexShaderConstantBuffer( _CameraConstantBuffer, EConstantBufferIdx::PerCamera );
	RenderContext.SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	for ( auto& pGameObject : _GameObjects )
	{
		pGameObject->Render( RenderContext, *_Camera );
	}
}

void CWorld::SpawnDefaultObjects()
{
	CCube* Cube = SpawnGameObject<CCube>();
	Cube->SetPosition( CVector3f{ 0.0f, 0.0f, 5.0f } );
}

void CWorld::HandleUserInput( const SKeyInput& Input )
{
	switch ( Input._InputCode )
	{
	case EInputCode::KeyW:
		_Camera->StrafeUp();
		break;
	case EInputCode::KeyD:
		_Camera->StrafeRight();
		break;
	case EInputCode::KeyS:
		_Camera->StrafeDown();
		break;
	case EInputCode::KeyA:
		_Camera->StrafeLeft();
		break;
	case EInputCode::LeftArrow:
		_Camera->RotateLeft( 1.0f );
		break;
	case EInputCode::RightArrow:
		_Camera->RotateRight( 1.0f );
		break;
	case EInputCode::DownArrow:
		_Camera->RotateDown( 1.0f );
		break;
	case EInputCode::UpArrow:
		_Camera->RotateUp( 1.0f );
		break;
	default:
		break;
	}
}
