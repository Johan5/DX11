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
constexpr size_t MAX_BONE_COUNT_PER_VERTEX = 4;

struct SCubeVertex {
  CVector3f _Position;
  CVector3f _Normal;
  CVector2f _UV;
};
struct STypicalVertex {
  CVector3f _Position;
  CVector3f _Normal;
};
struct SBoneVertex {
  CVector3f _Position;
  CVector3f _Normal;
  CVector2f _UV;
  // The indices of the bones that affect this vertex
  uint32_t _BoneIndices[MAX_BONE_COUNT_PER_VERTEX] = {};
  // The weight of each corresponding entry in _BoneIndices (sums to 1)
  float _BoneWeights[MAX_BONE_COUNT_PER_VERTEX] = {};

  void AddBone(uint32_t boneIdx, float weight) {
    size_t idx = GetNextFreeBoneIdx();
    _BoneIndices[idx] = boneIdx;
    _BoneWeights[idx] = weight;
  }

  size_t GetNextFreeBoneIdx() const {
    for (size_t i = 0; i < MAX_BONE_COUNT_PER_VERTEX; ++i) {
      if (_BoneWeights[i] == 0.0f) {
        return i;
      }
    }
    ASSERT(false, "Failed to find free bone idx.");
    return 0;
  }
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
      Indices.data(), static_cast<uint32_t>(Indices.size() * sizeof(uint32_t)),
      ECpuAccessPolicy::CpuWrite);
  return SMesh{std::nullopt, std::nullopt, VertexBuffer, IndexBuffer,
               EMeshType::Cube};
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
  return SMesh{std::nullopt, std::nullopt, VertexBuffer, CIndexBuffer{},
               EMeshType::Sphere};
}

bool hasMeshRecursively(const aiNode* pNode) {
  if (!pNode) {
    return false;
  }
  if (pNode->mNumMeshes > 0) {
    return true;
  }
  for (int32_t i = 0; i < pNode->mNumChildren; ++i) {
    if (hasMeshRecursively(pNode->mChildren[i])) {
      return true;
    }
  }
  return false;
}

CMatrix4x4f aiMatrixToCMatrix(const aiMatrix4x4& aiMatrix) {
  return CMatrix4x4f{aiMatrix.a1, aiMatrix.a2, aiMatrix.a3, aiMatrix.a4,
                     aiMatrix.b1, aiMatrix.b2, aiMatrix.b3, aiMatrix.b4,
                     aiMatrix.c1, aiMatrix.c2, aiMatrix.c3, aiMatrix.c4,
                     aiMatrix.d1, aiMatrix.d2, aiMatrix.d3, aiMatrix.d4};
}

template <typename T>
CVector3f vec3(const T& aiVec3f) {
  return CVector3f{aiVec3f.x, aiVec3f.y, aiVec3f.z};
}

struct MeshData {
  std::vector<SBoneVertex> vertices;
  std::vector<uint32_t> indices;
};

MeshData processMesh(
    const aiMesh& mesh,
    const std::unordered_map<std::string, uint32_t>& boneNameToIdMap) {
  MeshData data;

  data.vertices.resize(mesh.mNumVertices);

  // ## 1. Populate base vertex data (Position, Normal, UV)
  for (uint32_t i = 0; i < mesh.mNumVertices; ++i) {
    data.vertices[i]._Position = CVector3f(
        mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z);

    if (mesh.HasNormals()) {
      data.vertices[i]._Normal =
          CVector3f(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z);
    }

    // Check if the mesh has texture coordinates
    if (mesh.mTextureCoords[0]) {
      data.vertices[i]._UV =
          CVector2f(mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y);
    } else {
      data.vertices[i]._UV = CVector2f(0.0f, 0.0f);
    }
  }

  // ## 2. Populate bone influence data (Indices and Weights)
  for (uint32_t boneIndex = 0; boneIndex < mesh.mNumBones; ++boneIndex) {
    const aiBone* bone = mesh.mBones[boneIndex];
    std::string boneName = bone->mName.C_Str();

    // Find the final ID for this bone from the map you created earlier
    uint32_t boneId = boneNameToIdMap.at(boneName);

    // Add the bone's influence to each vertex it affects
    for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights;
         ++weightIndex) {
      const aiVertexWeight& weight = bone->mWeights[weightIndex];
      uint32_t vertexId = weight.mVertexId;
      float vertexWeight = weight.mWeight;

      // Call your AddBone method to populate the data
      data.vertices[vertexId].AddBone(boneId, vertexWeight);
    }
  }

  // ## 3. Populate Index Buffer Data
  data.indices.reserve(mesh.mNumFaces * 3);
  for (unsigned int i = 0; i < mesh.mNumFaces; ++i) {
    const aiFace& face = mesh.mFaces[i];
    ASSERT(face.mNumIndices == 3, "Found non-triangulated face");
    data.indices.push_back(face.mIndices[0]);
    data.indices.push_back(face.mIndices[1]);
    data.indices.push_back(face.mIndices[2]);
  }

  return data;
}

void buildBoneMap(const aiScene& scene,
                  std::unordered_map<std::string, uint32_t>& mapOut) {
  for (size_t meshIdx = 0; meshIdx < scene.mNumMeshes; ++meshIdx) {
    aiMesh* mesh = scene.mMeshes[meshIdx];
    for (size_t boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx) {
      aiBone* bone = mesh->mBones[boneIdx];
      std::string boneName = bone->mName.C_Str();
      const auto it = mapOut.find(boneName);
      if (it == mapOut.end()) {
        mapOut.try_emplace(boneName, mapOut.size());
      }
    }
  }
}

void calculateGlobalBindPoseTransforms(
    const aiNode* node, const CMatrix4x4f& parentTransform,
    const std::unordered_map<std::string, uint32_t>& boneNameToIdMap,
    std::vector<CMatrix4x4f>& bindPoseTransformsOut) {
  std::string nodeName = node->mName.C_Str();
  CMatrix4x4f localTransform = aiMatrixToCMatrix(node->mTransformation);
  CMatrix4x4f globalTransform = localTransform * parentTransform;

  // If this node is a bone that we care about, store its final transform
  const auto boneIdIt = boneNameToIdMap.find(nodeName);
  if (boneIdIt != boneNameToIdMap.end()) {
    bindPoseTransformsOut[boneIdIt->second] = globalTransform;
  }

  for (int32_t i = 0; i < node->mNumChildren; ++i) {
    calculateGlobalBindPoseTransforms(node->mChildren[i], globalTransform,
                                      boneNameToIdMap, bindPoseTransformsOut);
  }
}

SMesh LoadXBotMesh(CGraphics& Graphics) {
  // Create vertex buffer, index buffer, mesh type,
  Assimp::Importer importer;

  std::string characterFbxPath = "assets/Character/erika_archer.fbx";
  const aiScene* pScene = importer.ReadFile(
      characterFbxPath, aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder |
                            aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                            aiProcess_JoinIdenticalVertices |
                            aiProcess_SortByPType);

  if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !pScene->mRootNode || pScene->mNumMeshes == 0) {
    ASSERT(false, "Scene import failed");
    return SMesh{};
  }

  // Step 0: Scale down the model
  float scaleFactor = 0.01f;
  aiMatrix4x4 scaleMatrix;
  aiMatrix4x4::Scaling(aiVector3D(scaleFactor, scaleFactor, scaleFactor),
                       scaleMatrix);
  pScene->mRootNode->mTransformation =
      scaleMatrix * pScene->mRootNode->mTransformation;

  // Step 1: Discover all unique bones
  std::unordered_map<std::string, uint32_t> boneNameToIdMap;
  buildBoneMap(*pScene, boneNameToIdMap);

  // Step 2: Calculate the global bind pose transforms for those bones.
  std::vector<CMatrix4x4f> bindPoseTransforms;
  bindPoseTransforms.resize(boneNameToIdMap.size());
  // IMPORTANT: Start traversal from pScene->mRootNode with an identity matrix.
  calculateGlobalBindPoseTransforms(pScene->mRootNode, CMatrix4x4f::Identity(),
                                    boneNameToIdMap, bindPoseTransforms);

  // Step 3: Get the offsetMatrices by inverting the bindPoseTransforms
  std::vector<CMatrix4x4f> offsetMatrices;
  for (const CMatrix4x4f& bindPoseTransform : bindPoseTransforms) {
    std::optional<CMatrix4x4f> inverted = bindPoseTransform.CalcInvert();
    ASSERT(inverted, "Cannot invert bind pose, corrupt mesh?");
    offsetMatrices.emplace_back(inverted.value());
  }

  const aiMesh& mesh = *pScene->mMeshes[1];
  const auto& [BoneVertices, Indices] = processMesh(mesh, boneNameToIdMap);

  // Vertex buffer
  uint32_t VertexBufferSize =
      static_cast<uint32_t>(BoneVertices.size() * sizeof(SBoneVertex));
  SVertexBufferProperties Properties;
  Properties._VertexDataSizeInBytes = VertexBufferSize;
  Properties._SingleVertexSizeInBytes = sizeof(SBoneVertex);
  Properties._VertexCount = static_cast<uint32_t>(BoneVertices.size());
  CVertexBuffer VertexBuffer = Graphics.CreateVertexBuffer(
      BoneVertices.data(), VertexBufferSize, Properties);

  // Index buffer
  CIndexBuffer IndexBuffer = Graphics.CreateIndexBuffer(
      Indices.data(), static_cast<uint32_t>(Indices.size() * sizeof(uint32_t)),
      ECpuAccessPolicy::CpuWrite);

  return SMesh{offsetMatrices, bindPoseTransforms, VertexBuffer, IndexBuffer,
               EMeshType::XBot};
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