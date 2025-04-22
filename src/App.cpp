#include "App.hpp"

App::App() : appWindow(600, 800, "Spirytus") {}    

int App::Start() {
    while (true) {
        if (const auto ecode = Window::ProccessMessages()) {
            return *ecode;
        }
        Update();
    }   
}

void App::Update() {};