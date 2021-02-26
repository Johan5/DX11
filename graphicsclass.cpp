#include "graphicsclass.h"

#include "vector.h"


CGraphics::~CGraphics()
{
	Shutdown();
}

bool CGraphics::Initialize(int ScreenWidth, int ScreenHeight, HWND Wnd)
{
	_Direct3D = std::make_unique<CDirectX3D>();
	_ScreenWidthInPix = ScreenWidth;
	_ScreenHeightInPix = ScreenHeight;
	bool Result = _Direct3D->Initialize( ScreenWidth, ScreenHeight, NGraphicsDefines::IsVSyncEnabled,
		Wnd, NGraphicsDefines::IsFullScreen, NGraphicsDefines::ScreenDepth, NGraphicsDefines::ScreenNear );
	if ( !Result )
	{
		MessageBox( Wnd, L"Could not initialize Direct3D", L"Error", MB_OK );
		return false;
	}

	//_Camera = std::make_unique<CCamera>();
	float ViewportHeightInWorldUnits = 10.0f;
	float AspectRatio = (float)ScreenWidth / (float)ScreenHeight;
	float ViewportWidthInWorldUnits = ViewportHeightInWorldUnits * AspectRatio;
	_Camera = std::make_unique<COrthographicCamera>( ViewportWidthInWorldUnits, ViewportHeightInWorldUnits );

	{
		std::unique_ptr<CCubeModel> Model1 = std::make_unique<CCubeModel>();
		Model1->Initialize( _Direct3D->AccessDevice() );
		Model1->SetPosition( CVector3f{ 0.0, 0.0f, 5.0f } );
		_Models.push_back( std::move( Model1 ) );
	}
	// {
	// 	std::unique_ptr<CModel> Model2 = std::make_unique<CModel>();
	// 	float Depth = 5.0f;
	// 	Model2->Initialize( _Direct3D->AccessDevice(), CreateModelTestData( 0.25f, Depth ) );
	// 	_Models.push_back( std::move( Model2 ) );
	// }
	
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
	double TimeElapsedMs = _FrameTimer.StartNewFrame();
	if ( !Render( TimeElapsedMs ) )
	{
		return false;
	}
	return true;
}

bool CGraphics::Render( double TimeElapsedMs )
{
	// Set to black
	_Direct3D->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );

	{
		// static float TotElapsedTimeSec = 0;
		// TotElapsedTimeSec += static_cast<float>( TimeElapsedMs ) / 1000.0f;
		//
		// float CameraMovementSpeed = 0.5f;
		// float XOffset = 0.5f * cosf( CameraMovementSpeed * TotElapsedTimeSec );
		// float YOffset = 0.5f * sinf( CameraMovementSpeed * TotElapsedTimeSec );
		// CVector3f NewPos = CVector3f{ XOffset, YOffset, 0.0f };
		// _Camera->SetPosition( NewPos );
	}

	
	CMatrix4x4f ViewAndProjectionMatrix = _Camera->GetViewAndProjection();
	for ( const std::unique_ptr<CModelInterface>& pModel : _Models )
	{
		const CMatrix4x4f& LocalToWorld = pModel->GetLocalToWorldTransform();
		pModel->PrepForRender( _Direct3D->AccessDeviceContext() );
		_ColorShader->Render( _Direct3D->AccessDeviceContext(), pModel->GetNumVertices(), LocalToWorld, ViewAndProjectionMatrix );
	}
	
	_Direct3D->EndScene();
	return true;
}
