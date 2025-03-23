#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h> // Microsoft ComPtr

// Handles the DX11 initialization and shutdown.
// Most of the code in this class was heavily influenced by some online guide.
// TODO: I should probably rewrite it now that I've learned so much about DX11.
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
	ID3D11RenderTargetView* AccessRenderTargetView();
	ID3D11RenderTargetView** AccessAddrOfRenderTargetView();
	ID3D11DepthStencilView* AccessDepthStencilView();

	D3D11_VIEWPORT GetViewport() const;

	void GetVideoCardInfo( char* CardName, int& VRamInMb );

private:
	bool _VSyncIsEnabled;
	int _VideoCardMemoryInMb;
	char _VideoCardDescription[128];
	Microsoft::WRL::ComPtr<IDXGISwapChain> _pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> _pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _pDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _pDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> _pRasterState;

	D3D11_VIEWPORT _DefaultViewport;

	bool _GfxDebugEnabled = true;
};