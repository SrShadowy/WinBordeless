#include "Bordeless.h"

void Bordeless::SetWindowBorderlessFullscreen(HWND hwnd)
{
    winNames NewWindow;
    NewWindow.originalStyle = GetWindowLong(hwnd, GWL_STYLE);
    char windowTitle[255];
    GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));

    GetWindowRect(hwnd, &NewWindow.originalRect);


    SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);


    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);


    if (SetWindowPos(hwnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW))
    {
        NewWindow.hwindow = hwnd;
        NewWindow.name = windowTitle;
        winList.push_back(NewWindow);
    }


}

void Bordeless::RestoreWindow()
{

    for (auto& win : winList)
    {
        SetWindowLong(win.hwindow, GWL_STYLE, win.originalStyle);
        SetWindowPos(win.hwindow, HWND_TOP, win.originalRect.left, win.originalRect.top, win.originalRect.right - win.originalRect.left, win.originalRect.bottom - win.originalRect.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }
 }
