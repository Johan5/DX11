#pragma once

#include "core/vector.h"
#include "input_enums.h"

#include <cstdint>
#include <mutex>
#include <optional>
#include <vector>

struct SMouseClick {
  EInputCode _Input;
  CVector2f _ViewportPos;
};

struct SInputData {
  std::vector<EInputCode> _NewKeyDowns;
  std::vector<EInputCode> _NewKeyUps;
  std::vector<EInputCode> _CurrentlyHeldDown;
  std::vector<SMouseClick> _NewMouseClicks;
  std::vector<SMouseClick> _NewMouseReleases;
  // Raw mouse position has origin in top-left and spans [0, Resolution]
  CVector2f _RawMousePos;
  CVector2f _RawMousePosPrevFrame;
};

// Input is stored in order it was received, with oldest event first.
// Front buffer may be used by game main thread.
// Back buffer is invoked by OS.
class CDoubleBufferedInput {
  struct SKeyInput {
    EInputType _InputType = EInputType::None;
    EInputCode _InputCode = EInputCode::Unknown;
    // Typically only relevant for mouse input
    CVector2f _CoordinateData;
  };

 public:
  // Swaps the front and backbuffers. Clears the new back buffer, returns the new front buffer.
  const SInputData& SwapBuffers();
  // Mouse and Keyboard input
  void AddKeyInput(EInputType InputType, EInputCode InputCode);
  void AddMousePos(CVector2f MousePos);

 private:
  std::vector<SKeyInput> _KeyInput[2];
  CVector2f _MousePos[2];

  int32_t _FrontBufferIdx = 0;
  int32_t _BackBufferIdx = 1;

  std::optional<SInputData> _pPrevFrameData;

  // This mutex protects the back buffer data (and _BackBufferIdx)
  std::mutex _Mutex;
};