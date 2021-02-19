#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>


class CDirectX3D
{
public:
	CDirectX3D() = default;
	CDirectX3D( const CDirectX3D& Other ) = delete;

	bool Initialize( int ScreenWidth, int ScreenHeight, bool ShouldVSync, HWND Wnd, bool ShouldRunFullscreen, float ScreenDepth, float ScreenNear );
	void Shutdown();

	void BeginScene( float Red, float Green, float Blue, float Alpha );
	void EndScene();

	ID3D11Device* AccessDevice();
	ID3D11DeviceContext* AccessDeviceContext();

	void GetProjectionMatrix( DirectX::XMMATRIX& MatrixOut );
	void GetOrthoMatrix( DirectX::XMMATRIX& MatrixOut );

	void GetVideoCardInfo( char* CardName, int& VRamInMb );

private:
	bool _VSyncIsEnabled;
	int _VideoCardMemoryInMb;
	char _VideoCardDescription[128];
	IDXGISwapChain* _pSwapChain = nullptr;
	ID3D11Device* _pDevice = nullptr;
	ID3D11DeviceContext* _pDeviceContext = nullptr;
	ID3D11RenderTargetView* _pRenderTargetView = nullptr;
	ID3D11Texture2D* _pDepthStencilBuffer = nullptr;
	ID3D11DepthStencilState* _pDepthStencilState = nullptr;
	ID3D11DepthStencilView* _pDepthStencilView = nullptr;
	ID3D11RasterizerState* _pRasterState = nullptr;
	DirectX::XMMATRIX _ProjectionMatrix;
	DirectX::XMMATRIX _OrthoMatrix;

	bool _GfxDebugEnabled = true;
};