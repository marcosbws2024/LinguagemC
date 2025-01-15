#include<stdio.h>

int main (void){
    int idade  = 20;
    float altura =1.75;
    double salario = 1.800;
    char letra = 'b';
    char nome[20] = "Marcos";
    
    printf("idade: %d\n", idade);
    printf("altura: %e\n", altura);
    printf("salario: %.2f\n", salario);
    printf("letra: %c\n", letra);
    printf("nome: %s\n", nome);

   return 0;

}