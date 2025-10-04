#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa cada sala (nC3 da C!rvore)
typedef struct Sala {
	char nome[50];             // Nome da sala, como "Hall de Entrada" ou "Biblioteca"
	struct Sala* esquerda;     // Ponteiro para a prC3xima sala C  esquerda (subC!rvore esquerda)
	struct Sala* direita;      // Ponteiro para a prC3xima sala C  direita (subC!rvore direita)
} Sala;

/**
 * FunC'C#o: criarSala
 * ------------------
 * Cria dinamicamente uma sala (nC3) com um nome.
 * Aloca a memC3ria necessC!ria, copia o nome para a estrutura e inicializa
 * os ponteiros 'esquerda' e 'direita' como NULL, indicando que a sala
 * ainda nC#o tem caminhos definidos.
 */
Sala* criarSala(const char* nome) {
	Sala* nova = (Sala*) malloc(sizeof(Sala));
	if (nova == NULL) {
		printf("Erro ao alocar memC3ria!\n");
		exit(1);
	}
	strcpy(nova->nome, nome);
	nova->esquerda = NULL;
	nova->direita = NULL;
	return nova;
}

/**
 * FunC'C#o: explorarSalas
 * ---------------------
 * Permite ao jogador navegar interativamente pela mansC#o,
 * escolhendo os caminhos C  esquerda ou direita atC) chegar em um nC3-folha.
 * Usa um loop 'while' que continua enquanto houver salas para explorar.
 */
void explorarSalas(Sala* atual) {
	char escolha;

	while (atual != NULL) {
		printf("\nVocC* estC! em: %s\n", atual->nome);

		// Caso seja um nC3-folha (sem caminhos), a exploraC'C#o termina aqui.
		if (atual->esquerda == NULL && atual->direita == NULL) {
			printf("VocC* chegou ao fim da exploraC'C#o! NC#o hC! mais caminhos.\n");
			break;
		}

		printf("Escolha um caminho: (e) esquerda, (d) direita, (s) sair: ");
		scanf(" %c", &escolha);

		if (escolha == 'e' || escolha == 'E') {
			if (atual->esquerda != NULL) {
				// Se o caminho existe, o ponteiro 'atual' se move para a prC3xima sala C  esquerda.
				atual = atual->esquerda;
			} else {
				printf("NC#o hC! caminho C  esquerda!\n");
			}
		}
		else if (escolha == 'd' || escolha == 'D') {
			if (atual->direita != NULL) {
				// Se o caminho existe, o ponteiro 'atual' se move para a prC3xima sala C  direita.
				atual = atual->direita;
			} else {
				printf("NC#o hC! caminho C  direita!\n");
			}
		}
		else if (escolha == 's' || escolha == 'S') {
			printf("VocC* decidiu sair da exploraC'C#o.\n");
			break;
		}
		else {
			printf("OpC'C#o invC!lida! Tente novamente.\n");
		}
	}
}

/**
 * FunC'C#o: liberar
 * ---------------
 * Libera recursivamente toda a memC3ria da C!rvore, evitando vazamentos de memC3ria.
 * Ela visita primeiro os nC3s da esquerda, depois os da direita e, por fim, o nC3 atual.
 * Este C) um exemplo de percurso pC3s-ordem.
 */
void liberar(Sala* raiz) {
	if (raiz != NULL) {
		liberar(raiz->esquerda);
		liberar(raiz->direita);
		free(raiz); // Libera o nC3 atual apC3s liberar seus "filhos"
	}
}

/**
 * FunC'C#o Principal (main)
 * -----------------------
 * Monta manualmente o mapa da mansC#o (C!rvore binC!ria)
 * e inicia a exploraC'C#o pelo Hall de Entrada.
 * * A estrutura da C!rvore C) a seguinte:
 * * Hall de Entrada (raiz)
 * /                 \
 * Sala de Estar      Biblioteca
 * /      \                \
 * Cozinha  Jardim         Quarto
 */
int main() {
	// ConstruC'C#o manual do mapa da mansC#o
	Sala* hall = criarSala("Hall de Entrada");
	hall->esquerda = criarSala("Sala de Estar");
	hall->direita = criarSala("Biblioteca");

	hall->esquerda->esquerda = criarSala("Cozinha");
	hall->esquerda->direita = criarSala("Jardim");

	hall->direita->direita = criarSala("Quarto");

	// Inicia a exploraC'C#o
	printf("=== Detective Quest: Explorando a MansC#o ===\n");
	explorarSalas(hall);

	// Libera memC3ria alocada, garantindo que o programa seja eficiente.
	liberar(hall);

	return 0;
}