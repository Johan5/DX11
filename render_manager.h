#pragma once


#include "graphics.h"
#include "render_packet.h"

#include <stdint.h>

class CRenderManager
{
public:
	struct LocalCbData
	{
		void clear()
		{
			_BatchSize = 0;
			_CbData.clear();
		}

		uint32_t _BatchSize = 0;
		std::vector<uint8_t> _CbData;
	};

	void Initialize(CGraphics& Graphics);
	void RenderInstanced(CRenderContext& RenderContext, CGraphics& Graphics, ERenderPass Pass);
	void QueForInstancedRendering(const SRenderPacket& Packet, ERenderPass Pass);

private:
	void UpdateContext(CRenderContext& Context, CGraphics& Graphics, SRenderPacket& CurrPacket, SRenderPacket* pPrevPacket) const;
	void RenderBatch(CRenderContext& RenderContext, const LocalCbData& CbData, const SRenderPacket& LastPacket);

	std::vector<SRenderPacket> _NormalRenderQue;
	std::vector<SRenderPacket> _ShadowRenderQue;
	CConstantBuffer _ConstantBuffer;

	static inline uint32_t _CbSize = 32768;
};