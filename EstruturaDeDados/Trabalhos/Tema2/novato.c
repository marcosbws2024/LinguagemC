#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10   // capacidade máxima da mochila

// Definição da struct Item
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Funções do sistema
void inserirItem(Item mochila[], int *qtd);
void removerItem(Item mochila[], int *qtd);
void listarItens(Item mochila[], int qtd);
void buscarItem(Item mochila[], int qtd);

int main() {
    Item mochila[MAX_ITENS]; // vetor que armazena os itens
    int qtd = 0;             // quantidade atual de itens
    int opcao;

    do {
        printf("\n======= MENU MOCHILA =======\n");
        printf("Itens na mochila: %d/%d\n", qtd, MAX_ITENS);
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpar buffer do ENTER

        switch (opcao) {
            case 1:
                inserirItem(mochila, &qtd);
                listarItens(mochila, qtd); // lista após inserção
                break;
            case 2:
                removerItem(mochila, &qtd);
                listarItens(mochila, qtd); // lista após remoção
                break;
            case 3:
                listarItens(mochila, qtd);
                break;
            case 4:
                buscarItem(mochila, qtd);
                listarItens(mochila, qtd); // lista após busca
                break;
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// Função para inserir um item na mochila
void inserirItem(Item mochila[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("Erro: Mochila cheia!\n");
        return;
    }

    Item novo;
    printf("Digite o nome do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // remove \n

    printf("Digite o tipo do item (arma, municao, cura...): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar(); // limpar buffer

    mochila[*qtd] = novo; // adiciona ao vetor
    (*qtd)++;
    printf("Item \"%s\" inserido com sucesso!\n", novo.nome);
}

// Função para remover um item da mochila pelo nome
void removerItem(Item mochila[], int *qtd) {
    if (*qtd == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int i, pos = -1;
    for (i = 0; i < *qtd; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Item \"%s\" nao encontrado.\n", nome);
        return;
    }

    // desloca itens para preencher a posição removida
    for (i = pos; i < *qtd - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    (*qtd)--;

    printf("Item \"%s\" removido com sucesso!\n", nome);
}

// Função para listar os itens da mochila
void listarItens(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    printf("\nItens na mochila:\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Função de busca sequencial pelo nome do item
void buscarItem(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a buscar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (int i = 0; i < qtd; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("Item encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("Item \"%s\" nao encontrado.\n", nome);
}
