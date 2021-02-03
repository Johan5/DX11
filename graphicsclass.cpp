#include "graphicsclass.h"


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
	_Camera->SetPosition( 0.0f, 0.0f, -5.0f );

	_Model = std::make_unique<CModel>();
	Result = _Model->Initialize( _Direct3D->AccessDevice() );
	if ( !Result )
	{
		MessageBox( Wnd, L"Could not initialize the model object.", L"Error", MB_OK );
		return false;
	}

	_ColorShader = std::make_unique<CColorShader>();
	Result = _ColorShader->Initialize( _Direct3D->AccessDevice(), Wnd );
	if ( !Result )
	{
		MessageBox( Wnd, L"Could not initialize the model object.", L"Error", MB_OK );
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
	_Camera->Render();
	_Model->Render( _Direct3D->AccessDeviceContext() );

	XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix;
	_Direct3D->GetWorldMatrix( WorldMatrix );
	_Camera->GetViewMatrix( ViewMatrix );
	_Direct3D->GetProjectionMatrix( ProjectionMatrix );
	_ColorShader->Render( _Direct3D->AccessDeviceContext(), _Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix );
	
	_Direct3D->EndScene();
	return true;
}
