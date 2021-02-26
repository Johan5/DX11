#include "model.h"

#include <vector>
#include "3d_math.h"

namespace
{
	std::vector<CModelInterface::SVertex> GenerateCubeModelVertices()
	{
		std::vector<CModelInterface::SVertex> ModelData{ 36 };
		ModelData[0]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[0]._Color = CVector4f{ 1.0f, 0.0f, 0.0f, 1.0f };
		ModelData[1]._Position = CVector3f{ -1.0f, -1.0f, 1.0f };
		ModelData[1]._Color = CVector4f{ 1.0f, 0.0f, 0.0f, 1.0f };
		ModelData[2]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[2]._Color = CVector4f{ 1.0f, 0.0f, 0.0f, 1.0f };
		
		ModelData[3]._Position = CVector3f{ 1.0f, 1.0f, -1.0f };
		ModelData[3]._Color = CVector4f{ 1.0f, 1.0f, 0.0f, 1.0f };
		ModelData[4]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[4]._Color = CVector4f{ 1.0f, 1.0f, 0.0f, 1.0f };
		ModelData[5]._Position = CVector3f{ -1.0f, 1.0f, -1.0f, };
		ModelData[5]._Color = CVector4f{ 1.0f, 1.0f, 0.0f, 1.0f };
		
		ModelData[6]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[6]._Color = CVector4f{ 0.0f, 0.0f, 1.0f, 1.0f };
		ModelData[7]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[7]._Color = CVector4f{ 0.0f, 0.0f, 1.0f, 1.0f };
		ModelData[8]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[8]._Color = CVector4f{ 0.0f, 0.0f, 1.0f, 1.0f };
		
		ModelData[9]._Position = CVector3f{ 1.0f, 1.0f, -1.0f };
		ModelData[9]._Color = CVector4f{ 0.0f, 1.0f, 1.0f, 1.0f };
		ModelData[10]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[10]._Color = CVector4f{ 0.0f, 1.0f, 1.0f, 1.0f };
		ModelData[11]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[11]._Color = CVector4f{ 0.0f, 1.0f, 1.0f, 1.0f };
		
		ModelData[12]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[12]._Color = CVector4f{ 1.0f, 0.0f, 1.0f, 1.0f };
		ModelData[13]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[13]._Color = CVector4f{ 1.0f, 0.0f, 1.0f, 1.0f };
		ModelData[14]._Position = CVector3f{ -1.0f, 1.0f, -1.0f };
		ModelData[14]._Color = CVector4f{ 1.0f, 0.0f, 1.0f, 1.0f };
		
		ModelData[15]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[15]._Color = CVector4f{ 0.3f, 0.8f, 0.3f, 1.0f };
		ModelData[16]._Position = CVector3f{ -1.0f, -1.0f, 1.0f };
		ModelData[16]._Color = CVector4f{ 0.3f, 0.8f, 0.3f, 1.0f };
		ModelData[17]._Position = CVector3f{ -1.0f, -1.0f, -1.0f };
		ModelData[17]._Color = CVector4f{ 0.3f, 0.8f, 0.3f, 1.0f };
		
		ModelData[18]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[18]._Color = CVector4f{ 0.8f, 0.3f, 0.3f, 1.0f };
		ModelData[19]._Position = CVector3f{ -1.0f, -1.0f, 1.0f };
		ModelData[19]._Color = CVector4f{ 0.8f, 0.3f, 0.3f, 1.0f };
		ModelData[20]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[20]._Color = CVector4f{ 0.8f, 0.3f, 0.3f, 1.0f };
		
		ModelData[21]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[21]._Color = CVector4f{ 0.3f, 0.3f, 0.8f, 1.0f };
		ModelData[22]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[22]._Color = CVector4f{ 0.3f, 0.3f, 0.8f, 1.0f };
		ModelData[23]._Position = CVector3f{ 1.0f, 1.0f, -1.0f };
		ModelData[23]._Color = CVector4f{ 0.3f, 0.3f, 0.8f, 1.0f };
		
		ModelData[24]._Position = CVector3f{ 1.0f, -1.0f, -1.0f };
		ModelData[24]._Color = CVector4f{ 0.7f, 0.7f, 0.4f, 1.0f };
		ModelData[25]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[25]._Color = CVector4f{ 0.7f, 0.7f, 0.4f, 1.0f };
		ModelData[26]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[26]._Color = CVector4f{ 0.7f, 0.7f, 0.4f, 1.0f };
		
		ModelData[27]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[27]._Color = CVector4f{ 0.4f, 0.7f, 0.7f, 1.0f };
		ModelData[28]._Position = CVector3f{ 1.0f, 1.0f, -1.0f };
		ModelData[28]._Color = CVector4f{ 0.4f, 0.7f, 0.7f, 1.0f };
		ModelData[29]._Position = CVector3f{ -1.0f, 1.0f, -1.0f };
		ModelData[29]._Color = CVector4f{ 0.4f, 0.7f, 0.7f, 1.0f };

		ModelData[30]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[30]._Color = CVector4f{ 0.7f, 0.4f, 0.7f, 1.0f };
		ModelData[31]._Position = CVector3f{ -1.0f, 1.0f, -1.0f };
		ModelData[31]._Color = CVector4f{ 0.7f, 0.4f, 0.7f, 1.0f };
		ModelData[32]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[32]._Color = CVector4f{ 0.7f, 0.4f, 0.7f, 1.0f };
		
		ModelData[33]._Position = CVector3f{ 1.0f, 1.0f, 1.0f };
		ModelData[33]._Color = CVector4f{ 0.9f, 0.9f, 0.9f, 1.0f };
		ModelData[34]._Position = CVector3f{ -1.0f, 1.0f, 1.0f };
		ModelData[34]._Color = CVector4f{ 0.9f, 0.9f, 0.9f, 1.0f };
		ModelData[35]._Position = CVector3f{ 1.0f, -1.0f, 1.0f };
		ModelData[35]._Color = CVector4f{ 0.9f, 0.9f, 0.9f, 1.0f };

		return ModelData;
	}
}


CCubeModel::~CCubeModel()
{
	CCubeModel::Shutdown();
}

void CCubeModel::SetPosition(const CVector3f& NewPosition)
{
	_Position = NewPosition;
	_LocalToWorldTransformIsStale = true;
}

void CCubeModel::SetScale(const CVector3f& NewScale)
{
	_Scale = NewScale;
	_LocalToWorldTransformIsStale = true;
}

bool CCubeModel::Initialize( ID3D11Device* pDevice )
{
	std::vector<SVertex> Vertices = GenerateCubeModelVertices();
	
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

void CCubeModel::Shutdown()
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

void CCubeModel::PrepForRender(ID3D11DeviceContext* pDeviceContext) const
{
	unsigned int Stride = sizeof( SVertex );
	unsigned int Offset = 0U;
	pDeviceContext->IASetVertexBuffers( 0, 1, &_pVertexBuffer, &Stride, &Offset );
	pDeviceContext->IASetIndexBuffer( _pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	// Set type of primitive that should be rendered (i.e. triangles)
	pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

int CCubeModel::GetNumVertices() const
{
	return _NumVertices;
}

const CMatrix4x4f& CCubeModel::GetLocalToWorldTransform() const
{
	if ( _LocalToWorldTransformIsStale )
	{
		// TODO: Apply scale
		_LocalToWorldTransform = N3DMath::CreateCoordinateTransform( _Position, _Forward, _Up, N3DMath::ECoordinateTransformType::LocalToWorld );
		_LocalToWorldTransformIsStale = false;
	}
	return _LocalToWorldTransform;
}

