#pragma once

#include <Windows.h>
#include <memory>

#include "game_application.h"
#include "graphics/graphics.h"
#include "input/input_handler.h"

class CSystem {
 public:
  CSystem() = default;
  CSystem(const CSystem&) = delete;

  bool Initialize();
  void Shutdown();
  void Run();

  LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

 private:
  bool Frame();
  void InitializeWindows(int&, int&);
  void ShutdownWindows();

 private:
  LPCWSTR _ApplicationName;
  HINSTANCE _Instance;
  HWND _Wnd;

  std::unique_ptr<CInputHandler> _InputHandler;
  std::unique_ptr<CGraphics> _Graphics;
  std::unique_ptr<CGameApplication> _GameApplication;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static CSystem* pApplicationHandle = nullptr;
