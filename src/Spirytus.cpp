#include "Platform/Window/window.hpp"

#include <iostream>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    try {
        Window wnd(800, 600, "Spirytus");
        // message pump
        MSG msg;
        BOOL gResult;
        while (gResult = (GetMessage(&msg, nullptr, 0, 0)) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (gResult == -1) {
            return -1;
        } else {
            return msg.wParam;
        }
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