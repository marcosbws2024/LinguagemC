#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5   // Capacidade máxima da fila

// -------------------- Structs --------------------

// Define a estrutura de uma peça do Tetris
typedef struct {
    char nome;   // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;      // Identificador único da peça
} Peca;

// Define a estrutura da fila circular de peças
typedef struct {
    Peca itens[MAX]; // Array que armazena as peças
    int inicio;      // Índice da frente da fila
    int fim;         // Índice do final da fila
    int total;       // Quantidade de elementos atualmente na fila
} Fila;

// -------------------- Funções da fila --------------------

// Inicializa a fila, zerando índices e contagem
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->total == MAX;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->total == 0;
}

// Insere peça no final da fila (enqueue)
// Retorna 1 se inseriu, 0 se não foi possível (fila cheia)
int inserir(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("A fila está cheia! Não é possível inserir nova peça.\n");
        return 0; // Inserção falhou
    }
    f->itens[f->fim] = p;               // Armazena a peça no índice do fim
    f->fim = (f->fim + 1) % MAX;        // Atualiza o índice circularmente
    f->total++;                          // Incrementa a quantidade de elementos
    return 1;                            // Inserção bem-sucedida
}

// Remove peça da frente da fila (dequeue)
// Retorna a peça removida, ou uma peça "vazia" se a fila estiver vazia
Peca remover(Fila *f) {
    Peca vazia = {'-', -1}; // Retorno padrão se a fila estiver vazia
    if (filaVazia(f)) {
        printf("A fila está vazia! Nenhuma peça para jogar.\n");
        return vazia;
    }
    Peca p = f->itens[f->inicio];       // Armazena a peça da frente
    f->inicio = (f->inicio + 1) % MAX;  // Atualiza o índice de início circularmente
    f->total--;                          // Decrementa a quantidade de elementos
    return p;                            // Retorna a peça removida
}

// Exibe todas as peças atualmente na fila
void mostrarFila(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia.\n");
        return;
    }

    printf("Fila de peças: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}

// -------------------- Função de geração de peças --------------------

// Gera uma peça aleatória do Tetris, evitando repetição consecutiva
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};   // Tipos possíveis de peças
    static char ultimo = '\0';             // Guarda o último tipo sorteado
    int sorteio;
    char novoTipo;

    // Garante que a peça não seja igual à última
    do {
        sorteio = rand() % 4;              // Sorteia índice de 0 a 3
        novoTipo = tipos[sorteio];         // Seleciona o tipo
    } while (novoTipo == ultimo);          // Repete se for igual ao último

    ultimo = novoTipo;                      // Atualiza o último tipo sorteado

    Peca p;
    p.nome = novoTipo;                      // Define o tipo da peça
    p.id = id;                              // Define o identificador único
    return p;
}

// -------------------- Função principal --------------------
int main() {
    Fila fila;
    inicializarFila(&fila);                 // Inicializa a fila

    srand(time(NULL));                      // Inicializa semente para números aleatórios

    int contador = 0;                       // Contador para IDs das peças

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < MAX; i++) {
        inserir(&fila, gerarPeca(contador++));
    }

    int opcao;
    do {
        printf("\n");
        mostrarFila(&fila);                 // Mostra a fila antes de qualquer ação

        // Menu de opções para o usuário
        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            // Remove peça da frente da fila
            Peca jogada = remover(&fila);
            if (jogada.id != -1) {
                printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
            }
        } else if (opcao == 2) {
            // Tenta inserir nova peça no final da fila
            if (inserir(&fila, gerarPeca(contador++))) {
                printf("Nova peça adicionada à fila!\n");
            }
        } else if (opcao != 0) {
            printf("Opção inválida! Digite 0, 1 ou 2.\n");
        }

    } while (opcao != 0);                   // Continua até o usuário escolher sair

    printf("Encerrando o jogo Tetris Stack!\n");
    return 0;
}