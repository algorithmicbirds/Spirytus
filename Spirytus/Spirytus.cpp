#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
  // Register the window class.
    const auto pclassName = "Spirytus";
    WNDCLASS wc = { };

    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = hInstance;
    wc.style = CS_OWNDC;
    wc.lpszClassName = pclassName;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    RegisterClass(&wc);
    // create window instance
    HWND hWnd = CreateWindowExA(
        0, pclassName, "Happy Window", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        200, 200, 600, 400, nullptr, nullptr, hInstance, nullptr
        );
    if (hWnd == NULL) {
        return 0;
    }
    ShowWindow(hWnd, nCmdShow);
    while (true);
    return 0;
}