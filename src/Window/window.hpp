#pragma once

#include "../Windows/SpyritusWin.hpp"
#include "../SpirytusException.hpp"
#include "../MSVC/resource.h"
#include "Input/Keyboard/Keyboard.hpp"
#include "Input/Mouse/Mouse.hpp"
#include "../Graphics/Graphics.hpp"

// std
#include <memory>

class Window {
  public:
    class Exception : SpirytusException {
      public:
        Exception(const char *file, int line, HRESULT hr) noexcept;
        const char *what() const noexcept override;
        virtual const char *GetType() const noexcept;
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;

      private:
        HRESULT hr;
    };

  private:
    // singleton manages registration / cleanup of window class
    class WindowClass {
      public:
        static const char *GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;

      private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass &) = delete;
        WindowClass &operator=(const WindowClass &) = delete;
        static constexpr const char *wndClassName = "Spirytus";
        static WindowClass wndClass;
        HINSTANCE hInst;
    };

  public:
    Window(int width, int height, const char *name) noexcept;
    ~Window();
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    static std::optional<int> ProccessMessages();
    Keyboard keyboard;
    Mouse mouse;
    Graphics &Gfx() { return *pGfx; }
  private:
    static LRESULT CALLBACK
    HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK
    HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

  private:
    HWND hWnd;
    int width;
    int height;
    std::unique_ptr<Graphics> pGfx;
};

// error exception helper macro
#define SHWND_EXCEPT(hr) Window::Exception(__FILE__, __LINE__, hr)
#define SHWND_LAST_EXCEPT() Window::Exception(__FILE__, __LINE__, GetLastError())