#pragma once

#include "game_object.h"
#include "orthographic_camera.h"
#include "graphics.h"

#include <vector>
#include <memory>
#include <type_traits>


class CGraphics;

struct SCameraConstantBuffer
{
	CMatrix4x4f _ViewAndProjection;
};

class CWorld
{
public:	
	void Initialize( CGraphics& Graphics );

	void Update();
	void Render( CRenderContext& RenderContext );

	// Will create the object and run Initialize() on it.
	template< class CGameObjectDerived, class ...ArgsT>
	CGameObjectDerived* SpawnGameObject( ArgsT&&... Args );

private:
	void SpawnDefaultObjects();

	CGraphics* _pGraphics;
	std::unique_ptr<COrthographicCamera> _Camera;
	CConstantBuffer _CameraConstantBuffer;

	std::vector< std::unique_ptr<CGameObject> > _GameObjects;
	
	uint64_t _NextGameObjectId = 0;
};






template< class CGameObjectDerived, class ...ArgsT>
CGameObjectDerived* CWorld::SpawnGameObject( ArgsT&&... Args )
{
	static_assert( std::is_base_of< CGameObject, CGameObjectDerived >::value, "Can only spawn objects derived from CGameObject" );
	std::unique_ptr<CGameObjectDerived> Object = std::make_unique<CGameObjectDerived>( std::forward<ArgsT>( Args )... );
	Object->SetId( _NextGameObjectId++ );
	Object->Initialize( *_pGraphics );

	_GameObjects.push_back( std::move( Object ) );
	std::unique_ptr<CGameObject>& BasePtr = _GameObjects.back();
	return static_cast< CGameObjectDerived* >( BasePtr.get() );
}