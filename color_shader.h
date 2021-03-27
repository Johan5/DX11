//#pragma once
//
//#include <d3d11.h>
//#include <d3dcompiler.h>
//#include <fstream>
//
//#include "matrix.h"
//
//
///// <summary>
///// This class is responsible for handling of the color shader. 
///// </summary>
//class CColorShader
//{
//private:
//	struct SMatrixCb
//	{
//		CMatrix4x4f _World;
//		CMatrix4x4f _ViewAndProjection;
//	};
//public:
//	CColorShader() = default;
//	CColorShader( const CColorShader& Other ) = delete;
//	~CColorShader();
//
//	bool Initialize( ID3D11Device* pDevice, HWND Wnd );
//	void Shutdown();
//	bool Render( ID3D11DeviceContext* pDeviceContext, int IndexCount, const CMatrix4x4f& WorldMatrix, const CMatrix4x4f& ViewAndProjectionMatrix );
//
//private:
//	bool InitializeShader( ID3D11Device* pDevice, HWND Wnd, const std::wstring& VsFilename, const std::wstring& PsFilename );
//	void ShutdownShader();
//	void OutputShaderErrorMessage( ID3D10Blob* pErrorMessage, HWND Wnd, const std::wstring& ShaderFilename);
//
//	bool SetShaderParameters( ID3D11DeviceContext* pDeviceContext, const CMatrix4x4f& WorldMatrix, const CMatrix4x4f& ViewAndProjectionMatrix );
//	void RenderShader( ID3D11DeviceContext* pDeviceContext, int IndexCount );
//
//private:
//	ID3D11VertexShader* _pVertexShader = nullptr;
//	ID3D11PixelShader* _pPixelShader = nullptr;
//	ID3D11InputLayout* _pLayout = nullptr;
//	ID3D11Buffer* _pMatrixBuffer = nullptr;
//};
//
//
