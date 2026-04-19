#include <stdio.h>
#include <unistd.h>

int main() {
    int i;

    // O loop executa 3 vezes
    for(i = 0; i < 3; i++) {
        // A função fork() cria uma cópia do processo atual
        fork();
    }

    // Mantém todos os processos criados ativos para observação
    while(1);

    return 0;
}