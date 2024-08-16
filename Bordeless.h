#pragma once
#include <Windows.h>
#include <map>
#include <vector>
#include <string>


struct winNames
{
    std::string name;
    HWND hwindow;
    LONG originalStyle;
    RECT originalRect;
};

class Bordeless
{
private:
    std::vector<winNames> winList;
public:
    void SetWindowBorderlessFullscreen(HWND hwnd);
    void RestoreWindow();

};

