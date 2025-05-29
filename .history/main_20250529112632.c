#include "parte1/parte1.h"
#include "parte2/parte2.h"
#include "parte4/parte4.h"
#include "tipos.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

void leArgumentosEValida(char *line, char *cmd, char *arg, char *resto, bool *continuar, GAME *game, int *num_args)
{
    line = readline("> ");
    if (line == NULL)
    {
        (*game).estado.looping = false;
        *continuar = false;
    }
    else
    {
    }
    if (*continuar && line[strlen(line) - 1] != '\n')
        *continuar = false;

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

void trataComandoaAR(char comando, GAME *game, bool *continuar, bool *comandoProcessado)
{
    if (comando == 'a' && *continuar)
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

    else if (comando == 'A' && *continuar)
    {
        printf("%sA executar o comando de ajuda acumulada...%s\n", HINT_COLOR, RESET);
        comandoA(&(*game).tab, continuar);
    }

    else if (comando == 'R' && *continuar)
    {
        printf("%sA executar o comando de resolver o jogo...%s\n", HINT_COLOR, RESET);
        TABELA aux = resolve((*game).stackTabs->tabelas[0]); // Recebe a primeira tabela carregada (em principio so com letras minusculas)
        if (aux != NULL)
        {
            TABELA temp = (*game).tab;
            (*game).tab = aux;
            freeTabela(temp);
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
        if (!pintarBranco(game.tab, linha, coluna))
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
}

int comandoQAlteraTab(char comando, bool continuar, bool comandoProcessado)
{
    return (!comandoProcessado && continuar && (comando == 'b' || comando == 'r' || comando == 'a' || comando == 'R' || comando == 'A'));
}

void comandovdu(bool *continuar, char comando, GAME *game, bool *comandoProcessado)
{
    // Executa o comando de verificar restrições: 'v'
    if (*continuar && comando == 'v')
        v(*game, continuar, comandoProcessado);

    // Executa o comando de voltar atrás
    if (*continuar && comando == 'd')
        d(game, comandoProcessado);

    if (*continuar && comando == 'u')
        u(game, comandoProcessado);

    if (!(*comandoProcessado) && (*continuar))
        fprintf(stderr, "%sErro: Comando não reconhecido!%s\n", ERROR_COLOR, RESET);

    mostrarTabela((*game).tab);
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

void desenhaBemVindo()
{
    printf("%s%s", CLEAR_SCREEN, WELCOME_COLOR);
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
    printf("%s%s%s             Bem Vindo ao Hitor!%s\n\n", GREEN, BOLD, BLINK, RESET);
    printf("%sUtilize '?' para ver os comandos disponíveis%s\n", HINT_COLOR, RESET);
}

// Função principal responsável por rodar o jogo
int main()
{
    desenhaBemVindo();

    GAME game;
    game.estado.looping = true;
    game.tab = NULL;                                   // Inicialmente, nenhum tabuleiro está carregado
    game.stackTabs = malloc(sizeof(struct StackTabs)); // Cria a estrutura de dados que vai ser usada para armazenar os diferentes estados do jogo
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

        if (game.tab == NULL && !comandoProcessado) // Para não ser incluido quando usado o comando '?'
        {
            fprintf(stderr, "Erro: tabuleiro não carregado.\n");
            continuar = false;
        }

        // Executa os comandos de modificação do tabuleiro de forma a condensar o codigo de cópia de tabueiros para a stackTabs: 'b' (pintar), 'r' (riscar), 'a' (ajuda) e 'R' (resolve)
        if (comandoQAlteraTab(cmd[0], continuar, comandoProcessado))
        {
            if (cmd[0] == 'b' || cmd[0] == 'r')
            {
                trataComandobEr(cmd[0], arg, &continuar, game); // Função que trata dos comando 'b' e 'r'
            }
            else
                trataComandoaAR(cmd[0], &game, &continuar, &comandoProcessado); // Função que trata dos comando 'a', 'A' e 'R'

            // Trata da parte de guardar a nova alteração no array de tabelas
            guardaNovoTab(&continuar, game, &comandoProcessado);
        }

        comandovdu(&continuar, cmd[0], &game, &comandoProcessado);
    }

    libertaMemoria(game);

    return 0;
}