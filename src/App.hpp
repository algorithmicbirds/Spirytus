#pragma once
#include "Window/window.hpp"

class App {
  public:
    App();
    int Start();

  private: 
      void Update();

  private:
      Window appWindow;
};