#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ITENS 10

// Struct para representar cada item da mochila
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// ------------------- FUNÇÕES AUXILIARES -------------------

// Converte string para minúsculas
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// ------------------- FUNÇÕES PRINCIPAIS -------------------
void inserirItem(Item mochila[], int *qtd);
void removerItem(Item mochila[], int *qtd);
void listarItens(Item mochila[], int qtd);
void buscarItemMenu(Item mochila[], int qtd);
void buscarSequencial(Item mochila[], int qtd, char nome[]);
void ordenarBubbleSort(Item mochila[], int qtd);
int buscarBinaria(Item mochila[], int qtd, char nome[]);

// ------------------- MENU PRINCIPAL -------------------
int main() {
    Item mochila[MAX_ITENS];
    int qtd = 0; // quantidade de itens cadastrados
    int opcao;

    do {
        printf("\n======= MENU MOCHILA =======\n");
        printf("Itens na mochila: %d/%d\n", qtd, MAX_ITENS);
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item por Nome\n");
        printf("0. Fechar a Mochila\n");
        printf("Escolha a opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpar buffer

        switch(opcao) {
            case 1:
                inserirItem(mochila, &qtd);
                break;
            case 2:
                removerItem(mochila, &qtd);
                break;
            case 3:
                listarItens(mochila, qtd);
                break;
            case 4:
                buscarItemMenu(mochila, qtd);
                break;
            case 0:
                printf("Fechando Mochila...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);

    return 0;
}

// ------------------- FUNÇÕES -------------------

// Inserir item
void inserirItem(Item mochila[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("Mochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    printf("Digite o nome do item: ");
    fgets(mochila[*qtd].nome, sizeof(mochila[*qtd].nome), stdin);
    mochila[*qtd].nome[strcspn(mochila[*qtd].nome, "\n")] = 0;

    printf("Digite o tipo do item (arma, municao, cura): ");
    fgets(mochila[*qtd].tipo, sizeof(mochila[*qtd].tipo), stdin);
    mochila[*qtd].tipo[strcspn(mochila[*qtd].tipo, "\n")] = 0;

    printf("Digite a quantidade: ");
    scanf("%d", &mochila[*qtd].quantidade);
    getchar();

    (*qtd)++;
    printf("Item adicionado com sucesso!\n");
}

// Remover item
void removerItem(Item mochila[], int *qtd) {
    if (*qtd == 0) {
        printf("Mochila vazia! Nao ha itens para remover.\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    char nomeBusca[30];
    strcpy(nomeBusca, nome);
    toLowerCase(nomeBusca);

    for (int i = 0; i < *qtd; i++) {
        char nomeItem[30];
        strcpy(nomeItem, mochila[i].nome);
        toLowerCase(nomeItem);

        if (strcmp(nomeItem, nomeBusca) == 0) {
            for (int j = i; j < *qtd - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*qtd)--;
            printf("Item removido com sucesso!\n");
            return;
        }
    }
    printf("Item nao encontrado!\n");
}

// Listar itens
void listarItens(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    printf("\n--- Itens na mochila ---\n");
    for (int i = 0; i < qtd; i++) {
        printf("Nome: %s | Tipo: %s | Quantidade: %d\n", 
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Submenu de busca
void buscarItemMenu(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    int opcao;
    char nome[30];

    printf("Digite o nome do item a buscar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("\n=== Submenu de Busca ===\n");
    printf("1. Busca Sequencial\n");
    printf("2. Busca Binaria (ordena antes)\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    getchar();

    switch(opcao) {
        case 1:
            buscarSequencial(mochila, qtd, nome);
            break;
        case 2:
            ordenarBubbleSort(mochila, qtd); // precisa estar ordenado
            if (buscarBinaria(mochila, qtd, nome) == -1)
                printf("Item nao encontrado!\n");
            break;
        default:
            printf("Opcao invalida!\n");
    }
}

// Busca Sequencial
void buscarSequencial(Item mochila[], int qtd, char nome[]) {
    int comparacoes = 0;
    char nomeBusca[30];
    strcpy(nomeBusca, nome);
    toLowerCase(nomeBusca);

    for (int i = 0; i < qtd; i++) {
        comparacoes++;
        char nomeItem[30];
        strcpy(nomeItem, mochila[i].nome);
        toLowerCase(nomeItem);

        if (strcmp(nomeItem, nomeBusca) == 0) {
            printf("Item encontrado: Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            return;
        }
    }
    printf("Item nao encontrado! Comparacoes realizadas: %d\n", comparacoes);
}

// Ordenar vetor com Bubble Sort (ignora maiúsculas/minúsculas)
void ordenarBubbleSort(Item mochila[], int qtd) {
    Item temp;
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - i - 1; j++) {
            char nome1[30], nome2[30];
            strcpy(nome1, mochila[j].nome);
            strcpy(nome2, mochila[j + 1].nome);
            toLowerCase(nome1);
            toLowerCase(nome2);

            if (strcmp(nome1, nome2) > 0) {
                temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
}

// Busca Binaria
int buscarBinaria(Item mochila[], int qtd, char nome[]) {
    int inicio = 0, fim = qtd - 1, meio;
    int comparacoes = 0;

    char nomeBusca[30];
    strcpy(nomeBusca, nome);
    toLowerCase(nomeBusca);

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        comparacoes++;

        char nomeItem[30];
        strcpy(nomeItem, mochila[meio].nome);
        toLowerCase(nomeItem);

        int cmp = strcmp(nomeItem, nomeBusca);

        if (cmp == 0) {
            printf("Item encontrado: Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[meio].nome, mochila[meio].tipo, mochila[meio].quantidade);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            return meio;
        }
        else if (cmp < 0) {
            inicio = meio + 1;
        }
        else {
            fim = meio - 1;
        }
    }
    printf("Item nao encontrado! Comparacoes realizadas: %d\n", comparacoes);
    return -1;
}
