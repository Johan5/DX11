#include "input/input_handler_types.h"

const SInputData& CDoubleBufferedInput::SwapBuffers() {
  {
    std::lock_guard<std::mutex> Lock{_Mutex};
    std::swap(_FrontBufferIdx, _BackBufferIdx);
    _KeyInput[_BackBufferIdx].clear();
    _MousePos[_BackBufferIdx] = _MousePos[_FrontBufferIdx];
  }

  static const std::vector<EInputCode> MouseInputCodes = {
      EInputCode::LeftMouseButton, EInputCode::RightMouseButton,
      EInputCode::MiddleMouseButton};
  auto IsMouseInput = [](EInputCode InputCode) {
    return std::find(MouseInputCodes.begin(), MouseInputCodes.end(),
                     InputCode) != MouseInputCodes.end();
  };

  SInputData InputData;

  if (_pPrevFrameData) {
    InputData._CurrentlyHeldDown = _pPrevFrameData->_CurrentlyHeldDown;
  }

  for (const SKeyInput& KeyInput : _KeyInput[_FrontBufferIdx]) {
    if (KeyInput._InputType == EInputType::KeyUp) {
      std::erase_if(InputData._CurrentlyHeldDown,
                    [ReleasedKey = KeyInput._InputCode](EInputCode InputCode) {
                      return InputCode == ReleasedKey;
                    });
      if (IsMouseInput(KeyInput._InputCode)) {
        InputData._NewMouseReleases.push_back(
            SMouseClick{KeyInput._InputCode, KeyInput._CoordinateData});
      } else {
        InputData._NewKeyUps.push_back(KeyInput._InputCode);
      }
    } else {
      bool IsNewInput =
          std::find(InputData._CurrentlyHeldDown.begin(),
                    InputData._CurrentlyHeldDown.end(),
                    KeyInput._InputCode) == InputData._CurrentlyHeldDown.end();
      if (IsNewInput) {
        InputData._CurrentlyHeldDown.push_back(KeyInput._InputCode);
        if (IsMouseInput(KeyInput._InputCode)) {
          InputData._NewMouseClicks.push_back(
              SMouseClick{KeyInput._InputCode, KeyInput._CoordinateData});
        } else {
          InputData._NewKeyDowns.push_back(KeyInput._InputCode);
        }
      }
    }
  }

  InputData._RawMousePos = _MousePos[_FrontBufferIdx];
  if (_pPrevFrameData) {
    InputData._RawMousePosPrevFrame = _pPrevFrameData->_RawMousePos;
  } else {
    InputData._RawMousePosPrevFrame = InputData._RawMousePos;
  }

  _pPrevFrameData = InputData;
  return _pPrevFrameData.value();
}

void CDoubleBufferedInput::AddKeyInput(EInputType InputType,
                                       EInputCode InputCode) {
  std::lock_guard<std::mutex> Lock{_Mutex};
  _KeyInput[_BackBufferIdx].push_back(
      SKeyInput{InputType, InputCode, _MousePos[_BackBufferIdx]});
}

void CDoubleBufferedInput::AddMousePos(CVector2f MousePos) {
  std::lock_guard<std::mutex> Lock{_Mutex};
  _MousePos[_BackBufferIdx] = MousePos;
}

////////////////////////////////////////////////////////////////////////////////