#include <stdio.h>  // Inclui a biblioteca padrão de entrada/saída (printf, scanf, etc.)
#include <stdlib.h> // Inclui a biblioteca padrão (malloc, calloc, free, rand, etc.)
#include <string.h> // Inclui a biblioteca de manipulação de strings (strcspn, strcmp, strcpy, strstr)
#include <time.h>   // Inclui a biblioteca de tempo (necessária para a função srand)

#define TAM_NOME 30    // Define o tamanho máximo para o nome do território.
#define TAM_COR 10     // Define o tamanho máximo para o nome da cor do exército/jogador.
#define TAM_MISSAO 100 // Define o tamanho máximo da string de missão.

// Definição da estrutura Territorio
typedef struct
{
    char nome[TAM_NOME]; // Nome do território.
    char cor[TAM_COR];   // Cor do exército que controla o território (simula o jogador).
    int tropas;          // Número de tropas no território.
} Territorio;

// Definição da estrutura Jogador
typedef struct
{
    char *missao; // Ponteiro para a string da missão, alocada dinamicamente.
} Jogador;

// ============================
// Funções auxiliares
// ============================
// Função para limpar o buffer de entrada (consumir o '\n' restante após scanf).
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Função para listar todos os territórios cadastrados no mapa.
void listarTerritorios(Territorio *mapa, int totalTerritorios)
{
    // Verifica se o mapa está vazio.
    if (totalTerritorios == 0)
    {
        printf("Nenhum território cadastrado.\n");
        return;
    }
    // Itera sobre os territórios e imprime seus dados.
    for (int i = 0; i < totalTerritorios; i++)
    {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}
// ============================
// Cadastro de territórios
// ============================
// Função para cadastrar um novo território no mapa.
void cadastrarTerritorio(Territorio *mapa, int *totalTerritorios, int maxTerritorios)
{
    // Verifica se o limite máximo de territórios foi atingido.
    if (*totalTerritorios >= maxTerritorios)
    {
        printf("Limite de territórios atingido!\n");
        return;
    }
    printf("Digite o nome do Território: ");
    fgets(mapa[*totalTerritorios].nome, TAM_NOME, stdin);
    // Remove o caractere de nova linha '\n' lido pelo fgets.
    mapa[*totalTerritorios].nome[strcspn(mapa[*totalTerritorios].nome, "\n")] = '\0';
    printf("Digite a cor do Exército: ");
    fgets(mapa[*totalTerritorios].cor, TAM_COR, stdin);
    // Remove o caractere de nova linha '\n' lido pelo fgets.
    mapa[*totalTerritorios].cor[strcspn(mapa[*totalTerritorios].cor, "\n")] = '\0';
    printf("Digite o número de Tropas: ");
    scanf("%d", &mapa[*totalTerritorios].tropas);
    limparBufferEntrada(); // Limpa o buffer após o scanf.
    (*totalTerritorios)++; // Incrementa o número total de territórios.
    printf("Território cadastrado com sucesso!\n");
    printf("\n");
}
// ============================
// Missões
// ============================
// Função que verifica se uma missão específica foi cumprida.
// A verificação é feita por substrings simples dentro da string da missão.
int verificarMissao(char *missao, Territorio *mapa, int tamanho, int rodada)
{
    // Missão: Conquistar 3 territórios seguidos
    if (strstr(missao, "Conquistar 3 territórios"))
    {
        int conquistados = 0;
        // Conta quantos territórios têm tropas (> 0), assumindo que o controle se mantém.
        for (int i = 0; i < tamanho; i++)
            if (mapa[i].tropas > 0)
                conquistados++;
        return (conquistados >= 3);
    }
    // Missão: Eliminar todas as tropas da cor vermelha
    if (strstr(missao, "cor vermelha"))
    {
        for (int i = 0; i < tamanho; i++)
            // Se encontrar algum território com a cor "vermelha" e com tropas, a missão não foi cumprida.
            if (strcmp(mapa[i].cor, "vermelha") == 0 && mapa[i].tropas > 0)
                return 0;
        return 1; // Se o loop terminar sem encontrar, a missão foi cumprida.
    }
    // Missão: Manter pelo menos 5 tropas em um território
    if (strstr(missao, "5 tropas"))
    {
        for (int i = 0; i < tamanho; i++)
            if (mapa[i].tropas >= 5)
                return 1; // Encontrou, missão cumprida.
        return 0;         // Não encontrou nenhum com 5 ou mais tropas.
    }
    // Missão: Controlar dois territórios diferentes
    if (strstr(missao, "2 territórios"))
    {
        int comTropas = 0;
        // Conta quantos territórios têm tropas.
        for (int i = 0; i < tamanho; i++)
            if (mapa[i].tropas > 0)
                comTropas++;
        return (comTropas >= 2);
    }
    // Missão: Sobreviver até o final de 5 rodadas
    if (strstr(missao, "5 rodadas"))
        return (rodada >= 5);
    return 0; // Retorno padrão se a missão não for reconhecida.
}
// Função para verificar e mostrar o status das missões de todos os jogadores.
void verificarMissoesJogadores(Jogador *jogadores, int qtdJogadores,
                               Territorio *mapa, int totalTerritorios, int rodada)
{
    // Itera sobre cada jogador e verifica sua missão.
    for (int i = 0; i < qtdJogadores; i++)
    {
        if (jogadores[i].missao)
        {
            if (verificarMissao(jogadores[i].missao, mapa, totalTerritorios, rodada))
                printf("Jogador %d cumpriu a missão: %s\n", i + 1, jogadores[i].missao);
            else
                printf("Jogador %d ainda não cumpriu a missão: %s\n", i + 1, jogadores[i].missao);
        }
    }
}
// Função para sortear missões aleatórias para os jogadores.
void sortearMissoes(Jogador *jogadores, int qtdJogadores, char *missoes[], int totalMissoes)
{
    // Itera sobre cada jogador.
    for (int i = 0; i < qtdJogadores; i++)
    {
        int sorteio = rand() % totalMissoes; // Gera um índice aleatório.
        // Aloca dinamicamente memória para a string da missão do jogador.
        jogadores[i].missao = (char *)malloc(TAM_MISSAO * sizeof(char));
        // Copia a missão sorteada para a estrutura do jogador.
        strcpy(jogadores[i].missao, missoes[sorteio]);
        printf("Missão do Jogador %d: %s\n", i + 1, jogadores[i].missao);
    }
}
// ============================
// Batalha
// ============================
// Simula uma batalha contínua entre dois territórios baseada em rolagem de dados.
void atacar(Territorio *atacante, Territorio *defensor)
{
    // Verifica se algum dos territórios está sem tropas (impossível atacar/defender).
    if (atacante->tropas == 0 || defensor->tropas == 0)
    {
        printf("Território sem tropas não pode atacar ou ser atacado.\n");
        return;
    }
    printf("\nBatalha entre '%s' e '%s'\n", atacante->nome, defensor->nome);
    // Loop da batalha: continua enquanto ambos tiverem tropas.
    while (atacante->tropas > 0 && defensor->tropas > 0)
    {
        int dadoAtacante = rand() % 6 + 1; // Rola um dado (1 a 6) para o atacante.
        int dadoDefensor = rand() % 6 + 1; // Rola um dado (1 a 6) para o defensor.
        printf("Dado do atacante: %d | Dado do defensor: %d\n", dadoAtacante, dadoDefensor);
        // Atacante vence o duelo.
        if (dadoAtacante > dadoDefensor)
        {
            defensor->tropas--; // Defensor perde 1 tropa.
            atacante->tropas++; // Atacante ganha 1 tropa (regra de simulação).
            // Defensor vence o duelo.
        }
        else if (dadoAtacante < dadoDefensor)
        {
            atacante->tropas--; // Atacante perde 1 tropa.
            defensor->tropas++; // Defensor ganha 1 tropa (regra de simulação).
        } // Em caso de empate, nada acontece.
        printf("Status: %s=%d | %s=%d\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
    }
    // Resultados finais da batalha.
    if (atacante->tropas == 0)
        printf("%s perdeu todas as tropas!\n", atacante->nome);
    if (defensor->tropas == 0)
        printf("%s perdeu todas as tropas!\n", defensor->nome);
}
// ============================
// Liberar memória
// ============================
// Função para liberar toda a memória alocada dinamicamente (mapa e missões dos jogadores).
void liberarMemoria(Territorio *mapa, Jogador *jogadores, int qtdJogadores)
{
    free(mapa); // Libera o array de territórios.
    // Libera a string de missão alocada para cada jogador.
    for (int i = 0; i < qtdJogadores; i++)
        free(jogadores[i].missao);
    free(jogadores); // Libera o array de jogadores.
}
// ============================
// Programa principal
// ============================
int main()
{
    srand(time(NULL)); // Inicializa a semente para a geração de números aleatórios (dados).
    int totalTerritorios = 0, maxTerritorios;
    printf("Quantos territórios deseja cadastrar?\n");
    printf("Qtde:");
    scanf("%d", &maxTerritorios); // Lê o número máximo de territórios.
    printf("\n");
    limparBufferEntrada();
    // Aloca dinamicamente o array de territórios (o mapa).
    Territorio *mapa = (Territorio *)calloc(maxTerritorios, sizeof(Territorio));
    if (!mapa)
    {
        printf("Erro ao alocar memória!\n");
        return 1;
    } // Verifica falha na alocação.
    int qtdJogadores = 2; // Define o número fixo de jogadores (2).
    // Aloca dinamicamente o array de jogadores.
    Jogador *jogadores = (Jogador *)malloc(qtdJogadores * sizeof(Jogador));
    // Array de strings contendo as missões disponíveis no jogo.
    char *missoes[] = {
        "Conquistar 3 territórios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Manter pelo menos 5 tropas em um território",
        "Controlar dois territórios diferentes",
        "Sobreviver até o final de 5 rodadas"};
    // Calcula o número total de missões no array.
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);
    int rodada = 0; // Inicializa o contador de rodadas.
    // ====================
    // Cadastro inicial de territórios
    // ====================
    // Cadastra todos os territórios definidos pelo usuário (maxTerritorios).
    for (int i = 0; i < maxTerritorios; i++)
        cadastrarTerritorio(mapa, &totalTerritorios, maxTerritorios);
    // Sorteia e atribui as missões aos jogadores.
    sortearMissoes(jogadores, qtdJogadores, missoes, totalMissoes);
    // ====================
    // Loop principal do jogo
    // ====================
    int opcao;
    do
    {
        // Exibe o menu de opções.
        printf("\n=============================\n");
        printf("SISTEMA WAR - MENU\n");
        printf("=============================\n");
        printf("1 - Listar territórios\n");
        printf("2 - Atacar\n");
        printf("3 - Verificar missões\n");
        printf("0 - Sair\n");
        printf("=============================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();
        switch (opcao)
        {
        case 1:
            listarTerritorios(mapa, totalTerritorios);
            break;
        case 2:
        { // Opção de ataque.
            // Verifica se há territórios suficientes para atacar.
            if (totalTerritorios < 2)
            {
                printf("Cadastre pelo menos 2 territórios.\n");
                break;
            }
            int ataque, defesa;
            listarTerritorios(mapa, totalTerritorios);
            // Pede os índices do atacante e defensor (usuário digita índice + 1).
            printf("Escolha atacante: ");
            scanf("%d", &ataque);
            limparBufferEntrada();
            printf("Escolha defensor: ");
            scanf("%d", &defesa);
            limparBufferEntrada();
            // Chama a função atacar, passando os endereços corretos (índice - 1).
            atacar(&mapa[ataque - 1], &mapa[defesa - 1]);
            rodada++; // Incrementa o contador de rodadas após a batalha.
            // Verifica o status das missões após a ação da rodada.
            verificarMissoesJogadores(jogadores, qtdJogadores, mapa, totalTerritorios, rodada);
            break;
        }
        case 3:
            // Opção para verificar as missões a qualquer momento.
            verificarMissoesJogadores(jogadores, qtdJogadores, mapa, totalTerritorios, rodada);
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opção inválida!\n");
        }
    } while (opcao != 0); // O loop continua enquanto a opção não for '0'.
    // Libera toda a memória alocada antes de encerrar o programa.
    liberarMemoria(mapa, jogadores, qtdJogadores);
    return 0; // Fim do programa.
}