#pragma once
#include <Windows.h>
#include <map>
#include <vector>
#include <string>



struct WinNames
{
    std::string windowTitle{};
    std::string className{};
    HWND hwnd = nullptr;
    LONG style = 0;
    LONG exStyle = 0;
    DWORD pid = 0;
    DWORD tid = 0;
    bool isVisible = false;
    bool isMinimized = false;
    bool isMaximized = false;
    RECT windowRect{};
    POINT position{};
};

struct NDisplay {
    std::string name{};
    int width = 0;
    int height = 0;
    int colorDepth = 0; 
    int refreshRate = 0;
    RECT monitorPos{0,0};
};

class Bordeless
{
private:
    std::vector< WinNames > winList;
    std::vector< NDisplay > screenList;
    std::vector< std::string > ignoreProcessName = {
        "AggregatorHost.exe",
        "backgroundTaskHost.exe",
        "conhost.exe",
        "csrss.exe",
        "System",
        "smss.exe",
        "wininit.exe",
        "winlogon.exe",
        "services.exe",
        "lsass.exe",
        "explorer.exe",
        "SearchHost.exe",
        "ApplicationFrameHost.exe"
    };

public:
    void SetWindowBorderlessFullscreen(HWND hwnd);
    bool RestoreWindow(WinNames hwn);
    void RestoreWindow();


    bool ignoreWindow(WinNames hwn);

    std::vector< WinNames > updateWindowList();
    std::vector< NDisplay > updateMonitorList();
    std::vector< WinNames > getWindowList()  { return winList; };
    std::vector< NDisplay > getMonitorList()  { return screenList; };

    void addIgnoredProcess(std::string procName) 
    { ignoreProcessName.push_back(procName); };
    std::vector< std::string > getIgnoredProcessList() 
    { return ignoreProcessName; };




    
};

