#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define FILA_CAPACIDADE 5
#define PILHA_CAPACIDADE 3

typedef struct {
    char nome; // Tipo da peça: I, O, T, L
    int id;    // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[FILA_CAPACIDADE];
    int inicio;
    int fim;
    int tamanho;
} FilaCircular;

// Estrutura da pilha
typedef struct {
    Peca itens[PILHA_CAPACIDADE];
    int topo;
} Pilha;

// Variável global para gerar IDs únicos
int proximoID = 0;

// ---------- Cores ANSI ----------
#define GREEN "\x1b[32m"
#define RED   "\x1b[31m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"

// ---------- Protótipos ----------
Peca gerarPeca();
void inicializarFila(FilaCircular *fila);
void inicializarPilha(Pilha *pilha);
void exibirEstado(FilaCircular *fila, Pilha *pilha);
void jogarPeca(FilaCircular *fila);
void reservarPeca(FilaCircular *fila, Pilha *pilha);
void usarPecaReservada(Pilha *pilha);
void trocarSimples(FilaCircular *fila, Pilha *pilha);
void trocarMultipla(FilaCircular *fila, Pilha *pilha);
int lerOpcao();

// ---------- Função Principal ----------
int main() {
    FilaCircular fila;
    Pilha pilha;
    int opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        printf("\n================ ESTADO ATUAL ================\n");
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças do topo da pilha\n");
        printf("0 - Sair\n");

        opcao = lerOpcao();

        switch(opcao) {
            case 1: jogarPeca(&fila); break;
            case 2: reservarPeca(&fila, &pilha); break;
            case 3: usarPecaReservada(&pilha); break;
            case 4: trocarSimples(&fila, &pilha); break;
            case 5: trocarMultipla(&fila, &pilha); break;
            case 0: printf(GREEN "[✓] Encerrando o programa.\n" RESET); break;
            default: printf(RED "[✗] Opção inválida!\n" RESET);
        }

    } while(opcao != 0);

    return 0;
}

// ---------- Funções ----------

// Gera uma peça aleatória com ID único
Peca gerarPeca() {
    Peca p;
    char tipos[4] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = proximoID++;
    return p;
}

// Inicializa a fila com 5 peças
void inicializarFila(FilaCircular *fila) {
    fila->inicio = 0;
    fila->fim = FILA_CAPACIDADE - 1;
    fila->tamanho = FILA_CAPACIDADE;
    for(int i = 0; i < FILA_CAPACIDADE; i++) {
        fila->itens[i] = gerarPeca();
    }
}

// Inicializa pilha vazia
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Exibe fila e pilha
void exibirEstado(FilaCircular *fila, Pilha *pilha) {
    printf("Fila de peças: ");
    if(fila->tamanho == 0) printf("[vazia]");
    else {
        for(int i = 0; i < fila->tamanho; i++) {
            int idx = (fila->inicio + i) % FILA_CAPACIDADE;
            printf("[%c %d] ", fila->itens[idx].nome, fila->itens[idx].id);
        }
    }
    printf("\nPilha de reserva (Topo -> Base): ");
    if(pilha->topo == -1) printf("[vazia]");
    else {
        for(int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
        }
    }
    printf("\n==============================================\n");
}

// Lê opção do usuário
int lerOpcao() {
    int opcao;
    char buffer[10];
    printf("Opção escolhida: ");
    fgets(buffer, sizeof(buffer), stdin);
    if(sscanf(buffer, "%d", &opcao) != 1) return -1;
    return opcao;
}

// Remove peça da frente da fila
void jogarPeca(FilaCircular *fila) {
    if(fila->tamanho == 0) {
        printf(RED "[✗] Fila vazia! Nenhuma peça para jogar.\n" RESET);
        return;
    }
    printf(GREEN "[✓] Peça jogada: [%c %d]\n" RESET, fila->itens[fila->inicio].nome, fila->itens[fila->inicio].id);
    fila->inicio = (fila->inicio + 1) % FILA_CAPACIDADE;
    fila->itens[(fila->inicio + fila->tamanho - 1) % FILA_CAPACIDADE] = gerarPeca();
}

// Move peça da frente da fila para a pilha
void reservarPeca(FilaCircular *fila, Pilha *pilha) {
    if(fila->tamanho == 0) {
        printf(RED "[✗] Fila vazia! Nenhuma peça para reservar.\n" RESET);
        return;
    }
    if(pilha->topo >= PILHA_CAPACIDADE - 1) {
        printf(YELLOW "[!!!] Pilha cheia! Não é possível reservar mais peças.\n" RESET);
        return;
    }
    pilha->topo++;
    pilha->itens[pilha->topo] = fila->itens[fila->inicio];
    printf(GREEN "[✓] Peça [%c %d] reservada na pilha.\n" RESET, fila->itens[fila->inicio].nome, fila->itens[fila->inicio].id);
    fila->inicio = (fila->inicio + 1) % FILA_CAPACIDADE;
    fila->itens[(fila->inicio + fila->tamanho - 1) % FILA_CAPACIDADE] = gerarPeca();
}

// Remove peça do topo da pilha
void usarPecaReservada(Pilha *pilha) {
    if(pilha->topo == -1) {
        printf(RED "[✗] Pilha vazia! Nenhuma peça para usar.\n" RESET);
        return;
    }
    printf(GREEN "[✓] Peça usada da pilha: [%c %d]\n" RESET, pilha->itens[pilha->topo].nome, pilha->itens[pilha->topo].id);
    pilha->topo--;
}

// Troca peça da frente da fila com o topo da pilha
void trocarSimples(FilaCircular *fila, Pilha *pilha) {
    if(fila->tamanho == 0 || pilha->topo == -1) {
        printf(RED "[✗] Não há peças suficientes para troca simples.\n" RESET);
        return;
    }
    Peca temp = fila->itens[fila->inicio];
    fila->itens[fila->inicio] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;
    printf(GREEN "[✓] Troca simples realizada.\n" RESET);
}

// Troca as 3 primeiras peças da fila com as 3 do topo da pilha
void trocarMultipla(FilaCircular *fila, Pilha *pilha) {
    if(fila->tamanho < 3 || pilha->topo < 2) {
        printf(RED "[✗] Não há peças suficientes para troca múltipla.\n" RESET);
        return;
    }
    for(int i = 0; i < 3; i++) {
        int idxFila = (fila->inicio + i) % FILA_CAPACIDADE;
        Peca temp = fila->itens[idxFila];
        fila->itens[idxFila] = pilha->itens[pilha->topo - i];
        pilha->itens[pilha->topo - i] = temp;
    }
    printf(GREEN "[✓] Troca múltipla realizada.\n" RESET);
}