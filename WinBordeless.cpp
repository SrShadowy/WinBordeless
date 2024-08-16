// WinBordeless.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <windows.h>
#include "Bordeless.h"


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {

    if (hwnd) {
        char windowTitle[256];
        GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));

  
        if (strlen(windowTitle) > 0) {
            std::cout << "Janela: " << windowTitle << " : " << std::hex << hwnd << std::endl;
        }
    }
    return TRUE; 
}

int main() {

    Bordeless setBordeless;

    while (1)
    {
        if (GetAsyncKeyState(VK_F5))
        {
            EnumWindows(EnumWindowsProc, 0);
            Sleep(100);
        }

        if (GetAsyncKeyState(VK_F6))
        {
            std::cout << "Ponha o hwnd da janela: ";
            uint64_t hwnd = 0;
            std::cin >> std::hex >> hwnd;
            setBordeless.SetWindowBorderlessFullscreen((HWND)hwnd);
            Sleep(100);
        }

        if (GetAsyncKeyState(VK_F7))
        {
            setBordeless.RestoreWindow();
            Sleep(100);
        }

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            setBordeless.RestoreWindow();
            break;
        }

        Sleep(100);

    }


    return 0;
}


// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
