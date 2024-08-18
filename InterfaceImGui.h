#pragma once
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <GLFW/glfw3.h>
#include <vector>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


class InterfaceImGui
{
private:
    GLFWwindow* window;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    int width_resolution = 1280, height_resolution = 720;
    ImGuiIO io;
    
    std::vector<void(*)()> UIs;

    int init();
    int visualize();
    int clear_buffer();
    int exit();


public:


    InterfaceImGui(int w, int h): width_resolution(w), height_resolution(h)
    {
        window = nullptr;
        clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
        io = ImGuiIO();
        UIs.clear();
    }

    InterfaceImGui() {
        width_resolution = 1280, height_resolution = 720;
        window = nullptr;
        clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
        io = ImGuiIO();
        UIs.clear();
    };

    void addUI(void theFunctionUI())
    {
        UIs.push_back(theFunctionUI);
    }

    void setResolution(int w, int h)
    {
        width_resolution = w;
        height_resolution = h;
    }

    ImVec2 getResolution() {
        return { (float)width_resolution ,  (float)height_resolution };
    }


    int run();

};

