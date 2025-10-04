#include <stdio.h>   // Inclui a biblioteca padrão de entrada e saída (printf, scanf, etc.).
#include <stdlib.h>  // Inclui a biblioteca padrão (funções gerais).
#include <string.h>  // Inclui a biblioteca para manipulação de strings (strcmp, strcpy, strcspn).
#include <ctype.h>   // Inclui a biblioteca para manipulação de caracteres (tolower).
#define MAX_ITENS 10 // Define a capacidade máxima de itens que a mochila suporta.
// Struct para representar cada item da mochila
typedef struct
{
    char nome[30];  // Nome do item.
    char tipo[20];  // Tipo do item (ex: arma, cura).
    int quantidade; // Quantidade em estoque.
} Item;
// ------------------- FUNÇÕES AUXILIARES -------------------
// Converte string para minúsculas (essencial para buscar e ordenar sem diferenciar maiúsculas/minúsculas).
void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower((unsigned char)str[i]); // Converte cada caractere.
    }
}
// ------------------- FUNÇÕES PRINCIPAIS -------------------
void inserirItem(Item mochila[], int *qtd);                  // Adiciona um item na mochila.
void removerItem(Item mochila[], int *qtd);                  // Remove um item pelo nome.
void listarItens(Item mochila[], int qtd);                   // Exibe todos os itens.
void buscarItemMenu(Item mochila[], int qtd);                // Apresenta o submenu de busca.
void buscarSequencial(Item mochila[], int qtd, char nome[]); // Implementa a busca sequencial.
void ordenarBubbleSort(Item mochila[], int qtd);             // Implementa a ordenação Bubble Sort.
int buscarBinaria(Item mochila[], int qtd, char nome[]);     // Implementa a busca binária.
// ------------------- MENU PRINCIPAL -------------------
int main()
{
    Item mochila[MAX_ITENS]; // Cria o vetor principal (a mochila).
    int qtd = 0;             // Variável que conta a quantidade atual de itens (começa em 0).
    int opcao;               // Variável para a escolha do menu.
    do
    {
        printf("\n======= MENU MOCHILA =======\n");
        printf("Itens na mochila: %d/%d\n", qtd, MAX_ITENS); // Mostra o status de ocupação.
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item por Nome\n");
        printf("0. Fechar a Mochila\n");
        printf("Escolha a opcao: ");
        scanf("%d", &opcao); // Lê a opção.
        getchar();           // Limpa o buffer de entrada (remove o '\n' após o scanf).
        switch (opcao)
        {
        case 1:
            inserirItem(mochila, &qtd); // Adicionar.
            break;
        case 2:
            removerItem(mochila, &qtd); // Remover.
            break;
        case 3:
            listarItens(mochila, qtd); // Listar.
            break;
        case 4:
            buscarItemMenu(mochila, qtd); // Submenu de busca.
            break;
        case 0:
            printf("Fechando Mochila...\n"); // Sair.
            break;
        default:
            printf("Opcao invalida!\n"); // Trata opção inválida.
        }
    } while (opcao != 0); // Continua o loop até que a opção 0 seja escolhida.
    return 0; // Fim do programa.
}
// ------------------- FUNÇÕES -------------------
// Inserir item
void inserirItem(Item mochila[], int *qtd)
{
    // Verifica se a mochila está cheia.
    if (*qtd >= MAX_ITENS)
    {
        printf("Mochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }
    printf("Digite o nome do item: ");
    fgets(mochila[*qtd].nome, sizeof(mochila[*qtd].nome), stdin); // Leitura segura do nome.
    mochila[*qtd].nome[strcspn(mochila[*qtd].nome, "\n")] = 0;    // Remove o '\n' do final.
    printf("Digite o tipo do item (arma, municao, cura): ");
    fgets(mochila[*qtd].tipo, sizeof(mochila[*qtd].tipo), stdin); // Leitura segura do tipo.
    mochila[*qtd].tipo[strcspn(mochila[*qtd].tipo, "\n")] = 0;    // Remove o '\n' do final.
    printf("Digite a quantidade: ");
    scanf("%d", &mochila[*qtd].quantidade); // Lê a quantidade.
    getchar();                              // Limpa o buffer.
    (*qtd)++;                               // Incrementa o contador de itens.
    printf("Item adicionado com sucesso!\n");
}
// Remover item
void removerItem(Item mochila[], int *qtd)
{
    // Verifica se a mochila está vazia.
    if (*qtd == 0)
    {
        printf("Mochila vazia! Nao ha itens para remover.\n");
        return;
    }
    char nome[30];
    printf("Digite o nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin); // Lê o nome a ser removido.
    nome[strcspn(nome, "\n")] = 0;    // Remove o '\n'.
    char nomeBusca[30];
    strcpy(nomeBusca, nome);
    toLowerCase(nomeBusca); // Converte para minúsculas.
    // Busca o item para remoção.
    for (int i = 0; i < *qtd; i++)
    {
        char nomeItem[30];
        strcpy(nomeItem, mochila[i].nome);
        toLowerCase(nomeItem); // Converte o nome do item para minúsculas.
        // Compara os nomes.
        if (strcmp(nomeItem, nomeBusca) == 0)
        {
            // Se encontrado, desloca todos os itens seguintes para preencher o espaço.
            for (int j = i; j < *qtd - 1; j++)
            {
                mochila[j] = mochila[j + 1];
            }
            (*qtd)--; // Decrementa a quantidade de itens.
            printf("Item removido com sucesso!\n");
            return;
        }
    }
    printf("Item nao encontrado!\n"); // Se o loop terminar sem encontrar.
}
// Listar itens
void listarItens(Item mochila[], int qtd)
{
    if (qtd == 0)
    {
        printf("Mochila vazia!\n");
        return;
    }
    printf("\n--- Itens na mochila ---\n");
    // Itera e imprime os detalhes de todos os itens cadastrados.
    for (int i = 0; i < qtd; i++)
    {
        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}
// Submenu de busca
void buscarItemMenu(Item mochila[], int qtd)
{
    if (qtd == 0)
    {
        printf("Mochila vazia!\n");
        return;
    }
    int opcao;
    char nome[30];
    printf("Digite o nome do item a buscar: ");
    fgets(nome, sizeof(nome), stdin); // Lê o nome que será buscado nas funções.
    nome[strcspn(nome, "\n")] = 0;    // Remove o '\n'.
    printf("\n=== Submenu de Busca ===\n");
    printf("1. Busca Sequencial\n");
    printf("2. Busca Binaria (ordena antes)\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    getchar();
    switch (opcao)
    {
    case 1:
        buscarSequencial(mochila, qtd, nome); // Executa a busca sequencial.
        break;
    case 2:
        ordenarBubbleSort(mochila, qtd);             // Ordena o vetor (obrigatório para busca binária).
        if (buscarBinaria(mochila, qtd, nome) == -1) // Executa a busca binária.
            printf("Item nao encontrado!\n");        // A mensagem de erro é duplicada para fins didáticos.
        break;
    default:
        printf("Opcao invalida!\n");
    }
}
// Busca Sequencial
void buscarSequencial(Item mochila[], int qtd, char nome[])
{
    int comparacoes = 0;
    char nomeBusca[30];
    strcpy(nomeBusca, nome);
    toLowerCase(nomeBusca); // Prepara o nome de busca para comparação case-insensitive.
    // Percorre todos os elementos da mochila.
    for (int i = 0; i < qtd; i++)
    {
        comparacoes++; // Conta cada comparação.
        char nomeItem[30];
        strcpy(nomeItem, mochila[i].nome);
        toLowerCase(nomeItem); // Converte o nome do item atual para comparação.
        if (strcmp(nomeItem, nomeBusca) == 0)
        { // Se os nomes forem iguais.
            printf("Item encontrado: Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            printf("Comparacoes realizadas: %d\n", comparacoes); // Mostra a eficiência.
            return;                                              // Sai da função após encontrar.
        }
    }
    printf("Item nao encontrado! Comparacoes realizadas: %d\n", comparacoes);
}
// Ordenar vetor com Bubble Sort (ignora maiúsculas/minúsculas)
void ordenarBubbleSort(Item mochila[], int qtd)
{
    Item temp;
    // Loop externo: controla o número de passagens.
    for (int i = 0; i < qtd - 1; i++)
    {
        // Loop interno: realiza as comparações e trocas.
        for (int j = 0; j < qtd - i - 1; j++)
        {
            char nome1[30], nome2[30];
            // Copia os nomes e converte para minúsculas antes de comparar.
            strcpy(nome1, mochila[j].nome);
            strcpy(nome2, mochila[j + 1].nome);
            toLowerCase(nome1);
            toLowerCase(nome2);
            // Compara os nomes. Se o primeiro for "maior" (ordem alfabética posterior), troca.
            if (strcmp(nome1, nome2) > 0)
            {
                // Realiza o swap (troca) de toda a struct.
                temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
}
// Busca Binaria
int buscarBinaria(Item mochila[], int qtd, char nome[])
{
    int inicio = 0, fim = qtd - 1, meio;
    int comparacoes = 0;
    char nomeBusca[30];
    strcpy(nomeBusca, nome);
    toLowerCase(nomeBusca); // Prepara o nome de busca.
    // O loop continua enquanto o intervalo de busca for válido.
    while (inicio <= fim)
    {
        meio = (inicio + fim) / 2; // Calcula o ponto médio.
        comparacoes++;             // Conta a comparação.
        char nomeItem[30];
        strcpy(nomeItem, mochila[meio].nome);
        toLowerCase(nomeItem);                 // Converte o nome do item do meio.
        int cmp = strcmp(nomeItem, nomeBusca); // Compara o item do meio com o buscado.
        if (cmp == 0)
        {
            printf("Item encontrado: Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[meio].nome, mochila[meio].tipo, mochila[meio].quantidade);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            return meio; // Item encontrado, retorna o índice.
        }
        else if (cmp < 0)
        {
            inicio = meio + 1; // Busca na metade direita (nome buscado é maior).
        }
        else
        {
            fim = meio - 1; // Busca na metade esquerda (nome buscado é menor).
        }
    }
    // Sai do loop: item não encontrado.
    printf("Item nao encontrado! Comparacoes realizadas: %d\n", comparacoes);
    return -1;
}