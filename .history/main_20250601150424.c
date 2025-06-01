#include "parte1/parte1.h"
#include "parte2/parte2.h"
#include "parte4/parte4.h"
#include "tipos.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define LINE_SIZE 1024

void leArgumentosEValida(char *line, char *cmd, char *arg, char *resto, bool *continuar, GAME *game, int *num_args)
{
    printf("> ");
    if (fgets(line, LINE_SIZE, stdin) == NULL)
    {
        (*game).estado.looping = false;
        *continuar = false;
    }

    if (*continuar && line[strlen(line) - 1] != '\n')
    {
        *continuar = false;
    }
    *num_args = sscanf(line, "%s %s %[^\n]", cmd, arg, resto);
    if (*continuar && strlen(cmd) != 1)
    {
        fprintf(stderr, "%sErro: comando %s não é válido!%s\n", ERROR_COLOR, cmd, RESET);
        *continuar = false;
    }
    if (*continuar && *num_args >= 3)
    { // Se houver argumentos extras
        fprintf(stderr, "%sErro: comando %s foi invocado com argumentos extra: %s%s\n", ERROR_COLOR, cmd, resto, RESET);
        *continuar = false;
    }
}

void guardaNovoTab(bool *continuar, GAME game, bool *comandoProcessado)
{
    if (*continuar)
    {
        TABELA new = copiarTabela(game.tab);
        if (new == NULL)
        {
            fprintf(stderr, "%sErro: falha ao copiar o tabuleiro%s\n", ERROR_COLOR, RESET);
            *continuar = false;
        }
        insereTabela(game.stackTabs, new);
        *comandoProcessado = true;
    }
}

void chamaa(bool *continuar, bool *comandoProcessado, GAME *game)
{
    printf("%sA executar o comando de ajuda...%s\n", HINT_COLOR, RESET);
    bool changed = 0;
    // Chama a função ajuda e substitui o tabuleiro atual pelo tabuleiro auxiliar retornado
    TABELA aux = ajuda((*game).tab, true, &changed); // 1 para escrever no terminal as mudanças que foram feitas
    if (changed == 0)
    {
        printf("Não existem alterações a ser feitas.\n");
        *continuar = false; // Para não se guardar um tabuleiro que não tenha nada
        *comandoProcessado = true;
    }
    if (aux == NULL)
    {
        fprintf(stderr, "%sErro: falha ao executar o comando de ajuda.%s\n", ERROR_COLOR, RESET);
        *continuar = false;
    }
    else
    {
        // Liberta o tabuleiro atual e substitui pelo auxiliar
        freeTabela((*game).tab);
        (*game).tab = aux;
    }
}

void chamaR(GAME *game)
{
}

void trataComandoaARi(char comando, char *arg, GAME *game, bool *continuar, bool *comandoProcessado)
{
    if (arg[0] != '\0')
    {
        *continuar = false;
        printf("%sO comando %c não tem argumentos.%s", ERROR_COLOR, comando, RESET);
    }
    else
    {
        if (comando == 'a' && *continuar)
            chamaa(continuar, comandoProcessado, game);

        else if (comando == 'A' && *continuar)
        {
            printf("%sA executar o comando de ajuda acumulada...%s\n", HINT_COLOR, RESET);
            comandoA(&(*game).tab, continuar);
        }

        else if (comando == 'R' && *continuar)
        {
            chamaR(*game);
            printf("%sA executar o comando de resolver o jogo...%s\n", HINT_COLOR, RESET);

            TABELA aux = game->tab;
            if (game->solution != NULL)
            {
                game->tab = copiarTabela(game->solution);
                freeTabela(aux);
                printf("\n%sTabuleiro resolvido%s\n", GREEN, RESET);
            }
            else
            {
                printf("\n%sNão é possível resolver este tabuleiro!%s\n", ERROR_COLOR, RESET);
            }
        }
        else if (comando == 'i' && *continuar)
        {
            printf("%sDe volta ao tabuleiro inicial%s", HINT_COLOR, RESET);
            i(game, comandoProcessado);
        }
    }
}

void trataComandobEr(char comando, char *arg, bool *continuar, GAME game)
{
    if (arg[0] == '\0')
    {
        fprintf(stderr, "%sErro: comando %c necessita de uma coordenada!%s\n", ERROR_COLOR, comando, RESET);
        *continuar = false;
    }
    int linha, coluna;
    if (!coordenadaParaIndice(arg, &linha, &coluna))
    {
        fprintf(stderr, "%sErro: coordenada inválida!%s\n", ERROR_COLOR, RESET);
        *continuar = false;
    }

    if (*continuar && comando == 'b')
    {
        if (!pintarBranco(game.tab, linha, coluna, game.solution))
        {
            fprintf(stderr, "%sErro: coordenada fora dos limites!%s\n", ERROR_COLOR, RESET);
            *continuar = false;
        }
    }
    else if (*continuar && comando == 'r')
    { // comando 'r'
        if (!riscar(game.tab, linha, coluna))
        {
            fprintf(stderr, "%sErro: coordenada fora dos limites!%s\n", ERROR_COLOR, RESET);
            *continuar = false;
        }
    }
}

void libertaMemoria(GAME game)
{
    // Liberta a memória alocada para a lista de tabuleiros
    if (game.stackTabs != NULL)
        freeStackTabs(game.stackTabs);

    // Liberta a memória alocada para o tabuleiro
    if (game.tab != NULL)
        freeTabela(game.tab);
    if (game.solution != NULL)
        freeTabela(game.solution);
}

int comandoQAlteraTab(char comando, bool continuar, bool comandoProcessado)
{
    return (!comandoProcessado && continuar && (comando == 'b' || comando == 'r' || comando == 'a' || comando == 'R' || comando == 'A' || comando == 'i'));
}

void tratavdce(bool *continuar, bool *comandoProcessado, GAME *game, char *arg, char comando)
{
    if (arg[0] != '\0')
    {
        *continuar = false;
        printf("%sO comando %c não tem argumentos.%s\n", ERROR_COLOR, comando, RESET);
    }
    // Executa o comando de verificar restrições: 'v'
    else if (*continuar && comando == 'v')
        v(*game, continuar, comandoProcessado);

    // Executa o comando de voltar atrás
    else if (*continuar && comando == 'd')
        d(game, comandoProcessado);

    // Executa o comando de ir para frente
    else if (*continuar && comando == 'u')
        u(game, comandoProcessado);

    else if (*continuar)
        e(*game, comandoProcessado);
}

void comandovduce(bool *continuar, char comando, char *arg, GAME *game, bool *comandoProcessado)
{
    if (comando == 'v' || comando == 'd' || comando == 'u' || comando == 'e')
        tratavdce(continuar, comandoProcessado, game, arg, comando);

    // Executa o comando de verificar o tabuleiro como esta atualmente com a solução
    if (comando == 'c' && *continuar)
    {
        printf("\n %s  %s -> A peça esta colocada corretamente\n", CAPITAL_GREEN, RESET);
        printf(" %s  %s -> A peça esta mal colocada\n\n", CAPITAL_RED, RESET);
        mostrarTabela(*game, 1);
        *comandoProcessado = true;
    }
    else
        mostrarTabela(*game, 0);

    if (!(*comandoProcessado) && (*continuar))
        fprintf(stderr, "%sErro: Comando não reconhecido!%s\n", ERROR_COLOR, RESET);
}

// Função que imprime os comandos possíveis
void mostrarAjuda()
{
    printf("\n%sComandos disponíveis:%s\n", COMMAND_COLOR, RESET);
    printf("%sg <jogo>%s       - grava o estado atual do jogo num ficheiro\n", COMMAND_COLOR, RESET);
    printf("%sl <jogo>%s       - lê o estado do jogo de um ficheiro\n", COMMAND_COLOR, RESET);
    printf("%s%s<coordenada>%s   - no formato <letra minúscula><número> onde a letra corresponde a uma coluna e o número a uma linha\n", BOLD, COORDINATE_COLOR, RESET);
    printf("%sb %s<coordenada>%s - colocar a letra da casa correspondente à coordenada em maiúsculas\n", COMMAND_COLOR, COORDINATE_COLOR, RESET);
    printf("%sr %s<coordenada>%s - colocar um # no local da letra\n", COMMAND_COLOR, COORDINATE_COLOR, RESET);
    printf("%sv%s              - verificar o estado do jogo e apontar todas as restrições violadas\n", COMMAND_COLOR, RESET);
    printf("%sa%s              - ajudar mudando o estado de todas as casas que se conseguem inferir através do estado atual do tabuleiro\n", COMMAND_COLOR, RESET);
    printf("%sA%s              - invocar o comando 'a' enquanto o jogo sofrer alterações\n", COMMAND_COLOR, RESET);
    printf("%sR%s              - resolver o jogo\n", COMMAND_COLOR, RESET);
    printf("%sd%s              - desfazer o último comando executado\n", COMMAND_COLOR, RESET);
    printf("%su%s              - voltar a fazer o último comando desfeito\n", COMMAND_COLOR, RESET);
    printf("%sc%s              - verificar se alterações ate agora feitas no tabuleiro estão corretas\n", COMMAND_COLOR, RESET);
    printf("%si%s              - voltar ao estado inicial\n", COMMAND_COLOR, RESET);
    printf("%se%s              - mostrar as estatísticas atuais do jogo\n", COMMAND_COLOR, RESET);
    printf("%ss%s              - sair do programa\n\n", COMMAND_COLOR, RESET);
}

void comandoAjuda(char comando, bool *comandoProcessado)
{
    if (comando == '?')
    {
        mostrarAjuda();
        *comandoProcessado = true;
    }
}

void escreveVitoria()
{

    printf("%s%s", CLEAR_SCREEN, GREEN);
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                                                           ║\n");
    printf("║   ██╗  ██╗  ██████╗ ██╗  ██╗   ██╗   ██╗██╗ ████╗  ██╗    ║\n");
    printf("║   ██║  ██║ ██╔═══██╗██║  ██║   ██║   ██║██║ ██║██╗ ██║    ║\n");
    printf("║    ██ ██╔╝ ██║   ██║██║  ██║   ██║   ██║██║ ██║ ██╗██║    ║\n");
    printf("║     ███╔╝  ██║   ██║██║  ██║   ██║██ ██║██║ ██║  ████║    ║\n");
    printf("║     ███║   ╚██████╔╝███████║   ███║ ███║██║ ██║    ██║    ║\n");
    printf("║      ╚═╝    ╚═════╝   ╚════╝    ╚═╝  ╚═╝╚═╝ ╚═╝    ╚═╝    ║\n");
    printf("║                                                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝%s\n\n", RESET);

    printf("                   %s%s%s GANHOU! Parabéns! %s\n", BLINK, GREEN, BOLD, RESET);
    printf("%sPara continuar a jogar carregue um novo tabuleiro\n\n%s", HINT_COLOR, RESET);
}

void desenhaBemVindo()
{
    printf("%s%s", CLEAR_SCREEN, YELLOW);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                                              ║\n");
    printf("║   ██╗  ██╗██╗████████╗ ██████╗ ██████╗ ██╗   ║\n");
    printf("║   ██║  ██║██║╚══██╔══╝██╔═══██╗██╔══██╗██║   ║\n");
    printf("║   ███████║██║   ██║   ██║   ██║██████╔╝██║   ║\n");
    printf("║   ██╔══██║██║   ██║   ██║   ██║██╔══██╗██║   ║\n");
    printf("║   ██║  ██║██║   ██║   ╚██████╔╝██║  ██║██║   ║\n");
    printf("║   ╚═╝  ╚═╝╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚═╝   ║\n");
    printf("║                                              ║\n");
    printf("╚══════════════════════════════════════════════╝%s\n\n", RESET);
    printf("%s%s%s             Bem Vindo ao Hitori!%s\n\n", GREEN, BOLD, BLINK, RESET);
    printf("%sUtilize '?' para ver os comandos disponíveis%s\n", HINT_COLOR, RESET);
}

void trataComandoQAlteraTab(char comando, char *arg, bool *continuar, GAME *game, bool *comandoProcessado)
{
    if (comando == 'b' || comando == 'r')
    {
        trataComandobEr(comando, arg, continuar, *game); // Função que trata dos comando 'b' e 'r'
        if (jogoResolvido(game->tab))
            escreveVitoria(*game);
    }
    else
        trataComandoaARi(comando, arg, game, continuar, comandoProcessado); // Função que trata dos comando 'a', 'A', 'R' e 'i'

    // Trata da parte de guardar a nova alteração no array de tabelas

    guardaNovoTab(continuar, *game, comandoProcessado);
}

void initGame(GAME *game)
{
    game->estado.looping = true;
    game->solution = NULL;
    game->tab = NULL;                                   // Inicialmente, nenhum tabuleiro está carregado
    game->stackTabs = malloc(sizeof(struct StackTabs)); // Cria a estrutura de dados que vai ser usada para armazenar os diferentes estados do jogo
}

void verificaExisteTab(GAME game, bool *continuar, bool comandoProcessado)
{
    if (game.tab == NULL && !comandoProcessado) // Para não ser incluido quando usado o comando '?'
    {
        fprintf(stderr, "%sErro: tabuleiro não carregado.%s\n", ERROR_COLOR, RESET);
        *continuar = false;
    }
}

// Função principal responsável por rodar o jogo
int main()
{
    desenhaBemVindo();
    GAME game;
    initGame(&game);

    initStackTabs(game.stackTabs);

    // Array de comandos: sair, ler, gravar
    COMANDO comandos[] = {sair, lerCmd, gravar, NULL};

    while (game.estado.looping)
    {
        bool continuar = true; // se true quer dizer que é para continuar neste ciclo do loop, ou se false ent é para passar para o próxima repetição do loop (o objetivo é não usar continue)
        char line[LINE_SIZE] = {0};

        char cmd[LINE_SIZE] = {0};
        char arg[LINE_SIZE] = {0};
        char resto[LINE_SIZE] = {0};
        int num_args;
        leArgumentosEValida(line, cmd, arg, resto, &continuar, &game, &num_args);

        bool comandoProcessado = false;

        // Processa o comando de ajuda '?'
        comandoAjuda(cmd[0], &comandoProcessado);

        // Processa os comandos padrão (s, l, g)
        for (int i = 0; continuar && !comandoProcessado && comandos[i] != NULL; i++)
            comandoProcessado = comandos[i](cmd[0], (num_args >= 2) ? arg : NULL, &game);

        // Caso não exista um tabuleiro carregado neste ponto então salta-se para a proxima iteração do loop
        verificaExisteTab(game, &continuar, comandoProcessado);

        // Executa os comandos de modificação do tabuleiro de forma a condensar o codigo de cópia de tabueiros para a stackTabs: 'b' (pintar), 'r' (riscar), 'a' (ajuda) e 'R' (resolve)
        if (comandoQAlteraTab(cmd[0], continuar, comandoProcessado))
            trataComandoQAlteraTab(cmd[0], arg, &continuar, &game, &comandoProcessado);

        comandovduce(&continuar, cmd[0], arg, &game, &comandoProcessado);
    }

    libertaMemoria(game);

    return 0;
}