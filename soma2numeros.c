
//https://www.youtube.com/watch?v=aXVzbzwZuYc&ab_channel=C%C3%B3digoF%C3%A1cil
/*Seguindo a aula da Faculdade https://www.msys2.org/ 
Usado com interface de instação do MInGW*/
//#include <stdio.h>
 
int main() {
    int num1, num2, soma;
 
    // Lendo dois números do usuário
    printf("Digite o primeiro número: ");
    scanf("%d", &num1);
 
    printf("Digite o segundo número: ");
    scanf("%d", &num2);
 
    // Calculando a soma dos dois números
    soma = num1 + num2;
 
    // Exibindo o resultado
    printf("A soma de %d e %d é: %d\n", num1, num2, soma);
 
    return 0;
}