#include <stdio.h>  // Inclui funções de entrada e saída (printf, scanf).
#include <stdlib.h> // Inclui funções de alocação de memória (malloc, free, exit).
#include <string.h> // Inclui funções de manipulação de strings (strcpy, strlen, strcmp).
// ====================== Estrutura das Salas (Árvore Binária) ======================
// Define a estrutura para cada sala da mansão (nó do mapa).
typedef struct Sala
{
    char nome[50];         // Nome da sala (ex: "Hall de Entrada").
    char pista[100];       // Pista associada a este cômodo (string vazia se não houver).
    struct Sala *esquerda; // Ponteiro para a próxima sala à esquerda.
    struct Sala *direita;  // Ponteiro para a próxima sala à direita.
} Sala;
// ====================== Estrutura da BST de Pistas ======================
// Define a estrutura para armazenar as pistas coletadas pelo jogador (BST).
typedef struct PistaNode
{
    char pista[100];            // Texto da pista (chave de ordenação da BST).
    struct PistaNode *esquerda; // Filho à esquerda (pistas alfabeticamente menores).
    struct PistaNode *direita;  // Filho à direita (pistas alfabeticamente maiores).
} PistaNode;
// ====================== Função: criarSala ======================
// Cria dinamicamente um novo nó de sala.
Sala *criarSala(const char *nome, const char *pista)
{
    Sala *nova = (Sala *)malloc(sizeof(Sala)); // Aloca memória.
    if (nova == NULL)
    {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome); // Define o nome da sala.
    if (pista != NULL)
        strcpy(nova->pista, pista); // Define a pista, se fornecida.
    else
        strcpy(nova->pista, ""); // Define string vazia se a pista for NULL.
    nova->esquerda = NULL;       // Inicializa ponteiros dos filhos.
    nova->direita = NULL;
    return nova; // Retorna o ponteiro para a nova sala.
}
// ====================== Função: criar nó da BST de Pistas ======================
// Cria dinamicamente um novo nó para a BST de pistas.
PistaNode *criarPistaNode(const char *pista)
{
    PistaNode *novo = (PistaNode *)malloc(sizeof(PistaNode)); // Aloca memória.
    if (novo == NULL)
    {
        printf("Erro ao alocar memória para pista!\n");
        exit(1);
    }
    strcpy(novo->pista, pista); // Define o texto da pista.
    novo->esquerda = NULL;      // Inicializa ponteiros dos filhos.
    novo->direita = NULL;
    return novo;
}
// ====================== Função: inserirPista ======================
// Insere uma nova pista na BST de forma recursiva.
PistaNode *inserirPista(PistaNode *raiz, const char *pista)
{
    if (raiz == NULL)
    {
        return criarPistaNode(pista); // Se o nó é nulo, insere aqui.
    }
    // Compara a nova pista com a pista do nó atual.
    if (strcmp(pista, raiz->pista) < 0)
    {
        raiz->esquerda = inserirPista(raiz->esquerda, pista); // Se menor, vai para a esquerda.
    }
    else if (strcmp(pista, raiz->pista) > 0)
    {
        raiz->direita = inserirPista(raiz->direita, pista); // Se maior, vai para a direita.
    }
    // Se for igual (strcmp == 0), a pista já existe, então não faz nada (evita duplicação).
    return raiz; // Retorna a raiz modificada.
}
// ====================== Função: exibirPistas ======================
// Exibe todas as pistas coletadas em ordem alfabética (percurso In-Order da BST).
void exibirPistas(PistaNode *raiz)
{
    if (raiz != NULL)
    {
        exibirPistas(raiz->esquerda);  // 1. Visita a subárvore esquerda.
        printf("- %s\n", raiz->pista); // 2. Imprime o nó atual.
        exibirPistas(raiz->direita);   // 3. Visita a subárvore direita.
    }
}
// ====================== Função: liberar memória da BST ======================
// Libera recursivamente a memória alocada para os nós da BST (percurso Pós-Ordem).
void liberarPistas(PistaNode *raiz)
{
    if (raiz != NULL)
    {
        liberarPistas(raiz->esquerda); // Libera esquerda.
        liberarPistas(raiz->direita);  // Libera direita.
        free(raiz);                    // Libera o nó atual.
    }
}
// ====================== Função: explorarSalasComPistas ======================
// Permite navegação interativa e coleta de pistas na mansão.
void explorarSalasComPistas(Sala *atual, PistaNode **raizPistas)
{
    char escolha;
    while (atual != NULL)
    { // Continua a exploração enquanto o nó atual for válido.
        printf("\nVocê está em: %s\n", atual->nome);
        // Se a sala tiver pista (o campo não for vazio), insere na BST.
        if (strlen(atual->pista) > 0)
        {
            printf(">> Pista encontrada: %s\n", atual->pista);
            // Insere a pista na BST usando o endereço da raiz da BST (ponteiro para ponteiro).
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        }
        // Caso seja folha (sem caminhos), para a exploração.
        if (atual->esquerda == NULL && atual->direita == NULL)
        {
            printf("Fim da exploração, não há mais caminhos.\n");
            break;
        }
        printf("Escolha um caminho: (e) esquerda, (d) direita, (s) sair: ");
        scanf(" %c", &escolha); // Lê a escolha do usuário (o espaço ignora quebra de linha).
        if (escolha == 'e' || escolha == 'E')
        {
            if (atual->esquerda != NULL)
                atual = atual->esquerda; // Move para a sala da esquerda.
            else
                printf("Não há caminho à esquerda!\n");
        }
        else if (escolha == 'd' || escolha == 'D')
        {
            if (atual->direita != NULL)
                atual = atual->direita; // Move para a sala da direita.
            else
                printf("Não há caminho à direita!\n");
        }
        else if (escolha == 's' || escolha == 'S')
        {
            printf("Você decidiu sair da exploração.\n");
            break; // Sai do loop.
        }
        else
        {
            printf("Opção inválida!\n");
        }
    }
}
// ====================== Função: liberar memória das salas ======================
// Libera recursivamente a memória alocada para a árvore de salas (percurso Pós-Ordem).
void liberarSalas(Sala *raiz)
{
    if (raiz != NULL)
    {
        liberarSalas(raiz->esquerda); // Libera subárvore esquerda.
        liberarSalas(raiz->direita);  // Libera subárvore direita.
        free(raiz);                   // Libera o nó atual.
    }
}
// ====================== Função principal ======================
int main()
{
    // Construção manual do mapa da mansão (montando a Árvore Binária).
    Sala *hall = criarSala("Hall de Entrada", "Pegada de lama"); // Raiz.
    hall->esquerda = criarSala("Sala de Estar", "Chave perdida");
    hall->direita = criarSala("Biblioteca", NULL); // Sem pista.
    hall->esquerda->esquerda = criarSala("Cozinha", "Copo quebrado");
    hall->esquerda->direita = criarSala("Jardim", "Portão aberto");
    hall->direita->direita = criarSala("Quarto", "Diário rasgado"); // Biblioteca aponta só para a direita.
    // Raiz da BST de pistas (começa vazia).
    PistaNode *pistas = NULL;
    // Inicia a exploração
    printf("=== Detective Quest: Explorando a Mansão ===\n");
    explorarSalasComPistas(hall, &pistas); // Passa o endereço do ponteiro 'pistas'.
    // Exibe pistas coletadas em ordem alfabética
    printf("\n=== Pistas coletadas ===\n");
    if (pistas == NULL)
        printf("Nenhuma pista coletada.\n");
    else
        exibirPistas(pistas); // Exibe o conteúdo da BST.
    // Libera memória alocada.
    liberarSalas(hall);
    liberarPistas(pistas);
    return 0; // Fim do programa.
}