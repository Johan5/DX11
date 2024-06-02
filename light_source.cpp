#include "light_source.h"

#include "misc_math.h"
#include "assert.h"

#include <cmath>

namespace
{
	std::vector<CLightSource::SVertex> GenerateSphereModelVertices(int32_t LatitudeLineCount, int32_t LongitudeLineCount)
	{
		auto CalcPos = [LatitudeLineCount, LongitudeLineCount](int32_t LatIdx, int32_t LongIdx) -> CVector3f
		{
			// LatitudeLineCount is number of horizonital lines 
			// Calculate spherical coordinates and then translate to cartesian
			using namespace NMiscMath;
			float AzimuthalAngle = 2 * Pi * (LongIdx / (float)(LongitudeLineCount));
			// minus one in denominator so that we hit both ends in [0, Pi] interval
			float PolarAngle = Pi * (LatIdx / (float)(LatitudeLineCount - 1));
			float X = std::sin(PolarAngle) * std::cos(AzimuthalAngle);
			float Y = std::cos(PolarAngle);
			float Z = std::sin(PolarAngle) * std::sin(AzimuthalAngle);
			return CVector3f{ X, Y, Z };
		};

		std::vector< CLightSource::SVertex > Vertices;
		CVector3f TopPos{ 0.0f, 1.0f, 0.0f };
		std::vector<CVector3f> PrevLat(LongitudeLineCount, TopPos);

		for (int32_t LatIdx = 1; LatIdx < LatitudeLineCount; ++LatIdx)
		{
			std::vector<CVector3f> ThisLat;
			for (int32_t LongIdx = 0; LongIdx < LongitudeLineCount; ++LongIdx)
			{
				const CVector3f& PointUp = PrevLat[LongIdx];
				const CVector3f& PointUpRight = PrevLat[(LongIdx + 1) % LongitudeLineCount];
				CVector3f PointThis = CalcPos(LatIdx, LongIdx);
				CVector3f PointRight = CalcPos(LatIdx, LongIdx + 1);

				ThisLat.push_back(PointThis);
				if (LatIdx > 1)
				{
					Vertices.push_back(CLightSource::SVertex{ PointThis, PointThis });
					Vertices.push_back(CLightSource::SVertex{ PointUp, PointUp });
					Vertices.push_back(CLightSource::SVertex{ PointUpRight, PointUpRight });
				}
				if (LatIdx < LatitudeLineCount - 1)
				{
					Vertices.push_back(CLightSource::SVertex{ PointThis, PointThis });
					Vertices.push_back(CLightSource::SVertex{ PointUpRight, PointUpRight });
					Vertices.push_back(CLightSource::SVertex{ PointRight, PointRight });
				}
			}
			PrevLat = std::move(ThisLat);
		}

		return Vertices;
	}
}


void CLightSource::Initialize(CGraphics& Graphics)
{
	_Material._VS = shader_names::DefaultVertexShaderFileName;
	_Material._PS = shader_names::DefaultPixelShaderFileName;
	SMesh* pMesh = Graphics.AccessMesh(EMeshType::Sphere);
	if (pMesh)
	{
		_Mesh = *pMesh;
	}
	else
	{
		ASSERT(false, "Failed to retrieve sphere mesh");
		return;
	}

	_CbData._ColorData._AmbientStrength = 1.0f;
	_CbData._ColorData._Color = CVector4f{ 0.9f, 0.9f, 0.9f, 1.0f };
	_IsInitialized = true;
}

void CLightSource::Shutdown()
{
	// TODO cleanup vertex buffer?
	_IsInitialized = false;
}

bool CLightSource::IsInitialized() const
{
	return _IsInitialized;
}

void CLightSource::Render(CRenderManager& RenderManager, const CCameraBase& Camera)
{
	_CbData._WorldMatrix = GetLocalToWorldTransform();
	_CbData._NormalWorldMatrix = GetNormalLocalToWorldTransform();
	SRenderPacket RenderPacket{ 
		._Mesh = _Mesh, 
		._Material = _Material, 
		._ConstantBufferData = SRawPtrConstantBufferData{._ConstantData = static_cast<void*>(&_CbData), ._ConstantDataByteSize = sizeof(_CbData)}
	};
	RenderManager.QueForInstancedRendering(RenderPacket, ERenderPass::Normal);
}
