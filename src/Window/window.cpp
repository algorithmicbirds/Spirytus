#include "Window.hpp"


Window::WindowClass Window::WindowClass::wndClass;
Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr)) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.hInstance = hInst;
    wc.lpszClassName = wndClassName;
    wc.hIcon =
        static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, 128, 128, 0)
        );
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = GetName();
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIconSm =
        static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_APP_ICON), IMAGE_ICON, 16, 16, 0));
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() { UnregisterClass(wndClassName, GetInstance()); }

const char *Window::WindowClass::GetName() noexcept { return wndClassName; }

HINSTANCE Window::WindowClass::GetInstance() noexcept { return wndClass.hInst; }

// window

Window::Window(int width, int height, const char *name) noexcept {
    // calculate window size based on desired client region size
    RECT wr;
    wr.left = 100;
    wr.top = 100;
    wr.bottom = height + wr.top;
    wr.right = width + wr.bottom;

    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    hWnd = CreateWindow(
        WindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT,
        CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr,
        WindowClass::GetInstance(), this
    );

    if (hWnd == nullptr) {
        throw SHWND_LAST_EXCEPT();
    }


    ShowWindow(hWnd, SW_SHOWDEFAULT);
    pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window() { DestroyWindow(hWnd); }

std::optional<int> Window::ProccessMessages() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return msg.wParam;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
}

LRESULT CALLBACK
Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    // use create paremeter in from CreateWindow() to store window class pinter
    if (msg == WM_NCCREATE) {
        // extract ptr to window class creation data
        const CREATESTRUCTA *const pCreate = reinterpret_cast<CREATESTRUCTA *>(lParam);
        Window *const pWnd = static_cast<Window *>(pCreate->lpCreateParams);
        // set win api user managed data to store pointer to window class
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler that is now finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window class handler
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK
Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    // retrieve pointer to window class from user data
    Window *const pWnd = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window class handler
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    switch (msg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_KILLFOCUS:
        keyboard.ClearState();
        break;
    // keyboard messages
    case WM_KEYDOWN:
        // syskey commands need to be handled to track ALT key (VK_MENU) and F10
    case WM_SYSKEYDOWN:
        if (!(lParam & 0x40000000) || keyboard.AutorepeatIsEnabled()) {
            keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        keyboard.OnChar(static_cast<unsigned char>(wParam));
        break;
    case WM_MOUSEMOVE: {
        const POINTS pt = MAKEPOINTS(lParam);

        // in client region -> log move, and log enter + capture mouse (if not previously in window)
        if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
            mouse.OnMouseMove(pt.x, pt.y);
            if (!mouse.IsInWindow()) {
                SetCapture(hWnd);
                mouse.OnMouseEnter();
            }
        }
        // not in client -> log move / maintain capture if button down
        else {
            if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
                mouse.OnMouseMove(pt.x, pt.y);
            }
            // button up -> release capture / log event for leaving
            else {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
        }
        break;
    }
    case WM_LBUTTONDOWN: {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN: {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightPressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP: {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftReleased(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONUP: {
        const POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightReleased(pt.x, pt.y);
        break;
    }
    case WM_MOUSEWHEEL: {
        const POINTS pt = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.OnWheelDelta(pt.x, pt.y, delta);
        break;
    }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Exception::Exception(const char *file, int line, HRESULT hr) noexcept
    : SpirytusException(file, line), hr(hr) {}

const char *Window::Exception::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[ErrorCode]" << GetErrorCode() << std::endl
        << "[Description]" << GetErrorString() << std::endl
        << GetErrorString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *Window::Exception::GetType() const noexcept { return "Spirytus Window Exception"; }

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept {
    char *pMsgBuff = nullptr;
    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuff),
        0, nullptr
    );
    if (nMsgLen == 0) {
        return "Undefined Error Code";
    }
    std::string errorString = pMsgBuff;
    LocalFree(pMsgBuff);
}

HRESULT Window::Exception::GetErrorCode() const noexcept { return hr; }

std::string Window::Exception::GetErrorString() const noexcept { return TranslateErrorCode(hr); }
