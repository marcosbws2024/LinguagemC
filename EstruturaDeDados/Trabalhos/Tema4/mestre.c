#include <stdio.h> // Inclui a biblioteca padrão de I/O (entrada/saída), essencial para printf/scanf.
#include <stdlib.h> // Inclui a biblioteca padrão, crucial para alocação de memória (malloc, free) e exit.
#include <string.h> // Inclui a biblioteca de strings, usada para manipulação de strings (strcmp, strlen, strncpy).
#include <ctype.h> // Inclui a biblioteca para funções de caracteres (como tolower para ignorar caixa).
#include <locale.h>

#define TAMANHO_HASH 31 // Define o tamanho da Tabela Hash para Suspeitos/Pistas (tamanho primo).
#define MAX_STR 100 // Define o tamanho máximo de caracteres para a maioria das strings de I/O.
#define QUIT_FLAG 1 // Define um sinal de retorno para forçar a saída da recursão de navegação (opção 'q').
#define TOTAL_COMODOS 10 // AGORA GLOBAL E DEFINIDO PELO PRÉ-PROCESSADOR


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

/********************* ESTRUTURAS DE DADOS ************************/
typedef struct Comodo {
	char nome[50]; // Nome do cômodo (ex: "Biblioteca").
	struct Comodo* esquerda; // Ponteiro para o nó (cômodo) à esquerda (navegação na Árvore).
	struct Comodo* direita; // Ponteiro para o nó (cômodo) à direita (navegação na Árvore).
} Comodo; // Estrutura para os nós da Árvore Binária que representa o mapa da mansão.

typedef struct NoPista {
	char pista[100]; // Descrição da pista (chave de ordenação na BST).
	int contagem; // Quantas vezes o jogador coletou esta pista (força da evidência).
	char origem[50]; // Cômodo onde a pista foi encontrada.
	struct NoPista* esquerda; // Ponteiro para a sub-árvore esquerda (BST).
	struct NoPista* direita; // Ponteiro para a sub-árvore direita (BST).
} NoPista; // Estrutura para os nós da Árvore de Busca Binária (BST) de Pistas Coletadas pelo jogador.

typedef struct NoDispersao {
	char pista[100]; // A chave de busca na Hash: a pista.
	char suspeito[100]; // O valor associado: o suspeito ligado à pista (gabarito).
	struct NoDispersao* proximo; // Ponteiro usado para Lista Ligada (encadeamento) para resolver colisões na Hash.
} NoDispersao; // Estrutura para os nós da Tabela Hash que mapeia Pista -> Suspeito.

typedef struct NoVisitado {
	char nomeComodo[50]; // Nome do cômodo já explorado.
	struct NoVisitado* proximo; // Ponteiro para o próximo cômodo visitado na lista ligada.
} NoVisitado; // Estrutura para os nós da Lista Ligada de rastreamento de visitação (para o mapa).

typedef struct PontuacaoSuspeito {
	char nome[100]; // Nome do suspeito.
	int pontuacao; // Pontuação total de evidências agregadas.
} PontuacaoSuspeito; // Estrutura auxiliar usada no julgamento para somar as evidências por suspeito.

/* Tabelas globais */
NoDispersao* tabela_dispersao[TAMANHO_HASH]; // O array da Tabela Hash (global).
Comodo* raiz_mansao = NULL; // Ponto de entrada (raiz) da Árvore da Mansão.
NoPista* raiz_pistas = NULL; // Raiz da BST de pistas do jogador (o conjunto de evidências).
NoVisitado* comodos_visitados = NULL; // Início da Lista Ligada de cômodos visitados.
const char* todos_os_comodos[] = { // Lista estática de todos os cômodos para verificação.
	"Entrada", "Sala de estar", "Corredor", "Biblioteca", "Cozinha",
	"Escritorio", "Quarto principal", "Closet", "Jardim", "Porão"
};

const int TOTAL_PISTAS_UNICAS = 10; // Conta fixa de pistas únicas (uma por cômodo).

/********************* PROTÓTIPOS DE FUNÇÕES ************************/

NoPista* encontrarNoPista(NoPista* raiz, const char* pista); // Procura um nó de pista específico na BST de pistas do jogador.
void removeQuebraLinha(char* s); // Remove o caractere de nova linha (\n) de strings lidas por fgets.
void converteTextoMinusculo(char* s); // Converte uma string para minúsculas.
int jaVisitouComodo(const char* nome); // Verifica se um cômodo está na lista de visitados.
void marcarVisitado(const char* nome); // Adiciona um cômodo à lista de visitados (Lista Ligada).
int pistaJaColetada(NoPista* raiz, const char* pista); // Verifica se uma pista já está presente na BST de evidências.
Comodo* criarComodo(const char* nome); // Aloca e inicializa um novo nó de cômodo (para a árvore da mansão).
NoPista* inserirPista(NoPista* raiz, const char* pista, const char* comodoOrigem); // Insere ou incrementa uma pista na BST de evidências.
void inserirNaHash(const char* pista, const char* suspeito); // Insere um par pista/suspeito na Tabela Hash.
const char* encontrarSuspeito(const char* pista); // Busca o suspeito associado a uma pista na Hash.
void listarRelacionamentosHash(); // Exibe o mapeamento Pista -> Suspeito contido na Hash.
int verificarSuspeitoFinal(NoPista* raiz, const char* acusado); // Calcula a pontuação total de evidências contra o acusado final.
void listarPistasInOrder(NoPista* raiz); // Percorre a BST de pistas em ordem (In-Order) para listagem alfabética.
const char* pistaParaComodo(const char* nomeComodo); // Mapeia estaticamente um cômodo à sua pista correspondente (gabarito).
void listarComodosFaltantes(); // Lista os cômodos que ainda não foram visitados.
void listarPistasColetadasDuranteNavegacao(); // Exibe as pistas coletadas pelo jogador usando a travessia In-Order.
int explorarComodos(Comodo* atual); // Função recursiva para navegação pela árvore da mansão (retorna int para controle).
void liberarPistas(NoPista* raiz); // Libera recursivamente a memória da BST de Pistas.
void liberarHash(); // Libera a memória alocada para os nós da Tabela Hash (encadeamento).
void liberarMansao(Comodo* r); // Libera recursivamente a memória da Árvore da Mansão.
void liberarVisitados(); // Libera a memória da Lista Ligada de Visitados.
void agregarPontuacoesRecursivo(NoPista* raiz, PontuacaoSuspeito pontuacoes[], int* contador); // Percorre a BST de Pistas e soma os pontos para cada suspeito.
int contarTotalPistasColetadas(NoPista* raiz); // Conta o total de instâncias de pistas coletadas.
int contarPistasUnicasColetadas(NoPista* raiz); // Conta o número de tipos diferentes de pistas coletadas.
int contarPistasPerdidas(); // Conta as pistas disponíveis em cômodos visitados que o jogador não coletou.

/********************* FUNÇÕES UTILITÁRIAS ************************/

void removeQuebraLinha(char* s) {
	size_t n = strlen(s); // Obtém o comprimento da string.
	if (n == 0) return; // Se a string estiver vazia, retorna.
	if (s[n-1] == '\n') s[n-1] = '\0'; // Se o último caractere for '\n' (do fgets), substitui por '\0'.
}

void converteTextoMinusculo(char* s) {
	for (int i = 0; s[i]; i++) { // Percorre a string até o terminador '\0'.
		s[i] = (char)tolower((unsigned char)s[i]); // Converte o caractere atual para minúscula.
	}
}

int jaVisitouComodo(const char* nome) {
	NoVisitado* atual = comodos_visitados; // Começa na cabeça da lista de visitados.
	while (atual) { // Percorre a lista.
		if (strcmp(atual->nomeComodo, nome) == 0) {
			return 1; // Encontrou o cômodo na lista.
		}
		atual = atual->proximo; // Move para o próximo nó.
	}
	return 0; // Não encontrou.
}

void marcarVisitado(const char* nome) {
	if (jaVisitouComodo(nome)) return; // Evita duplicatas na lista de visitados.

	NoVisitado* novo = (NoVisitado*)malloc(sizeof(NoVisitado)); // Aloca memória para o novo nó.
	if (!novo) {
		fprintf(stderr, "Erro de alocacao ao marcar comodo visitado\n"); // Trata falha de alocação.
		exit(EXIT_FAILURE);
	}
	strncpy(novo->nomeComodo, nome, sizeof(novo->nomeComodo)-1); // Copia o nome do cômodo.
	novo->nomeComodo[sizeof(novo->nomeComodo)-1] = '\0';

	novo->proximo = comodos_visitados; // O novo nó aponta para a cabeça atual.
	comodos_visitados = novo; // O novo nó se torna a nova cabeça (inserção no início).
}

int pistaJaColetada(NoPista* raiz, const char* pista) {
	if (raiz == NULL) return 0; // Condição de parada: não encontrou a pista na árvore.
	int cmp = strcmp(pista, raiz->pista); // Compara a pista procurada com a pista do nó atual.
	if (cmp == 0) return 1; // Pista encontrada.
	if (cmp < 0) return pistaJaColetada(raiz->esquerda, pista); // Se menor, busca à esquerda.
	return pistaJaColetada(raiz->direita, pista); // Se maior, busca à direita.
}

NoPista* encontrarNoPista(NoPista* raiz, const char* pista) {
	if (raiz == NULL) return NULL; // Nó nulo, pista não encontrada.
	int cmp = strcmp(pista, raiz->pista);
	if (cmp == 0) return raiz; // Encontrou e retorna o ponteiro do nó.
	if (cmp < 0) return encontrarNoPista(raiz->esquerda, pista);
	return encontrarNoPista(raiz->direita, pista);
}

int contarTotalPistasColetadas(NoPista* raiz) {
	if (raiz == NULL) return 0; // Caso base da recursão.
	// Soma a contagem do nó atual mais a contagem recursiva das sub-árvores.
	return raiz->contagem + contarTotalPistasColetadas(raiz->esquerda) + contarTotalPistasColetadas(raiz->direita);
}

int contarPistasUnicasColetadas(NoPista* raiz) {
	if (raiz == NULL) return 0; // Caso base da recursão.
	// Retorna 1 (para o nó atual) mais a contagem recursiva das sub-árvores.
	return 1 + contarPistasUnicasColetadas(raiz->esquerda) + contarPistasUnicasColetadas(raiz->direita);
}

int contarPistasPerdidas() {
	NoVisitado* atual = comodos_visitados; // Inicia a lista de cômodos visitados.
	int contagem_perdidas = 0;
	char pistas_processadas[TOTAL_COMODOS * 2][100]; // Array auxiliar para evitar checar a mesma pista duas vezes.
	int contagem_processadas = 0;

	while (atual) { // Percorre a lista de cômodos visitados.
		const char* pista = pistaParaComodo(atual->nomeComodo); // Obtém a pista associada.
		if (pista) {
			int ja_checada = 0;
			for (int i = 0; i < contagem_processadas; i++) { // Verifica se esta pista já foi processada.
				if (strcmp(pistas_processadas[i], pista) == 0) {
					ja_checada = 1;
					break;
				}
			}

			if (!ja_checada) {
				if (!pistaJaColetada(raiz_pistas, pista)) { // Se a pista NÃO foi coletada...
					contagem_perdidas++; // ... incrementa o contador.
				}
				// Adiciona a pista ao array de processadas.
				if (contagem_processadas < TOTAL_COMODOS * 2) {
					strncpy(pistas_processadas[contagem_processadas], pista, 99);
					pistas_processadas[contagem_processadas][99] = '\0';
					contagem_processadas++;
				}
			}
		}
		atual = atual->proximo; // Próximo cômodo visitado.
	}
	return contagem_perdidas;
}


/********************* FUNÇÕES PRINCIPAIS ************************/

Comodo* criarComodo(const char* nome) {
	Comodo* c = (Comodo*)malloc(sizeof(Comodo)); // Aloca memória.
	if (!c) {
		fprintf(stderr, "Erro de alocação ao criar comodo '%s'\n", nome); // Trata erro.
		exit(EXIT_FAILURE);
	}
	strncpy(c->nome, nome, sizeof(c->nome)-1); // Copia o nome.
	c->nome[sizeof(c->nome)-1] = '\0';
	c->esquerda = c->direita = NULL; // Inicializa os filhos.
	return c;
}

NoPista* inserirPista(NoPista* raiz, const char* pista, const char* comodoOrigem) {
	if (raiz == NULL) { // Se a raiz for nula, cria o novo nó.
		NoPista* novo = (NoPista*)malloc(sizeof(NoPista));
		if (!novo) {
			fprintf(stderr, "Erro de alocação ao inserir pista\n");
			exit(EXIT_FAILURE);
		}
		strncpy(novo->pista, pista, sizeof(novo->pista)-1); novo->pista[sizeof(novo->pista)-1] = '\0';
		novo->contagem = 1;
		strncpy(novo->origem, comodoOrigem, sizeof(novo->origem)-1); novo->origem[sizeof(novo->origem)-1] = '\0';
		novo->esquerda = novo->direita = NULL;
		return novo;
	}
	int cmp = strcmp(pista, raiz->pista);
	if (cmp == 0) {
		raiz->contagem++; // Pista duplicada, incrementa a contagem.
	} else if (cmp < 0) {
		raiz->esquerda = inserirPista(raiz->esquerda, pista, comodoOrigem); // Recursão à esquerda.
	} else {
		raiz->direita = inserirPista(raiz->direita, pista, comodoOrigem); // Recursão à direita.
	}
	return raiz; // Retorna a raiz da sub-árvore (ou o novo nó).
}

void inserirNaHash(const char* pista, const char* suspeito) {
	unsigned long soma = 0;
	// Função Hash simples (multiplicação por primo)
	for (int i = 0; pista[i]; i++) soma = soma * 31 + (unsigned char)pista[i];
	int idx = (int)(soma % TAMANHO_HASH); // Calcula o índice no array.

	NoDispersao* atual = tabela_dispersao[idx];
	while (atual) { // Percorre a lista ligada (tratamento de colisão).
		if (strcmp(atual->pista, pista) == 0) {
			strncpy(atual->suspeito, suspeito, sizeof(atual->suspeito)-1); // Atualiza o suspeito se a pista já existe.
			atual->suspeito[sizeof(atual->suspeito)-1] = '\0';
			return;
		}
		atual = atual->proximo;
	}

	// Cria novo nó.
	NoDispersao* novo = (NoDispersao*)malloc(sizeof(NoDispersao));
	if (!novo) {
		fprintf(stderr, "Erro de alocação ao inserir na hash\n");
		exit(EXIT_FAILURE);
	}
	strncpy(novo->pista, pista, sizeof(novo->pista)-1); novo->pista[sizeof(novo->pista)-1] = '\0';
	strncpy(novo->suspeito, suspeito, sizeof(novo->suspeito)-1); novo->suspeito[sizeof(novo->suspeito)-1] = '\0';
	novo->proximo = tabela_dispersao[idx]; // Insere no início da lista.
	tabela_dispersao[idx] = novo;
}

const char* encontrarSuspeito(const char* pista) {
	unsigned long soma = 0;
	// Recalcula o índice hash para a pista.
	for (int i = 0; pista[i]; i++) soma = soma * 31 + (unsigned char)pista[i];
	int idx = (int)(soma % TAMANHO_HASH);

	NoDispersao* atual = tabela_dispersao[idx];
	while (atual) { // Percorre a lista ligada.
		if (strcmp(atual->pista, pista) == 0) return atual->suspeito; // Encontrado.
		atual = atual->proximo;
	}
	return NULL; // Não encontrado.
}

void listarRelacionamentosHash() {
	printf("--- Suspeitos e Pistas Relacionadas (Mapeamento Base) ---\n");
	char pistas_listadas[MAX_STR * 2][100]; // Array para evitar listar a mesma pista mais de uma vez.
	int contador = 0;

	for (int i = 0; i < TAMANHO_HASH; i++) { // Percorre o array da Hash.
		NoDispersao* atual = tabela_dispersao[i];
		while (atual) { // Percorre a lista ligada em cada índice.
			int ja_listada = 0;
			for(int j = 0; j < contador; j++) {
				if(strcmp(pistas_listadas[j], atual->pista) == 0) {
					ja_listada = 1;
					break;
				}
			}

			if (!ja_listada) { // Se a pista não foi listada, imprime.
				printf(" - Suspeito: **%s** -> Pista: **%s**\n", atual->suspeito, atual->pista);
				strncpy(pistas_listadas[contador], atual->pista, 99);
				pistas_listadas[contador][99] = '\0';
				contador++;
			}
			atual = atual->proximo;
		}
	}
	printf("----------------------------------------------------------\n");
}

int verificarSuspeitoFinal(NoPista* raiz, const char* acusado) {
	if (!raiz) return 0; // Caso base da recursão.
	int subtotal = 0;

	// Recursão In-Order para percorrer todas as pistas coletadas.
	subtotal += verificarSuspeitoFinal(raiz->esquerda, acusado);
	subtotal += verificarSuspeitoFinal(raiz->direita, acusado);

	const char* s = encontrarSuspeito(raiz->pista); // Busca o suspeito original ligado à pista.
	if (s) {
		char susp_temp[100], acc_temp[100];
		// Prepara as strings para comparação sem distinção de caixa.
		strncpy(susp_temp, s, sizeof(susp_temp)-1); susp_temp[sizeof(susp_temp)-1] = '\0';
		strncpy(acc_temp, acusado, sizeof(acc_temp)-1); acc_temp[sizeof(acc_temp)-1] = '\0';
		converteTextoMinusculo(susp_temp);
		converteTextoMinusculo(acc_temp);

		if (strcmp(susp_temp, acc_temp) == 0) { // Compara suspeito da pista com o acusado.
			subtotal += raiz->contagem; // Adiciona a contagem da pista à pontuação total.
			printf(" - Pista **'%s'** aponta para '%s' (coletada %d vez(es)).\n", raiz->pista, s, raiz->contagem);
		}
	}
	return subtotal; // Retorna a soma das evidências.
}

void listarPistasInOrder(NoPista* raiz) {
	if (!raiz) return; // Caso base da recursão.
	listarPistasInOrder(raiz->esquerda); // Percorre esquerda.
	printf(" - %s (coletada %d vez(es)) [Origem: %s]\n", raiz->pista, raiz->contagem, raiz->origem); // Imprime o nó.
	listarPistasInOrder(raiz->direita); // Percorre direita.
}

const char* pistaParaComodo(const char* nomeComodo) {
	// Mapeamento estático Cômodo -> Pista.
	if (strcmp(nomeComodo, "Entrada") == 0) return "Pegada de lama";
	if (strcmp(nomeComodo, "Sala de estar") == 0) return "Copo quebrado";
	if (strcmp(nomeComodo, "Biblioteca") == 0) return "Pagina arrancada";
	if (strcmp(nomeComodo, "Cozinha") == 0) return "Panelas sujas";
	if (strcmp(nomeComodo, "Escritorio") == 0) return "Carta rasurada";
	if (strcmp(nomeComodo, "Quarto principal") == 0) return "Perfume caro";
	if (strcmp(nomeComodo, "Closet") == 0) return "Tecido rasgado";
	if (strcmp(nomeComodo, "Jardim") == 0) return "Pegada de botas";
	if (strcmp(nomeComodo, "Porão") == 0) return "Poeira estranha";
	if (strcmp(nomeComodo, "Corredor") == 0) return "Mancha de tinta";
	return NULL; // Se não houver pista no cômodo.
}

void listarComodosFaltantes() {
	printf("\n Comodos Faltantes (Ainda Não Visitados):\n");
	int faltantes_cont = 0;
	for (int i = 0; i < TOTAL_COMODOS; i++) { // Percorre a lista estática de todos.
		if (!jaVisitouComodo(todos_os_comodos[i])) { // Verifica se não está na lista de visitados.
			printf("  [ ] %s\n", todos_os_comodos[i]);
			faltantes_cont++;
		}
	}
	if (faltantes_cont == 0) {
		printf("  (Parabéns! Todos os comodos foram visitados.)\n");
	}
	printf("--------------------------------------\n");
}


void listarPistasColetadasDuranteNavegacao() {
	printf("\n--- Pistas Coletadas Ate Agora (Provas) ---\n");
	if (!raiz_pistas) {
		printf(" (Nenhuma pista coletada ainda.)\n");
	} else {
		listarPistasInOrder(raiz_pistas); // Chama a travessia In-Order.
	}
	printf("--------------------------------------\n");
}


// FUNÇÃO ATUALIZADA: Retorna um int para sinalizar QUIT_FLAG
int explorarComodos(Comodo* atual) {
	if (!atual) return 0; // Se nó nulo, retorna (volta).

	marcarVisitado(atual->nome); // Marca o cômodo atual como visitado.

	while (1) { // Loop do menu dentro do cômodo.
		printf("\n> Voce esta no comodo: **%s**\n", atual->nome);

		// 1. PISTAS NO CÔMODO (Lógica de coleta)
		const char* pista = pistaParaComodo(atual->nome);
		if (pista) {
			if (pistaJaColetada(raiz_pistas, pista)) {
				printf("A pista **\"%s\"** ja foi coletada anteriormente. (Contagem: %d)\n", pista, encontrarNoPista(raiz_pistas, pista)->contagem);
			} else {
				printf("Ha uma nova pista aqui: **\"%s\"**.\n", pista);
				char opc[MAX_STR];
				printf("Deseja **coletar** essa pista? (s/n) [s]: ");
				if (!fgets(opc, sizeof(opc), stdin)) opc[0] = '\n';
				removeQuebraLinha(opc);
				if (opc[0] == '\0' || tolower((unsigned char)opc[0]) == 's') {
					raiz_pistas = inserirPista(raiz_pistas, pista, atual->nome); // Insere na BST.
					printf("[Pista Coletada] %s\n", pista);
				} else {
					printf("Pista deixada para tras.\n");
				}
			}
		} else {
			printf("Nenhuma pista aparente neste comodo.\n");
		}

		// 2. STATUS DETALHADO (Exibe as métricas)
		listarPistasColetadasDuranteNavegacao();
		
		printf("\n--- Status de Exploração da Mansão ---\n");
		printf(" Comodo Atual : %s\n", atual->nome);
		printf(" [Pistas: %d/%d Coletadas]\n", contarPistasUnicasColetadas(raiz_pistas), TOTAL_PISTAS_UNICAS);
		printf(" Qtde Pistas Deixadas para Tras: %d\n", contarPistasPerdidas());
		
		printf("\n Comodos Visitados:\n");
		NoVisitado* atual_visitado = comodos_visitados;
		int visitados_cont = 0;
		while(atual_visitado) {
			printf("  [X] %s\n", atual_visitado->nomeComodo);
			atual_visitado = atual_visitado->proximo;
			visitados_cont++;
		}
		if (visitados_cont == 0) {
			printf("  (Nenhum comodo visitado ainda.)\n");
		}
		
		listarComodosFaltantes();

		// 3. MENU DE NAVEGAÇÃO COM MÉTRICAS
		printf("\n--- Opcoes de Navegacao ---\n");
		printf("(e) esquerda: %s\n", atual->esquerda ? atual->esquerda->nome : "Nao ha caminho.");
		printf("(d) direita: %s\n", atual->direita ? atual->direita->nome : "Nao ha caminho.");
		printf("(s) sair: Voltar para o comodo anterior.\n");
		printf("(q) quit: Sair da mansao e ir para o julgamento final.\n");
		printf("---------------------------\n");

		// Leitura da opção
		printf("Digite a opcao: ");
		char cmd[16];
		if (!fgets(cmd, sizeof(cmd), stdin)) return 0;
		removeQuebraLinha(cmd);
		if (cmd[0] == '\0') continue;

		char c = (char)tolower((unsigned char)cmd[0]);
		if (c == 'e') {
			if (atual->esquerda) {
				if (explorarComodos(atual->esquerda) == QUIT_FLAG) return QUIT_FLAG; // Propaga 'q'.
			}
			else printf("Caminho esquerdo bloqueado.\n");
		} else if (c == 'd') {
			if (atual->direita) {
				if (explorarComodos(atual->direita) == QUIT_FLAG) return QUIT_FLAG; // Propaga 'q'.
			}
			else printf("Caminho direito bloqueado.\n");
		} else if (c == 's') {
			printf("Saindo do comodo %s.\n", atual->nome);
			return 0; // Sai do loop e retorna (volta para o nível anterior da recursão).
		} else if (c == 'q') {
			printf("\nOpcao 'quit' selecionada. Preparando para o julgamento.\n");
			return QUIT_FLAG; // Retorna o sinal para sair de todas as chamadas.
		} else {
			printf("Opcao invalida. Use 'e', 'd', 's' ou 'q'.\n");
		}
	}
}


/* Funções de limpeza de memória e agregação */

void liberarPistas(NoPista* raiz) {
	if (!raiz) return;
	liberarPistas(raiz->esquerda); // Libera sub-árvore esquerda.
	liberarPistas(raiz->direita); // Libera sub-árvore direita.
	free(raiz); // Libera o nó atual.
}

void liberarHash() {
	for (int i = 0; i < TAMANHO_HASH; i++) { // Percorre o array da Hash.
		NoDispersao* atual = tabela_dispersao[i];
		while (atual) { // Percorre a lista ligada no índice.
			NoDispersao* temp = atual;
			atual = atual->proximo;
			free(temp); // Libera o nó.
		}
		tabela_dispersao[i] = NULL;
	}
}

void liberarMansao(Comodo* r) {
	if (!r) return;
	liberarMansao(r->esquerda); // Libera recursivamente a esquerda.
	liberarMansao(r->direita); // Libera recursivamente a direita.
	free(r); // Libera o nó atual.
}

void liberarVisitados() {
	NoVisitado* atual = comodos_visitados;
	while (atual) { // Percorre a lista ligada de visitados.
		NoVisitado* temp = atual;
		atual = atual->proximo;
		free(temp); // Libera o nó.
	}
	comodos_visitados = NULL;
}

void agregarPontuacoesRecursivo(NoPista* raiz, PontuacaoSuspeito pontuacoes[], int* contador) {
	if (raiz == NULL) return;

	agregarPontuacoesRecursivo(raiz->esquerda, pontuacoes, contador); // Percorre esquerda.

	const char* suspeito = encontrarSuspeito(raiz->pista); // Busca o suspeito.
	if (suspeito) {
		int encontrado = 0;
		for (int i = 0; i < *contador; i++) { // Procura o suspeito na lista de pontuações.
			if (strcmp(pontuacoes[i].nome, suspeito) == 0) {
				pontuacoes[i].pontuacao += raiz->contagem; // Soma a pontuação.
				encontrado = 1;
				break;
			}
		}
		if (!encontrado && *contador < TOTAL_COMODOS) { // Se for um novo suspeito, adiciona.
			strncpy(pontuacoes[*contador].nome, suspeito, 99); pontuacoes[*contador].nome[99] = '\0';
			pontuacoes[*contador].pontuacao = raiz->contagem;
			(*contador)++;
		}
	}

	agregarPontuacoesRecursivo(raiz->direita, pontuacoes, contador); // Percorre direita.
}


/********************* PROGRAMA PRINCIPAL *************************/

int main() {
	set_utf8_console();
	printf("=== Detective Quest: Coletor de Pistas (versao texto) ===\n\n");

	// Montagem manual da árvore da mansão (representação do mapa).
	raiz_mansao = criarComodo("Entrada");
	raiz_mansao->esquerda = criarComodo("Sala de estar");
	raiz_mansao->direita = criarComodo("Corredor");
	raiz_mansao->esquerda->esquerda = criarComodo("Biblioteca");
	raiz_mansao->esquerda->direita = criarComodo("Cozinha");
	raiz_mansao->direita->esquerda = criarComodo("Escritorio");
	raiz_mansao->direita->direita = criarComodo("Quarto principal");
	raiz_mansao->esquerda->esquerda->esquerda = criarComodo("Closet");
	raiz_mansao->direita->direita->direita = criarComodo("Jardim");
	raiz_mansao->direita->esquerda->esquerda = criarComodo("Porão");

	// Tabela hash pista -> suspeito (Populando o gabarito do caso).
	inserirNaHash("Pegada de lama", "Carlos");
	inserirNaHash("Copo quebrado", "Roberta");
	inserirNaHash("Pagina arrancada", "Lucas");
	inserirNaHash("Panelas sujas", "Paulo");
	inserirNaHash("Carta rasurada", "Marina");
	inserirNaHash("Perfume caro", "Amanda");
	inserirNaHash("Tecido rasgado", "Roberto");
	inserirNaHash("Pegada de botas", "Carlos");
	inserirNaHash("Mancha de tinta", "Zeca");
	inserirNaHash("Poeira estranha", "Paulo");

	printf("Mapa e pistas iniciais carregados. Comece a explorar a mansao!\n");

	// Inicia a exploração. O retorno controla se o jogo continua ou vai para o julgamento.
	explorarComodos(raiz_mansao);

	printf("\n\n#################################################\n");
	printf("=== FIM DA EXPLORACAO: HORA DO JULGAMENTO ===\n");
	printf("#################################################\n");

	if (raiz_pistas) {
		// Passo 1: Lista Suspeitos e Pistas Relacionadas (Mapeamento Hash)
		listarRelacionamentosHash();

		// Passo 2: Agrega e mostra a pontuação de todos os suspeitos
		PontuacaoSuspeito pontuacoes[TOTAL_COMODOS];
		int contagem_suspeitos_unicos = 0;

		agregarPontuacoesRecursivo(raiz_pistas, pontuacoes, &contagem_suspeitos_unicos); // Agregação de pontos.

		printf("\n* --- Resumo de Evidencias Coletadas por Suspeito ---\n");
		if (contagem_suspeitos_unicos == 0) {
			printf(" (Nenhuma pista coletada. O caso nao pode ser analisado.)\n");
		} else {
			for (int i = 0; i < contagem_suspeitos_unicos; i++) {
				printf(" - **%s**: %d ponto(s) de evidencia.\n", pontuacoes[i].nome, pontuacoes[i].pontuacao);
			}
		}
		printf(" (Regra: São necessários 2 ou mais pontos de evidencia para condenar.)\n");
		printf("----------------------------------------------------------\n");

		// Passo 3: Lista Pistas Coletadas (Provas) pelo jogador
		listarPistasColetadasDuranteNavegacao();

		// Passo 4: Acusação Final e Veredito
		char acusado[100];
		printf("\n* Com base nas evidencias, quem voce acusa? (digite o nome): ");
		if (!fgets(acusado, sizeof(acusado), stdin)) { strcpy(acusado, ""); }
		removeQuebraLinha(acusado);

		if (strlen(acusado) == 0) {
			printf("Nenhum acusado informado. Encerrando sem julgamento.\n");
		} else {
			printf("\n- Analisando evidencias contra **'%s'**...\n", acusado);
			int total = verificarSuspeitoFinal(raiz_pistas, acusado); // Verifica e calcula a pontuação.

			if (total >= 2) {
				printf("\n[VEREDITO FAVORAVEL] **HA** evidencias suficientes (%d pista(s)) contra **'%s'**.\n", total, acusado);
				printf("Resultado: **Acusacao sustentada!** Parabens, voce resolveu o caso.\n");
			} else {
				printf("\n[VEREDITO INSUFICIENTE] **INSUFICIENTE** (%d pista(s)) para sustentar a acusacao contra **'%s'**.\n", total, acusado);
				printf("Resultado: Acusacao nao sustentada. Voce precisava de pelo menos 2 provas que o ligassem.\n");
			}
		}
	} else {
		printf("Nenhuma pista coletada. O caso termina sem acusacao.\n");
	}

	// Lista Comodos Visitados e Faltantes (Revisão Final)
	listarComodosFaltantes();

	// Limpeza de memória (Importante para evitar memory leaks).
	liberarPistas(raiz_pistas);
	liberarHash();
	liberarMansao(raiz_mansao);
	liberarVisitados();

	printf("\nSessao encerrada. Obrigado por jogar!\n");
	return 0;
}