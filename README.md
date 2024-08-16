# WinBordeless

## Controlador de Janela em Tela Cheia Sem Bordas

O **WinBordeless** é um software que permite a você alternar facilmente entre o modo tela cheia sem bordas e o modo janela normal para qualquer aplicação no Windows que não ofereça essa funcionalidade nativamente. Com atalhos de teclado intuitivos, você pode controlar o comportamento da janela em tempo real, proporcionando uma experiência imersiva ou uma rápida restauração ao modo original.

## Funcionalidades

- **F5**: Lista todas as janelas abertas, exibindo seus respectivos `hwnd` e títulos.
- **F6**: Solicita o `hwnd` da janela e a coloca em modo tela cheia sem bordas.
- **F7**: Restaura a janela selecionada ao estado original (com bordas e barra de título).
- **ESC**: Restaura a janela e encerra o programa.

## Como Usar

1. Compile o código C++ com um compilador compatível com a API do Windows (recomenda-se o MSVC).
2. Execute o programa e utilize os seguintes atalhos para gerenciar as janelas:

   - **F5**: Exibe uma lista de todas as janelas visíveis no sistema, mostrando o `hwnd` e o título.
   - **F6**: Após pressionar, insira o `hwnd` da janela que deseja modificar (em hexadecimal) para alterná-la para o modo tela cheia sem bordas.
   - **F7**: Restaura a janela selecionada ao estado original (com bordas).
   - **ESC**: Restaura a janela ao estado original (se necessário) e fecha o programa.

## Requisitos

- Windows 7 ou superior.
- Compilador C++ compatível com a API do Windows (recomenda-se o MSVC).

## Exemplo de Uso

Para utilizar o software, coloque o jogo em modo janela com uma resolução igual à do seu monitor e execute o **WinBordeless**. Ao iniciar, pressione **F5** para listar todas as janelas e seus `hwnd`. Identifique a janela desejada, pressione **F6** e insira o `hwnd` (em hexadecimal) para colocá-la em modo tela cheia sem bordas. Use **F7** para restaurar a janela ao estado original e pressione **ESC** para encerrar o programa.

Esse recurso é especialmente útil para **jogos** que não possuem o modo janela sem bordas, permitindo uma rápida transição de volta ao modo original.

## Contribuição

Contribuições são bem-vindas! Sinta-se à vontade para abrir uma issue ou enviar um pull request para adicionar novas funcionalidades ou melhorar o código.

---

Faça sua experiencia não ter bordas :D
