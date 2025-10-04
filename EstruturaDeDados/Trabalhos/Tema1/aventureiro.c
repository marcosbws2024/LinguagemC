#include <stdio.h>  // Inclui a biblioteca padrão de entrada/saída (printf, scanf, etc.)
#include <stdlib.h> // Inclui a biblioteca padrão (calloc, free, rand, etc.)
#include <string.h> // Inclui a biblioteca de manipulação de strings (strcspn, strcpy)
#include <time.h>   // Inclui a biblioteca de tempo (necessária para a função srand)

#define TAM_NOME 30 // Define o tamanho máximo para o nome do território
#define TAM_COR 10  // Define o tamanho máximo para a cor do exército/jogador

// Definição da estrutura Territorio
typedef struct
{
    char nome[TAM_NOME]; // Nome do território
    char cor[TAM_COR];   // Cor do exército
    int tropas;          // Número de tropas
} Territorio;
// Limpa buffer do teclado
// Função para consumir o caractere de nova linha (\n) que sobra no buffer após o uso de scanf.
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
// Cadastrar território
// Adiciona um novo território ao mapa, verificando o limite máximo.
void cadastrarTerritorio(Territorio *mapa, int *totalTerritorios, int maxTerritorios)
{
    // Verifica se o limite de territórios foi atingido.
    if (*totalTerritorios >= maxTerritorios)
    {
        printf("Limite de territórios atingido!\n");
        return;
    }
    printf("Digite o nome do Território: ");
    // Lê o nome usando fgets (leitura segura de strings).
    fgets(mapa[*totalTerritorios].nome, TAM_NOME, stdin);
    // Remove o '\n' final adicionado pelo fgets.
    mapa[*totalTerritorios].nome[strcspn(mapa[*totalTerritorios].nome, "\n")] = '\0';
    printf("Digite a cor do Exército: ");
    fgets(mapa[*totalTerritorios].cor, TAM_COR, stdin);
    mapa[*totalTerritorios].cor[strcspn(mapa[*totalTerritorios].cor, "\n")] = '\0';
    printf("Digite o número de Tropas: ");
    scanf("%d", &mapa[*totalTerritorios].tropas);
    limparBufferEntrada(); // Limpa o buffer após o scanf.
    (*totalTerritorios)++; // Incrementa o contador de territórios.
    printf("Território cadastrado com sucesso!\n");
}
// Listar territórios
// Exibe os detalhes de todos os territórios cadastrados.
void listarTerritorios(Territorio *mapa, int totalTerritorios)
{
    // Verifica se há algum território para listar.
    if (totalTerritorios == 0)
    {
        printf("Nenhum território cadastrado.\n");
        return;
    }
    // Loop para percorrer o array e imprimir as informações.
    for (int i = 0; i < totalTerritorios; i++)
    {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}
// Função de ataque contínuo
// Simula uma batalha entre dois territórios com base na rolagem de dados.
void atacar(Territorio *atacante, Territorio *defensor)
{
    // Verifica se o atacante tem tropas para atacar.
    if (atacante->tropas == 0)
    {
        printf("O território atacante '%s' não possui tropas e não pode atacar.\n", atacante->nome);
        return;
    }
    // Verifica se o defensor tem tropas para ser atacado.
    if (defensor->tropas == 0)
    {
        printf("O território defensor '%s' não possui tropas e não pode ser atacado.\n", defensor->nome);
        return;
    }
    printf("\n=== Início da batalha: %s (atacante) x %s (defensor) ===\n",
           atacante->nome, defensor->nome);
    // Batalha contínua: duelos são repetidos enquanto ambos tiverem tropas.
    while (atacante->tropas > 0 && defensor->tropas > 0)
    {
        // Rolagem de dados (simulando 1d6).
        int dadoAtacante = rand() % 6 + 1;
        int dadoDefensor = rand() % 6 + 1;
        printf("Dado do atacante (%s): %d | Dado do defensor (%s): %d\n",
               atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);
        // O atacante vence o duelo.
        if (dadoAtacante > dadoDefensor)
        {
            defensor->tropas--; // Defensor perde 1.
            atacante->tropas++; // Atacante ganha 1 (lógica de simulação).
            // O defensor vence o duelo.
        }
        else if (dadoAtacante < dadoDefensor)
        {
            atacante->tropas--; // Atacante perde 1.
            defensor->tropas++; // Defensor ganha 1 (lógica de simulação).
            // Empate.
        }
        else
        {
            printf("Empate nesta rodada! Nenhum território perde tropas.\n");
        }
        printf("Status atual: %s=%d | %s=%d\n",
               atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
    }
    // Resultado final da batalha
    if (defensor->tropas == 0)
    {
        // Vitória: Defensor perdeu e território é conquistado.
        printf("\n=== Vitória do atacante '%s'! ===\n", atacante->nome);
        printf("O defensor '%s' perdeu todas as tropas.\n", defensor->nome);
        // O território passa para a cor do atacante.
        strcpy(defensor->cor, atacante->cor);
        // Metade das tropas restantes do atacante são movidas para o novo território.
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
        printf("O território '%s' agora pertence a '%s' com %d tropas.\n",
               defensor->nome, defensor->cor, defensor->tropas);
    }
    else if (atacante->tropas == 0)
    {
        // Defesa: Atacante perdeu todas as tropas.
        printf("\n=== Vitória do defensor '%s'! ===\n", defensor->nome);
        printf("O atacante '%s' perdeu todas as tropas.\n", atacante->nome);
    }
    printf("=== Fim da batalha ===\n");
}
// Libera memória
// Libera o bloco de memória alocado dinamicamente para o mapa.
void liberarMemoria(Territorio *mapa)
{
    free(mapa);
}
int main()
{
    // Inicializa o gerador de números aleatórios com o tempo atual para simulação de dados.
    srand(time(NULL));
    int totalTerritorios = 0;
    int maxTerritorios;
    printf("Quantos territórios deseja cadastrar?\n");
    printf("Qtde: ");
    scanf("%d", &maxTerritorios); // Define o tamanho máximo do mapa.
    limparBufferEntrada();
    // Aloca dinamicamente o array de estruturas Territorio (o mapa) e inicializa com zero.
    Territorio *mapa = (Territorio *)calloc(maxTerritorios, sizeof(Territorio));
    // Verifica se a alocação de memória falhou.
    if (!mapa)
    {
        printf("Erro ao alocar memória!\n");
        return 1;
    }
    int opcao;
    // Loop principal do menu, executa até que a opção 0 seja escolhida.
    do
    {
        // Exibe o menu.
        printf("\n=============================\n");
        printf("SISTEMA WAR - TERRITÓRIOS\n");
        printf("=============================\n");
        printf("1 - Cadastrar território\n");
        printf("2 - Listar territórios\n");
        printf("3 - Atacar\n");
        printf("0 - Sair\n");
        printf("=============================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();
        // Trata a opção escolhida.
        switch (opcao)
        {
        case 1:
            cadastrarTerritorio(mapa, &totalTerritorios, maxTerritorios);
            break;
        case 2:
            listarTerritorios(mapa, totalTerritorios);
            break;
        case 3:
        { // Bloco de ataque.
            // Verifica se há territórios suficientes para o ataque.
            if (totalTerritorios < 2)
            {
                printf("É necessário cadastrar pelo menos 2 territórios para atacar.\n");
                break;
            }
            int ataque, defesa;
            // Seleção do atacante com validações (índice válido, tem tropas).
            while (1)
            {
                listarTerritorios(mapa, totalTerritorios);
                printf("\nEscolha o território atacante (0 para voltar): ");
                scanf("%d", &ataque);
                limparBufferEntrada();
                if (ataque == 0)
                    break;
                if (ataque < 1 || ataque > totalTerritorios)
                {
                    printf("Número inválido!\n");
                    continue;
                }
                if (mapa[ataque - 1].tropas == 0)
                {
                    printf("Este território não possui tropas e não pode atacar.\n");
                    continue;
                }
                break;
            }
            if (ataque == 0)
                break;
            // Seleção do defensor com validações (índice válido, não é o atacante, tem tropas).
            while (1)
            {
                printf("Escolha o território defensor (0 para voltar): ");
                scanf("%d", &defesa);
                limparBufferEntrada();
                if (defesa == 0)
                    break;
                if (defesa < 1 || defesa > totalTerritorios)
                {
                    printf("Número inválido!\n");
                    continue;
                }
                if (defesa == ataque)
                {
                    printf("Não é possível atacar o próprio território!\n");
                    continue;
                }
                if (mapa[defesa - 1].tropas == 0)
                {
                    printf("Este território não possui tropas e não pode ser atacado.\n");
                    continue;
                }
                break;
            }
            if (defesa == 0)
                break;
            // Executa a função de ataque.
            atacar(&mapa[ataque - 1], &mapa[defesa - 1]);
            break;
        }
        case 0:
            printf("Saindo do sistema...\n");
            break;
        default:
            printf("Opção inválida!\n");
            break;
        }
    } while (opcao != 0); // Repete o menu até que a opção 0 seja digitada.
    liberarMemoria(mapa); // Libera a memória alocada dinamicamente.
    printf("Memória liberada com sucesso!\n");
    return 0; // Encerramento bem-sucedido do programa.
}