#include "renderer/world.h"

#include "graphics/graphics.h"
#include "renderer/cube.h"
#include "core/matrix.h"
#include "input/perspective_camera.h"
#include "utils/logger.h"
#include "input/input_enums.h"
#include "utils/assert.h"

/// DEBUG CODE. REMOVE
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <functional>

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
		_Light->SetScale(CVector3f{ 0.3f, 0.3f, 0.3f });
	}

	InputHandler.RegisterKeyInputEventCallback( this, std::bind( &CWorld::HandleUserInput, this, _1 ) );
	_CameraConstantBuffer = Graphics.CreateConstantBuffer( sizeof(NWorldPass::SCameraConstantBuffer), ECpuAccessPolicy::CpuWrite);
	_LightConstantBuffer = Graphics.CreateConstantBuffer(sizeof(NWorldPass::SLightConstantBuffer), ECpuAccessPolicy::CpuWrite);
	_ShadowHandler.Initialize(Graphics, Graphics.GetScreenWidth(), Graphics.GetScreenWidth());
	_BatchRenderHelper.Initialize(Graphics);
	SpawnDefaultObjects(Graphics);
}

void CWorld::ShutDown()
{
	_pGraphics = nullptr;
	_pInputHandler->DeRegisterKeyInputEventCallback(this);
	_pInputHandler = nullptr;
}

void CWorld::Update()
{
	Assimp::Importer importer;

	std::string characterFbxPath = "assets/Character/XBot.fbx";
	const aiScene* scene = importer.ReadFile(characterFbxPath, aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	int asd = 2;
}

void CWorld::Render(CRenderContext& RenderContext)
{
	SShadowData ShadowData = _ShadowHandler.CreateShadowMap(*_pGraphics, _BatchRenderHelper, _Light.get()->GetPosition());

	PerCameraSetup(RenderContext, *_Camera.get());
	PerLightSetup(RenderContext, *_Light.get(), ShadowData);
	RenderObjects(RenderContext, *_Camera.get());

	RenderContext.ClearTextureSlot(NGraphicsDefines::ShadowMapTextureSlot);
}

void CWorld::SpawnDefaultObjects(CGraphics& Graphics)
{
	auto SpawnWall = [this](const CVector3f& Position, const CVector3f& Scale)
		{
			CCube* Object = SpawnGameObject<CCube>();
			Object->SetPosition(Position);
			Object->SetScale(Scale);
			Object->DisableShadowRendering();
			Object->AccessConstantBuffer()._ColorData._TextureSlot = NGraphicsDefines::ShadowMapTextureSlot + 1;
		};

	// Bottom
	SpawnWall(CVector3f{ 0.0f, -15.0f, 0.0f }, CVector3f{ 30.0f, 0.01f, 30.0f });
	// Top
	SpawnWall(CVector3f{ 0.0f, 15.0f, 0.0f }, CVector3f{ 30.0f, 0.01f, 30.0f });
	// Right
	SpawnWall(CVector3f{ 15.0f, 0.0f, 0.0f }, CVector3f{ 0.01f, 30.0f, 30.0f });
	// Left
	SpawnWall(CVector3f{ -15.0f, 0.0f, 0.0f }, CVector3f{ 0.01f, 30.0f, 30.0f });
	// Front
	SpawnWall(CVector3f{ 0.0f, 0.0f, 15.0f }, CVector3f{ 30.0f, 30.0f, 0.01f });
	// Back
	SpawnWall(CVector3f{ 0.0f, 0.0f, -15.0f }, CVector3f{ 30.0f, 30.0f, 0.01f });

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

	RenderContext.SetPixelShaderTexture(ShadowData._ShadowMap, NGraphicsDefines::ShadowMapTextureSlot);
	RenderContext.SetPixelShaderSampler(ShadowData._ShadowMapSampler, NGraphicsDefines::ShadowMapSamplerSlot);
}

void CWorld::RenderObjects(CRenderContext& RenderContext, CCameraBase& Camera)
{
	RenderContext.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_Light->Render(_BatchRenderHelper, Camera);
	for (auto& pGameObject : _GameObjects)
	{
		pGameObject->Render(_BatchRenderHelper, Camera);
	}
	_BatchRenderHelper.RenderInstanced(RenderContext, *_pGraphics, ERenderPass::Normal);
}
