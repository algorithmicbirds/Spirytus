#include "Graphics.hpp"

Graphics::Graphics(HWND hWnd) {
    constexpr D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2;
    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    D3D_FEATURE_LEVEL createdFL;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, _countof(featureLevels),
        D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &createdFL, &pContext
    );

    if (FAILED(hr)) {
        MessageBox(nullptr, "Failed to create device and swap chain", "Error", MB_OK);
        return;
    }

    ID3D11Resource *pBackBuffer = nullptr;
    hr =
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void **>(&pBackBuffer));

    if (FAILED(hr)) {
        MessageBox(nullptr, "Failed to get back buffer", "Error", MB_OK);
        return;
    }

    hr = pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTargetView);
    pBackBuffer->Release();

    if (FAILED(hr)) {
        MessageBox(nullptr, "Failed to create render target view", "Error", MB_OK);
        return;
    }
}

Graphics::~Graphics() {
    if (pTargetView != nullptr) {
        pTargetView->Release();
    }
    if (pContext != nullptr) {
        pContext->Release();
    }
    if (pSwapChain != nullptr) {
        pSwapChain->Release();
    }
    if (pDevice != nullptr) {
        pDevice->Release();
    }
}

void Graphics::EndFrame() { pSwapChain->Present(1u, 0u); }

void Graphics::ClearBuffer(float red, float green, float blue) {
    const float color[] = {red, green, blue, 1.0f};
    pContext->ClearRenderTargetView(pTargetView, color);
}
