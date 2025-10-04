#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TERRITORIO 5
#define TAM_STRING 100

typedef struct {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropas;
} Territorio;

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    Territorio territorios[MAX_TERRITORIO];
    int totalTerritorios = 0;
    int opcao;

    do {
        printf("=============================\n");
        printf("SISTEMA DE TERRITÓRIOS\n");
        printf("=============================\n");
        printf("1- Cadastrar novo Território \n");
        printf("2- Listar Territórios \n");
        printf("3- Sair \n");
        printf("=============================\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        system("clear");
        
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                printf("----Cadastro de Novo Território----\n\n");

                if (totalTerritorios < MAX_TERRITORIO) {
                    printf("Digite o nome do Território: ");
                    fgets(territorios[totalTerritorios].nome, TAM_STRING, stdin);

                    printf("Digite a cor do Exército: ");
                    fgets(territorios[totalTerritorios].cor, TAM_STRING, stdin);

                    printf("Digite o número de Tropas: ");
                    scanf("%d", &territorios[totalTerritorios].tropas);
                    limparBufferEntrada();

                    // remover \n do final
                    territorios[totalTerritorios].nome[strcspn(territorios[totalTerritorios].nome, "\n")] = '\0';
                    territorios[totalTerritorios].cor[strcspn(territorios[totalTerritorios].cor, "\n")] = '\0';

                    totalTerritorios++;

                    printf("\nTerritório cadastrado com sucesso!\n");
                } else {
                    printf("Limite de territórios atingido (%d).\n", MAX_TERRITORIO);
                }

                printf("\nPressione Enter para continuar...\n");
                getchar();
                break;

            case 2:
                if (totalTerritorios == 0) {
                    printf("Nenhum território cadastrado ainda.\n");
                } else {
                     printf("=============================\n");
                     printf("Territorios Cadastrados\n");
                    for (int i = 0; i < totalTerritorios; i++) {
                        printf("=============================\n");
                        printf("Território %d\n", i + 1);
                        printf("Nome: %s\n", territorios[i].nome);
                        printf("Cor: %s\n", territorios[i].cor);
                        printf("Tropas: %d\n", territorios[i].tropas);
                    }
                    printf("=============================\n");
                }

                printf("\nPressione Enter para continuar...\n");
                getchar();
                break;

            case 3:
                printf("Saindo do sistema...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
        }

    } while (opcao != 3);

    return 0;
}