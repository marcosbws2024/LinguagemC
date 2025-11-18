#include <stdio.h>	// Inclui a biblioteca padrão de entrada/saída (printf, scanf).
#include <stdlib.h> // Inclui a biblioteca padrão (malloc, exit).
#include <string.h> // Inclui a biblioteca para manipulação de strings (strcpy).
#include <locale.h>

// Adiciona a API do Windows para mudar a página de código, nessa pré-compilação que vê se o windows 32bits
// E decide se é essa biblioteca windows.h, pois o terminal muitas vezes ainda é do windows 32 para o mingw
#ifdef _WIN32 
#include <windows.h>
#endif

// Função que configura a codificação do console (específico para Windows)
void set_utf8_console() {
    // 1. Tenta configurar a localidade C padrão para UTF-8
    setlocale(LC_ALL, "C.UTF-8");
    
    // 2. Se for Windows, força a página de código do console
    #ifdef _WIN32
        // 65001 é o identificador da página de código para UTF-8 no Windows
        SetConsoleOutputCP(65001);
    #endif
}

// Estrutura que representa cada sala (nó da árvore)
typedef struct Sala
{
	char nome[50];		   // Nome da sala, como "Hall de Entrada" ou "Biblioteca".
	struct Sala *esquerda; // Ponteiro para a próxima sala à esquerda (subárvore esquerda).
	struct Sala *direita;  // Ponteiro para a próxima sala à direita (subárvore direita).
} Sala;
/**
 * Função: criarSala
 * ------------------
 * Cria dinamicamente uma sala (nó) com um nome.
 * Aloca a memória necessária, copia o nome para a estrutura e inicializa
 * os ponteiros 'esquerda' e 'direita' como NULL.
 */
Sala *criarSala(const char *nome)
{
	Sala *nova = (Sala *)malloc(sizeof(Sala)); // Aloca memória para a nova sala.
	if (nova == NULL)
	{ // Verifica se a alocação de memória falhou.
		printf("Erro ao alocar memória!\n");
		exit(1); // Encerra o programa com erro.
	}
	strcpy(nova->nome, nome); // Copia o nome fornecido para o campo 'nome' da struct.
	nova->esquerda = NULL;	  // Inicializa o caminho da esquerda como nulo.
	nova->direita = NULL;	  // Inicializa o caminho da direita como nulo.
	return nova;			  // Retorna o ponteiro para a sala recém-criada.
}
/**
 * Função: explorarSalas
 * ---------------------
 * Permite ao jogador navegar interativamente pela mansão,
 * escolhendo os caminhos à esquerda ou direita.
 * O processo é iterativo, usando um loop 'while' até que a exploração termine.
 */
void explorarSalas(Sala *atual)
{
	char escolha;
	while (atual != NULL)
	{ // Continua a exploração enquanto o nó atual for válido.
		printf("\nVocê está em: %s\n", atual->nome);
		// Caso seja um nó-folha (sem caminhos), a exploração termina aqui.
		if (atual->esquerda == NULL && atual->direita == NULL)
		{
			printf("Você chegou ao fim da exploração! Não há mais caminhos.\n");
			break; // Sai do loop.
		}
		printf("Escolha um caminho: (e) esquerda, (d) direita, (s) sair: ");
		// O espaço ' ' antes de %c ignora quaisquer espaços em branco e '\n' pendentes no buffer.
		scanf(" %c", &escolha);
		if (escolha == 'e' || escolha == 'E')
		{
			if (atual->esquerda != NULL)
			{
				// Move-se para o nó da esquerda.
				atual = atual->esquerda;
			}
			else
			{
				printf("Não há caminho à esquerda!\n");
			}
		}
		else if (escolha == 'd' || escolha == 'D')
		{
			if (atual->direita != NULL)
			{
				// Move-se para o nó da direita.
				atual = atual->direita;
			}
			else
			{
				printf("Não há caminho à direita!\n");
			}
		}
		else if (escolha == 's' || escolha == 'S')
		{
			printf("Você decidiu sair da exploração.\n");
			break; // Sai do loop.
		}
		else
		{
			printf("Opção inválida! Tente novamente.\n");
		}
	}
}
/**
 * Função: liberar
 * ---------------
 * Libera recursivamente toda a memória da árvore, evitando vazamentos de memória (Memory Leaks).
 * É um percurso pós-ordem: libera os filhos antes de liberar o pai.
 */
void liberar(Sala *raiz)
{
	if (raiz != NULL)
	{
		liberar(raiz->esquerda); // Chama recursivamente para a subárvore esquerda.
		liberar(raiz->direita);	 // Chama recursivamente para a subárvore direita.
		free(raiz);				 // Libera a memória do nó atual (pai).
	}
}
/**
 * Função Principal (main)
 * -----------------------
 * Monta manualmente o mapa da mansão (árvore binária)
 * e inicia a exploração.
 */
int main()
{
	set_utf8_console();
	// Construção manual do mapa da mansão, criando os nós e as ligações
	Sala *hall = criarSala("Hall de Entrada"); // Cria a raiz da árvore.
	hall->esquerda = criarSala("Sala de Estar");
	hall->direita = criarSala("Biblioteca");
	hall->esquerda->esquerda = criarSala("Cozinha");
	hall->esquerda->direita = criarSala("Jardim"); // Nó-folha.
	hall->direita->direita = criarSala("Quarto");  // Nó-folha (Biblioteca tem caminho só para a direita).
	// Inicia a exploração
	printf("=== Detective Quest: Explorando a Mansão ===\n");
	explorarSalas(hall); // Começa a exploração a partir da raiz ('hall').
	// Libera memória alocada, garantindo que o programa seja eficiente.
	liberar(hall);
	return 0;
}