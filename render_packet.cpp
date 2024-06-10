#include "render_packet.h"

bool CanUseSameDraw(const SRenderPacket& a, const SRenderPacket& b)
{
	//return false;
	return a._Mesh == b._Mesh && a._Material.CalcRenderHash() == b._Material.CalcRenderHash();
}