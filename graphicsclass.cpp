#include "graphicsclass.h"

#include "vector.h"


namespace
{
	std::vector<CModel::SVertex> CreateModelTestData( float X, float Depth )
	{
		std::vector<CModel::SVertex> ModelData{ 3 };
		ModelData[0]._Position = CVector3f{ X, 0.0f, Depth }; // bot left
		ModelData[1]._Position = CVector3f{ X + 0.25f, 1.0f, Depth }; // top
		ModelData[2]._Position = CVector3f{ X + 0.5f, 0.0f, Depth }; // bot right
		
		ModelData[0]._Color = CVector4f{ 0.9f, 1.0f, 0.2f, 1.0f };
		ModelData[1]._Color = CVector4f{ 0.9f, 1.0f, 0.2f, 1.0f };
		ModelData[2]._Color = CVector4f{ 0.9f, 1.0f, 0.2f, 1.0f };

		return ModelData;
	}
}

CGraphics::~CGraphics()
{
	Shutdown();
}

bool CGraphics::Initialize(int ScreenWidth, int ScreenHeight, HWND Wnd)
{
	_Direct3D = std::make_unique<CDirectX3D>();
	bool Result = _Direct3D->Initialize( ScreenWidth, ScreenHeight, NGraphicsDefines::IsVSyncEnabled,
		Wnd, NGraphicsDefines::IsFullScreen, NGraphicsDefines::ScreenDepth, NGraphicsDefines::ScreenNear );
	if ( !Result )
	{
		MessageBox( Wnd, L"Could not initialize Direct3D", L"Error", MB_OK );
		return false;
	}

	_Camera = std::make_unique<CCamera>();

	{
		std::unique_ptr<CModel> Model1 = std::make_unique<CModel>();
		float Depth = 0.2f;
		Model1->Initialize( _Direct3D->AccessDevice(), CreateModelTestData( 0.0f, Depth ) );
		_Models.push_back( std::move( Model1 ) );
	}
	{
		std::unique_ptr<CModel> Model2 = std::make_unique<CModel>();
		float Depth = 5.0f;
		Model2->Initialize( _Direct3D->AccessDevice(), CreateModelTestData( 0.25f, Depth ) );
		_Models.push_back( std::move( Model2 ) );
	}
	
	if ( !Result )
	{
		MessageBox( Wnd, L"Could not initialize the model object.", L"Error", MB_OK );
		return false;
	}

	_ColorShader = std::make_unique<CColorShader>();
	Result = _ColorShader->Initialize( _Direct3D->AccessDevice(), Wnd );
	if ( !Result )
	{
		MessageBox( Wnd, L"Could not initialize ColorShader.", L"Error", MB_OK );
		return false;
	}
	
	return true;
}

void CGraphics::Shutdown()
{
	_Direct3D.release();
}


bool CGraphics::Frame()
{
	if ( !Render() )
	{
		return false;
	}
	return true;
}

bool CGraphics::Render()
{
	// Set to black
	_Direct3D->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );

	DirectX::XMMATRIX WorldMatrix, ViewAndProjectionMatrix;
	WorldMatrix = DirectX::XMMatrixIdentity();
	_Camera->GetViewAndProjection( ViewAndProjectionMatrix );
	for ( const std::unique_ptr<CModel>& pModel : _Models )
	{
		pModel->PrepForRender( _Direct3D->AccessDeviceContext() );
		_ColorShader->Render( _Direct3D->AccessDeviceContext(), pModel->GetNumVertices(), WorldMatrix, ViewAndProjectionMatrix );
	}
	
	_Direct3D->EndScene();
	return true;
}
