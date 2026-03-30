#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>

#define TAMANHO_PILHA 65536

typedef struct {
    double saldo;
} Registro;

Registro registro;

Registro le_registro(int conta) {
    return registro;
}

void grava_registro(Registro reg, int conta) {
    registro = reg;
}

void atualiza_saldo(double valor, int conta) {
    Registro reg_local;
    printf("Iniciando operação [%.2f]\n", valor);
    
    reg_local = le_registro(conta);
    usleep(1000); // Provoca a condição de corrida [cite: 340]
    reg_local.saldo = reg_local.saldo + valor;
    grava_registro(reg_local, conta);
    
    printf("Terminada operação [%.2f]\n", valor);
}

int funcaoDeposito(void *arg) {
    atualiza_saldo(100, 0); // Depósito de 100 [cite: 434]
    return 0;
}

int funcaoSaque(void *arg) {
    atualiza_saldo(-200, 0); // Saque de 200 [cite: 436]
    return 0;
}

int main() {
    void *pilha1, *pilha2;
    int pid1, pid2;

    registro.saldo = 500.0; // Inicializa saldo [cite: 468]
    printf("Saldo antes das operações = %.2f\n", registro.saldo);

    pilha1 = malloc(TAMANHO_PILHA);
    pilha2 = malloc(TAMANHO_PILHA);

    // Inicia as threads concorrentes
    pid1 = clone(funcaoDeposito, pilha1 + TAMANHO_PILHA, CLONE_VM | SIGCHLD, NULL); [cite: 462, 471]
    pid2 = clone(funcaoSaque, pilha2 + TAMANHO_PILHA, CLONE_VM | SIGCHLD, NULL); [cite: 485, 489]

    waitpid(pid1, 0, 0);
    waitpid(pid2, 0, 0);

    printf("Saldo depois das operações = %.2f\n", registro.saldo); [cite: 505]
    return 0;
}