#include "graphics/d3dclass.h"

#include <iostream>

bool CDirectX3D::Initialize(int ScreenWidth, int ScreenHeight, bool ShouldVSync,
                            HWND Wnd, bool ShouldRunFullscreen,
                            float ScreenDepth, float ScreenNear) {
  _VSyncIsEnabled = ShouldVSync;

  HRESULT Result;

  // Create a DirectX graphics interface factory
  IDXGIFactory* pFactory;
  Result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
  if (FAILED(Result)) {
    return false;
  }

  // Use the factory to create an adapter for the primary graphics interface (video card)
  IDXGIAdapter* pAdapter;
  Result = pFactory->EnumAdapters(0, &pAdapter);
  if (FAILED(Result)) {
    return false;
  }

  // Enumerate the primary adapter output (monitor)
  IDXGIOutput* pAdapterOutput;
  Result = pAdapter->EnumOutputs(0, &pAdapterOutput);
  if (FAILED(Result)) {
    return false;
  }

  // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor)
  unsigned int NumModes;
  Result = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                                              DXGI_ENUM_MODES_INTERLACED,
                                              &NumModes, nullptr);
  if (FAILED(Result)) {
    return false;
  }

  // Create a list to hold all the possible display modes for this monitor/video card combination
  DXGI_MODE_DESC* pDisplayModeList = new DXGI_MODE_DESC[NumModes];

  // Fill the display mode list structures
  Result = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                                              DXGI_ENUM_MODES_INTERLACED,
                                              &NumModes, pDisplayModeList);
  if (FAILED(Result)) {
    return false;
  }

  // Go through all display modes and find the on that matches the screen with and height
  // When a match is found, store the numerator and denominator of the refresh rate for that monitor
  unsigned int Numerator, Denominator;
  bool Found = false;
  for (unsigned int i = 0; i < NumModes; ++i) {
    // Multiple display modes will match, with different framerates. This should leave us with the highest (?)
    if (pDisplayModeList[i].Width == (unsigned int)ScreenWidth &&
        pDisplayModeList[i].Height == (unsigned int)ScreenHeight) {
      Numerator = pDisplayModeList[i].RefreshRate.Numerator;
      Denominator = pDisplayModeList[i].RefreshRate.Denominator;
      Found = true;
    }
  }
  if (!Found) {
    std::cerr
        << "Failed to find appropriate display mode for given screen size."
        << std::endl;
    return false;
  }

  // Get the adapter (video card) description
  DXGI_ADAPTER_DESC AdapterDesc;
  Result = pAdapter->GetDesc(&AdapterDesc);
  if (FAILED(Result)) {
    return false;
  }

  // Store the dedicated video card memory in megabytes
  _VideoCardMemoryInMb = (int)(AdapterDesc.DedicatedVideoMemory / 1024 / 1024);

  // Convert the name of the video card to a character array and store it
  size_t StringLength;
  int Error = wcstombs_s(&StringLength, _VideoCardDescription, 128,
                         AdapterDesc.Description, 128);

  delete[] pDisplayModeList;
  pDisplayModeList = nullptr;
  pAdapterOutput->Release();
  pAdapterOutput = nullptr;
  pAdapter->Release();
  pAdapter = nullptr;
  pFactory->Release();
  pFactory = nullptr;

  // Init swap chain description
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;
  ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
  // Set to single back buffer
  SwapChainDesc.BufferCount = 1;
  SwapChainDesc.BufferDesc.Width = ScreenWidth;
  SwapChainDesc.BufferDesc.Height = ScreenHeight;
  // Set regular 32-bit surface for the back buffer
  SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  // Set refresh rate of the back buffer
  if (_VSyncIsEnabled) {
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = Numerator;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = Denominator;
  } else {
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  }

  // Set the usage of the back buffer
  SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  // Set the handle for the window to render to
  SwapChainDesc.OutputWindow = Wnd;
  // Turn off multisampling
  SwapChainDesc.SampleDesc.Count = 1;
  SwapChainDesc.SampleDesc.Quality = 0;

  SwapChainDesc.Windowed = ShouldRunFullscreen ? false : true;

  // Set the scan line ordering and scaling to unspecified
  SwapChainDesc.BufferDesc.ScanlineOrdering =
      DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  // Discard backbuffer contents after presenting
  SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  // Don't set the advanced flags
  SwapChainDesc.Flags = 0;

  // Set feature level to DirectX 11.0
  D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
  UINT CreationFlags = _GfxDebugEnabled ? D3D11_CREATE_DEVICE_DEBUG : 0;
  // Create the swap chain, Direct3D device, and Direct3D device context
  Result = D3D11CreateDeviceAndSwapChain(
      nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, CreationFlags, &FeatureLevel,
      1, D3D11_SDK_VERSION, &SwapChainDesc, _pSwapChain.GetAddressOf(),
      _pDevice.GetAddressOf(), nullptr, _pDeviceContext.GetAddressOf());
  if (FAILED(Result)) {
    return false;
  }

  // Get ptr to back buffer
  ID3D11Texture2D* pBackBuffer;
  Result = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                  (LPVOID*)&pBackBuffer);
  if (FAILED(Result)) {
    return false;
  }
  // Create the render target view with the back buffer ptr
  Result = _pDevice->CreateRenderTargetView(pBackBuffer, nullptr,
                                            _pRenderTargetView.GetAddressOf());
  if (FAILED(Result)) {
    return false;
  }
  // Release ptr to the back buffer as we no longer need it.
  pBackBuffer->Release();
  pBackBuffer = nullptr;

  // Initialize the description of the depth buffer (including stencil buffer)
  D3D11_TEXTURE2D_DESC DepthBufferDesc;
  ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));
  DepthBufferDesc.Width = ScreenWidth;
  DepthBufferDesc.Height = ScreenHeight;
  DepthBufferDesc.MipLevels = 1;
  DepthBufferDesc.ArraySize = 1;
  DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  DepthBufferDesc.SampleDesc.Count = 1;
  DepthBufferDesc.SampleDesc.Quality = 0;
  DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  DepthBufferDesc.CPUAccessFlags = 0;
  DepthBufferDesc.MiscFlags = 0;

  // Create the texture for the depth buffer using the filled out description
  Result = _pDevice->CreateTexture2D(&DepthBufferDesc, nullptr,
                                     _pDepthStencilBuffer.GetAddressOf());
  if (FAILED(Result)) {
    return false;
  }

  // The depth stencil controls what type of depth test Direct3D will do for each pixel
  // Setup the depth stencil description
  D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
  ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));
  DepthStencilDesc.DepthEnable = true;
  DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
  DepthStencilDesc.StencilEnable = true;
  DepthStencilDesc.StencilReadMask = 0xFF;
  DepthStencilDesc.StencilWriteMask = 0xFF;
  // Stencil operations if pixel is front facing
  DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  // Stencil operations if pixel is back facing
  DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Create depth stencil
  Result = _pDevice->CreateDepthStencilState(
      &DepthStencilDesc, _pDepthStencilState.GetAddressOf());
  if (FAILED(Result)) {
    return false;
  }
  _pDeviceContext->OMSetDepthStencilState(_pDepthStencilState.Get(), 1);

  // Setup depth stencil view
  D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
  ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
  DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  DepthStencilViewDesc.Texture2D.MipSlice = 0;
  Result = _pDevice->CreateDepthStencilView(_pDepthStencilBuffer.Get(),
                                            &DepthStencilViewDesc,
                                            _pDepthStencilView.GetAddressOf());
  if (FAILED(Result)) {
    return false;
  }

  // Bind the render target view and depth stencil buffer to the output render pipeline
  _pDeviceContext->OMSetRenderTargets(1, _pRenderTargetView.GetAddressOf(),
                                      _pDepthStencilView.Get());

  // Setup the raster description which will determine how and what polygons will be drawn
  D3D11_RASTERIZER_DESC RasterDesc;
  RasterDesc.FillMode = D3D11_FILL_SOLID;
  RasterDesc.CullMode = D3D11_CULL_BACK;
  RasterDesc.FrontCounterClockwise = false;
  RasterDesc.DepthBias = 0;
  RasterDesc.DepthBiasClamp = 0.0f;
  RasterDesc.SlopeScaledDepthBias = 0.0f;
  RasterDesc.DepthClipEnable = true;
  RasterDesc.ScissorEnable = false;
  RasterDesc.MultisampleEnable = false;
  RasterDesc.AntialiasedLineEnable = false;
  // Create rasterizer state
  Result = _pDevice->CreateRasterizerState(&RasterDesc,
                                           _pRasterState.GetAddressOf());
  if (FAILED(Result)) {
    return false;
  }

  //Setup viewport
  _DefaultViewport.TopLeftX = 0.0f;
  _DefaultViewport.TopLeftY = 0.0f;
  _DefaultViewport.Width = (float)ScreenWidth;
  _DefaultViewport.Height = (float)ScreenHeight;
  _DefaultViewport.MinDepth = 0.0f;
  _DefaultViewport.MaxDepth = 1.0f;
  // Create viewport
  _pDeviceContext->RSSetViewports(1, &_DefaultViewport);

  return true;
}

void CDirectX3D::Shutdown() {
  // Set to windowed mode before shutdown, or swap chain will throw exception upon release
  if (_pSwapChain) {
    _pSwapChain->SetFullscreenState(false, nullptr);
  }
}

void CDirectX3D::BeginScene(float Red, float Green, float Blue, float Alpha) {
  float Color[4];
  Color[0] = Red;
  Color[1] = Green;
  Color[2] = Blue;
  Color[3] = Alpha;

  // Clear the back buffer.
  _pDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), Color);
  // Clear the depth buffer.
  _pDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(),
                                         D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CDirectX3D::EndScene() {
  unsigned int SyncInterval = _VSyncIsEnabled ? 1 : 0;
  _pSwapChain->Present(SyncInterval, 0);
}

ID3D11Device* CDirectX3D::AccessDevice() {
  return _pDevice.Get();
}

ID3D11DeviceContext* CDirectX3D::AccessDeviceContext() {
  return _pDeviceContext.Get();
}

ID3D11RenderTargetView* CDirectX3D::AccessRenderTargetView() {
  return _pRenderTargetView.Get();
}

ID3D11RenderTargetView** CDirectX3D::AccessAddrOfRenderTargetView() {
  return _pRenderTargetView.GetAddressOf();
}

ID3D11DepthStencilView* CDirectX3D::AccessDepthStencilView() {
  return _pDepthStencilView.Get();
}

D3D11_VIEWPORT CDirectX3D::GetViewport() const {
  return _DefaultViewport;
}

void CDirectX3D::GetVideoCardInfo(char* CardName, int& VRamInMb) {
  strcat_s(CardName, 128, _VideoCardDescription);
  VRamInMb = _VideoCardMemoryInMb;
}
