#include "Bordeless.h"
#include "InterfaceImGui.h"
#define WM_USER_MOVE (WM_USER + 1)

namespace UI {

    // Variaveis da ui e utilidade graficas
    Bordeless utility;
    InterfaceImGui GraphicUI;
    struct timer
    {
        bool enable = true;
        uint64_t tick = 0;
        uint64_t interval = 10000;
        bool on_tick() {

            if (enable && tick < GetTickCount64())
            {
                tick = GetTickCount64() + interval;
                return true;
            }
            return false;
        }

    }tm1;



    // Enumerado da janela principal
    enum WinOptionFlag_
    {
        WinOptionFlag_None      = 0,
        WinOptionFlag_Window    = 1,
        WinOptionFlag_Options   = 1 << 1,
        WinOptionFlag_Help      = 1 << 2
    };

    // Oque mostrar no botão
    enum WinShowFlag_
    {
        WinShowFlag_None        = 0,
        WinShowFlag_Title       = 1,
        WinShowFlag_ClassName   = 1 << 1,
        WinShowFlag_pid         = 1 << 2

    };

    // Oque ocultar
    enum WinConfigFlag_ 
    {
        WinConfigFlag_None          = 0,
        WinConfigFlag_Iconic        = 1,
        WinConfigFlag_Invisible     = 1 << 1,
        WinConfigFlag_HideSys       = 1 << 2
    };

    // Estrutura de renderização da janela
    struct window1 {

        bool close_me = true;
        int GetNameStyle = WinShowFlag_Title;
        int WindowOptionFlag = WinOptionFlag_Options;
        int winFlag = WinConfigFlag_HideSys | WinConfigFlag_Invisible | WinConfigFlag_Iconic;
        void renderList();
        void renderOption();
        void renderWindowOptions();
        void main();
        WinNames WinSelected;

    } MainInterface;

    static WinNames Testing;
    static POINT MaxResolution{10,10};
    static void CreateWindowTesting();


    // !Não USE! é apenas para manter a função organizada
    void showInterface();



    // Entrypoint :D
    void start();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
                 return 0;
    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        RECT rect;
        GetWindowRect(hwnd, &rect);
        UI::Testing.windowRect = rect;
    }
                return 0;
    case WM_MOVE: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        UI::Testing.position.x = x;
        UI::Testing.position.y = y;
    }
                return 0;
    case WM_USER_MOVE: {

        RECT rect;
        GetWindowRect(hwnd, &rect);
        UI::Testing.windowRect = rect;
        if (wParam == 1)
        {
            SetWindowPos(hwnd, NULL, (int)lParam, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
        }
        if (wParam == 2)
        {
            SetWindowPos(hwnd, NULL, rect.left, (int)lParam, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
        }
        return 0;
    }

    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}
std::string GetMonitorNameForWindow(HWND hwnd) {

    HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);

    MONITORINFOEXA monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFOEXA);
    if (GetMonitorInfoA(hMonitor, &monitorInfo)) {
        return std::string(monitorInfo.szDevice);
    }

    return u8"Monitor não encontrado";
}

void UI::CreateWindowTesting()
{
    Testing.className = "STATIC";
    Testing.windowTitle = "Fullscreen Example";

    WNDCLASSA wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = nullptr;
    wc.lpszClassName = Testing.className.c_str();

    RegisterClassA(&wc);

    Testing.hwnd = CreateWindowExA(
        0,                              // Estilo adicional
        Testing.className.c_str(),                    // Nome da classe
        Testing.windowTitle.c_str(),                // Título da janela
        WS_OVERLAPPEDWINDOW,           // Estilo da janela
        CW_USEDEFAULT, CW_USEDEFAULT,  // Posição x, y
        800, 600,  // Largura, altura
        NULL,                           // Janela pai
        NULL,                           // Menu
        nullptr,                     // Instância do aplicativo
        NULL                            // Parâmetro adicional
    );

    if (Testing.hwnd == NULL) {
        return;
    }


    UpdateWindow(Testing.hwnd);

    ShowWindow(Testing.hwnd, true);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }



}


inline void UI::window1::renderList()
{
    ImGui::BeginChild("##WindowsList", { 300, -1 }, ImGuiChildFlags_Border);
    {
        auto windows = utility.getWindowList();
        for (auto& wins : windows)
        {
 
            if (winFlag & WinConfigFlag_HideSys && !utility.ignoreWindow(wins))
                continue;
            
            if (winFlag & WinConfigFlag_Iconic && wins.isMinimized)
                 continue;
               
            if (winFlag & WinConfigFlag_Invisible && !wins.isVisible)
                continue;


            std::string showName{};

            if (GetNameStyle & WinShowFlag_Title)
                showName.append(wins.windowTitle + " ");

            if (GetNameStyle & WinShowFlag_ClassName)
                showName.append(wins.className + " ");

            if (GetNameStyle & WinShowFlag_pid)
                showName.append( std::to_string( wins.pid ) );

            if (ImGui::Button( showName.c_str(), {-1, 50})) {
                WinSelected = wins;
                WindowOptionFlag = WinOptionFlag_Window;
            }
        }

        if (ImGui::Button(u8"Opções", { -1, 50 }))
            WindowOptionFlag = WinOptionFlag_Options | WinOptionFlag_Help;

        if (ImGui::Button(u8"Sair", { -1, 50 }))
            WindowOptionFlag = WinOptionFlag_None;

    }
    ImGui::EndChild();

    if (tm1.on_tick())
    {
        utility.updateWindowList();
    }

}



inline void UI::window1::renderWindowOptions()
{
    if (ImGui::BeginChild("WindowInfomations", { -1,120 }, ImGuiChildFlags_Border))
    {
        ImGui::Text("Title: %s", WinSelected.windowTitle.c_str());
        ImGui::Text("Class %s", WinSelected.className.c_str());
        ImGui::Text("HWND %X", WinSelected.hwnd);
        ImGui::Text("Resolution %d x %d", WinSelected.windowRect.right- WinSelected.windowRect.left , WinSelected.windowRect.bottom - WinSelected.windowRect.top);
        ImGui::Text("Position %d x %d", WinSelected.windowRect.left, WinSelected.windowRect.top);
        ImGui::Text("Pid: %d", WinSelected.pid);
        ImGui::EndChild();
    }


    if (ImGui::Button(u8"Criar uma janela de teste"))
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CreateWindowTesting, 0, 0, 0);

    if (Testing.hwnd)
    {
        ImGui::Text(u8"Posição %d %d", Testing.position.x, Testing.position.y);
        ImGui::Text(u8"Tamanho %d x %d", Testing.windowRect.right - Testing.windowRect.left, Testing.windowRect.bottom - Testing.windowRect.top);

        ImGui::Text(u8"Se localizando no %s", GetMonitorNameForWindow(Testing.hwnd).c_str());

        if (ImGui::SliderInt("Mover janela no eixo X", (int*)&Testing.position.x, 0, MaxResolution.x))
        {
            WindowProc(Testing.hwnd, WM_USER_MOVE, 1, Testing.position.x);
        }

        if (ImGui::SliderInt("Mover janela no eixo Y", (int*)&Testing.position.y, 0, MaxResolution.y))
        {
            WindowProc(Testing.hwnd, WM_USER_MOVE, 2, Testing.position.y);
        }

        if(ImGui::Button("Remover Bordas"))
        {
            SetWindowLong(Testing.hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        }

        if (ImGui::Button("Por Bordas"))
        {
            SetWindowLong(Testing.hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        }
        if (ImGui::Button("Chamar para cima"))
        {
            SetFocus(Testing.hwnd);
            SetForegroundWindow(Testing.hwnd);


        }

    }



        
}

inline void UI::window1::renderOption()
{
    ImGui::BeginChild("##WindowOptions", { -1, -1 }, ImGuiChildFlags_Border);
    {
        if (WindowOptionFlag & WinOptionFlag_Window)
            renderWindowOptions();
        if (WindowOptionFlag & WinOptionFlag_Options)
        {
            ImGui::Text(u8"Configurações gerais");
            if (ImGui::BeginChild("#Options", { -1,200 }, ImGuiChildFlags_Border))
            {
                if (ImGui::Button(u8"Atualizar janelas"))
                {
                    utility.updateWindowList();
                }
                ImGui::SameLine();
                if (ImGui::Button(u8"Atualizar monitores"))
                {
                    utility.updateMonitorList();
                }

                ImGui::Text("Oque mostrar:");

                static bool v[3] = { true, false, false };
                if (ImGui::Checkbox(u8"Exibir titulo", &v[0]))
                {
                    if (v[0])
                        GetNameStyle |= WinShowFlag_Title;
                    else
                        GetNameStyle &= ~WinShowFlag_Title;
                }
                ImGui::SameLine();

                if (ImGui::Checkbox(u8"Exibir pid", &v[1]))
                {
                    if (v[1])
                        GetNameStyle |= WinShowFlag_pid;
                    else
                        GetNameStyle &= ~WinShowFlag_pid;
                }
                ImGui::SameLine();
                if (ImGui::Checkbox(u8"Exibir ClassName", &v[2]))
                {
                    if (v[2])
                        GetNameStyle |= WinShowFlag_ClassName;
                    else
                        GetNameStyle &= ~WinShowFlag_ClassName;
                }


                ImGui::Text(u8"Configurações de exclusão");

                static bool e[] = {false, true, true, true};
                if (ImGui::Checkbox("Nada", &e[0]))
                {
                    if (e[0])
                        winFlag = WinConfigFlag_None;

                    e[1] = false;
                    e[2] = false;
                    e[3] = false;

                }
                ImGui::SameLine();
                if (ImGui::Checkbox(u8"Excluir os que estão minimizados", &e[1]))
                {
                    if (e[1])
                    {
                        e[0] = false;
                        winFlag |= WinConfigFlag_Iconic;
                    }else
                        winFlag &= ~WinConfigFlag_Iconic;
     
                }
                ImGui::SameLine();
                if (ImGui::Checkbox(u8"Excluir Janelas não visiveis", &e[2]))
                {
                    if (e[2])
                    {
                        e[0] = false;
                        winFlag |= WinConfigFlag_Invisible;
                    }
                    else
                        winFlag &= ~WinConfigFlag_Invisible;


                }
                ImGui::SameLine();
                if (ImGui::Checkbox(u8"Excluir janelas do sistema", &e[3]))
                {
                    if (e[3])
                    {
                        e[0] = false;
                        winFlag |= WinConfigFlag_HideSys;
                    }
                    else
                        winFlag &= ~WinConfigFlag_HideSys;

                }


                if (e[1] == false &&
                    e[2] == false &&
                    e[3] == false) {
                    e[0] = true;
                    winFlag = WinConfigFlag_None;
                }


                ImGui::EndChild();
            }

            ImGui::Text(u8"Monitores detectados:");
            if (ImGui::BeginChild("#Motitores", { -1, 300 }, ImGuiChildFlags_Border))
            {
                for (const auto mnt : utility.getMonitorList())
                {
                    ImGui::BeginChild(mnt.name.c_str(), { -1 , 100 }, ImGuiChildFlags_Border);

                    ImGui::LabelText("##MonitorName", u8"Nome do Dispositivo: %s", mnt.name.c_str());
                    ImGui::LabelText("##MonitorHz", u8"ScreenFrashRate: %d Hz", mnt.refreshRate);
                    ImGui::LabelText("##MonitorRes", u8"Resolução: %d x %d pixels", mnt.width, mnt.height);

                    ImGui::EndChild();
                }

                ImGui::EndChild();
            }
          
        }

    }

    ImGui::EndChild();
}

inline void UI::window1::main()
{
    if (!close_me)  return;
    
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowSize( GraphicUI.getResolution() );
    if (ImGui::Begin("MainWindow", &close_me, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoMove))
    {
        renderList();
        ImGui::SameLine();
        renderOption();
    }
    ImGui::End();
}



void UI::showInterface()
{
    MainInterface.main();
}

void UI::start()
{
    auto p = utility.updateMonitorList();

    MaxResolution = { 0,0 };
    for (const auto tx : p)
    {

        MaxResolution.x += tx.width;
        MaxResolution.y += tx.height;
    }
    utility.updateWindowList();
    GraphicUI.addUI(UI::showInterface);

    GraphicUI.run();
}

