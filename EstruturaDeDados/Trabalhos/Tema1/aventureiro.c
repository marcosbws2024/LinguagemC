#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_NOME 30
#define TAM_COR 10

typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// Limpa buffer do teclado
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Cadastrar território
void cadastrarTerritorio(Territorio* mapa, int* totalTerritorios, int maxTerritorios) {
    if (*totalTerritorios >= maxTerritorios) {
        printf("Limite de territórios atingido!\n");
        return;
    }

    printf("Digite o nome do Território: ");
    fgets(mapa[*totalTerritorios].nome, TAM_NOME, stdin);
    mapa[*totalTerritorios].nome[strcspn(mapa[*totalTerritorios].nome, "\n")] = '\0';

    printf("Digite a cor do Exército: ");
    fgets(mapa[*totalTerritorios].cor, TAM_COR, stdin);
    mapa[*totalTerritorios].cor[strcspn(mapa[*totalTerritorios].cor, "\n")] = '\0';

    printf("Digite o número de Tropas: ");
    scanf("%d", &mapa[*totalTerritorios].tropas);
    limparBufferEntrada();

    (*totalTerritorios)++;
    printf("Território cadastrado com sucesso!\n");
}

// Listar territórios
void listarTerritorios(Territorio* mapa, int totalTerritorios) {
    if (totalTerritorios == 0) {
        printf("Nenhum território cadastrado.\n");
        return;
    }

    for (int i = 0; i < totalTerritorios; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

// Função de ataque contínuo
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas == 0) {
        printf("O território atacante '%s' não possui tropas e não pode atacar.\n", atacante->nome);
        return;
    }
    if (defensor->tropas == 0) {
        printf("O território defensor '%s' não possui tropas e não pode ser atacado.\n", defensor->nome);
        return;
    }

    printf("\n=== Início da batalha: %s (atacante) x %s (defensor) ===\n",
           atacante->nome, defensor->nome);

    // Batalha contínua
    while (atacante->tropas > 0 && defensor->tropas > 0) {
        int dadoAtacante = rand() % 6 + 1;
        int dadoDefensor = rand() % 6 + 1;

        printf("Dado do atacante (%s): %d | Dado do defensor (%s): %d\n",
               atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);

        if (dadoAtacante > dadoDefensor) {
            defensor->tropas--;
            atacante->tropas++;
        } else if (dadoAtacante < dadoDefensor) {
            atacante->tropas--;
            defensor->tropas++;
        } else {
            printf("Empate nesta rodada! Nenhum território perde tropas.\n");
        }

        printf("Status atual: %s=%d | %s=%d\n",
               atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
    }

    // Resultado final da batalha
    if (defensor->tropas == 0) {
        printf("\n=== Vitória do atacante '%s'! ===\n", atacante->nome);
        printf("O defensor '%s' perdeu todas as tropas.\n", defensor->nome);
        // Transferência de cor e tropas restantes (metade do atacante)
        strcpy(defensor->cor, atacante->cor);
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
        printf("O território '%s' agora pertence a '%s' com %d tropas.\n",
               defensor->nome, defensor->cor, defensor->tropas);
    } else if (atacante->tropas == 0) {
        printf("\n=== Vitória do defensor '%s'! ===\n", defensor->nome);
        printf("O atacante '%s' perdeu todas as tropas.\n", atacante->nome);
    }

    printf("=== Fim da batalha ===\n");
}

// Libera memória
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

int main() {
    srand(time(NULL));

    int totalTerritorios = 0;
    int maxTerritorios;

    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &maxTerritorios);
    limparBufferEntrada();

    Territorio* mapa = (Territorio*) calloc(maxTerritorios, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    int opcao;
    do {
        printf("\n=============================\n");
        printf("SISTEMA WAR - TERRITÓRIOS\n");
        printf("=============================\n");
        printf("1 - Cadastrar território\n");
        printf("2 - Listar territórios\n");
        printf("3 - Atacar\n");
        printf("0 - Sair\n");
        printf("=============================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                cadastrarTerritorio(mapa, &totalTerritorios, maxTerritorios);
                break;
            case 2:
                listarTerritorios(mapa, totalTerritorios);
                break;
            case 3: {
                if (totalTerritorios < 2) {
                    printf("É necessário cadastrar pelo menos 2 territórios para atacar.\n");
                    break;
                }
                int ataque, defesa;

                // Seleção do atacante
                while (1) {
                    listarTerritorios(mapa, totalTerritorios);
                    printf("\nEscolha o território atacante (0 para voltar): ");
                    scanf("%d", &ataque);
                    limparBufferEntrada();
                    if (ataque == 0) break;
                    if (ataque < 1 || ataque > totalTerritorios) {
                        printf("Número inválido!\n");
                        continue;
                    }
                    if (mapa[ataque - 1].tropas == 0) {
                        printf("Este território não possui tropas e não pode atacar.\n");
                        continue;
                    }
                    break;
                }
                if (ataque == 0) break;

                // Seleção do defensor
                while (1) {
                    printf("Escolha o território defensor (0 para voltar): ");
                    scanf("%d", &defesa);
                    limparBufferEntrada();
                    if (defesa == 0) break;
                    if (defesa < 1 || defesa > totalTerritorios) {
                        printf("Número inválido!\n");
                        continue;
                    }
                    if (defesa == ataque) {
                        printf("Não é possível atacar o próprio território!\n");
                        continue;
                    }
                    if (mapa[defesa - 1].tropas == 0) {
                        printf("Este território não possui tropas e não pode ser atacado.\n");
                        continue;
                    }
                    break;
                }
                if (defesa == 0) break;

                atacar(&mapa[ataque - 1], &mapa[defesa - 1]);
                break;
            }
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida!\n");
                break;
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    printf("Memória liberada com sucesso!\n");

    return 0;
}
