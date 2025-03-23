#pragma once

#include "core/matrix.h"
#include "index_buffer.h"
#include "vertex_buffer.h"

#include <optional>
#include <vector>

enum class EMeshType { Cube, Sphere, XBot };

struct SMesh {
  bool operator==(const SMesh& Other) const;
  bool operator!=(const SMesh& Other) const;

  // This transform gives vertices into (root) bone space. Think of it as attaching the vertex to its bone.
  // Index with boneId.
  std::optional<std::vector<CMatrix4x4f>> _OffsetMatrices;
  // This transforms vertices from bone space back to model space, assuming the T-pose.
  // This will move the vertex according to all bone positions in the bone hierarchy.
  // Index with boneId
  std::optional<std::vector<CMatrix4x4f>> _GlobalBindPoseTransforms;
  CVertexBuffer _VertexBuffer;
  CIndexBuffer _IndexBuffer;
  EMeshType _MeshType;
};