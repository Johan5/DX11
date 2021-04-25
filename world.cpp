#include "world.h"

#include "graphics.h"
#include "cube.h"
#include "matrix.h"
#include "perspective_camera.h"
#include "logger.h"
#include "input_enums.h"

#include <functional>

void CWorld::Initialize( CGraphics& Graphics, CInputHandler& InputHandler )
{
	using namespace std::placeholders;

	_pGraphics = &Graphics;
	_pInputHandler = &InputHandler;
	float FieldOfView = NMiscMath::Pi / 2.0f;
	float AspectRatio = (float)Graphics.GetScreenWidth() / (float)Graphics.GetScreenHeight();
	_Camera = std::make_unique<CPerspectiveCamera>( FieldOfView, AspectRatio );
	InputHandler.RegisterKeyInputEventCallback( this, std::bind( &CWorld::HandleUserInput, this, _1 ) );
	_CameraConstantBuffer = Graphics.CreateConstantBuffer( sizeof( SCameraConstantBuffer ), ECpuAccessPolicy::CpuWrite );
	_PerFrameConstantBuffer = Graphics.CreateConstantBuffer( sizeof( SPerFrameConstantBuffer ), ECpuAccessPolicy::CpuWrite );

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
	SPerFrameConstantBuffer PerFrameCB;
	CVector4f LightPos4f = CVector4f{ PerFrameCB._Light1Pos, 1.0f };
	LightPos4f = _Camera->GetViewMatrix() * LightPos4f;
	PerFrameCB._Light1Pos = LightPos4f.XYZ() / LightPos4f._W;
	RenderContext.UpdateConstantBuffer( _PerFrameConstantBuffer, &PerFrameCB, sizeof( PerFrameCB ) );
	RenderContext.SetPixelShaderConstantBuffer( _PerFrameConstantBuffer, EConstantBufferIdx::PerFrame );
	// Set view and projection matrix
	SCameraConstantBuffer CameraCb;
	CameraCb._ViewMatrix = _Camera->GetViewMatrix();
	CameraCb._ViewAndProjection = _Camera->GetViewAndProjection();
	RenderContext.UpdateConstantBuffer( _CameraConstantBuffer, &CameraCb, sizeof(CameraCb) );
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
	Cube->SetPosition( CVector3f{ 0.0f, 0.0f, 4.0f } );
	CCube* Cube2 = SpawnGameObject<CCube>();
	Cube2->SetPosition( CVector3f{ 5.5f, 5.5f, 20.0f } );
}

void CWorld::HandleUserInput( const CInputEvent& Input )
{
	if ( Input.IsPressed( EInputCode::RightMouseButton ) )
	{
		static bool done = false;
		// When right mouse button is pressed, mouse movement is turned into rotation
		CVector2f NormalizedMouseMovement = Input.GetMouseMovement();
		_Camera->Yaw( NormalizedMouseMovement._X );
		_Camera->Pitch( -NormalizedMouseMovement._Y );
		// WASD used to move camera in (_Up, _Right) plane
		if ( Input.IsPressed( EInputCode::KeyW ) )
		{
			_Camera->MoveForward();
		}
		if ( Input.IsPressed( EInputCode::KeyA ) )
		{
			_Camera->StrafeLeft();
		}
		if ( Input.IsPressed( EInputCode::KeyS ) )
		{
			_Camera->MoveBackwards();
		}
		if ( Input.IsPressed( EInputCode::KeyD ) )
		{
			_Camera->StrafeRight();
		}
	}
	else
	{
		// WASD used to move camera in (_Up, _Right) plane
		if ( Input.IsPressed( EInputCode::KeyW ) )
		{
			_Camera->StrafeUp();
		}
		if ( Input.IsPressed( EInputCode::KeyA ) )
		{
			_Camera->StrafeLeft();
		}
		if ( Input.IsPressed( EInputCode::KeyS ) )
		{
			_Camera->StrafeDown();
		}
		if ( Input.IsPressed( EInputCode::KeyD ) )
		{
			_Camera->StrafeRight();
		}
	}

}
