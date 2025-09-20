#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

// --- Definicoes e Constantes ---
#define TAMANHO_ARQUIVO 1000000 // 1 milhao de inteiros (aprox. 4MB)
#define TAMANHO_RUN 500000      // 500 mil inteiros (aprox. 2MB)

// --- Funcoes Auxiliares ---

// Funcao para criar um arquivo grande com numeros aleatorios
void criar_arquivo_aleatorio(const char* nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "wb");
    if (!arq) {
        perror("Erro ao criar arquivo aleatorio");
        exit(1);
    }

    srand((unsigned int)time(NULL));
    int numero;
    for (int i = 0; i < TAMANHO_ARQUIVO; i++) {
        numero = rand() % 1000000;
        fwrite(&numero, sizeof(int), 1, arq);
    }
    fclose(arq);
    printf("Arquivo de entrada '%s' criado com sucesso. (Tamanho: %d inteiros)\n", nome_arquivo, TAMANHO_ARQUIVO);
}

// Funcao para ordenar um array na memoria
int comparar(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// --- FASE 1: Criacao dos Runs Ordenados ---
void criar_runs(const char* entrada, const char* run1, const char* run2) {
    FILE *arq_entrada = fopen(entrada, "rb");
    FILE *arq_run1 = fopen(run1, "wb");
    FILE *arq_run2 = fopen(run2, "wb");
    if (!arq_entrada || !arq_run1 || !arq_run2) {
        perror("Erro ao abrir arquivos para criacao de runs");
        exit(1);
    }

    int *buffer = (int*)malloc(TAMANHO_RUN * sizeof(int));
    if (!buffer) {
        printf("Erro: falha na alocacao de memoria para o buffer.\n");
        exit(1);
    }

    // Leitura e ordenacao do primeiro run
    fread(buffer, sizeof(int), TAMANHO_RUN, arq_entrada);
    qsort(buffer, TAMANHO_RUN, sizeof(int), comparar);
    fwrite(buffer, sizeof(int), TAMANHO_RUN, arq_run1);

    // Leitura e ordenacao do segundo run
    fread(buffer, sizeof(int), TAMANHO_RUN, arq_entrada);
    qsort(buffer, TAMANHO_RUN, sizeof(int), comparar);
    fwrite(buffer, sizeof(int), TAMANHO_RUN, arq_run2);

    free(buffer);
    fclose(arq_entrada);
    fclose(arq_run1);
    fclose(arq_run2);
    printf("Runs temporarios '%s' e '%s' criados e ordenados.\n", run1, run2);
}

// --- FASE 2: Mesclagem dos Runs ---
void mesclar_runs(const char* run1, const char* run2, const char* saida) {
    FILE *arq_run1 = fopen(run1, "rb");
    FILE *arq_run2 = fopen(run2, "rb");
    FILE *arq_saida = fopen(saida, "wb");
    if (!arq_run1 || !arq_run2 || !arq_saida) {
        perror("Erro ao abrir arquivos para mesclagem");
        exit(1);
    }

    int num1, num2;
    int lido1 = fread(&num1, sizeof(int), 1, arq_run1);
    int lido2 = fread(&num2, sizeof(int), 1, arq_run2);

    while (lido1 > 0 && lido2 > 0) {
        if (num1 <= num2) {
            fwrite(&num1, sizeof(int), 1, arq_saida);
            lido1 = fread(&num1, sizeof(int), 1, arq_run1);
        } else {
            fwrite(&num2, sizeof(int), 1, arq_saida);
            lido2 = fread(&num2, sizeof(int), 1, arq_run2);
        }
    }

    // Copia os elementos restantes, se houver
    while (lido1 > 0) {
        fwrite(&num1, sizeof(int), 1, arq_saida);
        lido1 = fread(&num1, sizeof(int), 1, arq_run1);
    }

    while (lido2 > 0) {
        fwrite(&num2, sizeof(int), 1, arq_saida);
        lido2 = fread(&num2, sizeof(int), 1, arq_run2);
    }

    fclose(arq_run1);
    fclose(arq_run2);
    fclose(arq_saida);
    printf("Arquivos '%s' e '%s' mesclados com sucesso no arquivo de saida '%s'.\n", run1, run2, saida);
}

int main() {
    const char* arq_entrada = "dados_desordenados.bin";
    const char* arq_saida = "dados_ordenados.bin";
    const char* run1 = "run1.bin";
    const char* run2 = "run2.bin";

    // 1. Cria um arquivo grande para simular o "ordenamento externo"
    criar_arquivo_aleatorio(arq_entrada);

    // 2. Cria os "runs" ordenados
    printf("\nIniciando a criacao dos runs...\n");
    criar_runs(arq_entrada, run1, run2);

    // 3. Mescla os runs em um unico arquivo ordenado
    printf("\nIniciando a mesclagem dos runs...\n");
    mesclar_runs(run1, run2, arq_saida);
    
    printf("\nProcesso de ordenacao externa finalizado com sucesso.\n");
    
    // (Opcional) Remova os arquivos temporarios
    remove(run1);
    remove(run2);
    
    return 0;
}