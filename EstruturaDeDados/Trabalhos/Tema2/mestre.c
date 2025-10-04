#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_COMPONENTES 20

// ------------------- STRUCTS -------------------
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
    int quantidade;
} Componente;

// ------------------- PROTÓTIPOS -------------------
// Funções principais
void adicionarComponente(Componente mochila[], int *qtd, int *ordenado);
void descartarComponente(Componente mochila[], int *qtd, int *ordenado);
void mostrarComponentes(Componente mochila[], int qtd);
void organizarMochila(Componente mochila[], int qtd, int *ordenado);
void buscarComponenteBinaria(Componente mochila[], int qtd, int ordenado);

// Funções de ordenação
void ordenarBubbleNome(Componente mochila[], int qtd, int *comparacoes);
void ordenarInsertionTipo(Componente mochila[], int qtd, int *comparacoes);
void ordenarSelectionPrioridade(Componente mochila[], int qtd, int *comparacoes);

// Busca binária
int buscaBinariaPorNome(Componente mochila[], int qtd, char nome[], int *comparacoes);

// Funções auxiliares
void toLowerCase(char *str);
void printLinha(int wID, int wNome, int wTipo, int wPrioridade, int wQtd);

// ------------------- FUNÇÕES AUXILIARES -------------------

// Converte string para minúsculas (case-insensitive)
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}

// Imprime uma linha da tabela formatada
void printLinha(int wID, int wNome, int wTipo, int wPrioridade, int wQtd) {
    printf("+");
    for (int i = 0; i < wID; i++) printf("-");
    printf("+");
    for (int i = 0; i < wNome; i++) printf("-");
    printf("+");
    for (int i = 0; i < wTipo; i++) printf("-");
    printf("+");
    for (int i = 0; i < wPrioridade; i++) printf("-");
    printf("+");
    for (int i = 0; i < wQtd; i++) printf("-");
    printf("+\n");
}

// ------------------- MAIN -------------------
int main() {
    Componente mochila[MAX_COMPONENTES];
    int qtd = 0;             // quantidade de componentes na mochila
    int ordenado = 0;        // indica se está ordenada por nome (1 = sim, 0 = não)
    int opcao;

    do {
        printf("\n======= MENU MOCHILA =======\n");
        printf("Componentes na mochila: %d/%d | Ordenada por nome: %s\n",
               qtd, MAX_COMPONENTES, ordenado ? "SIM" : "NAO");
        printf("1. Adicionar componente\n");
        printf("2. Descartar componente\n");
        printf("3. Listar componentes\n");
        printf("4. Organizar mochila (ordenar)\n");
        printf("5. Buscar componente por Nome (binaria)\n");
        printf("0. Ativar torre de fuga\n");
        printf("Escolha a opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpar buffer de entrada

        switch(opcao) {
            case 1:
                adicionarComponente(mochila, &qtd, &ordenado);
                break;
            case 2:
                descartarComponente(mochila, &qtd, &ordenado);
                break;
            case 3:
                mostrarComponentes(mochila, qtd);
                break;
            case 4:
                organizarMochila(mochila, qtd, &ordenado);
                break;
            case 5:
                buscarComponenteBinaria(mochila, qtd, ordenado);
                break;
            case 0:
                printf("Ativando torre de fuga...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 0);

    return 0;
}

// ------------------- FUNÇÕES PRINCIPAIS -------------------

// Adiciona um componente na mochila
void adicionarComponente(Componente mochila[], int *qtd, int *ordenado) {
    if (*qtd >= MAX_COMPONENTES) {
        printf("Mochila cheia! Nao e possivel adicionar mais componentes.\n");
        return;
    }

    printf("Digite o nome do componente: ");
    fgets(mochila[*qtd].nome, sizeof(mochila[*qtd].nome), stdin);
    mochila[*qtd].nome[strcspn(mochila[*qtd].nome, "\n")] = 0;

    printf("Digite o tipo do componente (controle, suporte, propulsao): ");
    fgets(mochila[*qtd].tipo, sizeof(mochila[*qtd].tipo), stdin);
    mochila[*qtd].tipo[strcspn(mochila[*qtd].tipo, "\n")] = 0;

    printf("Digite a prioridade (1 a 10): ");
    scanf("%d", &mochila[*qtd].prioridade);
    getchar();

    printf("Digite a quantidade: ");
    scanf("%d", &mochila[*qtd].quantidade);
    getchar();

    (*qtd)++;
    *ordenado = 0; // qualquer adição deixa a lista desordenada
    printf("Componente adicionado com sucesso!\n");
}

// Remove um componente pelo nome
void descartarComponente(Componente mochila[], int *qtd, int *ordenado) {
    if (*qtd == 0) {
        printf("Mochila vazia! Nao ha componentes para descartar.\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do componente a descartar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    char nomeBusca[30];
    strcpy(nomeBusca, nome);
    toLowerCase(nomeBusca);

    for (int i = 0; i < *qtd; i++) {
        char nomeComp[30];
        strcpy(nomeComp, mochila[i].nome);
        toLowerCase(nomeComp);

        if (strcmp(nomeComp, nomeBusca) == 0) {
            // desloca elementos para "fechar o buraco"
            for (int j = i; j < *qtd - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*qtd)--;
            *ordenado = 0; // alteração na lista invalida ordenação
            printf("Componente descartado com sucesso!\n");
            return;
        }
    }
    printf("Componente nao encontrado!\n");
}

// Mostra os componentes em formato de tabela
void mostrarComponentes(Componente mochila[], int qtd) {
    if (qtd == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    // Largura de cada coluna
    int wID = 4, wNome = 25, wTipo = 15, wPrioridade = 10, wQtd = 10;

    // Cabeçalho
    printLinha(wID, wNome, wTipo, wPrioridade, wQtd);
    printf("|%-*s|%-*s|%-*s|%-*s|%-*s|\n",
           wID, "ID",
           wNome, "NOME",
           wTipo, "TIPO",
           wPrioridade, "PRIORIDADE",
           wQtd, "QUANTIDADE");
    printLinha(wID, wNome, wTipo, wPrioridade, wQtd);

    // Linhas de dados
    for (int i = 0; i < qtd; i++) {
        printf("|%-*d|%-*s|%-*s|%-*d|%-*d|\n",
               wID, i+1,
               wNome, mochila[i].nome,
               wTipo, mochila[i].tipo,
               wPrioridade, mochila[i].prioridade,
               wQtd, mochila[i].quantidade);
    }

    printLinha(wID, wNome, wTipo, wPrioridade, wQtd);
}

// Submenu de organização (escolhe o algoritmo de ordenação)
void organizarMochila(Componente mochila[], int qtd, int *ordenado) {
    if (qtd == 0) {
        printf("Mochila vazia! Nao ha componentes para ordenar.\n");
        return;
    }

    int opcao, comparacoes = 0;
    clock_t inicio, fim;
    double tempo;

    printf("\n--- Submenu de Organizacao ---\n");
    printf("1. Bubble Sort por NOME\n");
    printf("2. Insertion Sort por TIPO\n");
    printf("3. Selection Sort por PRIORIDADE\n");
    printf("Escolha a opcao: ");
    scanf("%d", &opcao);
    getchar();

    switch(opcao) {
        case 1:
            inicio = clock();
            ordenarBubbleNome(mochila, qtd, &comparacoes);
            fim = clock();
            *ordenado = 1; // só Bubble Sort garante ordenação por NOME
            tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            printf("Ordenacao por NOME realizada.\nComparacoes: %d | Tempo: %.6f s\n", comparacoes, tempo);
            break;
        case 2:
            inicio = clock();
            ordenarInsertionTipo(mochila, qtd, &comparacoes);
            fim = clock();
            *ordenado = 0;
            tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            printf("Ordenacao por TIPO realizada.\nComparacoes: %d | Tempo: %.6f s\n", comparacoes, tempo);
            break;
        case 3:
            inicio = clock();
            ordenarSelectionPrioridade(mochila, qtd, &comparacoes);
            fim = clock();
            *ordenado = 0;
            tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            printf("Ordenacao por PRIORIDADE realizada.\nComparacoes: %d | Tempo: %.6f s\n", comparacoes, tempo);
            break;
        default:
            printf("Opcao invalida!\n");
            return;
    }

    mostrarComponentes(mochila, qtd);
}

// Busca binária por nome (somente se ordenado por Bubble Sort)
void buscarComponenteBinaria(Componente mochila[], int qtd, int ordenado) {
    if (qtd == 0) {
        printf("Mochila vazia!\n");
        return;
    }
    if (!ordenado) {
        printf("Mochila nao esta ordenada por NOME. Ordene antes de buscar binariamente.\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do componente a buscar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    int comparacoes = 0;
    int indice = buscaBinariaPorNome(mochila, qtd, nome, &comparacoes);

    if (indice != -1) {
        printf("Componente encontrado!\n");

        int wID = 4, wNome = 25, wTipo = 15, wPrioridade = 10, wQtd = 10;
        printLinha(wID, wNome, wTipo, wPrioridade, wQtd);
        printf("|%-*s|%-*s|%-*s|%-*s|%-*s|\n",
               wID, "ID",
               wNome, "NOME",
               wTipo, "TIPO",
               wPrioridade, "PRIORIDADE",
               wQtd, "QUANTIDADE");
        printLinha(wID, wNome, wTipo, wPrioridade, wQtd);
        printf("|%-*d|%-*s|%-*s|%-*d|%-*d|\n",
               wID, indice+1,
               wNome, mochila[indice].nome,
               wTipo, mochila[indice].tipo,
               wPrioridade, mochila[indice].prioridade,
               wQtd, mochila[indice].quantidade);
        printLinha(wID, wNome, wTipo, wPrioridade, wQtd);
    } else {
        printf("Componente nao encontrado.\n");
    }
    printf("Comparacoes realizadas: %d\n", comparacoes);
}

// ------------------- ALGORITMOS DE ORDENACAO -------------------

// Bubble Sort por nome
void ordenarBubbleNome(Componente mochila[], int qtd, int *comparacoes) {
    Componente temp;
    *comparacoes = 0;
    for (int i = 0; i < qtd-1; i++) {
        for (int j = 0; j < qtd-i-1; j++) {
            (*comparacoes)++;
            char nome1[30], nome2[30];
            strcpy(nome1, mochila[j].nome);
            strcpy(nome2, mochila[j+1].nome);
            toLowerCase(nome1);
            toLowerCase(nome2);
            if (strcmp(nome1, nome2) > 0) {
                temp = mochila[j];
                mochila[j] = mochila[j+1];
                mochila[j+1] = temp;
            }
        }
    }
}

// Insertion Sort por tipo
void ordenarInsertionTipo(Componente mochila[], int qtd, int *comparacoes) {
    Componente chave;
    int j;
    *comparacoes = 0;
    for (int i = 1; i < qtd; i++) {
        chave = mochila[i];
        j = i - 1;
        char chaveTipo[20];
        strcpy(chaveTipo, chave.tipo);
        toLowerCase(chaveTipo);

        while (j >= 0) {
            char tipoJ[20];
            strcpy(tipoJ, mochila[j].tipo);
            toLowerCase(tipoJ);
            (*comparacoes)++;
            if (strcmp(tipoJ, chaveTipo) > 0) {
                mochila[j+1] = mochila[j];
                j--;
            } else {
                break;
            }
        }
        mochila[j+1] = chave;
    }
}

// Selection Sort por prioridade
void ordenarSelectionPrioridade(Componente mochila[], int qtd, int *comparacoes) {
    int min_idx;
    Componente temp;
    *comparacoes = 0;

    for (int i = 0; i < qtd-1; i++) {
        min_idx = i;
        for (int j = i+1; j < qtd; j++) {
            (*comparacoes)++;
            if (mochila[j].prioridade < mochila[min_idx].prioridade) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = mochila[i];
            mochila[i] = mochila[min_idx];
            mochila[min_idx] = temp;
        }
    }
}

// ------------------- BUSCA BINARIA -------------------
int buscaBinariaPorNome(Componente mochila[], int qtd, char nome[], int *comparacoes) {
    int inicio = 0, fim = qtd - 1, meio;
    char nomeBusca[30];
    strcpy(nomeBusca, nome);
    toLowerCase(nomeBusca);

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        (*comparacoes)++;
        char nomeMeio[30];
        strcpy(nomeMeio, mochila[meio].nome);
        toLowerCase(nomeMeio);

        int cmp = strcmp(nomeMeio, nomeBusca);
        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}
