#include "parte1/parte1.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main()
{
    GAME game;
    game.estado.looping = true;
    game.tab = NULL; // Inicialmente, nenhum tabuleiro está carregado

    // Array de comandos: sair, ler, gravar
    COMANDO comandos[] = {sair, lerCmd, gravar, NULL};

    while (game.estado.looping)
    {
        char line[LINE_SIZE] = {0};
        printf("\n> "); // seta para dar ideia de terminal
        if (fgets(line, LINE_SIZE, stdin) == NULL)
        {
            game.estado.looping = false;
            break;
        }
        assert(line[strlen(line) - 1] == '\n');

        char cmd[LINE_SIZE] = {0};
        char arg[LINE_SIZE] = {0};
        char resto[LINE_SIZE] = {0};
        int num_args = sscanf(line, "%s %s %[^\n]", cmd, arg, resto);
        if (strlen(cmd) != 1)
        {
            fprintf(stderr, "Erro: comando %s não é válido!\n", cmd);
            continue;
        }
        if (num_args == 3)
        { // Se houver argumentos extras
            fprintf(stderr, "Erro: comando %s foi invocado com argumentos extra: %s\n", cmd, resto);
            continue;
        }

        bool comandoProcessado = false;
        // Processa os comandos padrão (s, l, g)
        for (int i = 0; !comandoProcessado && comandos[i] != NULL; i++)
        {
            comandoProcessado = comandos[i](cmd[0], (num_args >= 2) ? arg : NULL, &game);
        }

        // Executa os comandos de modificação do tabuleiro: 'b' (pintar) e 'r' (riscar)
        if (!comandoProcessado)
        {
            if (cmd[0] == 'b' || cmd[0] == 'r')
            {
                if (arg[0] == '\0')
                {
                    fprintf(stderr, "Erro: comando %c necessita de uma coordenada!\n", cmd[0]);
                    continue;
                }
                int linha, coluna;
                if (!coordenadaParaIndice(arg, &linha, &coluna))
                {
                    fprintf(stderr, "Coordenada inválida!\n");
                    continue;
                }
                if (game.tab == NULL)
                {
                    fprintf(stderr, "Erro: tabuleiro não carregado.\n");
                    continue;
                }
                if (cmd[0] == 'b')
                {
                    if (!pintarBranco(game.tab, linha, coluna))
                        fprintf(stderr, "Erro: Coordenada fora dos limites!\n");
                }
                else
                { // comando 'r'
                    if (!riscar(game.tab, linha, coluna))
                        fprintf(stderr, "Erro: Coordenada fora dos limites!\n");
                }
                comandoProcessado = true;
            }
        }

        if (!comandoProcessado)
        {
            fprintf(stderr, "Comando não reconhecido!\n");
        }

        printf("\nEstado atual do tabuleiro:\n");
        mostrarTabela(game.tab);
    }

    if (game.tab != NULL)
        freeTabela(game.tab);

    return 0;
}