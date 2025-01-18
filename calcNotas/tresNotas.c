#include<stdio.h>

int main(){
    float nota1,nota2,nota3, media;

    printf("Nota 1: ");
    scanf("%f", &nota1);

    printf("Nota 2: ");
    scanf("%f", &nota2);
    
    printf("Nota 3: ");
    scanf("%f", &nota3);

    media = (nota1 + nota2 + nota3) / 3;
    printf("\nMedia: %.2f", media);
}