#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ====================== Estrutura das Salas (Árvore Binária) ======================
typedef struct Sala {
    char nome[50];             // Nome da sala
    char pista[100];           // Pista encontrada neste cômodo (pode estar vazia)
    struct Sala* esquerda;     // Ponteiro para a sala à esquerda
    struct Sala* direita;      // Ponteiro para a sala à direita
} Sala;

// ====================== Estrutura da BST de Pistas ======================
typedef struct PistaNode {
    char pista[100];              // Texto da pista
    struct PistaNode* esquerda;   // Filho à esquerda
    struct PistaNode* direita;    // Filho à direita
} PistaNode;

// ====================== Função: criarSala ======================
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, ""); // sem pista
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ====================== Função: criar nó da BST de Pistas ======================
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novo == NULL) {
        printf("Erro ao alocar memória para pista!\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// ====================== Função: inserirPista ======================
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz;
}

// ====================== Função: exibirPistas ======================
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// ====================== Função: liberar memória da BST ======================
void liberarPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// ====================== Função: explorarSalasComPistas ======================
void explorarSalasComPistas(Sala* atual, PistaNode** raizPistas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se a sala tiver pista, insere na BST
        if (strlen(atual->pista) > 0) {
            printf(">> Pista encontrada: %s\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        }

        // Caso seja folha (sem caminhos), para
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim da exploração, não há mais caminhos.\n");
            break;
        }

        printf("Escolha um caminho: (e) esquerda, (d) direita, (s) sair: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Não há caminho à esquerda!\n");
        }
        else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Não há caminho à direita!\n");
        }
        else if (escolha == 's' || escolha == 'S') {
            printf("Você decidiu sair da exploração.\n");
            break;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}

// ====================== Função: liberar memória das salas ======================
void liberarSalas(Sala* raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

// ====================== Função principal ======================
int main() {
    // Construção manual do mapa da mansão
    Sala* hall = criarSala("Hall de Entrada", "Pegada de lama");
    hall->esquerda = criarSala("Sala de Estar", "Chave perdida");
    hall->direita = criarSala("Biblioteca", NULL);

    hall->esquerda->esquerda = criarSala("Cozinha", "Copo quebrado");
    hall->esquerda->direita = criarSala("Jardim", "Portão aberto");

    hall->direita->direita = criarSala("Quarto", "Diário rasgado");

    // Raiz da BST de pistas
    PistaNode* pistas = NULL;

    // Inicia a exploração
    printf("=== Detective Quest: Explorando a Mansão ===\n");
    explorarSalasComPistas(hall, &pistas);

    // Exibe pistas coletadas em ordem alfabética
    printf("\n=== Pistas coletadas ===\n");
    if (pistas == NULL)
        printf("Nenhuma pista coletada.\n");
    else
        exibirPistas(pistas);

    // Libera memória
    liberarSalas(hall);
    liberarPistas(pistas);

    return 0;
}
