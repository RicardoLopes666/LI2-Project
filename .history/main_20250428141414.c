#include "parte1/parte1.h"
#include "parte2/parte2.h"
#include "parte4/parte4.h"
#include "tipos.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main()
{
    GAME game;
    game.estado.looping = true;
    game.tab = NULL;                                   // Inicialmente, nenhum tabuleiro está carregado
    game.stackTabs = malloc(sizeof(struct StackTabs)); // Cria a estrutura de dados que vai ser usada para armazenar os diferentes estados do jogo
    initStackTabs(game.stackTabs);

    // Array de comandos: sair, ler, gravar
    COMANDO comandos[] = {sair, lerCmd, gravar, NULL};

    while (game.estado.looping)
    {
        bool continuar = true; // se true quer dizer que é para continuar neste ciclo do loop, ou se false ent é para passar para o próxima repetição do loop
        char line[LINE_SIZE] = {0};
        printf("\n> "); // seta para dar ideia de terminal
        if (fgets(line, LINE_SIZE, stdin) == NULL)
        {
            game.estado.looping = false;
            continuar = false;
        }
        if (continuar && line[strlen(line) - 1] != '\n')
            continuar = false;

        char cmd[LINE_SIZE] = {0};
        char arg[LINE_SIZE] = {0};
        char resto[LINE_SIZE] = {0};
        int num_args;
        num_args = sscanf(line, "%s %s %[^\n]", cmd, arg, resto);

        if (continuar && strlen(cmd) != 1)
        {
            fprintf(stderr, "Erro: comando %s não é válido!\n", cmd);
            continuar = false;
        }
        if (continuar && num_args >= 3)
        { // Se houver argumentos extras
            fprintf(stderr, "Erro: comando %s foi invocado com argumentos extra: %s\n", cmd, resto);
            continuar = false;
        }

        bool comandoProcessado = false;
        // Processa os comandos padrão (s, l, g)
        for (int i = 0; continuar && !comandoProcessado && comandos[i] != NULL; i++)
        {
            comandoProcessado = comandos[i](cmd[0], (num_args >= 2) ? arg : NULL, &game);
        }

        // Executa os comandos de modificação do tabuleiro de forma a condensar o codigo de cópia de tabueiros para a stackTabs: 'b' (pintar), 'r' (riscar), 'a' (ajuda) e 'R' (resolve)
        if (!comandoProcessado && continuar)
        {
            if (game.tab == NULL)
            {
                fprintf(stderr, "Erro: tabuleiro não carregado.\n");
                continuar = false;
            }
            if (cmd[0] == 'b' || cmd[0] == 'r' || cmd[0] == 'a' || cmd[0] == 'R')
            {
                if (cmd[0] == 'b' || cmd[0] == 'r')
                {
                    if (arg[0] == '\0')
                    {
                        fprintf(stderr, "Erro: comando %c necessita de uma coordenada!\n", cmd[0]);
                        continuar = false;
                    }
                    int linha, coluna;
                    if (!coordenadaParaIndice(arg, &linha, &coluna))
                    {
                        fprintf(stderr, "Erro: coordenada inválida!\n");
                        continuar = false;
                    }

                    if (continuar && cmd[0] == 'b')
                    {
                        if (!pintarBranco(game.tab, linha, coluna))
                        {
                            fprintf(stderr, "Erro: coordenada fora dos limites!\n");
                            continuar = false;
                        }
                    }
                    else if (continuar && cmd[0] == 'r')
                    { // comando 'r'
                        if (!riscar(game.tab, linha, coluna))
                        {
                            fprintf(stderr, "Erro: coordenada fora dos limites!\n");
                            continuar = false;
                        }
                    }
                }

                else if (cmd[0] == 'a' && continuar)
                {

                    printf("A executar o comando de ajuda...\n");
                    int changed = 0;
                    // Chama a função ajuda e substitui o tabuleiro atual pelo tabuleiro auxiliar retornado
                    TABELA aux = ajuda(game.tab, true, &changed); // 1 para escrever no terminal as mudanças que foram feitas
                    if (changed == 0)
                    {
                        fprintf(stdout, "Não existem alterações a ser feitas.\n");
                        continuar = 0; // Para não se guardar um tabuleiro que não tenha nada
                    }
                    if (aux == NULL)
                    {
                        fprintf(stderr, "Erro: falha ao executar o comando de ajuda.\n");
                        continuar = false;
                    }
                    else
                    {
                        // Liberta o tabuleiro atual e substitui pelo auxiliar
                        freeTabela(game.tab);
                        game.tab = aux;
                    }
                }

                if (cmd[0] == 'R' && continuar)
                {
                    printf("A executar o comando de resolver o jogo...\n");
                    TABELA aux = resolve(game.tab, &continuar);
                    if (aux != NULL)
                    {
                        freeTabela(game.tab);
                        game.tab = aux;
                    }
                }
                // Trata da parte de guardar a nova alteração no array de tabelas
                if (continuar)
                {
                    TABELA new = copiarTabela(game.tab);
                    if (new == NULL)
                    {
                        fprintf(stderr, "Erro: falha ao copiar o tabuleiro");
                        continuar = false;
                    }
                    insereTabela(game.stackTabs, new);
                    comandoProcessado = true;
                }
            }
        }

        // Executa o comando de verificar restrições: 'v'
        if (cmd[0] == 'v' && continuar)
        {
            if (game.tab == NULL)
            {
                fprintf(stderr, "Erro: tabuleiro não carregado.\n");
                continuar = false;
            }
            else
            {
                printf("A verificar as restrições do tabuleiro...\n");
                verificaRestricoes(game.tab, true);
                comandoProcessado = true;
            }
        }

        // Executa o comando de voltar atrás
        if (cmd[0] == 'd' && continuar)
        {
            if (!deleteTabela(&game))
            {
                fprintf(stderr, "Erro: não existem tabuleiros anteriores\n");
            }
            comandoProcessado = true;
        }

        if (!comandoProcessado && continuar)
        {
            fprintf(stderr, "Erro: Comando não reconhecido!\n");
        }

        printf("\nEstado atual do tabuleiro:\n");
        mostrarTabela(game.tab);
    }

    // Liberta a memória alocada para a lista de tabuleiros
    if (game.stackTabs != NULL)
        freeStackTabs(game.stackTabs);

    // Liberta a memória alocada para o tabuleiro
    if (game.tab != NULL)
        freeTabela(game.tab);

    return 0;
}