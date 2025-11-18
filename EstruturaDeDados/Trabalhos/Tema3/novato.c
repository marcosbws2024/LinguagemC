#include <stdio.h>  // Inclui a biblioteca padrão de entrada e saída (printf, scanf).
#include <stdlib.h> // Inclui a biblioteca padrão (malloc, free, etc.).
#include <time.h>   // Inclui a biblioteca para geração de números aleatórios (srand, time).
#include <locale.h> 
#define MAX 5       // Capacidade máxima da fila (tamanho fixo).


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

// -------------------- Structs --------------------
// Define a estrutura de uma peça do Tetris
typedef struct
{
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L', etc.
    int id;    // Identificador único da peça.
} Peca;
// Define a estrutura da fila circular de peças
typedef struct
{
    Peca itens[MAX]; // Array que armazena as peças (capacidade MAX).
    int inicio;      // Índice da frente da fila (onde a próxima remoção ocorre).
    int fim;         // Índice do final da fila (onde a próxima inserção ocorre).
    int total;       // Quantidade de elementos atualmente na fila.
} Fila;
// -------------------- Funções da fila --------------------
// Inicializa a fila, zerando índices e contagem.
void inicializarFila(Fila *f)
{
    f->inicio = 0; // O início aponta para o índice 0.
    f->fim = 0;    // O fim aponta para o índice 0.
    f->total = 0;  // A fila começa vazia.
}
// Verifica se a fila está cheia.
int filaCheia(Fila *f)
{
    return f->total == MAX; // Retorna verdadeiro se o total for igual à capacidade máxima.
}
// Verifica se a fila está vazia.
int filaVazia(Fila *f)
{
    return f->total == 0; // Retorna verdadeiro se o total for zero.
}
// Insere peça no final da fila (enqueue).
// Retorna 1 se inseriu, 0 se não foi possível (fila cheia).
int inserir(Fila *f, Peca p)
{
    if (filaCheia(f))
    {
        printf(u8"A fila está cheia! Não é possível inserir nova peça.\n");
        return 0; // Inserção falhou.
    }
    f->itens[f->fim] = p; // Armazena a peça no índice do fim.
    // Atualiza o índice do fim de forma circular:
    // (fim + 1) % MAX garante que o índice volte para 0 após atingir MAX-1.
    f->fim = (f->fim + 1) % MAX;
    f->total++; // Incrementa a quantidade de elementos.
    return 1;   // Inserção bem-sucedida.
}
// Remove peça da frente da fila (dequeue).
// Retorna a peça removida.
Peca remover(Fila *f)
{
    Peca vazia = {'-', -1}; // Peça "vazia" de retorno em caso de erro.
    if (filaVazia(f))
    {
        printf("A fila está vazia! Nenhuma peça para jogar.\n");
        return vazia;
    }
    Peca p = f->itens[f->inicio]; // Armazena a peça que está na frente (início).
    // Atualiza o índice de início de forma circular.
    f->inicio = (f->inicio + 1) % MAX;
    f->total--; // Decrementa a quantidade de elementos.
    return p;   // Retorna a peça removida.
}
// Exibe todas as peças atualmente na fila, começando pelo início.
void mostrarFila(Fila *f)
{
    if (filaVazia(f))
    {
        printf(u8"Fila vazia.\n");
        return;
    }
    printf("Fila de peças: ");
    // Loop: percorre 'total' elementos a partir de 'inicio', usando aritmética modular.
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX)
    {
        printf(u8"[%c %d] ", f->itens[idx].nome, f->itens[idx].id); // Imprime o tipo e o ID.
    }
    printf("\n");
}
// -------------------- Função de geração de peças --------------------
// Gera uma peça aleatória do Tetris, evitando repetição consecutiva (melhoria de UX).
Peca gerarPeca(int id)
{
    char tipos[] = {'I', 'O', 'T', 'L'}; // Tipos possíveis de peças.
    static char ultimo = '\0';           // Variável estática para guardar o último tipo sorteado (mantém o valor entre chamadas).
    int sorteio;
    char novoTipo;
    // Garante que a peça não seja igual à última (previne repetição).
    do
    {
        sorteio = rand() % 4;      // Sorteia índice de 0 a 3.
        novoTipo = tipos[sorteio]; // Seleciona o tipo correspondente.
    } while (novoTipo == ultimo); // Repete se o novo tipo for igual ao anterior.
    ultimo = novoTipo; // Atualiza o último tipo sorteado para a próxima chamada.
    Peca p;
    p.nome = novoTipo; // Define o tipo da peça.
    p.id = id;         // Define o identificador único.
    return p;
}
// -------------------- Função principal --------------------
int main()
{
    set_utf8_console();//chamando a funcao que resolve o problema de acentuação
    Fila fila;
    inicializarFila(&fila); // Inicializa a fila.
    srand(time(NULL));      // Inicializa a semente do gerador de números aleatórios com o tempo atual.
    int contador = 0;       // Contador para gerar IDs únicos para cada peça.
    // Preenche a fila inicial com 5 peças.
    for (int i = 0; i < MAX; i++)
    {
        inserir(&fila, gerarPeca(contador++)); // Insere peças e incrementa o contador de ID.
    }
    int opcao;
    do
    {
        printf("\n");
        mostrarFila(&fila); // Mostra o estado atual da fila.
        // Menu de opções para o usuário
        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");        // Opção para remover da frente.
        printf("2 - Inserir nova peça (enqueue)\n"); // Opção para inserir no final.
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        if (opcao == 1)
        {
            // Remove peça da frente da fila
            Peca jogada = remover(&fila);
            if (jogada.id != -1)
            { // Verifica se a remoção foi bem-sucedida.
                printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
            }
        }
        else if (opcao == 2)
        {
            // Tenta inserir nova peça no final da fila
            if (inserir(&fila, gerarPeca(contador++)))
            { // Gera uma nova peça com novo ID.
                printf("Nova peça adicionada à fila!\n");
            }
        }
        else if (opcao != 0)
        {
            printf("Opção inválida! Digite 0, 1 ou 2.\n");
        }
    } while (opcao != 0); // Continua até o usuário escolher sair (0).
    printf("Encerrando o jogo Tetris Stack!\n");
    return 0; // Fim do programa.
}