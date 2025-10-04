#include <stdio.h>   // Inclui a biblioteca padrão de entrada/saída (printf, scanf, fgets, etc.)
#include <stdlib.h>  // Inclui a biblioteca padrão (malloc, free, etc.)
#include <string.h>  // Inclui a biblioteca para manipulação de strings (strcmp, strcspn, etc.)
#define MAX_ITENS 10 // Define a capacidade máxima de itens que a mochila pode armazenar.
// Definição da struct Item: representa um objeto dentro da mochila.
typedef struct
{
    char nome[30];  // Nome do item (máximo de 30 caracteres).
    char tipo[20];  // Tipo do item (ex: arma, cura, poção).
    int quantidade; // Quantidade de unidades desse item.
} Item;
// Protótipos das Funções do sistema:
void inserirItem(Item mochila[], int *qtd); // Adiciona um novo item à mochila.
void removerItem(Item mochila[], int *qtd); // Remove um item da mochila pelo nome.
void listarItens(Item mochila[], int qtd);  // Exibe todos os itens na mochila.
void buscarItem(Item mochila[], int qtd);   // Procura e exibe um item específico pelo nome.
int main()
{
    Item mochila[MAX_ITENS]; // Declara o vetor principal (a mochila) com a capacidade máxima.
    int qtd = 0;             // Variável que rastreia a quantidade ATUAL de itens na mochila (índice do próximo item).
    int opcao;
    // Início do loop principal do menu.
    do
    {
        printf("\n======= MENU MOCHILA =======\n");
        // Mostra a ocupação atual da mochila.
        printf("Itens na mochila: %d/%d\n", qtd, MAX_ITENS);
        // Opções do menu.
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao); // Lê a opção do usuário.
        getchar();           // Limpa o buffer de entrada (consome o '\n' deixado pelo scanf).
        // Estrutura para tratar a opção escolhida.
        switch (opcao)
        {
        case 1:
            inserirItem(mochila, &qtd); // Chama a função de inserção.
            listarItens(mochila, qtd);  // Lista os itens após a inserção para mostrar o resultado.
            break;
        case 2:
            removerItem(mochila, &qtd); // Chama a função de remoção.
            listarItens(mochila, qtd);  // Lista os itens após a remoção para mostrar o resultado.
            break;
        case 3:
            listarItens(mochila, qtd); // Chama a função para listar.
            break;
        case 4:
            buscarItem(mochila, qtd); // Chama a função de busca.
            break;
        case 0:
            printf("Saindo do programa...\n"); // Opção de saída.
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n"); // Trata opções inválidas.
        }
    } while (opcao != 0); // Continua o loop enquanto a opção não for 0.
    return 0; // Encerra o programa.
}
// Função para inserir um item na mochila
void inserirItem(Item mochila[], int *qtd)
{
    // Verifica se a mochila atingiu o limite máximo.
    if (*qtd >= MAX_ITENS)
    {
        printf("Erro: Mochila cheia!\n");
        return;
    }
    Item novo; // Cria uma nova variável struct temporária.
    printf("Digite o nome do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin); // Leitura segura do nome.
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // Remove o caractere de nova linha '\n'.
    printf("Digite o tipo do item (arma, municao, cura...): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin); // Leitura segura do tipo.
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0'; // Remove o '\n'.
    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade); // Lê a quantidade.
    getchar();                     // Limpa o buffer após o scanf.
    mochila[*qtd] = novo;          // Adiciona a struct 'novo' na posição atual (livre) da mochila.
    (*qtd)++;                      // Incrementa o contador de itens.
    printf("Item \"%s\" inserido com sucesso!\n", novo.nome);
}
// Função para remover um item da mochila pelo nome
void removerItem(Item mochila[], int *qtd)
{
    // Verifica se a mochila está vazia.
    if (*qtd == 0)
    {
        printf("A mochila esta vazia.\n");
        return;
    }
    char nome[30];
    printf("Digite o nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin); // Lê o nome do item a remover.
    nome[strcspn(nome, "\n")] = '\0'; // Remove o '\n'.
    int i, pos = -1;
    // Busca sequencial: percorre a mochila para encontrar o item.
    for (i = 0; i < *qtd; i++)
    {
        // Compara os nomes. strcmp retorna 0 se forem iguais.
        if (strcmp(mochila[i].nome, nome) == 0)
        {
            pos = i; // Armazena a posição (índice) do item encontrado.
            break;   // Interrompe a busca.
        }
    }
    // Verifica se o item não foi encontrado.
    if (pos == -1)
    {
        printf("Item \"%s\" nao encontrado.\n", nome);
        return;
    }
    // Se o item foi encontrado (pos != -1):
    // Desloca todos os itens seguintes uma posição para a esquerda.
    for (i = pos; i < *qtd - 1; i++)
    {
        mochila[i] = mochila[i + 1]; // O item na posição 'i' recebe o item da posição 'i+1'.
    }
    (*qtd)--; // Decrementa o contador, reduzindo o tamanho lógico da mochila.
    printf("Item \"%s\" removido com sucesso!\n", nome);
}
// Função para listar os itens da mochila
void listarItens(Item mochila[], int qtd)
{
    // Verifica se há itens para listar.
    if (qtd == 0)
    {
        printf("A mochila esta vazia.\n");
        return;
    }
    printf("\nItens na mochila:\n");
    // Percorre apenas os itens cadastrados (de 0 até qtd - 1).
    for (int i = 0; i < qtd; i++)
    {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}
// Função de busca sequencial pelo nome do item
void buscarItem(Item mochila[], int qtd)
{
    // Verifica se há itens para buscar.
    if (qtd == 0)
    {
        printf("A mochila esta vazia.\n");
        return;
    }
    char nome[30];
    printf("Digite o nome do item a buscar: ");
    fgets(nome, sizeof(nome), stdin); // Lê o nome do item a buscar.
    nome[strcspn(nome, "\n")] = '\0'; // Remove o '\n'.
    // Busca sequencial: percorre a mochila.
    for (int i = 0; i < qtd; i++)
    {
        // Compara o nome.
        if (strcmp(mochila[i].nome, nome) == 0)
        {
            // Item encontrado: imprime os detalhes.
            printf("Item encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return; // Encerra a função após encontrar.
        }
    }
    // Se o loop terminar sem encontrar o item.
    printf("Item \"%s\" nao encontrado.\n", nome);
}
