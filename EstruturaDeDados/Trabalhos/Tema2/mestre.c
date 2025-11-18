#include <stdio.h>         // Biblioteca padrão para entrada e saída (printf, scanf).
#include <stdlib.h>        // Biblioteca padrão geral (uso em geral para funções de sistema).
#include <string.h>        // Biblioteca para manipulação de strings (strcmp, strcpy, strcspn).
#include <ctype.h>         // Biblioteca para manipular caracteres (tolower).
#include <time.h>          // Biblioteca para medir o tempo de execução (clock_t, clock()).
#include <locale.h>

#define MAX_COMPONENTES 20 // Define a capacidade máxima da mochila (tamanho do vetor).


// Adiciona a API do Windows para mudar a página de código
#ifdef _WIN32 
#include <windows.h>
#endif


// Função que configura a codificação do console (específico para Windows)
void set_utf8_console() {
    // 1. Tenta configurar a localidade C padrão para UTF-8
    setlocale(LC_ALL, "C.UTF-8");
    
    // 2. Se for Windows, força a página de código do console
    #ifdef _WIN32
        // 65001 é o identificador da página de código para UTF-8 no Windows
        SetConsoleOutputCP(65001);
    #endif
}


// ------------------- STRUCTS -------------------
// Definição da estrutura Componente, que armazena os dados de cada item na mochila.
typedef struct
{
    char nome[30];  // Nome do componente (string).
    char tipo[20];  // Tipo do componente (ex: controle, suporte).
    int prioridade; // Nível de importância (1 a 10) para ordenação.
    int quantidade; // Quantidade em estoque.
} Componente;

// ------------------- PROTÓTIPOS -------------------
// Funções principais
void adicionarComponente(Componente mochila[], int *qtd, int *ordenado);   // Adiciona um novo item.
void descartarComponente(Componente mochila[], int *qtd, int *ordenado);   // Remove um item.
void mostrarComponentes(Componente mochila[], int qtd);                    // Exibe o conteúdo da mochila.
void organizarMochila(Componente mochila[], int qtd, int *ordenado);       // Submenu de ordenação.
void buscarComponenteBinaria(Componente mochila[], int qtd, int ordenado); // Implementa a busca binária.

// Funções de ordenação
void ordenarBubbleNome(Componente mochila[], int qtd, int *comparacoes);          // Ordena por nome (Bubble Sort).
void ordenarInsertionTipo(Componente mochila[], int qtd, int *comparacoes);       // Ordena por tipo (Insertion Sort).
void ordenarSelectionPrioridade(Componente mochila[], int qtd, int *comparacoes); // Ordena por prioridade (Selection Sort).

// Busca binária
int buscaBinariaPorNome(Componente mochila[], int qtd, char nome[], int *comparacoes); // Lógica da busca binária.

// Funções auxiliares
void toLowerCase(char *str);                                               // Converte string para minúsculas.
void printLinha(int wID, int wNome, int wTipo, int wPrioridade, int wQtd); // Desenha a linha da tabela.

// ------------------- FUNÇÕES AUXILIARES -------------------

// Converte string para minúsculas (necessário para comparações case-insensitive).
void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]); // Converte cada caractere.
}

// Imprime uma linha de separação formatada para a tabela.
void printLinha(int wID, int wNome, int wTipo, int wPrioridade, int wQtd)
{
    printf("+"); // Início da linha
    for (int i = 0; i < wID; i++)
        printf("-"); // Linha para ID
    printf("+");
    for (int i = 0; i < wNome; i++)
        printf("-"); // Linha para Nome
    printf("+");
    for (int i = 0; i < wTipo; i++)
        printf("-"); // Linha para Tipo
    printf("+");
    for (int i = 0; i < wPrioridade; i++)
        printf("-"); // Linha para Prioridade
    printf("+");
    for (int i = 0; i < wQtd; i++)
        printf("-"); // Linha para Quantidade
    printf("+\n");   // Fim da linha
}

// ------------------- MAIN -------------------
int main()
{
    set_utf8_console();
    Componente mochila[MAX_COMPONENTES]; // Cria o vetor de structs para a mochila.
    int qtd = 0;                         // Inicializa a quantidade atual de componentes (índice do próximo item).
    int ordenado = 0;                    // Flag para indicar se a mochila está ordenada por nome (0 = Não, 1 = Sim).
    int opcao;                           // Variável para a opção do menu.

    do
    {
        printf("\n======= MENU MOCHILA =======\n");
        // Mostra o status atual da mochila e se está ordenada por nome.
        printf("Componentes na mochila: %d/%d | Ordenada por nome: %s\n",
               qtd, MAX_COMPONENTES, ordenado ? "SIM" : "NAO");
        printf("1. Adicionar componente\n");
        printf("2. Descartar componente\n");
        printf("3. Listar componentes\n");
        printf("4. Organizar mochila (ordenar)\n");
        printf("5. Buscar componente por Nome (binaria)\n");
        printf("0. Ativar torre de fuga\n"); // Opção para sair.
        printf("Escolha a opcao: ");
        scanf("%d", &opcao); // Lê a opção do menu.
        getchar();           // Limpa o buffer (remove o '\n' após o scanf).

        switch (opcao)
        {
        case 1:
            adicionarComponente(mochila, &qtd, &ordenado); // Chama a função de adicionar.
            break;
        case 2:
            descartarComponente(mochila, &qtd, &ordenado); // Chama a função de descartar.
            break;
        case 3:
            mostrarComponentes(mochila, qtd); // Chama a função de listar.
            break;
        case 4:
            organizarMochila(mochila, qtd, &ordenado); // Chama o submenu de ordenação.
            break;
        case 5:
            buscarComponenteBinaria(mochila, qtd, ordenado); // Chama a função de busca binária.
            break;
        case 0:
            printf("Ativando torre de fuga...\n"); // Mensagem de saída.
            break;
        default:
            printf("Opcao invalida!\n"); // Trata opção inválida.
        }
    } while (opcao != 0); // O loop continua enquanto a opção não for 0.

    return 0; // Fim do programa.
}

// ------------------- FUNÇÕES PRINCIPAIS -------------------

// Adiciona um componente na mochila
void adicionarComponente(Componente mochila[], int *qtd, int *ordenado)
{
    // Verifica se a mochila está cheia.
    if (*qtd >= MAX_COMPONENTES)
    {
        printf("Mochila cheia! Nao e possivel adicionar mais componentes.\n");
        return;
    }

    printf("Digite o nome do componente: ");
    fgets(mochila[*qtd].nome, sizeof(mochila[*qtd].nome), stdin); // Lê o nome.
    mochila[*qtd].nome[strcspn(mochila[*qtd].nome, "\n")] = 0;    // Remove o '\n'.

    printf("Digite o tipo do componente (controle, suporte, propulsao): ");
    fgets(mochila[*qtd].tipo, sizeof(mochila[*qtd].tipo), stdin); // Lê o tipo.
    mochila[*qtd].tipo[strcspn(mochila[*qtd].tipo, "\n")] = 0;    // Remove o '\n'.

    printf("Digite a prioridade (1 a 10): ");
    scanf("%d", &mochila[*qtd].prioridade); // Lê a prioridade.
    getchar();                              // Limpa o buffer.

    printf("Digite a quantidade: ");
    scanf("%d", &mochila[*qtd].quantidade); // Lê a quantidade.
    getchar();                              // Limpa o buffer.

    (*qtd)++;      // Incrementa o contador de itens.
    *ordenado = 0; // Qualquer adição invalida a ordenação por nome.
    printf("Componente adicionado com sucesso!\n");
}

// Remove um componente pelo nome
void descartarComponente(Componente mochila[], int *qtd, int *ordenado)
{
    // Verifica se a mochila está vazia.
    if (*qtd == 0)
    {
        printf("Mochila vazia! Nao ha componentes para descartar.\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do componente a descartar: ");
    fgets(nome, sizeof(nome), stdin); // Lê o nome para descarte.
    nome[strcspn(nome, "\n")] = 0;    // Remove o '\n'.

    char nomeBusca[30];
    strcpy(nomeBusca, nome);
    toLowerCase(nomeBusca); // Converte o nome de busca para minúsculas.

    // Busca sequencial (pois a lista pode não estar ordenada).
    for (int i = 0; i < *qtd; i++)
    {
        char nomeComp[30];
        strcpy(nomeComp, mochila[i].nome);
        toLowerCase(nomeComp); // Converte o nome do componente para minúsculas para comparação.

        // Compara o nome de busca com o nome do componente.
        if (strcmp(nomeComp, nomeBusca) == 0)
        {
            // Se encontrado, desloca os elementos para "fechar o buraco".
            for (int j = i; j < *qtd - 1; j++)
            {
                mochila[j] = mochila[j + 1]; // Sobrescreve o componente descartado.
            }
            (*qtd)--;      // Decrementa a quantidade de itens.
            *ordenado = 0; // Alteração invalida a ordenação.
            printf("Componente descartado com sucesso!\n");
            return;
        }
    }
    printf("Componente nao encontrado!\n"); // Se o loop terminar sem encontrar.
}

// Mostra os componentes em formato de tabela
void mostrarComponentes(Componente mochila[], int qtd)
{
    if (qtd == 0)
    {
        printf("Mochila vazia!\n");
        return;
    }

    // Largura de cada coluna para formatação da tabela.
    int wID = 4, wNome = 25, wTipo = 15, wPrioridade = 10, wQtd = 10;

    // Cabeçalho da tabela
    printLinha(wID, wNome, wTipo, wPrioridade, wQtd);
    printf("|%-*s|%-*s|%-*s|%-*s|%-*s|\n", // Formato da linha de texto.
           wID, "ID",
           wNome, "NOME",
           wTipo, "TIPO",
           wPrioridade, "PRIORIDADE",
           wQtd, "QUANTIDADE");
    printLinha(wID, wNome, wTipo, wPrioridade, wQtd);

    // Linhas de dados (conteúdo da mochila)
    for (int i = 0; i < qtd; i++)
    {
        printf("|%-*d|%-*s|%-*s|%-*d|%-*d|\n",
               wID, i + 1,
               wNome, mochila[i].nome,
               wTipo, mochila[i].tipo,
               wPrioridade, mochila[i].prioridade,
               wQtd, mochila[i].quantidade);
    }

    printLinha(wID, wNome, wTipo, wPrioridade, wQtd); // Linha de fechamento.
}

// Submenu de organização (escolhe o algoritmo de ordenação)
void organizarMochila(Componente mochila[], int qtd, int *ordenado)
{
    if (qtd == 0)
    {
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

    switch (opcao)
    {
    case 1:
        inicio = clock();                                  // Marca o tempo de início.
        ordenarBubbleNome(mochila, qtd, &comparacoes);     // Ordena por nome (necessário para busca binária).
        fim = clock();                                     // Marca o tempo final.
        *ordenado = 1;                                     // Bubble Sort por nome é o único que permite busca binária.
        tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC; // Calcula o tempo de execução.
        printf("Ordenacao por NOME realizada.\nComparacoes: %d | Tempo: %.6f s\n", comparacoes, tempo);
        break;
    case 2:
        inicio = clock();
        ordenarInsertionTipo(mochila, qtd, &comparacoes); // Ordena por tipo.
        fim = clock();
        *ordenado = 0; // Outras ordenações invalidam a busca binária por nome.
        tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Ordenacao por TIPO realizada.\nComparacoes: %d | Tempo: %.6f s\n", comparacoes, tempo);
        break;
    case 3:
        inicio = clock();
        ordenarSelectionPrioridade(mochila, qtd, &comparacoes); // Ordena por prioridade.
        fim = clock();
        *ordenado = 0;
        tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Ordenacao por PRIORIDADE realizada.\nComparacoes: %d | Tempo: %.6f s\n", comparacoes, tempo);
        break;
    default:
        printf("Opcao invalida!\n");
        return;
    }

    mostrarComponentes(mochila, qtd); // Mostra a lista após a ordenação.
}

// Busca binária por nome (somente se ordenado por Bubble Sort)
void buscarComponenteBinaria(Componente mochila[], int qtd, int ordenado)
{
    if (qtd == 0)
    {
        printf("Mochila vazia!\n");
        return;
    }
    // A busca binária exige que o array esteja ordenado pelo critério de busca (nome).
    if (!ordenado)
    {
        printf("Mochila nao esta ordenada por NOME. Ordene antes de buscar binariamente.\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do componente a buscar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    int comparacoes = 0;
    // Chama a função de busca binária.
    int indice = buscaBinariaPorNome(mochila, qtd, nome, &comparacoes);

    if (indice != -1)
    {
        printf("Componente encontrado!\n");
        // Exibe os detalhes do componente encontrado em formato de tabela.
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
               wID, indice + 1,
               wNome, mochila[indice].nome,
               wTipo, mochila[indice].tipo,
               wPrioridade, mochila[indice].prioridade,
               wQtd, mochila[indice].quantidade);
        printLinha(wID, wNome, wTipo, wPrioridade, wQtd);
    }
    else
    {
        printf("Componente nao encontrado.\n");
    }
    printf("Comparacoes realizadas: %d\n", comparacoes); // Mostra a eficiência da busca.
}

// ------------------- ALGORITMOS DE ORDENACAO -------------------

// Bubble Sort por nome
void ordenarBubbleNome(Componente mochila[], int qtd, int *comparacoes)
{
    Componente temp;
    *comparacoes = 0;
    // Loops aninhados para percorrer e trocar pares adjacentes.
    for (int i = 0; i < qtd - 1; i++)
    {
        for (int j = 0; j < qtd - i - 1; j++)
        {
            (*comparacoes)++;
            char nome1[30], nome2[30];
            // Copia e converte para minúsculas para garantir ordenação case-insensitive.
            strcpy(nome1, mochila[j].nome);
            strcpy(nome2, mochila[j + 1].nome);
            toLowerCase(nome1);
            toLowerCase(nome2);
            // Compara: se nome1 for maior que nome2, a troca é realizada.
            if (strcmp(nome1, nome2) > 0)
            {
                // Swap (troca) dos elementos.
                temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
}

// Insertion Sort por tipo
void ordenarInsertionTipo(Componente mochila[], int qtd, int *comparacoes)
{
    Componente chave;
    int j;
    *comparacoes = 0;
    // Percorre o array a partir do segundo elemento.
    for (int i = 1; i < qtd; i++)
    {
        chave = mochila[i]; // Elemento a ser inserido na posição correta.
        j = i - 1;
        char chaveTipo[20];
        strcpy(chaveTipo, chave.tipo);
        toLowerCase(chaveTipo); // Converte a chave de comparação.

        // Desloca elementos maiores que a chave para a direita.
        while (j >= 0)
        {
            char tipoJ[20];
            strcpy(tipoJ, mochila[j].tipo);
            toLowerCase(tipoJ);
            (*comparacoes)++;
            // Se o elemento anterior for maior que a chave (por tipo), desloca.
            if (strcmp(tipoJ, chaveTipo) > 0)
            {
                mochila[j + 1] = mochila[j];
                j--;
            }
            else
            {
                break; // Encontrou a posição correta.
            }
        }
        mochila[j + 1] = chave; // Insere a chave na posição correta.
    }
}

// Selection Sort por prioridade
void ordenarSelectionPrioridade(Componente mochila[], int qtd, int *comparacoes)
{
    int min_idx;
    Componente temp;
    *comparacoes = 0;

    // Percorre o array para encontrar o menor elemento restante.
    for (int i = 0; i < qtd - 1; i++)
    {
        min_idx = i;
        // Encontra o índice do menor elemento no restante do array.
        for (int j = i + 1; j < qtd; j++)
        {
            (*comparacoes)++;
            // Compara a prioridade (critério numérico, não string).
            if (mochila[j].prioridade < mochila[min_idx].prioridade)
            {
                min_idx = j;
            }
        }
        // Se o menor elemento não estiver na posição atual 'i', realiza a troca.
        if (min_idx != i)
        {
            temp = mochila[i];
            mochila[i] = mochila[min_idx];
            mochila[min_idx] = temp;
        }
    }
}

// ------------------- BUSCA BINARIA -------------------
int buscaBinariaPorNome(Componente mochila[], int qtd, char nome[], int *comparacoes)
{
    int inicio = 0, fim = qtd - 1, meio;
    char nomeBusca[30];
    strcpy(nomeBusca, nome);
    toLowerCase(nomeBusca); // Converte o nome de busca para minúsculas.

    // O loop continua enquanto o início for menor ou igual ao fim.
    while (inicio <= fim)
    {
        meio = (inicio + fim) / 2; // Calcula o índice do meio.
        (*comparacoes)++;          // Conta a comparação principal.
        char nomeMeio[30];
        strcpy(nomeMeio, mochila[meio].nome);
        toLowerCase(nomeMeio); // Converte o nome do elemento do meio para minúsculas.

        int cmp = strcmp(nomeMeio, nomeBusca);
        if (cmp == 0)
            return meio; // Elemento encontrado, retorna o índice.
        else if (cmp < 0)
            inicio = meio + 1; // Se o nome do meio for menor, busca na metade superior.
        else
            fim = meio - 1; // Se o nome do meio for maior, busca na metade inferior.
    }
    return -1; // Retorna -1 se o elemento não for encontrado.
}