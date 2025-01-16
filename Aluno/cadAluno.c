#include <stdio.h>

int main(void) {
    int idade, matricula;
    float altura;
    char nome[50];  // Corrigido para ser um vetor de char

    printf("Idade: ");
    scanf("%d", &idade);
    printf("Matricula: ");
    scanf("%d", &matricula);
    printf("Altura: ");
    scanf("%f", &altura);
    printf("Nome: ");
    scanf("%s", nome);  // Corrigido para passar diretamente o nome

    printf("\nNOME: %s\nIDADE: %d\nMATRICULA: %d\nALTURA: %.2f\n", nome, idade, matricula, altura);
    return 0;
}
