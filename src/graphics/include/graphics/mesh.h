#pragma once

#include "index_buffer.h"
#include "vertex_buffer.h"

enum class EMeshType { Cube, Sphere };

struct SMesh {
  bool operator==(const SMesh& Other) const;
  bool operator!=(const SMesh& Other) const;

  CVertexBuffer _VertexBuffer;
  CIndexBuffer _IndexBuffer;
  EMeshType _MeshType;
};