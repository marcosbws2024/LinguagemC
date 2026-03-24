#include <windows.h>

// Identificador do botão
#define ID_BOTAO_CADASTRAR 1

// Função que processa as mensagens da janela (cliques, fechamento, etc)
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            // Cria um botão dentro da janela
            CreateWindow("BUTTON", "Cadastrar no Firebase", 
                         WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                         50, 50, 200, 30, 
                         hwnd, (HMENU)ID_BOTAO_CADASTRAR, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wp) == ID_BOTAO_CADASTRAR) {
                MessageBox(hwnd, "Dados enviados! (Aqui chamamos sua função Firebase)", "Sucesso", MB_OK | MB_ICONINFORMATION);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "MinhaJanela";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc)) return -1;

    CreateWindow("MinhaJanela", "Sistema de Cadastro", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                 100, 100, 320, 200, NULL, NULL, hInst, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}