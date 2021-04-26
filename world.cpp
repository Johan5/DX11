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
	//_PerFrameConstantBuffer = Graphics.CreateConstantBuffer( sizeof( SPerFrameConstantBuffer ), ECpuAccessPolicy::CpuWrite );

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
	NewFrameSetup(RenderContext);
	PerCameraSetup(RenderContext, *_Camera.get());
	RenderObjects(RenderContext, *_Camera.get());
}

void CWorld::SpawnDefaultObjects()
{
	{
		std::unique_ptr<CLightSource> MainLight = std::make_unique<CLightSource>();
		MainLight->SetId(_NextGameObjectId++);
		MainLight->Initialize(*_pGraphics);
		MainLight->SetPosition(CVector3f{ 0.0f, 10.0f, 5.0f });
		_Lights.push_back(std::move(MainLight));
	}

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

void CWorld::NewFrameSetup(CRenderContext& RenderContext)
{
}

void CWorld::PerCameraSetup(CRenderContext& RenderContext, CCameraBase& Camera)
{
	SCameraConstantBuffer CameraCb;
	if (!_Lights.empty())
	{
		std::unique_ptr<CLightSource>& MainLight = _Lights[0];
		const CVector3f& W_LightPos = MainLight->GetPosition();
		CVector4f C_LightPos = Camera.GetViewMatrix() * CVector4f(W_LightPos, 1.0f);
		CameraCb._Light1Pos = C_LightPos.XYZ() / C_LightPos._W;
		CameraCb._HasLight1 = 1.0f;
	}

	// Set view and projection matrix
	CameraCb._ViewMatrix = Camera.GetViewMatrix();
	CameraCb._ViewAndProjection = Camera.GetViewAndProjection();
	RenderContext.UpdateConstantBuffer(_CameraConstantBuffer, &CameraCb, sizeof(CameraCb));
	RenderContext.SetVertexShaderConstantBuffer(_CameraConstantBuffer, EConstantBufferIdx::PerCamera);
	RenderContext.SetPixelShaderConstantBuffer(_CameraConstantBuffer, EConstantBufferIdx::PerCamera);
}

void CWorld::RenderObjects(CRenderContext& RenderContext, CCameraBase& Camera)
{
	RenderContext.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (auto& pLight : _Lights)
	{
		pLight->Render(RenderContext, Camera);
	}

	for (auto& pGameObject : _GameObjects)
	{
		pGameObject->Render(RenderContext, Camera);
	}
}
