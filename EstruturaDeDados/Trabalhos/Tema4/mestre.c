#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO_HASH 31   // tamanho da tabela hash (primo pequeno)
#define MAX_STR 100

/********************* ESTRUTURAS DE DADOS ************************/

/* Nó da árvore binária que representa uma sala (cômodo) da mansão */
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

/* Nó de BST para armazenar pistas coletadas (ordenado por string) */
typedef struct PistaNode {
    char pista[100];
    int cont;               // contagem de vezes que a pista foi coletada
    struct PistaNode* left;
    struct PistaNode* right;
} PistaNode;

/* Entrada para lista encadeada da tabela hash (encadeamento separado) */
typedef struct HashNode {
    char pista[100];        // chave
    char suspeito[100];     // valor
    struct HashNode* proximo;
} HashNode;

/* Tabelas globais (para simplicidade do exemplo) */
HashNode* tabela_hash[TAMANHO_HASH];   // associa pista -> suspeito
Sala* raiz_mansao = NULL;              // raiz da árvore de salas
PistaNode* raiz_pistas = NULL;         // raiz do BST de pistas coletadas

/********************* FUNÇÕES UTILITÁRIAS ************************/

/* removeQuebraLinha()
 * Remove a quebra de linha (\n) deixada pelo fgets.
 */
void removeQuebraLinha(char* s) {
    size_t n = strlen(s);
    if (n == 0) return;
    if (s[n-1] == '\n') s[n-1] = '\0';
}

/* converteTextoMinusculo()
 * Converte todo o texto para minúsculas (in-place) para comparações case-insensitive.
 */
void converteTextoMinusculo(char* s) {
    for (int i = 0; s[i]; i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
}

/********************* FUNÇÕES PRINCIPAIS ************************/

/* criarSala()
 * Cria dinamicamente uma sala (nó da árvore de salas) com o nome informado.
 */
Sala* criarSala(const char* nome) {
    Sala* s = (Sala*)malloc(sizeof(Sala));
    if (!s) {
        fprintf(stderr, "Erro de alocação ao criar sala '%s'\n", nome);
        exit(EXIT_FAILURE);
    }
    strncpy(s->nome, nome, sizeof(s->nome)-1);
    s->nome[sizeof(s->nome)-1] = '\0';
    s->esquerda = s->direita = NULL;
    return s;
}

/* inserirPista()
 * Insere a pista coletada na árvore BST (raiz_pistas). 
 * Se a pista já existe, incrementa o contador.
 */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        if (!novo) {
            fprintf(stderr, "Erro de alocação ao inserir pista\n");
            exit(EXIT_FAILURE);
        }
        strncpy(novo->pista, pista, sizeof(novo->pista)-1);
        novo->pista[sizeof(novo->pista)-1] = '\0';
        novo->cont = 1;
        novo->left = novo->right = NULL;
        return novo;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0) {
        raiz->cont++;
    } else if (cmp < 0) {
        raiz->left = inserirPista(raiz->left, pista);
    } else {
        raiz->right = inserirPista(raiz->right, pista);
    }
    return raiz;
}

/* inserirNaHash()
 * Insere (pista -> suspeito) na tabela hash com encadeamento.
 * Atualiza suspeito se a pista já existir.
 */
void inserirNaHash(const char* pista, const char* suspeito) {
    unsigned long soma = 0;
    for (int i = 0; pista[i]; i++) soma = soma * 31 + (unsigned char)pista[i];
    int idx = (int)(soma % TAMANHO_HASH);

    HashNode* atual = tabela_hash[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0) {
            strncpy(atual->suspeito, suspeito, sizeof(atual->suspeito)-1);
            atual->suspeito[sizeof(atual->suspeito)-1] = '\0';
            return;
        }
        atual = atual->proximo;
    }

    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    if (!novo) {
        fprintf(stderr, "Erro de alocação ao inserir na hash\n");
        exit(EXIT_FAILURE);
    }
    strncpy(novo->pista, pista, sizeof(novo->pista)-1);
    novo->pista[sizeof(novo->pista)-1] = '\0';
    strncpy(novo->suspeito, suspeito, sizeof(novo->suspeito)-1);
    novo->suspeito[sizeof(novo->suspeito)-1] = '\0';
    novo->proximo = tabela_hash[idx];
    tabela_hash[idx] = novo;
}

/* encontrarSuspeito()
 * Retorna o suspeito associado a uma pista ou NULL se não existir.
 */
const char* encontrarSuspeito(const char* pista) {
    unsigned long soma = 0;
    for (int i = 0; pista[i]; i++) soma = soma * 31 + (unsigned char)pista[i];
    int idx = (int)(soma % TAMANHO_HASH);

    HashNode* atual = tabela_hash[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0) return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}

/* verificarSuspeitoFinal()
 * Conta quantas pistas coletadas apontam para o suspeito acusado.
 * Retorna 1 se >=2 pistas confirmam; 0 caso contrário.
 */
int verificarSuspeitoFinal(PistaNode* raiz, const char* acusado) {
    if (!raiz) return 0;
    int subtotal = 0;

    subtotal += verificarSuspeitoFinal(raiz->left, acusado);
    subtotal += verificarSuspeitoFinal(raiz->right, acusado);

    const char* s = encontrarSuspeito(raiz->pista);
    if (s) {
        char susp_temp[100], acc_temp[100];
        strncpy(susp_temp, s, sizeof(susp_temp)-1); susp_temp[sizeof(susp_temp)-1] = '\0';
        strncpy(acc_temp, acusado, sizeof(acc_temp)-1); acc_temp[sizeof(acc_temp)-1] = '\0';
        converteTextoMinusculo(susp_temp);
        converteTextoMinusculo(acc_temp);

        if (strcmp(susp_temp, acc_temp) == 0) {
            subtotal += raiz->cont;
            printf("  - Pista '%s' aponta para '%s' (coletada %d vez(es)).\n", raiz->pista, s, raiz->cont);
        }
    }
    return subtotal;
}

/* listarPistasInOrder()
 * Mostra todas as pistas coletadas em ordem alfabética.
 */
void listarPistasInOrder(PistaNode* raiz) {
    if (!raiz) return;
    listarPistasInOrder(raiz->left);
    printf("  - %s (coletada %d vez(es))\n", raiz->pista, raiz->cont);
    listarPistasInOrder(raiz->right);
}

/* Função que associa pistas a salas */
const char* pistaParaSala(const char* nomeSala) {
    if (strcmp(nomeSala, "Entrada") == 0) return "Pegada de lama";
    if (strcmp(nomeSala, "Sala de estar") == 0) return "Copo quebrado";
    if (strcmp(nomeSala, "Biblioteca") == 0) return "Página arrancada";
    if (strcmp(nomeSala, "Cozinha") == 0) return "Panelas sujas";
    if (strcmp(nomeSala, "Escritório") == 0) return "Carta rasurada";
    if (strcmp(nomeSala, "Quarto principal") == 0) return "Perfume caro";
    if (strcmp(nomeSala, "Closet") == 0) return "Tecido rasgado";
    if (strcmp(nomeSala, "Jardim") == 0) return "Pegada de botas";
    if (strcmp(nomeSala, "Porão") == 0) return "Pegada de lama";
    if (strcmp(nomeSala, "Corredor") == 0) return "Mancha de tinta";
    return NULL;
}

/* explorarSalas()
 * Permite navegação interativa pela árvore de salas.
 */
void explorarSalas(Sala* atual) {
    if (!atual) return;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        const char* pista = pistaParaSala(atual->nome);
        if (pista) {
            printf("Há algo aqui: \"%s\".\n", pista);
            char opc[MAX_STR];
            printf("Deseja coletar essa pista? (s/n) [s]: ");
            if (!fgets(opc, sizeof(opc), stdin)) opc[0] = '\n';
            removeQuebraLinha(opc);
            if (opc[0] == '\0' || tolower((unsigned char)opc[0]) == 's') {
                raiz_pistas = inserirPista(raiz_pistas, pista);
                printf("Pista coletada: %s\n", pista);
            } else {
                printf("Pista deixada para trás.\n");
            }
        } else {
            printf("Nenhuma pista aparente nesta sala.\n");
        }

        printf("Escolha: (e) esquerda, (d) direita, (s) sair\nDigite a opção: ");
        char cmd[16];
        if (!fgets(cmd, sizeof(cmd), stdin)) return;
        removeQuebraLinha(cmd);
        if (cmd[0] == '\0') return;

        char c = (char)tolower((unsigned char)cmd[0]);
        if (c == 'e') {
            if (atual->esquerda) explorarSalas(atual->esquerda);
            else printf("Não há sala à esquerda.\n");
        } else if (c == 'd') {
            if (atual->direita) explorarSalas(atual->direita);
            else printf("Não há sala à direita.\n");
        } else if (c == 's') {
            printf("Saindo da sala %s.\n", atual->nome);
            return;
        } else {
            printf("Opção inválida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

/* Funções de limpeza de memória */
void liberarPistas(PistaNode* raiz) {
    if (!raiz) return;
    liberarPistas(raiz->left);
    liberarPistas(raiz->right);
    free(raiz);
}

void liberarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode* atual = tabela_hash[i];
        while (atual) {
            HashNode* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabela_hash[i] = NULL;
    }
}

void liberarMansao(Sala* r) {
    if (!r) return;
    liberarMansao(r->esquerda);
    liberarMansao(r->direita);
    free(r);
}

/********************* PROGRAMA PRINCIPAL *************************/

int main() {
    printf("=== Detective Quest: Coletor de Pistas (versão texto) ===\n\n");

    // Montagem manual da árvore da mansão
    raiz_mansao = criarSala("Entrada");
    raiz_mansao->esquerda = criarSala("Sala de estar");
    raiz_mansao->direita = criarSala("Corredor");
    raiz_mansao->esquerda->esquerda = criarSala("Biblioteca");
    raiz_mansao->esquerda->direita = criarSala("Cozinha");
    raiz_mansao->direita->esquerda = criarSala("Escritório");
    raiz_mansao->direita->direita = criarSala("Quarto principal");
    raiz_mansao->esquerda->esquerda->esquerda = criarSala("Closet");
    raiz_mansao->direita->direita->direita = criarSala("Jardim");
    raiz_mansao->direita->esquerda->esquerda = criarSala("Porão");

    // Tabela hash pista -> suspeito
    inserirNaHash("Pegada de lama", "Carlos");
    inserirNaHash("Copo quebrado", "Roberta");
    inserirNaHash("Página arrancada", "Lucas");
    inserirNaHash("Panelas sujas", "Paulo");
    inserirNaHash("Carta rasurada", "Marina");
    inserirNaHash("Perfume caro", "Amanda");
    inserirNaHash("Tecido rasgado", "Roberto");
    inserirNaHash("Pegada de botas", "Carlos");
    inserirNaHash("Mancha de tinta", "Zeca");

    printf("Mapa e pistas iniciais carregados. Explore a mansão!\n");

    explorarSalas(raiz_mansao);

    printf("\n=== Fim da Exploração ===\n");
    if (!raiz_pistas) {
        printf("Você não coletou nenhuma pista. O caso não pode ser julgado.\n");
        liberarHash();
        liberarMansao(raiz_mansao);
        return 0;
    }

    printf("Pistas coletadas (ordenadas):\n");
    listarPistasInOrder(raiz_pistas);

    char acusado[100];
    printf("\nAgora acuse um suspeito (digite o nome): ");
    if (!fgets(acusado, sizeof(acusado), stdin)) { strcpy(acusado, ""); }
    removeQuebraLinha(acusado);

    if (strlen(acusado) == 0) {
        printf("Nenhum acusado informado. Encerrando sem julgamento.\n");
        liberarPistas(raiz_pistas);
        liberarHash();
        liberarMansao(raiz_mansao);
        return 0;
    }

    printf("\nAnalisando evidências para '%s'...\n", acusado);
    int total = verificarSuspeitoFinal(raiz_pistas, acusado);

    if (total >= 2) {
        printf("\nVeredicto: HÁ evidências suficientes (%d pista(s)) contra '%s'.\n", total, acusado);
        printf("Resultado: Acusação sustentada! Parabéns, você resolveu o caso.\n");
    } else {
        printf("\nVeredicto: INSUFICIENTE (%d pista(s)) para sustentar a acusação contra '%s'.\n", total, acusado);
        printf("Resultado: Acusação não sustentada. Continue investigando.\n");
    }

    liberarPistas(raiz_pistas);
    liberarHash();
    liberarMansao(raiz_mansao);

    printf("\nSessão encerrada. Obrigado por jogar!\n");
    return 0;
}