#include "Bordeless.h"
#include <Psapi.h>


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) 
{
    std::vector<WinNames>* windows = reinterpret_cast<std::vector<WinNames>*>(lParam);
    if (hwnd) {
        char windowTitle[256]{};
        char windowClassA[256]{};
        RECT OriginalWindowRect;
        DWORD pid, tid;
        tid = GetWindowThreadProcessId(hwnd, &pid);
        auto style = GetWindowLong(hwnd, GWL_STYLE);
        auto exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
        bool isVisible = IsWindowVisible(hwnd);
        bool isMinimized = IsIconic(hwnd);
        bool isMaximized = IsZoomed(hwnd);

        GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
        RealGetWindowClassA(hwnd, windowClassA, sizeof(windowClassA));
        GetWindowRect(hwnd, &OriginalWindowRect);

        if (strlen(windowTitle) > 0) {
            windows->push_back(
                {
                    windowTitle,
                    windowClassA,
                    hwnd,
                    style,
                    exStyle,
                    pid,
                    tid,
                    isVisible,
                    isMinimized,
                    isMaximized,
                    OriginalWindowRect,
                    {OriginalWindowRect.left, OriginalWindowRect.top}
                });
        }
    }
    return TRUE;
}

DWORD displayMonitorIndex = 0;
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    std::vector<NDisplay>* monitors = reinterpret_cast<std::vector<NDisplay>*>(dwData);

    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);

    if (GetMonitorInfoA(hMonitor, &monitorInfo))
    {
        NDisplay display;
        display.width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
        display.height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;


        DISPLAY_DEVICEA device;
        ZeroMemory(&device, sizeof(device));
        device.cb = sizeof(device);


        if (EnumDisplayDevicesA(NULL, displayMonitorIndex, &device, 0))
        {
            if (device.StateFlags & DISPLAY_DEVICE_ACTIVE)
            {

                DEVMODEA devMode;
                ZeroMemory(&devMode, sizeof(devMode));
                devMode.dmSize = sizeof(devMode);

                if (EnumDisplaySettingsA(device.DeviceName, ENUM_CURRENT_SETTINGS, &devMode))
                {
                    display.colorDepth = devMode.dmBitsPerPel;
                    display.refreshRate = devMode.dmDisplayFrequency;
                    display.name = device.DeviceName;
                    //printf("\n\nMonitor: %s, Resolution: %dx%d, Color Depth: %d, Refresh Rate: %dHz\n", display.name.c_str(), display.width, display.height, display.colorDepth, display.refreshRate);
                    monitors->push_back(display);
                }

        
            }

            displayMonitorIndex++;
        }
    }

    return TRUE;
}


BOOL CALLBACK SystemProcess(const DWORD pid, const std::vector<std::string> processIgnored)
{
    auto hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);
    if (!hproc) return false;

    char ProcName[MAX_PATH];
    if (GetModuleFileNameExA(hproc, nullptr, ProcName, MAX_PATH))
    {
        std::string procNameStr(ProcName);


        for (const auto p : processIgnored)
        {
            if (procNameStr.find(p) != std::string::npos)
                return false;
            
              
        }
    }

    return true;
}


void Bordeless::SetWindowBorderlessFullscreen(HWND hwnd)
{
    WinNames NewWindow;

    char windowTitle[255];

    GetWindowTextA( hwnd, windowTitle, sizeof(windowTitle) );
    if ( strlen(windowTitle) > 0)
        NewWindow.windowTitle = windowTitle;


    memset( windowTitle, 0, sizeof( windowTitle ) );
    RealGetWindowClassA( hwnd, windowTitle, sizeof(windowTitle) );
    NewWindow.className = windowTitle;

    NewWindow.style = GetWindowLong(hwnd, GWL_STYLE);
    GetWindowRect(hwnd, &NewWindow.windowRect);
    SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);


    int screenWidth     = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight    = GetSystemMetrics(SM_CYSCREEN);


    if ( SetWindowPos(hwnd, HWND_TOP, NewWindow.position.x, NewWindow.position.y, screenWidth, screenHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW) )
    {
        NewWindow.hwnd = hwnd;

        winList.push_back(NewWindow);
    }


}

void Bordeless::RestoreWindow()
{

    for (auto& win : winList)
    {
        SetWindowLong(win.hwnd, GWL_STYLE, win.style);
        SetWindowLong(win.hwnd, GWL_EXSTYLE, win.exStyle);
        SetWindowPos(win.hwnd, HWND_TOP, win.windowRect.left, win.windowRect.top, win.windowRect.right - win.windowRect.left, win.windowRect.bottom - win.windowRect.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }
 }

bool Bordeless::ignoreWindow(WinNames hwn)
{
    return SystemProcess(hwn.pid, ignoreProcessName);
}

std::vector< WinNames > Bordeless::updateWindowList()
{
    winList.clear();
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&winList));
    return winList;
}

std::vector<NDisplay> Bordeless::updateMonitorList()
{
    screenList.clear();
    displayMonitorIndex = 0;
    EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&screenList));

    return screenList;
}
