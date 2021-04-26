#include "light_source.h"

#include "misc_math.h"
#include "assert.h"

namespace
{
	const std::string VertexShaderFileName = "light_source.vs";
	const std::string PixelShaderFileName = "light_source.ps";
	const std::string VertexShaderMainFunction = "LightSourceVertexShader";
	const std::string PixelShaderMainFunction = "LightSourcePixelShader";

	std::vector<CGameObject::STypicalVertex> GenerateSphereModelVertices(int32_t LatitudeLineCount, int32_t LongitudeLineCount)
	{
		auto CalcPos = [LatitudeLineCount, LongitudeLineCount](int32_t LatIdx, int32_t LongIdx) -> CVector3f
		{
			// LatitudeLineCount is number of horizonital lines 
			// Calculate spherical coordinates and then translate to cartesian
			using namespace NMiscMath;
			float AzimuthalAngle = 2 * Pi * (LongIdx / (float)(LongitudeLineCount));
			// minus one in denominator so that we hit both ends in [0, Pi] interval
			float PolarAngle = Pi * (LatIdx / (float)(LatitudeLineCount - 1));
			float X = sin(PolarAngle) * cos(AzimuthalAngle);
			float Y = cos(PolarAngle);
			float Z = sin(PolarAngle) * sin(AzimuthalAngle);
			return CVector3f{ X, Y, Z };
		};

		std::vector< CGameObject::STypicalVertex > Vertices;
		CVector3f TopPos{ 0.0f, 1.0f, 0.0f };
		std::vector<CVector3f> PrevLat(LongitudeLineCount, TopPos);

		for (int32_t LatIdx = 1; LatIdx < LatitudeLineCount; ++LatIdx)
		{
			std::vector<CVector3f> ThisLat;
			for (int32_t LongIdx = 0; LongIdx < LongitudeLineCount; ++LongIdx)
			{
				const CVector3f& PointUp = PrevLat[LongIdx];
				const CVector3f& PointUpRight = PrevLat[ (LongIdx + 1) % LongitudeLineCount];
				CVector3f PointThis = CalcPos(LatIdx, LongIdx);
				CVector3f PointRight = CalcPos(LatIdx, LongIdx + 1);

				ThisLat.push_back(PointThis);
				if (LatIdx > 1)
				{
					Vertices.push_back(CGameObject::STypicalVertex{ PointThis, PointThis });
					Vertices.push_back(CGameObject::STypicalVertex{ PointUp, PointUp });
					Vertices.push_back(CGameObject::STypicalVertex{ PointUpRight, PointUpRight });
				}
				if (LatIdx < LatitudeLineCount - 1)
				{
					Vertices.push_back(CGameObject::STypicalVertex{ PointThis, PointThis });
					Vertices.push_back(CGameObject::STypicalVertex{ PointUpRight, PointUpRight });
					Vertices.push_back(CGameObject::STypicalVertex{ PointRight, PointRight });
				}
			}
			PrevLat = std::move(ThisLat);
		}

		return Vertices;
	}
}


void CLightSource::Initialize(CGraphics& Graphics)
{
	int32_t LatitudeLineCount = 7;
	int32_t LongitudeLineCount = 7;

	_Vertices = GenerateSphereModelVertices(LatitudeLineCount, LongitudeLineCount);

	uint32_t VertexBufferSize = static_cast<uint32_t>(_Vertices.size() * sizeof(CGameObject::STypicalVertex));
	SVertexBufferProperties Properties;
	Properties._VertexDataSizeInBytes = VertexBufferSize;
	Properties._SingleVertexSizeInBytes = sizeof(CGameObject::STypicalVertex);
	_VertexBuffer = Graphics.CreateVertexBuffer(_Vertices.data(), VertexBufferSize, Properties);

	_ConstantBuffer = Graphics.CreateConstantBuffer(sizeof(SConstantBuffer), ECpuAccessPolicy::CpuWrite);

	std::vector<SShaderInputDescription> InputLayout;
	InputLayout.push_back(SShaderInputDescription{ "POSITION", EGfxResourceDataFormat::R32G32B32Float });
	InputLayout.push_back(SShaderInputDescription{ "NORMAL", EGfxResourceDataFormat::R32G32B32Float });
	_VertexShader = Graphics.CreateVertexShader(VertexShaderFileName, VertexShaderMainFunction, InputLayout);
	_PixelShader = Graphics.CreatePixelShader(PixelShaderFileName, PixelShaderMainFunction);

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

void CLightSource::Render(CRenderContext& RenderContext, const CCameraBase& Camera)
{
	ASSERT(_Vertices.size() > 0, "Trying to render 0 vertices?");
	RenderContext.SetVertexBuffer(_VertexBuffer);
	{
		SConstantBuffer CB;
		CB._ModelToWorld = GetLocalToWorldTransform();
		CB._NormalModelToWorld = GetNormalLocalToWorldTransform();
		RenderContext.UpdateConstantBuffer(_ConstantBuffer, &CB, sizeof(SConstantBuffer));
	}
	RenderContext.SetVertexShaderConstantBuffer(_ConstantBuffer, EConstantBufferIdx::PerObject);
	RenderContext.SetVertexShader(_VertexShader);
	RenderContext.SetPixelShader(_PixelShader);
	RenderContext.Draw(static_cast<int32_t>(_Vertices.size()));
}
