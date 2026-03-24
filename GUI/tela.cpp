#define _WIN32_IE 0x0300
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

// IDs dos componentes
#define ID_NOME 101
#define ID_END  102
#define ID_TEL  103
#define ID_MAIL 104
#define ID_BOTAO_ADD 105
#define ID_BOTAO_DEL 106
#define ID_TABELA 107

// Variáveis globais para os campos
HWND hNome, hEnd, hTel, hMail, hLista;

// Função para configurar as colunas da tabela
void ConfigurarTabela(HWND hL) {
    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    
    char *colunas[] = {"Nome", "Endereco", "Telefone", "E-mail"};
    int larguras[] = {120, 150, 100, 150};

    for (int i = 0; i < 4; i++) {
        lvc.pszText = colunas[i];
        lvc.cx = larguras[i];
        SendMessage(hL, LVM_INSERTCOLUMN, i, (LPARAM)&lvc);
    }
}

// Função para inserir na tabela
void AdicionarNaTabela(const char* n, const char* e, const char* t, const char* m) {
    LVITEM lvi = {0};
    lvi.mask = LVIF_TEXT;
    lvi.iItem = SendMessage(hLista, LVM_GETITEMCOUNT, 0, 0); 
    
    lvi.iSubItem = 0;
    lvi.pszText = (char*)n;
    int index = SendMessage(hLista, LVM_INSERTITEM, 0, (LPARAM)&lvi);
    
    SendMessage(hLista, LVM_SETITEMTEXT, index, (LPARAM)&(LVITEM){.iSubItem=1, .pszText=(char*)e});
    SendMessage(hLista, LVM_SETITEMTEXT, index, (LPARAM)&(LVITEM){.iSubItem=2, .pszText=(char*)t});
    SendMessage(hLista, LVM_SETITEMTEXT, index, (LPARAM)&(LVITEM){.iSubItem=3, .pszText=(char*)m});
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: {
            // Inicializa controles comuns (Importante para evitar o erro que voce teve)
            INITCOMMONCONTROLSEX icex;
            icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icex.dwICC = ICC_LISTVIEW_CLASSES;
            InitCommonControlsEx(&icex);

            // Labels e Campos de Entrada
            CreateWindow("STATIC", "Nome:", WS_VISIBLE | WS_CHILD, 10, 15, 80, 20, hwnd, NULL, NULL, NULL);
            hNome = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 15, 200, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Endereco:", WS_VISIBLE | WS_CHILD, 10, 45, 80, 20, hwnd, NULL, NULL, NULL);
            hEnd = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 45, 200, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "Telefone:", WS_VISIBLE | WS_CHILD, 10, 75, 80, 20, hwnd, NULL, NULL, NULL);
            hTel = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 75, 200, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "E-mail:", WS_VISIBLE | WS_CHILD, 10, 105, 80, 20, hwnd, NULL, NULL, NULL);
            hMail = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 105, 200, 20, hwnd, NULL, NULL, NULL);

            // Botoes
            CreateWindow("BUTTON", "ADICIONAR", WS_VISIBLE | WS_CHILD, 310, 15, 100, 50, hwnd, (HMENU)ID_BOTAO_ADD, NULL, NULL);
            CreateWindow("BUTTON", "EXCLUIR", WS_VISIBLE | WS_CHILD, 310, 75, 100, 50, hwnd, (HMENU)ID_BOTAO_DEL, NULL, NULL);

            // Tabela (ListView)
            hLista = CreateWindow(WC_LISTVIEW, "", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_SINGLESEL, 
                                  10, 150, 560, 200, hwnd, (HMENU)ID_TABELA, NULL, NULL);
            SendMessage(hLista, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
            ConfigurarTabela(hLista);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wp) == ID_BOTAO_ADD) {
                char n[50], e[50], t[20], m[50];
                GetWindowText(hNome, n, 50);
                GetWindowText(hEnd, e, 50);
                GetWindowText(hTel, t, 20);
                GetWindowText(hMail, m, 50);

                if (strlen(n) > 0) {
                    AdicionarNaTabela(n, e, t, m);
                    // Limpa os campos apos adicionar
                    SetWindowText(hNome, ""); SetWindowText(hEnd, "");
                    SetWindowText(hTel, ""); SetWindowText(hMail, "");
                } else {
                    MessageBox(hwnd, "Por favor, digite ao menos o nome.", "Aviso", MB_OK | MB_ICONEXCLAMATION);
                }
            }
            if (LOWORD(wp) == ID_BOTAO_DEL) {
                int item = SendMessage(hLista, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
                if (item != -1) {
                    SendMessage(hLista, LVM_DELETEITEM, item, 0);
                } else {
                    MessageBox(hwnd, "Selecione uma linha para excluir.", "Erro", MB_OK | MB_ICONERROR);
                }
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

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR args, int nShow) {
    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); // Cor cinza padrao de janelas
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "JanelaPrincipal";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc)) return -1;

    CreateWindow("JanelaPrincipal", "Sistema de Cadastro - CRUD Local", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                 200, 200, 600, 420, NULL, NULL, hInst, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}