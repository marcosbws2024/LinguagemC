#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h> // Necessário para strlen()

// ⚠️ PRÉ-COMPILAÇÃO PARA WINDOWS E BIBLIOTECAS ESPECÍFICAS
#ifdef _WIN32
    #include <windows.h> // Necessário para SetConsoleOutputCP() e GetConsoleOutputCP()
#endif

// ----------------------------------------------------
// FUNÇÃO 1: Configuração do Ambiente para UTF-8 e Unicode
// ----------------------------------------------------
void setup_utf8_environment() {
    // 1. Configuração de Localidade C (Portátil)
    // Tenta usar a localidade padrão do sistema (que em Linux/macOS modernos é UTF-8).
    if (setlocale(LC_ALL, "") == NULL) {
        setlocale(LC_ALL, "C.UTF-8");
    }

    // 2. Configuração Específica do Console Windows
    #ifdef _WIN32
        UINT original_cp = GetConsoleOutputCP();
        // ESSENCIAL: Força a página de código do console a ser UTF-8 (65001).
        if (SetConsoleOutputCP(65001)) {
            // MENSAGEM DE DIAGNÓSTICO: Ajuda a verificar se a correção funcionou.
            printf("Página de Código do Console: Original=%u, Nova=65001 (UTF-8).\n", original_cp);
        } else {
            printf("ERRO: Não foi possível definir a página de código do console para UTF-8. Tente usar o Windows Terminal ou PowerShell.\n");
        }
    #endif
}

// ----------------------------------------------------
// FUNÇÃO 2: Limpeza de Tela (Clear)
// ----------------------------------------------------
void clear_screen() {
    #ifdef _WIN32
        system("cls"); // Comando para Windows
    #else
        system("clear"); // Comando para Linux/macOS
    #endif
}

// ----------------------------------------------------
// FUNÇÃO 3: Pausa e Aguarda Input (Windows-specific)
// ----------------------------------------------------
void pause_and_wait() {
    #ifdef _WIN32
        // system("pause > nul") simula a funcionalidade de pausa e suprime o texto.
        system("pause > nul"); 
    #endif
    // Ignorado em Unix/Linux para não interromper a execução em ambientes de script.
}


// ----------------------------------------------------
// FUNÇÃO 4: Análise do Tamanho em Bytes (UTF-8)
// ----------------------------------------------------
void analyze_utf8_string(const char *str, const char *description) {
    size_t byte_size = strlen(str);
    size_t char_count = 0; 

    // Itera e conta os caracteres baseando-se na estrutura do byte inicial do UTF-8.
    for (size_t i = 0; i < byte_size; i++) {
        // Se o byte começa com 0 (0xxxxxxx) é ASCII de 1 byte
        if ((str[i] & 0x80) == 0x00) {
            char_count++;
        } 
        // Se o byte começa com 11 (11xxxxxx) é o início de um caractere multi-byte
        else if ((str[i] & 0xC0) == 0xC0) {
            char_count++;
        }
    }

    printf(u8"\n## Análise de Codificação UTF-8: %s\n", description);
    printf(u8"--------------------------------------------------\n");
    printf(u8"String: %s\n", str);
    printf(u8"Tamanho em BYTES (strlen): %zu bytes\n", byte_size);
    printf(u8"Tamanho APROXIMADO em CARACTERES: %zu caracteres\n", char_count);

    if (byte_size > char_count) {
        printf(u8"⚠️ Conclusão: A codificação UTF-8 usou múltiplos bytes para caracteres Unicode.\n");
    } else {
        printf(u8"✅ Conclusão: A string contém apenas caracteres ASCII de 1 byte.\n");
    }
}

// ----------------------------------------------------
// FUNÇÃO 5: Análise Visual de Bytes em Hexadecimal
// ----------------------------------------------------
void print_byte_analysis(const char *str) {
    printf(u8"\n>>> INSPEÇÃO DE BYTES BRUTOS (HEXADECIMAL)\n");
    
    int bytes_in_codepoint = 0; // Contador de quantos bytes faltam para completar o Code Point
    
    for (size_t i = 0; i < strlen(str); i++) {
        unsigned char byte = (unsigned char)str[i];
        
        printf("Byte %2zu: 0x%02X ", i, byte); 

        // 1. Byte ASCII (1 byte)
        if (byte < 0x80) { 
            printf(" | Char: '%c' (ASCII/1-Byte)\n", byte);
            bytes_in_codepoint = 0;
        } 
        // 2. Byte de INÍCIO de sequência UTF-8
        else if ((byte & 0xE0) == 0xC0) { // 110xxxxx (2 bytes no total)
            printf(" | INÍCIO (2-Bytes) - Caractere Unicode Básico\n");
            bytes_in_codepoint = 1;
        }
        else if ((byte & 0xF0) == 0xE0) { // 1110xxxx (3 bytes no total)
            printf(" | INÍCIO (3-Bytes) - Caractere Unicode (ex: CJK)\n");
            bytes_in_codepoint = 2;
        }
        else if ((byte & 0xF8) == 0xF0) { // 11110xxx (4 bytes no total)
            printf(" | INÍCIO (4-Bytes) - Caractere Unicode (ex: Emoji)\n");
            bytes_in_codepoint = 3;
        }
        // 3. Byte de CONTINUAÇÃO de sequência UTF-8
        else if ((byte & 0xC0) == 0x80) { // 10xxxxxx
            printf(" | CONTINUAÇÃO de sequência UTF-8 (Faltam: %d)\n", bytes_in_codepoint);
            if (bytes_in_codepoint > 0) {
                bytes_in_codepoint--;
            }
        }
        // 4. Byte Inválido
        else {
            printf(" | BYTE INVÁLIDO ou Codificação Quebrada\n");
            bytes_in_codepoint = 0;
        }
    }
    printf("--------------------------------------------------\n");
}

// ----------------------------------------------------
// FUNÇÃO 6: Simulação de Confusão de Codificação
// ----------------------------------------------------
void simulate_encoding_confusion() {
    printf(u8"\n## SIMULAÇÃO: Confusão entre Codificações (Problema que libiconv resolve)\n");
    printf(u8"-----------------------------------------------------------------------\n");
    
    // O Code Point 'á' em ISO-8859-1 (Latin-1) é um único byte: 0xE1
    // O Code Point 'á' em UTF-8 são dois bytes: 0xC3 0xA1
    
    // Isto simula a leitura de um byte ISO-8859-1 (0xE1) por um sistema UTF-8.
    const char *latin1_byte = "\xE1o"; // 'á' (0xE1) seguido de 'o' (0x6F)
    
    printf(u8"String Teórica (ISO-8859-1):\t 'á o'\n");
    printf(u8"Bytes Reais no C:\t\t 0x%02X 0x%02X\n", (unsigned char)latin1_byte[0], (unsigned char)latin1_byte[1]);
    
    printf(u8"\nProblema: Tentando ler 0xE1 0x6F como UTF-8:\n");
    printf(u8"1. O console lê o byte 0xE1 (1110xxxx) e o interpreta como o INÍCIO de um caractere de 3 bytes.\n");
    printf(u8"2. O próximo byte 0x6F é ASCII puro (0xxxxxxx), não um byte de CONTINUAÇÃO (10xxxxxx).\n");
    printf(u8"3. Resultado: A sequência UTF-8 é inválida, resultando em um caractere 'quebrado' seguido pelo 'o'.\n");
    
    // O console (configurado para UTF-8) vai interpretar 0xE1 0x6F de forma errada.
    printf(u8"Resultado Visual (Lido como UTF-8): %s\n", latin1_byte);
    
    printf(u8"\nSolução (libiconv): A biblioteca converteria 0xE1 (Latin-1) para 0xC3 0xA1 (UTF-8) ANTES da impressão.\n");
}


// ----------------------------------------------------
// ESTRUTURA E MAIN
// ----------------------------------------------------

// Definindo a estrutura do tipo Aluno
typedef struct {
    char nome[50];
    int idade;
    float media;
} Aluno;

int main() {
    // 1. CONFIGURAÇÃO AMBIENTE (OBRIGATÓRIO PARA UTF-8)
    setup_utf8_environment();
    
    printf(u8"Iniciando programa de Análise Avançada de UTF-8...\n");
    pause_and_wait();
    clear_screen();

    Aluno aluno1 = {u8"João Miguel", 20, 8.5}; 

    // --- SEÇÃO 1: Exibição e Contagem de Caracteres ---
    
    printf(u8"## Seção 1: Exibição de Dados e Contagem de Caracteres\n");
    printf(u8"--------------------------------------------------\n");
    printf(u8"Aluno: %s | Idade: %d | Média: %.2f\n", aluno1.nome, aluno1.idade, aluno1.media);
    printf(u8"Parabéns! Você tem um ótimo desempenho. 🎉\n");
    
    // Demonstração do Tamanho em Bytes
    analyze_utf8_string(u8"Hello", "1. String puramente ASCII");
    analyze_utf8_string(u8"Atenção", "2. String com acento ('ã')");
    analyze_utf8_string(u8"Coração ❤️", "3. String com acento e emoji (Unicode)");
    
    // --- SEÇÃO 2: Inspeção de Bytes Brutos ---

    printf(u8"\n\n## Seção 2: Inspeção Detalhada dos Bytes (Hexadecimal)\n");

    // Exemplo Acentuado (caracteres de 2 bytes: 0xC3 0xA3 0xC3 0xA3)
    print_byte_analysis(u8"ão");
    
    // Exemplo com Emoji (caracteres de 4 bytes: 0xE2 0x9D 0xA4 0xEF 0xB8 0x8F)
    print_byte_analysis(u8"❤️"); 

    // --- SEÇÃO 3: Simulação de Erro de Codificação (libiconv) ---
    simulate_encoding_confusion();

    printf(u8"\n\n--- Fim do Programa ---\n");
    pause_and_wait();
    return 0;
}