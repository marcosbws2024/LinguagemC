#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <semaphore.h> // Biblioteca de semáforos [cite: 534]

#define TAMANHO_PILHA 65536

sem_t mutex; // Criação do semáforo [cite: 538]

// Funções auxiliares para manipulação do semáforo
void up(sem_t *sem) {
    sem_wait(sem); // Operação Down/Wait [cite: 540]
}

void down(sem_t *sem) {
    sem_post(sem); // Operação Up/Post [cite: 547]
}

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
    
    up(&mutex); // Entra na região crítica [cite: 570]
    
    printf("Iniciando operação [%.2f]\n", valor);
    reg_local = le_registro(conta);
    usleep(1000);
    reg_local.saldo = reg_local.saldo + valor;
    grava_registro(reg_local, conta);
    printf("Terminada operação [%.2f]\n", valor);
    
    down(&mutex); // Sai da região crítica [cite: 586]
}

int funcaoDeposito(void *arg) {
    atualiza_saldo(100, 0);
    return 0;
}

int funcaoSaque(void *arg) {
    atualiza_saldo(-200, 0);
    return 0;
}

int main() {
    void *pilha1, *pilha2;
    int pid1, pid2;

    // Inicializa mutex com valor 1 [cite: 608]
    sem_init(&mutex, 0, 1); 

    registro.saldo = 500.0;
    printf("Saldo antes das operações %.2f\n", registro.saldo);

    pilha1 = malloc(TAMANHO_PILHA);
    pilha2 = malloc(TAMANHO_PILHA);

    pid1 = clone(funcaoDeposito, pilha1 + TAMANHO_PILHA, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD, NULL); [cite: 625, 626]
    pid2 = clone(funcaoSaque, pilha2 + TAMANHO_PILHA, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD, NULL); [cite: 650, 651]

    waitpid(pid1, 0, 0);
    waitpid(pid2, 0, 0);

    printf("Saldo depois das operações = %.2f\n", registro.saldo); [cite: 667]
    
    sem_destroy(&mutex);
    return 0;
}