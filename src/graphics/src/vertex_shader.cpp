#include "graphics/vertex_shader.h"

#include "utils/assert.h"
#include "utils/logger.h"
#include "utils/string_util.h"

bool CVertexShader::Initialize(
    ID3D11Device& Device, const std::string& FileName,
    const std::string& ShaderMainFunction,
    std::vector<SShaderInputDescription>& ShaderInputLayout) {
  using Microsoft::WRL::ComPtr;
  HRESULT Result;
  std::wstring FileNameW = NStringUtil::StringToWString(FileName);

  ComPtr<ID3D10Blob> pCompiledVertexShader;
  ComPtr<ID3D10Blob> pErrorMessage;
  Result = D3DCompileFromFile(
      FileNameW.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, ShaderMainFunction.c_str(), "vs_5_0",
      D3D10_SHADER_ENABLE_STRICTNESS, 0, pCompiledVertexShader.GetAddressOf(),
      pErrorMessage.GetAddressOf());

  if (FAILED(Result)) {
    if (pErrorMessage) {
      std::string msg1 =
          std::string{"Failed to compile shader file "} + FileName;
      std::string msg2{static_cast<char*>(pErrorMessage->GetBufferPointer())};
      CLogger::Log(msg1 + " Error: " + msg2);
    } else {
      CLogger::LogFormat("Failed to find shader file: %s", FileName.c_str());
    }
    return false;
  }

  Result = Device.CreateVertexShader(pCompiledVertexShader->GetBufferPointer(),
                                     pCompiledVertexShader->GetBufferSize(),
                                     nullptr, _pVertexShader.GetAddressOf());
  ASSERT(SUCCEEDED(Result), "Failed to create vertex shader");

  constexpr int32_t MaxInputLayouts = 10;
  ASSERT(ShaderInputLayout.size() <= MaxInputLayouts,
         "Input layout is unreasonably large (wont work)");
  // Create input layout
  D3D11_INPUT_ELEMENT_DESC pPolygonLayout[MaxInputLayouts];
  for (int32_t i = 0; i < ShaderInputLayout.size(); ++i) {
    pPolygonLayout[i].SemanticName = ShaderInputLayout[i]._SemanticName.c_str();
    pPolygonLayout[i].SemanticIndex = 0;
    pPolygonLayout[i].Format =
        static_cast<DXGI_FORMAT>(ShaderInputLayout[i]._Format);
    pPolygonLayout[i].InputSlot = 0;
    pPolygonLayout[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    pPolygonLayout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    pPolygonLayout[i].InstanceDataStepRate = 0;
  }

  int32_t NumInputLayouts = static_cast<int32_t>(ShaderInputLayout.size());
  Result = Device.CreateInputLayout(pPolygonLayout, NumInputLayouts,
                                    pCompiledVertexShader->GetBufferPointer(),
                                    pCompiledVertexShader->GetBufferSize(),
                                    _pLayout.GetAddressOf());
  ASSERT(SUCCEEDED(Result), "Failed to create input layout");

  return true;
}

bool CVertexShader::IsInitialized() const {
  return _pVertexShader && _pLayout;
}
