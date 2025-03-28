#include "graphics/mesh_loader.h"

#include "core/misc_math.h"
#include "core/vector.h"
#include "graphics/graphics.h"
#include "utils/assert.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <random>
#include <vector>

namespace {
struct SCubeVertex {
  CVector3f _Position;
  CVector3f _Normal;
  CVector2f _UV;
};
struct STypicalVertex {
  CVector3f _Position;
  CVector3f _Normal;
};
struct SVertexWithBones {
  CVector3f _Position;
  CVector3f _Normal;
  CVector2f _UV;

};

std::vector<SCubeVertex> GenerateCubeModelVertices() {
  std::vector<SCubeVertex> ModelData{36};
  ModelData[0]._Position = CVector3f{-0.5f, -0.5f, -0.5f};  // LEFT
  ModelData[0]._Normal = CVector3f{-1.0f, 0.0f, 0.0f};
  ModelData[0]._UV = CVector2f{1.0f, 1.0f};
  ModelData[1]._Position = CVector3f{-0.5f, -0.5f, 0.5f};
  ModelData[1]._Normal = CVector3f{-1.0f, 0.0f, 0.0f};
  ModelData[1]._UV = CVector2f{0.0f, 1.0f};
  ModelData[2]._Position = CVector3f{-0.5f, 0.5f, 0.5f};
  ModelData[2]._Normal = CVector3f{-1.0f, 0.0f, 0.0f};
  ModelData[2]._UV = CVector2f{0.0f, 0.0f};

  ModelData[3]._Position = CVector3f{0.5f, 0.5f, -0.5f};  // BEHIND
  ModelData[3]._Normal = CVector3f{0.0f, 0.0f, -1.0f};
  ModelData[3]._UV = CVector2f{1.0f, 0.0f};
  ModelData[4]._Position = CVector3f{-0.5f, -0.5f, -0.5f};
  ModelData[4]._Normal = CVector3f{0.0f, 0.0f, -1.0f};
  ModelData[4]._UV = CVector2f{0.0f, 1.0f};
  ModelData[5]._Position = CVector3f{
      -0.5f,
      0.5f,
      -0.5f,
  };
  ModelData[5]._Normal = CVector3f{0.0f, 0.0f, -1.0f};
  ModelData[5]._UV = CVector2f{0.0f, 0.0f};

  ModelData[6]._Position = CVector3f{0.5f, -0.5f, 0.5f};  // DOWN
  ModelData[6]._Normal = CVector3f{0.0f, -1.0f, 0.0f};
  ModelData[6]._UV = CVector2f{1.0f, 1.0f};
  ModelData[7]._Position = CVector3f{-0.5f, -0.5f, -0.5f};
  ModelData[7]._Normal = CVector3f{0.0f, -1.0f, 0.0f};
  ModelData[7]._UV = CVector2f{0.0f, 0.0f};
  ModelData[8]._Position = CVector3f{0.5f, -0.5f, -0.5f};
  ModelData[8]._Normal = CVector3f{0.0f, -1.0f, 0.0f};
  ModelData[8]._UV = CVector2f{1.0f, 0.0f};

  ModelData[9]._Position = CVector3f{0.5f, 0.5f, -0.5f};  // BEHIND
  ModelData[9]._Normal = CVector3f{0.0f, 0.0f, -1.0f};
  ModelData[9]._UV = CVector2f{1.0f, 0.0f};
  ModelData[10]._Position = CVector3f{0.5f, -0.5f, -0.5f};
  ModelData[10]._Normal = CVector3f{0.0f, 0.0f, -1.0f};
  ModelData[10]._UV = CVector2f{1.0f, 1.0f};
  ModelData[11]._Position = CVector3f{-0.5f, -0.5f, -0.5f};
  ModelData[11]._Normal = CVector3f{0.0f, 0.0f, -1.0f};
  ModelData[11]._UV = CVector2f{0.0f, 1.0f};

  ModelData[12]._Position = CVector3f{-0.5f, -0.5f, -0.5f};  // LEFT
  ModelData[12]._Normal = CVector3f{-1.0f, 0.0f, 0.0f};
  ModelData[12]._UV = CVector2f{1.0f, 1.0f};
  ModelData[13]._Position = CVector3f{-0.5f, 0.5f, 0.5f};
  ModelData[13]._Normal = CVector3f{-1.0f, 0.0f, 0.0f};
  ModelData[13]._UV = CVector2f{0.0f, 0.0f};
  ModelData[14]._Position = CVector3f{-0.5f, 0.5f, -0.5f};
  ModelData[14]._Normal = CVector3f{-1.0f, 0.0f, 0.0f};
  ModelData[14]._UV = CVector2f{1.0f, 0.0f};

  ModelData[15]._Position = CVector3f{0.5f, -0.5f, 0.5f};  // DOWN
  ModelData[15]._Normal = CVector3f{0.0f, -1.0f, 0.0f};
  ModelData[15]._UV = CVector2f{1.0f, 1.0f};
  ModelData[16]._Position = CVector3f{-0.5f, -0.5f, 0.5f};
  ModelData[16]._Normal = CVector3f{0.0f, -1.0f, 0.0f};
  ModelData[16]._UV = CVector2f{0.0f, 1.0f};
  ModelData[17]._Position = CVector3f{-0.5f, -0.5f, -0.5f};
  ModelData[17]._Normal = CVector3f{0.0f, -1.0f, 0.0f};
  ModelData[17]._UV = CVector2f{0.0f, 0.0f};

  ModelData[18]._Position = CVector3f{-0.5f, 0.5f, 0.5f};  // IN FRONT
  ModelData[18]._Normal = CVector3f{0.0f, 0.0f, 1.0f};
  ModelData[18]._UV = CVector2f{0.0f, 1.0f};
  ModelData[19]._Position = CVector3f{-0.5f, -0.5f, 0.5f};
  ModelData[19]._Normal = CVector3f{0.0f, 0.0f, 1.0f};
  ModelData[19]._UV = CVector2f{0.0f, 0.0f};
  ModelData[20]._Position = CVector3f{0.5f, -0.5f, 0.5f};
  ModelData[20]._Normal = CVector3f{0.0f, 0.0f, 1.0f};
  ModelData[20]._UV = CVector2f{1.0f, 0.0f};

  ModelData[21]._Position = CVector3f{0.5f, 0.5f, 0.5f};  // RIGHT
  ModelData[21]._Normal = CVector3f{1.0f, 0.0f, 0.0f};
  ModelData[21]._UV = CVector2f{0.0f, 0.0f};
  ModelData[22]._Position = CVector3f{0.5f, -0.5f, -0.5f};
  ModelData[22]._Normal = CVector3f{1.0f, 0.0f, 0.0f};
  ModelData[22]._UV = CVector2f{1.0f, 1.0f};
  ModelData[23]._Position = CVector3f{0.5f, 0.5f, -0.5f};
  ModelData[23]._Normal = CVector3f{1.0f, 0.0f, 0.0f};
  ModelData[23]._UV = CVector2f{0.0f, 1.0f};

  ModelData[24]._Position = CVector3f{0.5f, -0.5f, -0.5f};  // RIGHT
  ModelData[24]._Normal = CVector3f{1.0f, 0.0f, 0.0f};
  ModelData[24]._UV = CVector2f{1.0f, 1.0f};
  ModelData[25]._Position = CVector3f{0.5f, 0.5f, 0.5f};
  ModelData[25]._Normal = CVector3f{1.0f, 0.0f, 0.0f};
  ModelData[25]._UV = CVector2f{0.0f, 0.0f};
  ModelData[26]._Position = CVector3f{0.5f, -0.5f, 0.5f};
  ModelData[26]._Normal = CVector3f{1.0f, 0.0f, 0.0f};
  ModelData[26]._UV = CVector2f{1.0f, 0.0f};

  ModelData[27]._Position = CVector3f{0.5f, 0.5f, 0.5f};  // UP
  ModelData[27]._Normal = CVector3f{0.0f, 1.0f, 0.0f};
  ModelData[27]._UV = CVector2f{1.0f, 0.0f};
  ModelData[28]._Position = CVector3f{0.5f, 0.5f, -0.5f};
  ModelData[28]._Normal = CVector3f{0.0f, 1.0f, 0.0f};
  ModelData[28]._UV = CVector2f{1.0f, 1.0f};
  ModelData[29]._Position = CVector3f{-0.5f, 0.5f, -0.5f};
  ModelData[29]._Normal = CVector3f{0.0f, 1.0f, 0.0f};
  ModelData[29]._UV = CVector2f{0.0f, 1.0f};

  ModelData[30]._Position = CVector3f{0.5f, 0.5f, 0.5f};  // UP
  ModelData[30]._Normal = CVector3f{0.0f, 1.0f, 0.0f};
  ModelData[30]._UV = CVector2f{1.0f, 0.0f};
  ModelData[31]._Position = CVector3f{-0.5f, 0.5f, -0.5f};
  ModelData[31]._Normal = CVector3f{0.0f, 1.0f, 0.0f};
  ModelData[31]._UV = CVector2f{0.0f, 1.0f};
  ModelData[32]._Position = CVector3f{-0.5f, 0.5f, 0.5f};
  ModelData[32]._Normal = CVector3f{0.0f, 1.0f, 0.0f};
  ModelData[32]._UV = CVector2f{0.0f, 0.0f};

  ModelData[33]._Position = CVector3f{0.5f, 0.5f, 0.5f};  // IN FRONT
  ModelData[33]._Normal = CVector3f{0.0f, 0.0f, 1.0f};
  ModelData[33]._UV = CVector2f{1.0f, 1.0f};
  ModelData[34]._Position = CVector3f{-0.5f, 0.5f, 0.5f};
  ModelData[34]._Normal = CVector3f{0.0f, 0.0f, 1.0f};
  ModelData[34]._UV = CVector2f{0.0f, 1.0f};
  ModelData[35]._Position = CVector3f{0.5f, -0.5f, 0.5f};
  ModelData[35]._Normal = CVector3f{0.0f, 0.0f, 1.0f};
  ModelData[35]._UV = CVector2f{1.0f, 0.0f};

  return ModelData;
}

std::vector<uint32_t> GenerateCubeModelIndices() {
  std::vector<uint32_t> indices;
  for (uint32_t i = 0; i < 36; ++i) {
    indices.push_back(i);
  }
  return indices;
}

std::vector<STypicalVertex> GenerateSphereModelVertices(
    int32_t LatitudeLineCount, int32_t LongitudeLineCount) {
  auto CalcPos = [LatitudeLineCount, LongitudeLineCount](
                     int32_t LatIdx, int32_t LongIdx) -> CVector3f {
    // LatitudeLineCount is number of horizonital lines
    // Calculate spherical coordinates and then translate to cartesian
    using namespace NMiscMath;
    float AzimuthalAngle = 2 * Pi * (LongIdx / (float)(LongitudeLineCount));
    // minus one in denominator so that we hit both ends in [0, Pi] interval
    float PolarAngle = Pi * (LatIdx / (float)(LatitudeLineCount - 1));
    float X = std::sin(PolarAngle) * std::cos(AzimuthalAngle);
    float Y = std::cos(PolarAngle);
    float Z = std::sin(PolarAngle) * std::sin(AzimuthalAngle);
    return CVector3f{X, Y, Z};
  };

  std::vector<STypicalVertex> Vertices;
  CVector3f TopPos{0.0f, 1.0f, 0.0f};
  std::vector<CVector3f> PrevLat(LongitudeLineCount, TopPos);

  for (int32_t LatIdx = 1; LatIdx < LatitudeLineCount; ++LatIdx) {
    std::vector<CVector3f> ThisLat;
    for (int32_t LongIdx = 0; LongIdx < LongitudeLineCount; ++LongIdx) {
      const CVector3f& PointUp = PrevLat[LongIdx];
      const CVector3f& PointUpRight =
          PrevLat[(LongIdx + 1) % LongitudeLineCount];
      CVector3f PointThis = CalcPos(LatIdx, LongIdx);
      CVector3f PointRight = CalcPos(LatIdx, LongIdx + 1);

      ThisLat.push_back(PointThis);
      if (LatIdx > 1) {
        Vertices.push_back(STypicalVertex{PointThis, PointThis});
        Vertices.push_back(STypicalVertex{PointUp, PointUp});
        Vertices.push_back(STypicalVertex{PointUpRight, PointUpRight});
      }
      if (LatIdx < LatitudeLineCount - 1) {
        Vertices.push_back(STypicalVertex{PointThis, PointThis});
        Vertices.push_back(STypicalVertex{PointUpRight, PointUpRight});
        Vertices.push_back(STypicalVertex{PointRight, PointRight});
      }
    }
    PrevLat = std::move(ThisLat);
  }

  return Vertices;
}

SMesh LoadCubeMesh(CGraphics& Graphics) {
  std::vector<SCubeVertex> Vertices = GenerateCubeModelVertices();
  std::vector<uint32_t> Indices = GenerateCubeModelIndices();

  uint32_t VertexBufferSize =
      static_cast<uint32_t>(Vertices.size() * sizeof(SCubeVertex));
  SVertexBufferProperties Properties;
  Properties._VertexDataSizeInBytes = VertexBufferSize;
  Properties._SingleVertexSizeInBytes = sizeof(SCubeVertex);
  Properties._VertexCount = static_cast<uint32_t>(Vertices.size());
  CVertexBuffer VertexBuffer = Graphics.CreateVertexBuffer(
      Vertices.data(), VertexBufferSize, Properties);
  CIndexBuffer IndexBuffer = Graphics.CreateIndexBuffer(
      static_cast<uint32_t>(Indices.size() * sizeof(uint32_t)),
      ECpuAccessPolicy::CpuWrite);
  return SMesh{VertexBuffer, IndexBuffer, EMeshType::Cube};
}

SMesh LoadSphereMesh(CGraphics& Graphics) {
  int32_t LatitudeLineCount = 21;
  int32_t LongitudeLineCount = 21;
  std::vector<STypicalVertex> Vertices =
      GenerateSphereModelVertices(LatitudeLineCount, LongitudeLineCount);
  uint32_t VertexBufferSize =
      static_cast<uint32_t>(Vertices.size() * sizeof(STypicalVertex));
  SVertexBufferProperties Properties;
  Properties._VertexDataSizeInBytes = VertexBufferSize;
  Properties._SingleVertexSizeInBytes = sizeof(STypicalVertex);
  Properties._VertexCount = static_cast<uint32_t>(Vertices.size());
  CVertexBuffer VertexBuffer = Graphics.CreateVertexBuffer(
      Vertices.data(), VertexBufferSize, Properties);
  return SMesh{VertexBuffer, CIndexBuffer{}, EMeshType::Sphere};
}

SMesh LoadXBotMesh(CGraphics& Graphics) {
  // Create vertex buffer, index buffer, mesh type,
  Assimp::Importer importer;

  std::string characterFbxPath = "assets/Character/XBot.fbx";
  const aiScene* scene = importer.ReadFile(
      characterFbxPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                            aiProcess_JoinIdenticalVertices |
                            aiProcess_SortByPType);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode || scene->mNumMeshes == 0) {
    ASSERT(false, "Scene import failed");
  }

  // https://vlad.website/game-engine-skeletal-animation/

}
}  // namespace

namespace mesh_loader {

SMesh LoadMesh(CGraphics& Graphics, EMeshType MeshType) {
  switch (MeshType) {
    case EMeshType::Cube: {
      static SMesh CubeMesh = LoadCubeMesh(Graphics);
      return CubeMesh;
    }
    case EMeshType::Sphere: {
      static SMesh SphereMesh = LoadSphereMesh(Graphics);
      return SphereMesh;
    }
    case EMeshType::XBot: {
      static SMesh XBotMesh = LoadXBotMesh(Graphics);
      return XBotMesh;
    }
  }
  ASSERT(false, "Mesh loading not implemented.");
  return SMesh{};
}

}  // namespace mesh_loader