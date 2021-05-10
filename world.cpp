#include "world.h"

#include "graphics.h"
#include "cube.h"
#include "matrix.h"
#include "perspective_camera.h"
#include "logger.h"
#include "input_enums.h"

#include <functional>


namespace
{
	constexpr int32_t ShadowMapTextureSlotIdx = 0;
}


void CWorld::Initialize( CGraphics& Graphics, CInputHandler& InputHandler )
{
	using namespace std::placeholders;

	_pGraphics = &Graphics;
	_pInputHandler = &InputHandler;
	float FieldOfView = NMiscMath::Pi / 2.0f;
	float AspectRatio = (float)Graphics.GetScreenWidth() / (float)Graphics.GetScreenHeight();
	_Camera = std::make_unique<CPerspectiveCamera>( FieldOfView, AspectRatio );

	{
		_Light = std::make_unique<CLightSource>();
		_Light->SetId(_NextGameObjectId++);
		_Light->Initialize(*_pGraphics);
		_Light->SetPosition(CVector3f{ 0.0f, 0.0f, 0.0f });
	}

	InputHandler.RegisterKeyInputEventCallback( this, std::bind( &CWorld::HandleUserInput, this, _1 ) );
	_CameraConstantBuffer = Graphics.CreateConstantBuffer( sizeof(NWorldPass::SCameraConstantBuffer), ECpuAccessPolicy::CpuWrite);
	_LightConstantBuffer = Graphics.CreateConstantBuffer(sizeof(NWorldPass::SLightConstantBuffer), ECpuAccessPolicy::CpuWrite);
	_ShadowHandler.Initialize(Graphics, Graphics.GetScreenWidth(), Graphics.GetScreenWidth());

	SpawnDefaultObjects();
}

void CWorld::ShutDown()
{
	_pGraphics = nullptr;
	_pInputHandler->DeRegisterKeyInputEventCallback(this);
	_pInputHandler = nullptr;
}

void CWorld::Update()
{

}

void CWorld::Render(CRenderContext& RenderContext)
{
	SShadowData ShadowData = _ShadowHandler.CalculatePointLightShadows(RenderContext, *_Light.get(), _GameObjects);

	PerCameraSetup(RenderContext, *_Camera.get());
	PerLightSetup(RenderContext, *_Light.get(), ShadowData);
	RenderObjects(RenderContext, *_Camera.get());

	RenderContext.ClearTextureSlot(ShadowMapTextureSlotIdx);
}

void CWorld::SpawnDefaultObjects()
{
	CVector4f WallColor{ 0.2f, 0.2f, 0.2f, 1.0f };
	CCube* Bottom = SpawnGameObject<CCube>();
	Bottom->SetPosition(CVector3f{ 0.0f, -15.0f, 0.0f });
	Bottom->SetScale(CVector3f{ 30.0f, 0.01f, 30.0f });
	Bottom->DisableShadowRendering();
	Bottom->SetColor(WallColor);
	CCube* Top = SpawnGameObject<CCube>();
	Top->SetPosition(CVector3f{ 0.0f, 15.0f, 0.0f });
	Top->SetScale(CVector3f{ 30.0f, 0.01f, 30.0f });
	Top->DisableShadowRendering();
	Top->SetColor(WallColor);
	CCube* Right = SpawnGameObject<CCube>();
	Right->SetPosition(CVector3f{ 15.0f, 0.0f, 0.0f });
	Right->SetScale(CVector3f{ 0.01f, 30.0f, 30.0f });
	Right->DisableShadowRendering();
	Right->SetColor(WallColor);
	CCube* Left = SpawnGameObject<CCube>();
	Left->SetPosition(CVector3f{ -15.0f, 0.0f, 0.0f });
	Left->SetScale(CVector3f{ 0.01f, 30.0f, 30.0f });
	Left->DisableShadowRendering();
	Left->SetColor(WallColor);
	CCube* Front = SpawnGameObject<CCube>();
	Front->SetPosition(CVector3f{ 0.0f, 0.0f, 15.0f });
	Front->SetScale(CVector3f{ 30.0f, 30.0f, 0.01f });
	Front->DisableShadowRendering();
	Front->SetColor(WallColor);
	CCube* Back = SpawnGameObject<CCube>();
	Back->SetPosition(CVector3f{ 0.0f, 0.0f, -15.0f });
	Back->SetScale(CVector3f{ 30.0f, 30.0f, 0.01f });
	Back->DisableShadowRendering();
	Back->SetColor(WallColor);

	CCube* Cube1 = SpawnGameObject<CCube>();
	Cube1->SetPosition(CVector3f{ -3.0f, -2.0f, -2.5f });
	CCube* Cube2 = SpawnGameObject<CCube>();
	Cube2->SetPosition(CVector3f{ 1.0f, 3.0f, 0.0f });
	CCube* Cube3 = SpawnGameObject<CCube>();
	Cube3->SetPosition(CVector3f{ 5.0f, -7.0f, -3.0f });
	CCube* Cube4 = SpawnGameObject<CCube>();
	Cube4->SetPosition(CVector3f{ 0.0f, -0.5f, 8.0f });
}

void CWorld::HandleUserInput(const CInputEvent& Input)
{
	if (Input.IsPressed(EInputCode::RightMouseButton))
	{
		static bool done = false;
		// When right mouse button is pressed, mouse movement is turned into rotation
		CVector2f NormalizedMouseMovement = Input.GetMouseMovement();
		_Camera->Yaw(NormalizedMouseMovement._X);
		_Camera->Pitch(-NormalizedMouseMovement._Y);
		// WASD used to move camera in (_Up, _Right) plane
		if (Input.IsPressed(EInputCode::KeyW))
		{
			_Camera->MoveForward();
		}
		if (Input.IsPressed(EInputCode::KeyA))
		{
			_Camera->StrafeLeft();
		}
		if (Input.IsPressed(EInputCode::KeyS))
		{
			_Camera->MoveBackwards();
		}
		if (Input.IsPressed(EInputCode::KeyD))
		{
			_Camera->StrafeRight();
		}
	}
	else
	{
		// WASD used to move camera in (_Up, _Right) plane
		if (Input.IsPressed(EInputCode::KeyW))
		{
			_Camera->StrafeUp();
		}
		if (Input.IsPressed(EInputCode::KeyA))
		{
			_Camera->StrafeLeft();
		}
		if (Input.IsPressed(EInputCode::KeyS))
		{
			_Camera->StrafeDown();
		}
		if (Input.IsPressed(EInputCode::KeyD))
		{
			_Camera->StrafeRight();
		}
	}
}

void CWorld::PerCameraSetup(CRenderContext& RenderContext, CCameraBase& Camera)
{
	NWorldPass::SCameraConstantBuffer CameraCb;

	// Set view and projection matrix
	CameraCb._ViewMatrix = Camera.GetViewMatrix();
	CameraCb._ViewAndProjection = Camera.GetViewAndProjection();
	RenderContext.UpdateConstantBuffer(_CameraConstantBuffer, &CameraCb, sizeof(CameraCb));
	RenderContext.SetVertexShaderConstantBuffer(_CameraConstantBuffer, EConstantBufferIdx::PerCamera);
	RenderContext.SetPixelShaderConstantBuffer(_CameraConstantBuffer, EConstantBufferIdx::PerCamera);
}

void CWorld::PerLightSetup(CRenderContext& RenderContext, CLightSource& Light, SShadowData& ShadowData)
{
	NWorldPass::SLightConstantBuffer LightCb;
	LightCb._W_LightPos = Light.GetPosition();
	LightCb._LightIntensity = 2.0f;
	RenderContext.UpdateConstantBuffer(_LightConstantBuffer, &LightCb, sizeof(LightCb));
	RenderContext.SetVertexShaderConstantBuffer(_LightConstantBuffer, EConstantBufferIdx::PerLight);
	RenderContext.SetPixelShaderConstantBuffer(_LightConstantBuffer, EConstantBufferIdx::PerLight);

	RenderContext.SetPixelShaderTexture(ShadowData._ShadowMap, ShadowMapTextureSlotIdx);
	RenderContext.SetPixelShaderSampler(ShadowData._ShadowMapSampler, 0);
}

void CWorld::RenderObjects(CRenderContext& RenderContext, CCameraBase& Camera)
{
	RenderContext.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_Light->Render(RenderContext, Camera);

	for (auto& pGameObject : _GameObjects)
	{
		pGameObject->Render(RenderContext, Camera);
	}
}
