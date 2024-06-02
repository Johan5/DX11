#include "render_context.h"

#include "assert.h"

namespace
{
	void UpdateDx11Buffer(ID3D11DeviceContext& DeviceContext, ID3D11Buffer* pBuffer, const void* pNewData, size_t NewDataSize)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		ZeroMemory(&MappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		HRESULT Result = DeviceContext.Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		ASSERT(SUCCEEDED(Result), "Failed to update buffer");
		memcpy(MappedResource.pData, pNewData, NewDataSize);
		DeviceContext.Unmap(pBuffer, 0);
	}
}

void CRenderContext::Initialize(CDirectX3D* pDirectX3D)
{
	_pDirectX3D = pDirectX3D;
}

namespace
{
	struct DebugStruct
	{
		CVector3f _Position;
		CVector4f _Color;
	};
};

void CRenderContext::SetVertexBuffer(CVertexBuffer& VertexBuffer)
{
	ID3D11Buffer* pVertexBuffer = VertexBuffer.AccessVertexBuffer();
	ASSERT(pVertexBuffer, "Given vertex buffer is invalid");
	uint32_t SingleVertexSize = VertexBuffer.GetProperties()._SingleVertexSizeInBytes;
	ASSERT(SingleVertexSize > 0, "Creating vertex buffer with vertices of size 0?");
	uint32_t Stride = SingleVertexSize;
	uint32_t Offset = 0U;
	_pDirectX3D->AccessDeviceContext()->IASetVertexBuffers(0, 1, VertexBuffer.AccessVertexBufferAddr(), &Stride, &Offset);
}

void CRenderContext::SetIndexBuffer(CIndexBuffer& IndexBuffer)
{
	ID3D11Buffer* pIndexBuffer = IndexBuffer.AccessRawBuffer();
	ASSERT(pIndexBuffer, "Given vertex buffer is invalid");
	_pDirectX3D->AccessDeviceContext()->IASetIndexBuffer(pIndexBuffer, static_cast<DXGI_FORMAT>(EGfxResourceDataFormat::R32UInt), 0);
}

void CRenderContext::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology)
{
	_pDirectX3D->AccessDeviceContext()->IASetPrimitiveTopology(PrimitiveTopology);
}

void CRenderContext::SetVertexShaderConstantBuffer(CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index)
{
	ID3D11Buffer* pConstantBuffer = ConstantBuffer.AccessRawBuffer();
	_pDirectX3D->AccessDeviceContext()->VSSetConstantBuffers(static_cast<uint32_t>(Index), 1, &pConstantBuffer);
}

void CRenderContext::SetVertexShader(CVertexShader& VertexShader)
{
	_pDirectX3D->AccessDeviceContext()->IASetInputLayout(VertexShader.AccessInputLayout());
	_pDirectX3D->AccessDeviceContext()->VSSetShader(VertexShader.AccessVertexShader(), nullptr, 0);
}

void CRenderContext::SetPixelShaderConstantBuffer(CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index)
{
	ID3D11Buffer* pConstantBuffer = ConstantBuffer.AccessRawBuffer();
	_pDirectX3D->AccessDeviceContext()->PSSetConstantBuffers(static_cast<uint32_t>(Index), 1, &pConstantBuffer);
}

void CRenderContext::SetPixelShader(CPixelShader& PixelShader)
{
	_pDirectX3D->AccessDeviceContext()->PSSetShader(PixelShader.AccessPixelShader(), nullptr, 0);
}

void CRenderContext::SetGeometryShader(CGeometryShader& GeometryShader)
{
	_pDirectX3D->AccessDeviceContext()->GSSetShader(GeometryShader.AccessGeometryShader(), nullptr, 0);
}

void CRenderContext::SetGeometryShaderConstantBuffer(CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index)
{
	ID3D11Buffer* pConstantBuffer = ConstantBuffer.AccessRawBuffer();
	_pDirectX3D->AccessDeviceContext()->GSSetConstantBuffers(static_cast<uint32_t>(Index), 1, &pConstantBuffer);
}

void CRenderContext::SetPixelShaderTexture(CTextureView& TextureView, int32_t SlotIdx)
{
	ID3D11ShaderResourceView** ppTextureView = TextureView.AccessAddrOfTextureView();
	_pDirectX3D->AccessDeviceContext()->PSSetShaderResources(SlotIdx, 1, ppTextureView);
}

void CRenderContext::SetPixelShaderSampler(CSamplerState& SamplerState, int32_t SlotIdx)
{
	ID3D11SamplerState** ppSamplerState = SamplerState.AccessAddrOfSamplerState();
	_pDirectX3D->AccessDeviceContext()->PSSetSamplers(SlotIdx, 1, ppSamplerState);
}

void CRenderContext::SetRenderTarget(CRenderTargetView& RenderTarget)
{
	_pDirectX3D->AccessDeviceContext()->OMSetRenderTargets(1, RenderTarget.AccessAddrOfRenderTargetView(), _pDirectX3D->AccessDepthStencilView());
}

void CRenderContext::SetRenderTargets(int32_t NumTargets, ID3D11RenderTargetView** ppRTVs, ID3D11DepthStencilView* pDSVs)
{
	_pDirectX3D->AccessDeviceContext()->OMSetRenderTargets(NumTargets, ppRTVs, pDSVs);
}

void CRenderContext::SetViewport(CVector2f NewSize)
{
	D3D11_VIEWPORT Viewport;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;
	Viewport.Width = NewSize._X;
	Viewport.Height = NewSize._Y;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	_pDirectX3D->AccessDeviceContext()->RSSetViewports(1, &Viewport);
}

void CRenderContext::ClearShaders()
{
	ClearVertexShader();
	ClearGeometryShader();
	ClearPixelShader();
}

void CRenderContext::ClearVertexShader()
{
	_pDirectX3D->AccessDeviceContext()->VSSetShader(nullptr, nullptr, 0);
}

void CRenderContext::ClearGeometryShader()
{
	_pDirectX3D->AccessDeviceContext()->GSSetShader(nullptr, nullptr, 0);
}

void CRenderContext::ClearPixelShader()
{
	_pDirectX3D->AccessDeviceContext()->PSSetShader(nullptr, nullptr, 0);
}

void CRenderContext::ClearTextureSlot(int32_t SlotIdx)
{
	ID3D11ShaderResourceView* nullView[] = { nullptr };
	_pDirectX3D->AccessDeviceContext()->PSSetShaderResources(SlotIdx, 1, nullView);
}

void CRenderContext::RestoreRenderTarget()
{
	_pDirectX3D->AccessDeviceContext()->OMSetRenderTargets(1, _pDirectX3D->AccessAddrOfRenderTargetView(), _pDirectX3D->AccessDepthStencilView());
}

void CRenderContext::RestoreViewport()
{
	D3D11_VIEWPORT Viewport = _pDirectX3D->GetViewport();
	_pDirectX3D->AccessDeviceContext()->RSSetViewports(1, &Viewport);
}

void CRenderContext::UpdateVertexBuffer(CVertexBuffer& VertexBuffer, const void* pNewData, size_t NewDataSize)
{
	ASSERT(VertexBuffer.GetProperties()._VertexDataSizeInBytes == NewDataSize, "Size mismatch");
	UpdateDx11Buffer(*_pDirectX3D->AccessDeviceContext(), VertexBuffer.AccessVertexBuffer(), pNewData, NewDataSize);
}

void CRenderContext::UpdateIndexBuffer(CIndexBuffer& IndexBuffer, const void* pNewData, size_t NewDataSize)
{
	ASSERT(IndexBuffer.GetSizeInBytes() == NewDataSize, "Size mismatch, investigate.");
	ASSERT((uint32_t)IndexBuffer.GetAccessPolicy() | (uint32_t)ECpuAccessPolicy::CpuWrite, "Cannot update contents of buffer unless registered with CpuWrite");
	UpdateDx11Buffer(*_pDirectX3D->AccessDeviceContext(), IndexBuffer.AccessRawBuffer(), pNewData, NewDataSize);
}

void CRenderContext::UpdateConstantBuffer(CConstantBuffer& ConstantBuffer, const void* pNewData, size_t NewDataSize)
{
	ASSERT((uint32_t)ConstantBuffer.GetAccessPolicy() | (uint32_t)ECpuAccessPolicy::CpuWrite, "Cannot update contents of buffer unless registered with CpuWrite");
	UpdateDx11Buffer(*_pDirectX3D->AccessDeviceContext(), ConstantBuffer.AccessRawBuffer(), pNewData, NewDataSize);
}

void CRenderContext::Draw(int32_t VertexCount)
{
	_pDirectX3D->AccessDeviceContext()->Draw(VertexCount, 0);
}

void CRenderContext::DrawIndexed(uint32_t IndexCount)
{
	_pDirectX3D->AccessDeviceContext()->DrawIndexed(IndexCount, 0, 0);
}

void CRenderContext::DrawInstanced(uint32_t VertexCountPerInstance, uint32_t InstanceCount)
{
	_pDirectX3D->AccessDeviceContext()->DrawInstanced(VertexCountPerInstance, InstanceCount, 0, 0);
}