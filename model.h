#pragma once

#include <d3d11.h>

#include "vector.h"
#include "matrix.h"


/// <summary>
/// This class encapsulates the common geometry required to render 3D models
/// </summary>
class CModelInterface
{
public:
	struct SVertex
	{
		CVector3f _Position;
		CVector4f _Color;
	};
	
	virtual ~CModelInterface() = default;
	
	virtual bool Initialize( ID3D11Device* pDevice ) = 0;
	virtual void Shutdown() = 0;

	virtual void PrepForRender( ID3D11DeviceContext* pDeviceContext ) const = 0;

	virtual int GetNumVertices() const = 0;
	virtual const CMatrix4x4f& GetLocalToWorldTransform() const = 0;
};

class CCubeModel : public CModelInterface
{
public:
	~CCubeModel() override;

	void SetPosition( const CVector3f& NewPosition );
	void SetScale( const CVector3f& NewScale );
	
	bool Initialize( ID3D11Device* pDevice ) override;
	void Shutdown() override;

	void PrepForRender( ID3D11DeviceContext* pDeviceContext ) const override;

	int GetNumVertices() const override;
	const CMatrix4x4f& GetLocalToWorldTransform() const override;
	
private:
	CVector3f _Position;
	CVector3f _Scale = CVector3f{ 1.0f, 1.0f, 1.0f };
	// Normalized, in world coords
	//CVector3f _Forward = CVector3f::Forward();
	CVector3f _Forward = CVector3f{ 0.8f, 1.0f, 0.0f }.CalcNormalized();
	// Normalized, in world coords
	// CVector3f _Up = CVector3f::Up();
	CVector3f _Up = CVector3f{ 0.8f, 0.0f, 1.0f }.CalcNormalized();

	// these are just cached values
	mutable bool _LocalToWorldTransformIsStale = true;
	mutable CMatrix4x4f _LocalToWorldTransform;
	
	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;
	int _NumVertices = 0;
};
