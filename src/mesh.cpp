#include "mesh.h"


bool SMesh::operator==(const SMesh& Other) const
{
	return _MeshType == Other._MeshType;
}

bool SMesh::operator!=(const SMesh& Other) const
{
	return !(*this == Other);
}