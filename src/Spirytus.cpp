#include "Window/window.hpp"
#include "App.hpp"

#include <iostream>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    try {
        App app;
        return app.Start();
        // message pump
    } catch (const SpirytusException &e) {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    } catch (const std::exception &e) {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    } catch (...) {
        MessageBox(
            nullptr, "No details Available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION
        );
    }
    return -1;
}