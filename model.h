#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// This class encapsulates the common geometry required to render 3D models
/// </summary>
class CModel
{
private:
	struct SVertex
	{
		XMFLOAT3 _Position;
		//XMFLOAT4 _Color;
	};
	
public:
	~CModel();
	bool Initialize( ID3D11Device* pDevice );
	void Shutdown();

	void Render( ID3D11DeviceContext* pDeviceContext );
	int GetIndexCount();

private:
	bool InitializeBuffers( ID3D11Device* pDevice );
	void ShutdownBuffers();
	void RenderBuffers( ID3D11DeviceContext* pDeviceContext );

private:
	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;
	int _VertexCount = 0;
	int _IndexCount = 0;
};
