#include "InterfaceImGui.h"
#include <dwmapi.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#pragma comment(lib, "dwmapi.lib")
#include <stdio.h>



static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}



bool IsAeroEnabled() {
    BOOL isEnabled = FALSE;
    HRESULT hr = DwmIsCompositionEnabled(&isEnabled);
    return SUCCEEDED(hr) && isEnabled;
}


void SetWindowAeroAttributes(HWND hwnd) {


    if (hwnd) {
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
  
    }


}

void EnableTransparencyIfAeroDisabled(GLFWwindow* window) {
    if (!IsAeroEnabled()) {

        HWND hwnd = glfwGetWin32Window(window);
        SetWindowAeroAttributes(hwnd);
    }
}



int InterfaceImGui::init()
{

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    
    window = glfwCreateWindow(width_resolution, height_resolution, "Dear Window Manager", nullptr, nullptr);
    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);



    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();



    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    return 0;
}

int InterfaceImGui::visualize()
{
    glfwPollEvents();
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
    {
        ImGui_ImplGlfw_Sleep(10);
        return 1;
    }

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    EnableTransparencyIfAeroDisabled(window);

    return 0;
}


int InterfaceImGui::clear_buffer()
{
    int display_w, display_h;


    ImGui::Render();

    glfwGetFramebufferSize(window, &display_w, &display_h);

    height_resolution = display_h;
    width_resolution = display_w;

    glViewport(0, 0, display_w, display_h);

    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);



    return 0;
}

int InterfaceImGui::exit()
{
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}





int InterfaceImGui::run()
{
    init();
    static bool close_me = true;
    while (!glfwWindowShouldClose(window))
    {
        if (visualize())
            continue;

        for (const auto ui: UIs)
        {
            ui();
        }

        clear_buffer();
    }

    return exit();
}
