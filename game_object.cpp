#include "game_object.h"

#include "matrix.h"


bool CGameObject::IsInitialized() const
{
	return false;
}

void CGameObject::SetId( int64_t NewId )
{
	_Id = NewId;
}