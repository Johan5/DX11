#pragma once

#include "inputclass.h"
#include "graphicsclass.h"

#include <windows.h>

#include <memory>

//class CInput;
//class CGraphics;


class CSystem
{
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

	std::unique_ptr<CInput> _Input;
	std::unique_ptr<CGraphics> _Graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static CSystem* pApplicationHandle = nullptr;



