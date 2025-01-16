#include <stdio.h>

int main()
{
   int num1,num2;
   int soma, sub, mult, divisao;
   
   printf("Entre com o valor 1: ");
   scanf("%d",&num1);
   printf("Entre com o valor 2: ");
   scanf("%d",&num2);
   
   soma = num1 + num2;
   sub  = num1 - num2;
   mult = num1 * num2;
   divisao = num1 / num2;
   
   printf("\n\nResultado da Soma: %d\n", soma);
   printf("Resultado da Subtração: %d\n", sub);
   printf("Resultado da Multiplicação: %d\n", mult);
   printf("Resultado da Divisão: %d\n", divisao);
      
   return 0;
}