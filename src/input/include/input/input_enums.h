#pragma once

enum class EInputType {
  None,
  KeyDown,
  KeyUp,
};

enum class EInputCode {
  Unknown,

  LeftMouseButton,
  RightMouseButton,
  MiddleMouseButton,

  Backspace,
  Tab,
  Enter,
  Shift,
  Ctrl,
  Alt,
  CapsLock,
  Escape,
  Space,
  PgUp,
  PgDown,
  End,
  Home,
  LeftArrow,
  UpArrow,
  RightArrow,
  DownArrow,
  Select,
  Print,
  Execute,
  PrintScreen,
  Ins,
  Del,
  Help,

  Key0,
  Key1,
  Key2,
  Key3,
  Key4,
  Key5,
  Key6,
  Key7,
  Key8,
  Key9,

  KeyA,
  KeyB,
  KeyC,
  KeyD,
  KeyE,
  KeyF,
  KeyG,
  KeyH,
  KeyI,
  KeyJ,
  KeyK,
  KeyL,
  KeyM,
  KeyN,
  KeyO,
  KeyP,
  KeyQ,
  KeyR,
  KeyS,
  KeyT,
  KeyU,
  KeyV,
  KeyW,
  KeyX,
  KeyY,
  KeyZ,

  // Make sure this remains last
  NumEnumOptions
};

/* https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes */
