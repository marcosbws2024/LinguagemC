#include <stdio.h>

int main()
{
    /*Incremento(++)
     Pré-incremento ++a
     Pós-incremento a++

     Decremento(--) 
     Pré-decremento --a
     Pós-decremento a--
    */
    int a = 10;
    int resultado =0;

    printf("Antes Incremento:%d\n",a);
    resultado = a++;
    printf("Apos o  Pos-Incremento- num1: %d - Resultado:%d\n",a,resultado);

    resultado = ++a;
    printf("Apos o  Pre-Incremento- num1: %d - Resultado:%d\n",a,resultado);
    

   return 0;
}