#include <stdio.h>  // Inclui a biblioteca padrão de entrada/saída (printf, scanf).
#include <stdlib.h> // Inclui a biblioteca padrão (rand, srand).
#include <time.h>   // Inclui para gerar semente aleatória baseada no tempo.
#include <locale.h>

#define MAX 5       // Capacidade máxima da fila (tamanho fixo).
#define MAX_PILHA 3 // Capacidade máxima da pilha de reserva (tamanho fixo).


// Adiciona a API do Windows para mudar a página de código, nessa pré-compilação que vê se o windows 32bits
// E decide se é essa biblioteca windows.h, pois o terminal muitas vezes ainda é do windows 32 para o mingw
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
// Define a estrutura de uma peça (o dado).
typedef struct
{
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L', etc.
    int id;    // Identificador único da peça.
} Peca;
// Define a estrutura da fila circular (FIFO).
typedef struct
{
    Peca itens[MAX]; // Array de armazenamento.
    int inicio;      // Índice da frente da fila (próxima a sair).
    int fim;         // Índice do final da fila (próxima a entrar).
    int total;       // Quantidade atual de elementos na fila.
} Fila;
// Define a estrutura da pilha (LIFO).
typedef struct
{
    Peca itens[MAX_PILHA]; // Array de armazenamento.
    int topo;              // Índice do topo da pilha (-1 = vazia).
} Pilha;
// -------------------- Funções da fila --------------------
// Inicializa a fila, zerando índices e contagem.
void inicializarFila(Fila *f)
{
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}
// Verifica se a fila está cheia.
int filaCheia(Fila *f)
{
    return f->total == MAX;
}
// Verifica se a fila está vazia.
int filaVazia(Fila *f)
{
    return f->total == 0;
}
// Insere peça no final da fila (enqueue).
int inserir(Fila *f, Peca p)
{
    if (filaCheia(f))
        return 0;                // Se cheia, não insere e retorna 0.
    f->itens[f->fim] = p;        // Armazena a peça.
    f->fim = (f->fim + 1) % MAX; // Atualiza o índice do fim circularmente.
    f->total++;                  // Incrementa o total.
    return 1;                    // Sucesso.
}
// Remove peça da frente da fila (dequeue).
Peca remover(Fila *f)
{
    Peca vazia = {'-', -1}; // Peça "vazia" para retorno em caso de erro.
    if (filaVazia(f))
        return vazia;                  // Se vazia, retorna a peça vazia.
    Peca p = f->itens[f->inicio];      // Pega a peça do início.
    f->inicio = (f->inicio + 1) % MAX; // Atualiza o índice do início circularmente.
    f->total--;                        // Decrementa o total.
    return p;                          // Retorna a peça removida.
}
// Exibe todas as peças atualmente na fila, do início ao fim.
void mostrarFila(Fila *f)
{
    if (filaVazia(f))
    {
        printf("Fila vazia.\n");
        return;
    }
    printf("Fila de peças: ");
    // Percorre os 'total' elementos a partir de 'inicio' de forma circular.
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX)
    {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}
// -------------------- Funções da pilha --------------------
// Inicializa a pilha vazia.
void inicializarPilha(Pilha *p)
{
    p->topo = -1; // -1 indica que a pilha está vazia.
}
// Verifica se a pilha está cheia.
int pilhaCheia(Pilha *p)
{
    return p->topo == MAX_PILHA - 1; // O topo alcançou o último índice válido.
}
// Verifica se a pilha está vazia.
int pilhaVazia(Pilha *p)
{
    return p->topo == -1;
}
// Insere peça no topo da pilha (push).
int push(Pilha *p, Peca pe)
{
    if (pilhaCheia(p))
        return 0;           // Se cheia, retorna 0.
    p->topo++;              // Avança o topo.
    p->itens[p->topo] = pe; // Insere a peça no novo topo.
    return 1;               // Sucesso.
}
// Remove peça do topo da pilha (pop).
Peca pop(Pilha *p)
{
    Peca vazia = {'-', -1}; // Peça "vazia" para retorno em caso de erro.
    if (pilhaVazia(p))
        return vazia;            // Se vazia, retorna a peça vazia.
    Peca pe = p->itens[p->topo]; // Pega a peça do topo.
    p->topo--;                   // Recua o topo (removendo logicamente a peça).
    return pe;                   // Retorna a peça removida.
}
// Exibe todas as peças na pilha, do topo para a base.
void mostrarPilha(Pilha *p)
{
    if (pilhaVazia(p))
    {
        printf("Pilha de reserva vazia.\n");
        return;
    }
    printf("Pilha de reserva (Topo -> Base): ");
    // Percorre a pilha do índice 'topo' até a base (índice 0).
    for (int i = p->topo; i >= 0; i--)
    {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}
// -------------------- Função de geração de peças --------------------
// Gera uma peça aleatória com ID único, evitando repetição consecutiva (usando 'static').
Peca gerarPeca(int id)
{
    char tipos[] = {'I', 'O', 'T', 'L'}; // Tipos possíveis de peças.
    static char ultimo = '\0';           // Variável estática (mantém o valor entre chamadas).
    char novoTipo;
    // Garante que o tipo sorteado seja diferente do último.
    do
    {
        novoTipo = tipos[rand() % 4]; // Sorteia um novo tipo.
    } while (novoTipo == ultimo);
    ultimo = novoTipo; // Atualiza o último tipo.
    Peca p;
    p.nome = novoTipo;
    p.id = id;
    return p;
}
// -------------------- Função principal --------------------
int main()
{
    set_utf8_console();
    Fila fila;                // Declara a fila de peças.
    Pilha pilha;              // Declara a pilha de reserva.
    int contador = 0;         // Contador de ID único para as peças.
    int opcao;                // Variável para a escolha do menu.
    srand(time(NULL));        // Inicializa o gerador de números aleatórios.
    inicializarFila(&fila);   // Inicializa a fila.
    inicializarPilha(&pilha); // Inicializa a pilha.
    // Preenche a fila inicial com MAX peças.
    for (int i = 0; i < MAX; i++)
    {
        inserir(&fila, gerarPeca(contador++)); // Insere peça e incrementa o contador.
    }
    do
    {
        printf("\n----- Estado atual -----\n");
        mostrarFila(&fila);   // Exibe o conteúdo da fila.
        mostrarPilha(&pilha); // Exibe o conteúdo da pilha.
        printf("------------------------\n");
        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao); // Lê a opção do usuário.
        if (opcao == 1)
        {
            if (filaVazia(&fila))
            {
                printf("Não há mais peças para jogar!\n");
            }
            else
            {
                Peca jogada = remover(&fila); // Remove peça da frente (dequeue).
                printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                inserir(&fila, gerarPeca(contador++)); // Insere nova peça no final (simula o reabastecimento).
            }
        }
        else if (opcao == 2)
        {
            if (pilhaCheia(&pilha))
            {
                printf("Não é possível reservar! Pilha cheia.\n");
            }
            else if (filaVazia(&fila))
            {
                printf("Não há peças na fila para reservar!\n");
            }
            else
            {
                Peca reservada = remover(&fila); // Remove da fila (dequeue).
                push(&pilha, reservada);         // Insere na pilha (push).
                printf("Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                inserir(&fila, gerarPeca(contador++)); // Reabastece a fila.
            }
        }
        else if (opcao == 3)
        {
            if (pilhaVazia(&pilha))
            {
                printf("Não há peças reservadas para usar!\n");
            }
            else
            {
                Peca usada = pop(&pilha); // Remove do topo da pilha (pop).
                printf("Peça da reserva usada: [%c %d]\n", usada.nome, usada.id);
                // Nota: Peças usadas da reserva NÃO reabastecem a fila.
            }
        }
        else if (opcao != 0)
        {
            printf("Opção inválida! Digite 0, 1, 2 ou 3.\n");
        }
    } while (opcao != 0); // O loop continua até que a opção 0 seja escolhida.
    printf("Encerrando o jogo Tetris Stack!\n");
    return 0; // Fim do programa.
}