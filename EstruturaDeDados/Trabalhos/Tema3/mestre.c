#include <stdio.h>         // Inclui a biblioteca padrão de entrada/saída (printf, scanf).
#include <stdlib.h>        // Inclui a biblioteca padrão (rand, srand).
#include <time.h>          // Inclui para gerar semente aleatória baseada no tempo.
#include <ctype.h>         // Inclui para manipulação de caracteres (não usado diretamente, mas bom para I/O).
#include <locale.h>

#define FILA_CAPACIDADE 5  // Define a capacidade máxima da Fila Circular.
#define PILHA_CAPACIDADE 3 // Define a capacidade máxima da Pilha (reserva).
// Estrutura base para representar uma peça.

// Adiciona a API do Windows para mudar a página de código
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

typedef struct
{
    char nome; // Tipo da peça: I, O, T, L (identificação).
    int id;    // Identificador único (para rastrear peças).
} Peca;
// Estrutura da fila circular (FIFO).
typedef struct
{
    Peca itens[FILA_CAPACIDADE]; // Array de armazenamento.
    int inicio;                  // Índice da cabeça/frente da fila (próxima a sair).
    int fim;                     // Índice da cauda/final da fila (próxima a entrar).
    int tamanho;                 // Contador de elementos ativos na fila.
} FilaCircular;
// Estrutura da pilha (LIFO).
typedef struct
{
    Peca itens[PILHA_CAPACIDADE]; // Array de armazenamento.
    int topo;                     // Índice do topo da pilha (-1 = vazia).
} Pilha;
// Variável global para gerar IDs únicos (incrementada a cada nova peça).
int proximoID = 0;
// ---------- Cores ANSI ---------- (Para melhorar a visualização no console)
#define GREEN "\x1b[32m"  // Código de cor verde.
#define RED "\x1b[31m"    // Código de cor vermelha.
#define YELLOW "\x1b[33m" // Código de cor amarela.
#define RESET "\x1b[0m"   // Código para resetar a cor padrão.
// ---------- Protótipos ---------- (Declaração de todas as funções antes do main)
Peca gerarPeca();                                      // Gera uma nova peça aleatória com ID único.
void inicializarFila(FilaCircular *fila);              // Configura a fila inicial.
void inicializarPilha(Pilha *pilha);                   // Configura a pilha vazia.
void exibirEstado(FilaCircular *fila, Pilha *pilha);   // Mostra o conteúdo de ambas as estruturas.
void jogarPeca(FilaCircular *fila);                    // Remove a peça da frente da fila e gera uma nova no final.
void reservarPeca(FilaCircular *fila, Pilha *pilha);   // Move peça da fila para a pilha.
void usarPecaReservada(Pilha *pilha);                  // Remove peça do topo da pilha.
void trocarSimples(FilaCircular *fila, Pilha *pilha);  // Troca a peça da frente da fila com a do topo da pilha.
void trocarMultipla(FilaCircular *fila, Pilha *pilha); // Troca 3 peças da fila por 3 da pilha.
int lerOpcao();                                        // Função segura para ler a opção do menu.
// ---------- Função Principal ----------
int main()
{
    set_utf8_console();
    FilaCircular fila;        // Declara a fila de peças.
    Pilha pilha;              // Declara a pilha de reserva.
    int opcao;                // Variável para a escolha do menu.
    srand(time(NULL));        // Inicializa o gerador de números aleatórios.
    inicializarFila(&fila);   // Preenche a fila com peças iniciais.
    inicializarPilha(&pilha); // Deixa a pilha vazia.
    do
    {
        printf("\n================ ESTADO ATUAL ================\n");
        exibirEstado(&fila, &pilha); // Exibe o estado atual da fila e pilha.
        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças do topo da pilha\n");
        printf("0 - Sair\n");
        opcao = lerOpcao(); // Chama a função segura para ler a opção.
        switch (opcao)
        {
        case 1:
            jogarPeca(&fila);
            break; // Dequeue (remoção da frente).
        case 2:
            reservarPeca(&fila, &pilha);
            break; // Movimenta Fila -> Pilha.
        case 3:
            usarPecaReservada(&pilha);
            break; // Pop (remoção do topo).
        case 4:
            trocarSimples(&fila, &pilha);
            break; // Troca um elemento.
        case 5:
            trocarMultipla(&fila, &pilha);
            break; // Troca múltiplos elementos.
        case 0:
            printf(GREEN "[✓] Encerrando o programa.\n" RESET);
            break; // Opção de saída.
        default:
            printf(RED "[✗] Opção inválida!\n" RESET); // Trata opções inválidas.
        }
    } while (opcao != 0); // Continua o loop até a opção de sair ser selecionada.
    return 0;
}
// ---------- Funções ----------
// Gera uma peça aleatória com ID único
Peca gerarPeca()
{
    Peca p;
    char tipos[4] = {'I', 'O', 'T', 'L'}; // Tipos possíveis de peças.
    p.nome = tipos[rand() % 4];           // Sorteia um dos 4 tipos.
    p.id = proximoID++;                   // Atribui e incrementa o ID único.
    return p;
}
// Inicializa a fila com 5 peças
void inicializarFila(FilaCircular *fila)
{
    fila->inicio = 0;                // O início aponta para a primeira posição.
    fila->fim = FILA_CAPACIDADE - 1; // O fim aponta para a última posição.
    fila->tamanho = FILA_CAPACIDADE; // O tamanho é igual à capacidade (fila cheia na inicialização).
    for (int i = 0; i < FILA_CAPACIDADE; i++)
    {
        fila->itens[i] = gerarPeca(); // Preenche todas as posições com novas peças.
    }
}
// Inicializa pilha vazia
void inicializarPilha(Pilha *pilha)
{
    pilha->topo = -1; // O topo em -1 indica que a pilha está vazia.
}
// Exibe fila e pilha
void exibirEstado(FilaCircular *fila, Pilha *pilha)
{
    printf("Fila de peças: ");
    if (fila->tamanho == 0)
        printf("[vazia]"); // Se o tamanho for zero.
    else
    {
        // Percorre a fila a partir do 'inicio', usando aritmética modular.
        for (int i = 0; i < fila->tamanho; i++)
        {
            int idx = (fila->inicio + i) % FILA_CAPACIDADE; // Calcula o índice circular.
            printf("[%c %d] ", fila->itens[idx].nome, fila->itens[idx].id);
        }
    }
    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilha->topo == -1)
        printf("[vazia]"); // Se o topo for -1.
    else
    {
        // Percorre a pilha do 'topo' para a 'base' (índice 0).
        for (int i = pilha->topo; i >= 0; i--)
        {
            printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
        }
    }
    printf("\n==============================================\n");
}
// Lê opção do usuário de forma segura
int lerOpcao()
{
    int opcao;
    char buffer[10];
    printf("Opção escolhida: ");
    fgets(buffer, sizeof(buffer), stdin); // Lê a linha inteira para evitar problemas de buffer.
    if (sscanf(buffer, "%d", &opcao) != 1)
        return -1; // Tenta extrair um inteiro; se falhar, retorna -1 (inválido).
    return opcao;
}
// Remove peça da frente da fila (dequeue + enqueue de nova peça)
void jogarPeca(FilaCircular *fila)
{
    if (fila->tamanho == 0)
    {
        printf(RED "[✗] Fila vazia! Nenhuma peça para jogar.\n" RESET);
        return;
    }
    // A peça jogada é a que está no índice 'inicio'.
    printf(GREEN "[✓] Peça jogada: [%c %d]\n" RESET, fila->itens[fila->inicio].nome, fila->itens[fila->inicio].id);
    fila->inicio = (fila->inicio + 1) % FILA_CAPACIDADE; // Avança o índice 'inicio' circularmente.
    // Simula a entrada de uma nova peça no final para manter a fila cheia (Tetris padrão).
    // O índice do final é calculado como (inicio + tamanho - 1) % CAPACIDADE.
    fila->itens[(fila->inicio + fila->tamanho - 1) % FILA_CAPACIDADE] = gerarPeca();
}
// Move peça da frente da fila para a pilha (pop da fila, push na pilha)
void reservarPeca(FilaCircular *fila, Pilha *pilha)
{
    if (fila->tamanho == 0)
    {
        printf(RED "[✗] Fila vazia! Nenhuma peça para reservar.\n" RESET);
        return;
    }
    if (pilha->topo >= PILHA_CAPACIDADE - 1)
    { // Verifica se a pilha está cheia.
        printf(YELLOW "[!!!] Pilha cheia! Não é possível reservar mais peças.\n" RESET);
        return;
    }
    // Operação PUSH (Pilha):
    pilha->topo++;                                         // Incrementa o topo.
    pilha->itens[pilha->topo] = fila->itens[fila->inicio]; // Copia o item da frente da fila para o topo da pilha.
    printf(GREEN "[✓] Peça [%c %d] reservada na pilha.\n" RESET, fila->itens[fila->inicio].nome, fila->itens[fila->inicio].id);
    // Operação DEQUEUE (Fila):
    fila->inicio = (fila->inicio + 1) % FILA_CAPACIDADE; // Avança o índice 'inicio'.
    // Enqueue de nova peça para substituir a que foi movida.
    fila->itens[(fila->inicio + fila->tamanho - 1) % FILA_CAPACIDADE] = gerarPeca();
}
// Remove peça do topo da pilha (pop)
void usarPecaReservada(Pilha *pilha)
{
    if (pilha->topo == -1)
    { // Verifica se a pilha está vazia.
        printf(RED "[✗] Pilha vazia! Nenhuma peça para usar.\n" RESET);
        return;
    }
    // A peça usada é a que está no índice 'topo'.
    printf(GREEN "[✓] Peça usada da pilha: [%c %d]\n" RESET, pilha->itens[pilha->topo].nome, pilha->itens[pilha->topo].id);
    pilha->topo--; // Decrementa o topo (efetivamente remove a peça).
}
// Troca peça da frente da fila com o topo da pilha
void trocarSimples(FilaCircular *fila, Pilha *pilha)
{
    if (fila->tamanho == 0 || pilha->topo == -1)
    {
        printf(RED "[✗] Não há peças suficientes para troca simples.\n" RESET);
        return;
    }
    Peca temp = fila->itens[fila->inicio];                 // Guarda a peça da fila.
    fila->itens[fila->inicio] = pilha->itens[pilha->topo]; // Fila recebe a peça da pilha.
    pilha->itens[pilha->topo] = temp;                      // Pilha recebe a peça original da fila.
    printf(GREEN "[✓] Troca simples realizada.\n" RESET);
}
// Troca as 3 primeiras peças da fila com as 3 do topo da pilha
void trocarMultipla(FilaCircular *fila, Pilha *pilha)
{
    // Verifica se há pelo menos 3 itens na fila e 3 itens na pilha.
    if (fila->tamanho < 3 || pilha->topo < 2)
    {
        printf(RED "[✗] Não há peças suficientes para troca múltipla.\n" RESET);
        return;
    }
    // Loop para realizar 3 trocas consecutivas (i=0, 1, 2).
    for (int i = 0; i < 3; i++)
    {
        int idxFila = (fila->inicio + i) % FILA_CAPACIDADE; // Calcula o índice circular da peça da fila.
        // Realiza a troca:
        Peca temp = fila->itens[idxFila];                     // Guarda a peça da fila.
        fila->itens[idxFila] = pilha->itens[pilha->topo - i]; // Fila recebe a peça da pilha (topo - i).
        pilha->itens[pilha->topo - i] = temp;                 // Pilha recebe a peça da fila.
    }
    printf(GREEN "[✓] Troca múltipla realizada.\n" RESET);
}
