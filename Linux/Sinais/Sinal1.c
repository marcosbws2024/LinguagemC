#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

int main() {
    pid_t pid;
    int sinal;

    // Solicita o PID do processo destino
    printf("Digite o PID do processo: ");
    scanf("%d", &pid);

    // Solicita o número do sinal a ser enviado
    printf("Digite o sinal: ");
    scanf("%d", &sinal);

    // Envia o sinal para o processo destino
    if (kill(pid, sinal) == -1) {
        perror("Erro ao enviar sinal");
        exit(1);
    }

    printf("Sinal %d enviado ao processo %d com sucesso.\n", sinal, pid);
    return 0;
}