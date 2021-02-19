#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "vector.h"


/// <summary>
/// This class encapsulates the common geometry required to render 3D models
/// </summary>
class CModel
{
public:
	struct SVertex
	{
		CVector3f _Position;
		CVector4f _Color;
	};
	
public:
	~CModel();
	bool Initialize( ID3D11Device* pDevice, const std::vector<SVertex>& Vertices );
	void Shutdown();

	void PrepForRender( ID3D11DeviceContext* pDeviceContext ) const;

	int GetNumVertices() const;

private:
	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;
	int _NumVertices = 0;
};
