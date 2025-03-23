#pragma once


#include "graphics_enums.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "constant_buffer.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "geometry_shader.h"
#include "texture_view.h"
#include "render_target_view.h"
#include "d3dclass.h"
#include "sampler_state.h"

#include <cstdint>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft ComPtr

// The render context is the basic rendering API used by gameplay classes
class CRenderContext
{
public:
	void Initialize(CDirectX3D* pDirectX3D);

	void SetVertexBuffer(CVertexBuffer& VertexBuffer);
	void SetIndexBuffer(CIndexBuffer& IndexBuffer);
	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology);
	void SetVertexShaderConstantBuffer(CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index);
	void SetVertexShader(CVertexShader& VertexShader);
	void SetPixelShaderConstantBuffer(CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index);
	void SetPixelShader(CPixelShader& PixelShader);
	void SetGeometryShader(CGeometryShader& GeometryShader);
	void SetGeometryShaderConstantBuffer(CConstantBuffer& ConstantBuffer, EConstantBufferIdx Index);

	void SetPixelShaderTexture(CTextureView& TextureView, int32_t SlotIdx);
	void SetPixelShaderSampler(CSamplerState& SamplerState, int32_t SlotIdx);

	void SetRenderTarget(CRenderTargetView& RenderTarget);
	void SetRenderTargets(int32_t NumTargets, ID3D11RenderTargetView** ppRTVs, ID3D11DepthStencilView* pDSVs);
	void SetViewport(CVector2f NewSize);

	// Clears registered shaders
	void ClearShaders();
	void ClearVertexShader();
	void ClearGeometryShader();
	void ClearPixelShader();
	void ClearTextureSlot(int32_t SlotIdx);
	// Restores render target to the initial backbuffer
	void RestoreRenderTarget();
	void RestoreViewport();
	void UpdateVertexBuffer(CVertexBuffer& VertexBuffer, const void* pNewData, size_t NewDataSize);
	void UpdateIndexBuffer(CIndexBuffer& IndexBuffer, const void* pNewData, size_t NewDataSize);
	// optimally, ConstantBuffer data should be 16 byte aligned
	void UpdateConstantBuffer(CConstantBuffer& ConstantBuffer, const void* NewData, size_t NewDataSize);
	void Draw(int32_t VertexCount);
	void DrawIndexed(uint32_t IndexCount);
	void DrawInstanced(uint32_t VertexCountPerInstance, uint32_t InstanceCount);

	// TODO: Removes this
	CDirectX3D& Debug_AccessDxRaw() { return *_pDirectX3D; }
private:
	CDirectX3D* _pDirectX3D;
};