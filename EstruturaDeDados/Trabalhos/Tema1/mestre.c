#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 100

typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

typedef struct {
    char* missao;  // alocada dinamicamente
} Jogador;

// ============================
// Funções auxiliares
// ============================
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

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

// ============================
// Cadastro de territórios
// ============================
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

// ============================
// Missões
// ============================
int verificarMissao(char* missao, Territorio* mapa, int tamanho, int rodada) {
    if (strstr(missao, "Conquistar 3 territórios")) {
        int conquistados = 0;
        for (int i = 0; i < tamanho; i++)
            if (mapa[i].tropas > 0) conquistados++;
        return (conquistados >= 3);
    }
    if (strstr(missao, "cor vermelha")) {
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, "vermelha") == 0 && mapa[i].tropas > 0) return 0;
        return 1;
    }
    if (strstr(missao, "5 tropas")) {
        for (int i = 0; i < tamanho; i++)
            if (mapa[i].tropas >= 5) return 1;
        return 0;
    }
    if (strstr(missao, "2 territórios")) {
        int comTropas = 0;
        for (int i = 0; i < tamanho; i++)
            if (mapa[i].tropas > 0) comTropas++;
        return (comTropas >= 2);
    }
    if (strstr(missao, "5 rodadas")) return (rodada >= 5);
    return 0;
}

void verificarMissoesJogadores(Jogador* jogadores, int qtdJogadores,
                               Territorio* mapa, int totalTerritorios, int rodada) {
    for (int i = 0; i < qtdJogadores; i++) {
        if (jogadores[i].missao) {
            if (verificarMissao(jogadores[i].missao, mapa, totalTerritorios, rodada))
                printf("Jogador %d cumpriu a missão: %s\n", i + 1, jogadores[i].missao);
            else
                printf("Jogador %d ainda não cumpriu a missão: %s\n", i + 1, jogadores[i].missao);
        }
    }
}

void sortearMissoes(Jogador* jogadores, int qtdJogadores, char* missoes[], int totalMissoes) {
    for (int i = 0; i < qtdJogadores; i++) {
        int sorteio = rand() % totalMissoes;
        jogadores[i].missao = (char*) malloc(TAM_MISSAO * sizeof(char));
        strcpy(jogadores[i].missao, missoes[sorteio]);
        printf("Missão do Jogador %d: %s\n", i + 1, jogadores[i].missao);
    }
}

// ============================
// Batalha
// ============================
void atacar(Territorio* atacante, Territorio* defensor, 
            Jogador* jogadores, int qtdJogadores, 
            Territorio* mapa, int totalTerritorios, int rodada) {

    if (atacante->tropas == 0 || defensor->tropas == 0) {
        printf("Território sem tropas não pode atacar ou ser atacado.\n");
        return;
    }

    // Não atacar a si mesmo
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque inválido: não pode atacar território aliado.\n");
        return;
    }

    printf("\nBatalha entre '%s' e '%s'\n", atacante->nome, defensor->nome);

    while (atacante->tropas > 0 && defensor->tropas > 0) {
        int dadoAtacante = rand() % 6 + 1;
        int dadoDefensor = rand() % 6 + 1;

        printf("Dado do atacante: %d | Dado do defensor: %d\n", dadoAtacante, dadoDefensor);

        if (dadoAtacante > dadoDefensor) {
            defensor->tropas--;
            atacante->tropas++;
        } else if (dadoAtacante < dadoDefensor) {
            atacante->tropas--;
            defensor->tropas++;
        }

        printf("Status: %s=%d | %s=%d\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);

        // Verificar missão de todos os jogadores a cada rodada
        for (int i = 0; i < qtdJogadores; i++) {
            if (jogadores[i].missao) {
                if (verificarMissao(jogadores[i].missao, mapa, totalTerritorios, rodada)) {
                    printf("Jogador %d cumpriu a missão: %s\n", i+1, jogadores[i].missao);
                } else {
                    printf("Jogador %d ainda não cumpriu a missão: %s\n", i+1, jogadores[i].missao);
                }
            }
        }
    }

    if (atacante->tropas == 0) printf("%s perdeu todas as tropas!\n", atacante->nome);
    if (defensor->tropas == 0) printf("%s perdeu todas as tropas!\n", defensor->nome);

    // Mostrar resultado final da batalha
    printf("\nResultado final da batalha: %s=%d | %s=%d\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
}


// ============================
// Liberar memória
// ============================
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int qtdJogadores) {
    free(mapa);
    for (int i = 0; i < qtdJogadores; i++)
        free(jogadores[i].missao);
    free(jogadores);
}

// ============================
// Programa principal
// ============================
int main() {
    srand(time(NULL));

    int totalTerritorios = 0, maxTerritorios;
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &maxTerritorios);
    limparBufferEntrada();

    Territorio* mapa = (Territorio*) calloc(maxTerritorios, sizeof(Territorio));
    if (!mapa) { printf("Erro ao alocar memória!\n"); return 1; }

    int qtdJogadores = 2;
    Jogador* jogadores = (Jogador*) malloc(qtdJogadores * sizeof(Jogador));

    char* missoes[] = {
        "Conquistar 3 territórios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Manter pelo menos 5 tropas em um território",
        "Controlar dois territórios diferentes",
        "Sobreviver até o final de 5 rodadas"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    int rodada = 0;

    // Cadastro inicial de territórios
    for (int i = 0; i < maxTerritorios; i++)
        cadastrarTerritorio(mapa, &totalTerritorios, maxTerritorios);

    // Sorteio automático de missões
    sortearMissoes(jogadores, qtdJogadores, missoes, totalMissoes);

    // Loop principal do jogo
    int opcao;
    do {
        printf("\n=============================\n");
        printf("SISTEMA WAR - MENU\n");
        printf("=============================\n");
        printf("1 - Listar territórios\n");
        printf("2 - Atacar\n");
        printf("3 - Verificar missões\n");
        printf("0 - Sair\n");
        printf("=============================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch(opcao) {
            case 1:
                listarTerritorios(mapa, totalTerritorios);
                break;
            case 2: {
                if (totalTerritorios < 2) {
                    printf("Cadastre pelo menos 2 territórios.\n");
                    break;
                }
                int ataque, defesa;
                listarTerritorios(mapa, totalTerritorios);
                printf("Escolha atacante: "); scanf("%d", &ataque); limparBufferEntrada();
                printf("Escolha defensor: "); scanf("%d", &defesa); limparBufferEntrada();
                rodada++;
                atacar(&mapa[ataque-1], &mapa[defesa-1], jogadores, qtdJogadores,
                       mapa, totalTerritorios, rodada);
                break;
            }
            case 3:
                verificarMissoesJogadores(jogadores, qtdJogadores, mapa, totalTerritorios, rodada);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    liberarMemoria(mapa, jogadores, qtdJogadores);
    return 0;
}
