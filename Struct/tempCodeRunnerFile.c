#include <stdio.h>
#include <locale.h>

// Adiciona a API do Windows para mudar a página de código
#ifdef _WIN32 
#include <windows.h>
#endif

// Definindo a estrutura do tipo Aluno
typedef struct {
    char nome[50];
    int idade;
    float media;
} Aluno;

// Função que configura a codificação do console (específico para Windows)
void set_utf8_console() {
    // 1. Tenta configurar a localidade C padrão para UTF-8
    setlocale(LC_ALL, "C.UTF-8");
    
    // 2. Se for Windows, força a página de código do console
    #ifdef _WIN32
        // 65001 é o identificador da página de código para UTF-8 no Windows
        SetConsoleOutputCP(65001);
    #endif
}

int main() {
    // Chama a função de configuração no início
    set_utf8_console();

    // Criando uma variável do tipo Aluno
    Aluno aluno1 = {"João", 20, 8.5};

    // Usando acentos com o prefixo u8 (garante que a string literal é UTF-8)
    printf(u8"Aluno: %s Idade: %d Média: %.2f\n", aluno1.nome, aluno1.idade, aluno1.media);
    printf(u8"Parabéns! Você tem um ótimo desempenho.\n");
    
    return 0;
}