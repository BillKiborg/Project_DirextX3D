#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>

#pragma comment(lib, "d3d11.lib")

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

HWND                    g_hWnd              = NULL;
D3D10_DRIVER_TYPE       g_driverType        = D3D10_DRIVER_TYPE_NULL;
ID3D10Device*           g_pd3dDevice        = NULL;
IDXGISwapChain*         g_pSwapChain        = NULL;
ID3D10RenderTargetView* g_pRenderTargetView = NULL;

HRESULT InitWindow(HINSTANCE hInstane, int nCmdShow);
HRESULT InitDirect3D10();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Cleanup();
void RenderScene();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    if (FAILED(InitWindow(hInstance, nCmdShow))) {
        return 0;
    }

    if (FAILED(InitDirect3D10())) {
        Cleanup();
        return 0;
    }

    MSG msg{ 0 };
    while (WM_QUIT != msg.message) {

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
        else {

            RenderScene();

        }

    }

    return (int)msg.wParam;
            
}

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow) {

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance  = hInstance;

    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"SimpleWindowClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        return E_FAIL;
    }

    g_hWnd = CreateWindow(
        L"SimpleWindowClass",
        L"DirectX 10: First Application Direct3D 10",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!g_hWnd) {
        return E_FAIL;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    return S_OK;

}

HRESULT InitDirect3D10() {

    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    D3D10_DRIVER_TYPE driverTypes[] = {

        D3D10_DRIVER_TYPE_HARDWARE,
        D3D10_DRIVER_TYPE_REFERENCE,

    };

    UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

    DXGI_SWAP_CHAIN_DESC sd;

    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {

        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D10CreateDeviceAndSwapChain(NULL, g_driverType,
                                           NULL, 0, D3D10_SDK_VERSION, &sd,
                                           &g_pSwapChain, &g_pd3dDevice);

        if (SUCCEEDED(hr)) {
            break;
        }

    }

    if (FAILED(hr)) {
        return hr;
    }

    ID3D10Texture2D* pBackBuffer;
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr)) {
        return hr;
    }

    hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);

    pBackBuffer->Release();

    if (FAILED(hr)) {
        return hr;
    }

    g_pd3dDevice->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

    D3D10_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pd3dDevice->RSSetViewports(1, &vp);

    return S_OK;

}

void RenderScene() {

    float ClearColor[4] = { 0.1f, 0.5f, 0.1f, 1.0f };
    g_pd3dDevice->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
    g_pSwapChain->Present(0, 0);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void Cleanup() {

    if (g_pd3dDevice) g_pd3dDevice->ClearState();
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();

}