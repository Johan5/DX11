#include "model.h"


CModel::~CModel()
{
	Shutdown();
}


bool CModel::Initialize(ID3D11Device* pDevice)
{
	return InitializeBuffers( pDevice );
}

void CModel::Shutdown()
{
	ShutdownBuffers();
}

void CModel::Render(ID3D11DeviceContext* pDeviceContext)
{
	RenderBuffers(pDeviceContext);
}

int CModel::GetIndexCount()
{
	return _IndexCount;
}

////////////////////////////////////////////////////////////////

bool CModel::InitializeBuffers(ID3D11Device* pDevice)
{
	_VertexCount = 3;
	SVertex* pVertices = new SVertex[_VertexCount];
	{ // populate vertice data
		pVertices[0]._Position = XMFLOAT3( 0.0f, 0.0f, 5.0f ); // bot left
		//pVertices[0]._Color = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
		pVertices[1]._Position = XMFLOAT3( 0.5f, 1.0f, 5.0f ); // top
		//pVertices[1]._Color = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
		pVertices[2]._Position = XMFLOAT3( 1.0f, 0.0f, 5.0f ); // bot right
		//pVertices[2]._Color = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	}
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = _VertexCount * sizeof( SVertex );
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = pVertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;
	HRESULT VertexResult = pDevice->CreateBuffer( &VertexBufferDesc, &VertexData, &_pVertexBuffer );
	if ( FAILED(VertexResult) )
	{
		return false;
	}

	_IndexCount = 3;
	unsigned long* pIndices = new unsigned long[_IndexCount];
	{ // populate index data
		pIndices[0] = 0U;
		pIndices[1] = 1U;
		pIndices[2] = 2U;
	}
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = _IndexCount * sizeof( unsigned long );
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = pIndices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;
	HRESULT IndexResult = pDevice->CreateBuffer( &IndexBufferDesc, &IndexData, &_pIndexBuffer );
	if ( FAILED( IndexResult ) )
	{
		return false;
	}
	
	delete[] pVertices;
	delete[] pIndices;
	return true;
}

void CModel::ShutdownBuffers()
{
	if ( _pVertexBuffer )
	{
		_pVertexBuffer->Release();
		_pVertexBuffer = nullptr;
	}
	_VertexCount = 0;
	if ( _pIndexBuffer )
	{
		_pIndexBuffer->Release();
		_pIndexBuffer = nullptr;
	}
	_IndexCount = 0;
}

void CModel::RenderBuffers(ID3D11DeviceContext* pDeviceContext)
{
	unsigned int Stride = sizeof( SVertex );
	unsigned int Offset = 0U;
	pDeviceContext->IASetVertexBuffers( 0, 1, &_pVertexBuffer, &Stride, &Offset );
	pDeviceContext->IASetIndexBuffer( _pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	// Set type of primitive that should be rendered (i.e. triangles)
	pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}










