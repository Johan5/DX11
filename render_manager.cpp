#include "render_manager.h"

#include "assert.h"
#include "graphics_enums.h"

namespace
{
	void AppendConstantDataToVec(const SRenderPacket& RenderPacket, CRenderManager::LocalCbData& DataInOut)
	{
		const uint8_t* pData = static_cast<const uint8_t*>(RenderPacket._ConstantBufferData._ConstantData); // probably UB
		DataInOut._CbData.insert(DataInOut._CbData.end(), pData, pData + RenderPacket._ConstantBufferData._ConstantDataByteSize);
		DataInOut._BatchSize++;
	}
}

void CRenderManager::Initialize(CGraphics& Graphics)
{
	_ConstantBuffer = Graphics.CreateConstantBuffer(_CbSize, ECpuAccessPolicy::CpuWrite);
}

void CRenderManager::RenderInstanced(CRenderContext& RenderContext, CGraphics& Graphics, ERenderPass Pass)
{
	std::vector<SRenderPacket>& RenderQue = (Pass == ERenderPass::Normal ? _NormalRenderQue : _ShadowRenderQue);
	if (RenderQue.empty())
	{
		return;
	}

	//sort 

	LocalCbData CbData;
	SRenderPacket* pPrevPacket = nullptr;
	for (SRenderPacket& CurrPacket : RenderQue)
	{
		bool cbIsFull = (CbData._CbData.size() + CurrPacket._ConstantBufferData._ConstantDataByteSize) > _CbSize;
		bool ContextUpdateRequired = (pPrevPacket == nullptr) || !CanUseSameDraw(*pPrevPacket, CurrPacket) || cbIsFull;
		if (ContextUpdateRequired)
		{
			if (pPrevPacket)
			{
				// Render the previous batch, not including CurrPacket
				RenderBatch(RenderContext, CbData, *pPrevPacket);
				CbData.clear();
			}
			// Update Context for new batch, which starts with CurrPacket
			UpdateContext(RenderContext, Graphics, CurrPacket, pPrevPacket);
		}
		ASSERT(ContextUpdateRequired || !pPrevPacket || pPrevPacket->_ConstantBufferData._ConstantDataByteSize == CurrPacket._ConstantBufferData._ConstantDataByteSize, "Unexpected constant buffer size change");

		AppendConstantDataToVec(CurrPacket, CbData);

		pPrevPacket = &CurrPacket;
	}
	RenderQue.clear();
}

void CRenderManager::QueForInstancedRendering(const SRenderPacket& Packet, ERenderPass Pass)
{
	switch (Pass)
	{
	case ERenderPass::Normal:
		_NormalRenderQue.push_back(Packet);
		break;
	case ERenderPass::Shadow:
		_ShadowRenderQue.push_back(Packet);
		break;
	default:
		ASSERT(false, "This should never happen.");
		break;
	}
}

void CRenderManager::UpdateContext(CRenderContext& Context, CGraphics& Graphics, SRenderPacket& CurrPacket, SRenderPacket* pPrevPacket) const
{
	if (!pPrevPacket || pPrevPacket->_Mesh != CurrPacket._Mesh)
	{
		Context.SetVertexBuffer(CurrPacket._Mesh._VertexBuffer);
		if (CurrPacket._Mesh._IndexBuffer.IsValid())
		{
			Context.SetIndexBuffer(CurrPacket._Mesh._IndexBuffer);
		}
	}
	if (!pPrevPacket || pPrevPacket->_Material._VS != CurrPacket._Material._VS)
	{
		CVertexShader* pVS = Graphics.AccessVertexShader(CurrPacket._Material._VS);
		ASSERT(pVS, "Failed to find VertexShader");
		if (pVS)
		{
			Context.SetVertexShader(*pVS);
		}
	}
	if (!pPrevPacket || pPrevPacket->_Material._GS != CurrPacket._Material._GS)
	{
		CGeometryShader* pGS = Graphics.AccessGeometryShader(CurrPacket._Material._GS);
		if (pGS)
		{
			Context.SetGeometryShader(*pGS);
		}
	}
	if (!pPrevPacket || pPrevPacket->_Material._PS != CurrPacket._Material._PS)
	{
		CPixelShader* pPS = Graphics.AccessPixelShader(CurrPacket._Material._PS);
		ASSERT(pPS, "Failed to find PixelShader");
		if (pPS)
		{
			Context.SetPixelShader(*pPS);
		}
	}
}

void CRenderManager::RenderBatch(CRenderContext& RenderContext, const LocalCbData& CbData, const SRenderPacket& LastPacket)
{
	if (_ConstantBuffer.AccessRawBuffer() == nullptr)
	{
		ASSERT(false, "Trying to render with uninitialized RenderManager");
		return;
	}
	// 1. send CB data
	RenderContext.UpdateConstantBuffer(_ConstantBuffer, CbData._CbData.data(), CbData._CbData.size());
	RenderContext.SetVertexShaderConstantBuffer(_ConstantBuffer, EConstantBufferIdx::PerObject);
	RenderContext.SetPixelShaderConstantBuffer(_ConstantBuffer, EConstantBufferIdx::PerObject);
	// 2. Render.
	if (LastPacket._Mesh._VertexBuffer.GetProperties()._SingleVertexSizeInBytes == 0)
	{
		ASSERT(false, "Vertex size of 0");
		return;
	}
	if (CbData._BatchSize == 1)
	{
		RenderContext.Draw(LastPacket._Mesh._VertexBuffer.GetProperties()._VertexCount);
	}
	else
	{
		RenderContext.DrawInstanced(LastPacket._Mesh._VertexBuffer.GetProperties()._VertexCount, CbData._BatchSize);
	}
}