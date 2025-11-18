#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h> // Inclui o módulo de interface gráfica (GUI)

// ⚠️ Nota: Para compilar, você precisa ter a raylib instalada no seu sistema.
// O comando de compilação é bem mais simples que o do GTK.

// Configurações da Janela
#define SCREEN_WIDTH 450
#define SCREEN_HEIGHT 300

// Buffer de Input
#define MAX_INPUT_CHARS 50

// Variáveis do Formulário
char nomeText[MAX_INPUT_CHARS + 1] = "João Miguel";
char idadeText[5] = "20";
bool nomeEditMode = false;
bool idadeEditMode = false;

// ----------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------
int main(void) {
    // Inicialização
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cadastro de Usuário com RayGUI");
    SetTargetFPS(60); 

    // Define o estilo visual (opcional)
    GuiLoadStyleDefault();

    // Loop principal da aplicação
    while (!WindowShouldClose()) {
        // --- LÓGICA DE INPUT (PROCESSAMENTO) ---
        
        // Verifica se a tecla ENTER foi pressionada em algum campo de edição
        if (IsKeyPressed(KEY_ENTER)) {
            if (nomeEditMode) nomeEditMode = false;
            else if (idadeEditMode) idadeEditMode = false;
        }

        // --- DESENHO (RENDERIZAÇÃO) ---

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); // Fundo da Janela

        // Define a área de layout centralizada
        Rectangle layout = { 25, 25, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50 };

        // Título
        DrawText("Formulário de Cadastro", layout.x, layout.y - 15, 20, DARKGRAY);

        // 1. Campo NOME (Label e Caixa de Texto)
        GuiLabel((Rectangle){ layout.x, layout.y + 20, 80, 25 }, "Nome:");
        
        Rectangle nomeRect = { layout.x + 90, layout.y + 20, layout.width - 90, 25 };
        
        // Caixa de texto editável (o modo de edição é alterado pelo clique)
        if (GuiTextBox(nomeRect, nomeText, MAX_INPUT_CHARS, nomeEditMode)) {
            nomeEditMode = !nomeEditMode;
        }

        // 2. Campo IDADE (Label e Caixa de Texto)
        GuiLabel((Rectangle){ layout.x, layout.y + 60, 80, 25 }, "Idade:");
        
        Rectangle idadeRect = { layout.x + 90, layout.y + 60, 50, 25 };
        
        if (GuiTextBox(idadeRect, idadeText, 4, idadeEditMode)) {
            idadeEditMode = !idadeEditMode;
        }
        
        // 3. Botão Salvar
        Rectangle saveRect = { layout.x, layout.y + 120, layout.width, 30 };
        if (GuiButton(saveRect, "Salvar Cadastro")) {
            // Lógica de Salvar: Imprime os valores atuais
            printf("\n--- DADOS DE CADASTRO (RayGUI) ---\n");
            printf("Nome: %s\n", nomeText);
            printf("Idade: %s\n", idadeText);
            printf("----------------------------------\n");
        }
        
        EndDrawing();
    }

    // Desinicialização
    CloseWindow(); 
    return 0;
}