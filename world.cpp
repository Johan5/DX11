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
	//ViewandProjection.Transpose();
	RenderContext.UpdateConstantBuffer( _CameraConstantBuffer, &ViewandProjection, sizeof( ViewandProjection ) );
	RenderContext.SetVertexShaderConstantBuffer( _CameraConstantBuffer, EConstantBufferIdx::PerCamera );
	RenderContext.SetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	////////////////////////////////////////////////////////////////////////////////
	CMatrix4x4f TmpProjection1 = _Camera->GetViewMatrix();
	CVector4f Pt1 = TmpProjection1 * CVector4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	CVector4f Pt2 = TmpProjection1 * CVector4f{ 1.0f, -1.0f, -1.0f, 1.0f };
	CVector4f Pt3 = TmpProjection1 * CVector4f{ -1.0f, 1.0f, 1.0f, 1.0f };
	CVector4f Pt4 = TmpProjection1 * CVector4f{ -1.0f, -1.0f, -1.0f, 1.0f };
	CMatrix4x4f TmpProjection2 = _Camera->GetProjectionMatrix();
	CVector4f Qt1 = TmpProjection2 * Pt1;
	CVector4f Qt2 = TmpProjection2 * Pt2;
	CVector4f Qt3 = TmpProjection2 * Pt3;
	CVector4f Qt4 = TmpProjection2 * Pt4;
	CCube* pCube = reinterpret_cast<CCube*>(_GameObjects[0].get());
	CMatrix4x4f TmpProjection3 = pCube->GetLocalToWorldTransform();
	CVector4f Rt1 = TmpProjection3 * CVector4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	CVector4f Rt2 = TmpProjection3 * CVector4f{ 1.0f, -1.0f, -1.0f, 1.0f };
	CVector4f Rt3 = TmpProjection3 * CVector4f{ -1.0f, 1.0f, 1.0f, 1.0f };
	CVector4f Rt4 = TmpProjection3 * CVector4f{ -1.0f, -1.0f, -1.0f, 1.0f };
	////////////////////////////////////////////////////////////////////////////////

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
		_Camera->RotateLeft();
		break;
	case EInputCode::RightArrow:
		_Camera->RotateRight();
		break;
	default:
		break;
	}
}
