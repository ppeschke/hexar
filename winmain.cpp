#include "gamewindow.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void DisplayWindow(GAMEWINDOW* gw, HINSTANCE hInstance, int nCmdShow);
bool HandleMessages();
void InitDirect3D(GAMEWINDOW* gw);
void CloseDirect3D();
void InitDirectInput(HINSTANCE hInstance, GAMEWINDOW* gw);
void CloseDirectInput(void);
void OuterLoop(const char* ServerAddress);

// Starting Point
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    GAMEWINDOW gw;

    DisplayWindow(&gw, hInstance, nCmdShow);

    InitDirect3D(&gw);
    InitDirectInput(hInstance, &gw);

    OuterLoop("10.44.0.144");

    CloseDirect3D();
    CloseDirectInput();

    return 0;
}

// Create the Window Class and the Window
void DisplayWindow(GAMEWINDOW* gw, HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX)); wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = TEXT("WindowClass");

    RegisterClassEx(&wc);

    gw->hWnd = CreateWindowEx(NULL,
                              wc.lpszClassName,
                              TEXT("Hexar"),
                              gw->Windowed ? WS_OVERLAPPEDWINDOW : WS_EX_TOPMOST|WS_POPUP,
                              0, 0,
                              gw->Width, gw->Height,
                              NULL,
                              NULL,
                              hInstance,
                              NULL);

    ShowWindow(gw->hWnd, nCmdShow);
    return;
}

// Check for Messages and Handle
bool HandleMessages()
{
    static MSG msg;

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return FALSE;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return TRUE;
}

// Process the Messages
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}