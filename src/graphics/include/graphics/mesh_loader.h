#pragma once

#include "mesh.h"

class CGraphics;

namespace mesh_loader {
SMesh LoadMesh(CGraphics& Graphics, EMeshType MeshType);
}