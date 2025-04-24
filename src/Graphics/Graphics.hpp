#pragma once
#include "../Windows/SpyritusWin.hpp"

#include <d3d11.h>

class Graphics {
  public:
    Graphics(HWND hWnd);
    Graphics(const Graphics &) = delete;
    Graphics &operator=(const Graphics&) = delete;
    ~Graphics();
    void EndFrame();
    void ClearBuffer(float red, float green, float blue);
  private:
    ID3D11Device *pDevice = nullptr;
    ID3D11DeviceContext *pContext = nullptr;
    IDXGISwapChain *pSwapChain = nullptr;
    ID3D11RenderTargetView *pTargetView;
};