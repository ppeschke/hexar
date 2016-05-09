#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

struct GAMEWINDOW
{
    HWND hWnd;
    int Width, Height;
    bool Windowed;

    GAMEWINDOW()
    {
        hWnd = NULL;
        Width = 1024;
        Height = 768;
        Windowed = true;
    }
};