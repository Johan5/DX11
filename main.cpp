#include <Windows.h>
#include "src/renderer/include/renderer/systemclass.h"

#include <iostream>
#include <memory>
#include <sstream>

using namespace std;

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR pScmdline,
                   int Cmdshow) {
  cout << "WinMain running... Here we go" << endl;

  unique_ptr<CSystem> pSystem = make_unique<CSystem>();
  if (!pSystem) {
    return 0;
  }

  if (pSystem->Initialize()) {
    pSystem->Run();
  }

  pSystem->Shutdown();
  return 0;
}
