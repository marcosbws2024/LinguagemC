#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5       // Capacidade máxima da fila
#define MAX_PILHA 3 // Capacidade máxima da pilha de reserva

// -------------------- Structs --------------------
typedef struct {
    char nome;   // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;      // Identificador único da peça
} Peca;

typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// -------------------- Funções da fila --------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

int inserir(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->total++;
    return 1;
}

Peca remover(Fila *f) {
    Peca vazia = {'-', -1};
    if (filaVazia(f)) return vazia;
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
    return p;
}

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

// -------------------- Funções da pilha --------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int push(Pilha *p, Peca pe) {
    if (pilhaCheia(p)) return 0;
    p->topo++;
    p->itens[p->topo] = pe;
    return 1;
}

Peca pop(Pilha *p) {
    Peca vazia = {'-', -1};
    if (pilhaVazia(p)) return vazia;
    Peca pe = p->itens[p->topo];
    p->topo--;
    return pe;
}

void mostrarPilha(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Pilha de reserva vazia.\n");
        return;
    }
    printf("Pilha de reserva (Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

// -------------------- Função de geração de peças --------------------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    static char ultimo = '\0';
    char novoTipo;
    do {
        novoTipo = tipos[rand() % 4];
    } while (novoTipo == ultimo);
    ultimo = novoTipo;
    Peca p;
    p.nome = novoTipo;
    p.id = id;
    return p;
}

// -------------------- Função principal --------------------
int main() {
    Fila fila;
    Pilha pilha;
    int contador = 0;
    int opcao;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial
    for (int i = 0; i < MAX; i++) {
        inserir(&fila, gerarPeca(contador++));
    }

    do {
        printf("\n----- Estado atual -----\n");
        mostrarFila(&fila);
        mostrarPilha(&pilha);
        printf("------------------------\n");

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            if (filaVazia(&fila)) {
                printf("Não há mais peças para jogar!\n");
            } else {
                Peca jogada = remover(&fila);
                printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                inserir(&fila, gerarPeca(contador++));
            }
        } else if (opcao == 2) {
            if (pilhaCheia(&pilha)) {
                printf("Não é possível reservar! Pilha cheia.\n");
            } else if (filaVazia(&fila)) {
                printf("Não há peças na fila para reservar!\n");
            } else {
                Peca reservada = remover(&fila);
                push(&pilha, reservada);
                printf("Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                inserir(&fila, gerarPeca(contador++));
            }
        } else if (opcao == 3) {
            if (pilhaVazia(&pilha)) {
                printf("Não há peças reservadas para usar!\n");
            } else {
                Peca usada = pop(&pilha);
                printf("Peça da reserva usada: [%c %d]\n", usada.nome, usada.id);
            }
        } else if (opcao != 0) {
            printf("Opção inválida! Digite 0, 1, 2 ou 3.\n");
        }

    } while (opcao != 0);

    printf("Encerrando o jogo Tetris Stack!\n");
    return 0;
}
