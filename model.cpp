#include "model.h"


CModel::~CModel()
{
	Shutdown();
}


bool CModel::Initialize( ID3D11Device* pDevice, const std::vector<SVertex>& Vertices )
{
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = static_cast<UINT>( Vertices.size() * sizeof( SVertex ) );
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = Vertices.data();
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;
	HRESULT VertexResult = pDevice->CreateBuffer( &VertexBufferDesc, &VertexData, &_pVertexBuffer );
	if ( FAILED( VertexResult ) )
	{
		return false;
	}

	std::vector<unsigned long> Indices( Vertices.size() );
	for ( unsigned long i = 0; i < Indices.size(); ++i )
	{
		Indices[i] = i;
	}

	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = static_cast<UINT>( Indices.size() * sizeof( unsigned long ) );
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = Indices.data();
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;
	HRESULT IndexResult = pDevice->CreateBuffer( &IndexBufferDesc, &IndexData, &_pIndexBuffer );
	if ( FAILED( IndexResult ) )
	{
		return false;
	}

	_NumVertices = static_cast<UINT>( Vertices.size() );
	return true;
}

void CModel::Shutdown()
{
	if ( _pVertexBuffer )
	{
		_pVertexBuffer->Release();
		_pVertexBuffer = nullptr;
	}
	if ( _pIndexBuffer )
	{
		_pIndexBuffer->Release();
		_pIndexBuffer = nullptr;
	}
}

void CModel::PrepForRender(ID3D11DeviceContext* pDeviceContext) const
{
	unsigned int Stride = sizeof( SVertex );
	unsigned int Offset = 0U;
	pDeviceContext->IASetVertexBuffers( 0, 1, &_pVertexBuffer, &Stride, &Offset );
	pDeviceContext->IASetIndexBuffer( _pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	// Set type of primitive that should be rendered (i.e. triangles)
	pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

int CModel::GetNumVertices() const
{
	return _NumVertices;
}
