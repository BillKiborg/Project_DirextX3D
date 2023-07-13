#include <windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    // �������� ����
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(
        0,                                   // �������������� ����� ����
        CLASS_NAME,                          // ��� ������ ����
        L"DirectX Window",                   // ��������� ����
        WS_OVERLAPPEDWINDOW,                  // ����� ����
        CW_USEDEFAULT, CW_USEDEFAULT,         // ������� ����
        800, 600,                            // ������ ����
        NULL,                                // ������������ ����
        NULL,                                // ���� ����
        hInstance,                           // ��������� ����������
        NULL                                 // �������������� ������ ����
    );
    if (hwnd == NULL) {
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);

    // ������������� DirectX
    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;

    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferCount = 1;
    scDesc.BufferDesc.Width = 800;
    scDesc.BufferDesc.Height = 600;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.OutputWindow = hwnd;
    scDesc.SampleDesc.Count = 1;
    scDesc.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scDesc, &pSwapChain, &pDevice, nullptr, &pContext);

    // ������� ���� ���������
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // ������������ �������� DirectX
    pSwapChain->Release();
    pContext->Release();
    pDevice->Release();

    return 0;
}