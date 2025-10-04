#include <stdio.h>   // Inclui a biblioteca padrão de entrada e saída (para printf, scanf, etc.)
#include <stdlib.h>  // Inclui a biblioteca padrão (para funções como system)
#include <string.h>  // Inclui a biblioteca de manipulação de strings (para strcspn)

// --- Definições de Constantes ---
#define MAX_TERRITORIO 5   // Define o número máximo de territórios que podem ser cadastrados
#define TAM_STRING 100     // Define o tamanho máximo para as strings (nome e cor)

// --- Estrutura de Dados ---
// Define uma nova estrutura de dados chamada 'Territorio' para agrupar as informações
typedef struct {
    char nome[TAM_STRING]; // Nome do território (string de até 99 caracteres + '\0')
    char cor[TAM_STRING];  // Cor do exército ou facção (string de até 99 caracteres + '\0')
    int tropas;            // Número de tropas no território
} Territorio;

// --- Função para Limpar o Buffer de Entrada ---
// Essencial após usar scanf para números (como %d) antes de usar fgets (para strings)
// e para consumir o '\n' (nova linha) que sobra no buffer de entrada.
void limparBufferEntrada() {
    int c;
    // Lê caracteres do buffer de entrada até encontrar uma nova linha ('\n') ou o Fim de Arquivo (EOF)
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Função Principal ---
int main() {
    // Declara um array (vetor) de estruturas 'Territorio'. 
    // O tamanho máximo é definido por MAX_TERRITORIO (5).
    Territorio territorios[MAX_TERRITORIO];
    
    // Variável para contar quantos territórios estão de fato cadastrados.
    int totalTerritorios = 0;
    
    // Variável para armazenar a opção escolhida pelo usuário no menu.
    int opcao;

    // Inicia um loop 'do-while'. O código dentro do 'do' será executado pelo menos uma vez
    // e continuará a ser executado enquanto a condição do 'while' for verdadeira (opcao != 3).
    do {
        // --- Exibição do Menu ---
        printf("=============================\n");
        printf("SISTEMA DE TERRITÓRIOS\n");
        printf("=============================\n");
        printf("1- Cadastrar novo Território \n");
        printf("2- Listar Territórios \n");
        printf("3- Sair \n");
        printf("=============================\n");
        printf("Escolha uma opcao: ");

        // --- Leitura da Opção do Usuário ---
        scanf("%d", &opcao);
        
        // Limpa a tela do terminal. Nota: 'cls' é usado no Windows, 'clear' em sistemas Unix/Linux/macOS.
        // O código original usou "clear", assumindo um ambiente compatível com esse comando.
        system("clear");
        
        // Limpa o buffer de entrada. Isso é crucial após o 'scanf("%d", ...)' 
        // para remover o '\n' que o usuário digitou, evitando problemas nas próximas leituras de strings.
        limparBufferEntrada();

        // --- Tratamento da Opção (Switch-Case) ---
        switch (opcao) {
            case 1: // Opção: Cadastrar novo Território
                printf("----Cadastro de Novo Território----\n\n");

                // Verifica se o limite máximo de territórios (MAX_TERRITORIO) foi atingido.
                if (totalTerritorios < MAX_TERRITORIO) {
                    
                    // --- Leitura do Nome ---
                    printf("Digite o nome do Território: ");
                    // fgets é usado para ler strings com espaços de forma segura.
                    // Armazena no campo 'nome' do próximo slot livre no array (índice totalTerritorios).
                    fgets(territorios[totalTerritorios].nome, TAM_STRING, stdin);

                    // --- Leitura da Cor ---
                    printf("Digite a cor do Exército: ");
                    fgets(territorios[totalTerritorios].cor, TAM_STRING, stdin);

                    // --- Leitura do Número de Tropas ---
                    printf("Digite o número de Tropas: ");
                    // Usa scanf para ler o número inteiro.
                    scanf("%d", &territorios[totalTerritorios].tropas);
                    // Limpa o buffer após o scanf para evitar que o '\n' interfira em futuras chamadas.
                    limparBufferEntrada();

                    // --- Remoção do Caractere de Nova Linha ('\n') ---
                    // Quando o fgets lê uma string, ele inclui o '\n' final digitado pelo usuário.
                    // strcspn busca o primeiro '\n' na string, e a linha o substitui por '\0' (terminador de string).
                    territorios[totalTerritorios].nome[strcspn(territorios[totalTerritorios].nome, "\n")] = '\0';
                    territorios[totalTerritorios].cor[strcspn(territorios[totalTerritorios].cor, "\n")] = '\0';

                    // Incrementa o contador de territórios cadastrados, apontando para o próximo slot livre.
                    totalTerritorios++;

                    printf("\nTerritório cadastrado com sucesso!\n");
                } else {
                    // Mensagem se o limite for atingido.
                    printf("Limite de territórios atingido (%d).\n", MAX_TERRITORIO);
                }

                // Pausa a execução e aguarda o usuário pressionar Enter para voltar ao menu.
                printf("\nPressione Enter para continuar...\n");
                getchar(); // Captura a tecla Enter.
                break;     // Sai do bloco 'switch'.

            case 2: // Opção: Listar Territórios
                // Verifica se há algum território cadastrado.
                if (totalTerritorios == 0) {
                    printf("Nenhum território cadastrado ainda.\n");
                } else {
                    printf("=============================\n");
                    printf("Territorios Cadastrados\n");
                    
                    // Loop para percorrer todos os territórios cadastrados (de 0 até totalTerritorios - 1).
                    for (int i = 0; i < totalTerritorios; i++) {
                        printf("=============================\n");
                        // Exibe o número do território (i + 1 para começar a contar do 1).
                        printf("Território %d\n", i + 1);
                        // Exibe os dados de cada campo da estrutura.
                        printf("Nome: %s\n", territorios[i].nome);
                        printf("Cor: %s\n", territorios[i].cor);
                        printf("Tropas: %d\n", territorios[i].tropas);
                    }
                    printf("=============================\n");
                }

                // Pausa e aguarda Enter.
                printf("\nPressione Enter para continuar...\n");
                getchar();
                break;

            case 3: // Opção: Sair
                printf("Saindo do sistema...\n");
                break; // Sai do bloco 'switch'. A condição do 'do-while' será falsa, encerrando o programa.

            default: // Caso a opção digitada não seja 1, 2 ou 3.
                printf("\nOpção inválida! Tente novamente.\n");
                
                // Pausa e aguarda Enter.
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
        }

    // Condição para continuar o loop: executa enquanto a opção não for 3 (Sair).
    } while (opcao != 3);

    // Retorna 0 indicando que o programa foi executado com sucesso.
    return 0;
}