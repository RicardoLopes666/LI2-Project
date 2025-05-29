#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../tipos.h"
#include <ctype.h> // Para toupper
#include "../parte2/parte2.h"
#include "../colors.h"
#define LINE_SIZE 1024

// Função que cria e aloca o tabuleiro com as dimensões fornecidas
void initTabela(TABELA t, int l, int c)
{
    t->l = l;
    t->c = c;
    t->tabela = malloc(l * sizeof(char *));
    for (int i = 0; i < l; i++)
    {
        t->tabela[i] = malloc((c + 1) * sizeof(char)); // + 1 para colocar o '\0'
        for (int j = 0; j <= c; j++)
        {
            if (j == c)
                t->tabela[i][j] = '\0';
            else
                t->tabela[i][j] = ' '; // Inicializa com espaços
        }
    }
}

// Função para libertar a memória alocada para um tabuleiro
void freeTabela(TABELA t)
{
    if (t == NULL)
        return;
    for (int i = 0; i < t->l; i++)
    {
        free(t->tabela[i]);
    }
    free(t->tabela);
    free(t);
}

// Tipo de função para comandos. Agora os comandos recebem um GAME* para acessar o estado e o tabuleiro.
typedef bool (*COMANDO)(char cmd, char *arg, GAME *game);

// Função responsável por escrever no ficheiro o conteúdo de uma tabela
void escreveTabela(TABELA tabela, FILE *f)
{
    fprintf(f, "%d %d\n", tabela->l, tabela->c);
    for (int i = 0; i < tabela->l; i++)
    {
        fprintf(f, "%s\n", tabela->tabela[i]);
    }
    putchar('\n');
}

// Comando para gravar o estado do jogo num ficheiro
bool gravar(char cmd, char *arg, GAME *game)
{
    if (cmd == 'g')
    {
        if (arg == NULL)
        {
            fprintf(stderr, "Erro: o comando gravar precisa de um argumento!\n");
            return false;
        }
        if (game->tab == NULL)
        {
            fprintf(stderr, "Erro: a tabela não deve ser nula antes de a gravar!\n");
            return false;
        }
        FILE *f = fopen(arg, "w"); // Abre o ficheiro (caso existe o que estiver lá escrito vai ser sobrescrito)
        if (f == NULL)
        {
            fprintf(stderr, "Erro a abrir o ficheiro");
            return false;
        }
        for (int i = 0; i < game->stackTabs->comprimento; i++)
        {
            escreveTabela(game->stackTabs->tabelas[i], f);
        }
        fclose(f);
        printf("Gravado em %s\n", arg);
        return true;
    }

    return false;
}

// Comando para sair
bool sair(char cmd, char *arg, GAME *game)
{
    if (arg != NULL)
        return false;
    if (cmd == 's')
    {
        game->estado.looping = false;
        return true;
    }
    return false;
}

bool leTabuleiro(TABELA *t, int linhas, int colunas, FILE *file)
{
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            if (fscanf(file, " %c", &(*t)->tabela[i][j]) != 1)
            {
                fprintf(stderr, "Erro: na leitura do conteúdo do tabuleiro\n");
                return false;
            }
        }
    }
    return true;
}

bool colocaTabelaNaStack(GAME *game, TABELA tabela)
{
    if (!insereTabela(game->stackTabs, tabela))
    {
        if (tabela)
            freeTabela(tabela);
        freeStackTabs(game->stackTabs);
        fprintf(stderr, "Erro: na criação da stack de tabuleiros");
        return false;
    }
    return true;
}

// Comando para ler o tabuleiro de um ficheiro
bool lerCmd(char cmd, char *arg, GAME *game)
{
    if (cmd != 'l' || arg == NULL)
        return false;

    FILE *file = fopen(arg, "r"); // r é para leitura
    if (!file)
    {
        fprintf(stderr, "Erro: ao abrir o ficheiro %s para leitura\n", arg);
        return false;
    }

    freeStackTabs(game->stackTabs);
    STACKTABS s = malloc(sizeof(struct StackTabs));
    if (!initStackTabs(s))
    {
        free(s);
        return false;
    }
    game->stackTabs = s;

    int linhas, colunas;
    // Loop que le os vários tabuleiros enquanto o ficheiro não estiver vazio e coloca-os da stackTabs
    while (fscanf(file, "%d %d", &linhas, &colunas) == 2)
    {
        TABELA t = malloc(sizeof(struct Tabela));

        initTabela(t, linhas, colunas);

        // Lê o conteúdo do tabuleiro do ficheiro e passa-o para a tabela t
        if (!leTabuleiro(&t, linhas, colunas, file) || !colocaTabelaNaStack(game, t))
        {
            freeTabela(t);
            freeStackTabs(s);
            fclose(file);
            return false;
        }
    }
    if (game->stackTabs->comprimento == 0)
    {
        printf("Erro: tabuleiro não contem nenhum tabuleiro");
        fclose(file);
        return false;
    }

    fclose(file);
    if (game->tab != NULL)
        freeTabela(game->tab);
    TABELA temp = copiarTabela(game->stackTabs->tabelas[game->stackTabs->comprimento - 1]);
    if (temp != NULL)
        game->tab = temp;
    else
        return false;
    return true;
}

// Converte coordenada do formato "a3" para  coordenadas (linha e coluna)
bool coordenadaParaIndice(const char *coord, int *linha, int *coluna)
{
    if (strlen(coord) < 2)
        return false;
    *coluna = coord[0] - 'a';     // 'a' corresponde à coluna 0
    *linha = atoi(coord + 1) - 1; // atoi (ascii to integer):  atoi -> converte uma string para um int   NOTA (coord + 1) é para comecar na segunda letra da string, e tirmaos 1, por causa do indice zero
    return true;
}

// Pinta de branco (transforma para maiuscula)
bool pintarBranco(TABELA t, int linha, int coluna)
{
    if (linha >= 0 && linha < t->l && coluna >= 0 && coluna < t->c)
    {
        t->tabela[linha][coluna] = toupper(t->tabela[linha][coluna]);
        return true;
    }
    return false;
}

// Risca a casa (substitui por '#')
bool riscar(TABELA t, int linha, int coluna)
{
    if (linha < 0 || linha >= t->l || coluna < 0 || coluna >= t->c)
        return false;

    t->tabela[linha][coluna] = '#'; // Marca a célula com '#'
    return true;
}

// Função que mostra o tabuleiro
void mostrarTabela(TABELA t)
{
    printf("\n%sEstado atual do tabuleiro:%s\n", COMMAND_COLOR, RESET);
    if (t == NULL)
    {
        printf("%sTabuleiro não inicializado.%s\n", ERROR_COLOR, RESET);
        return;
    }

    // Print column headers (a, b, c, ...)
    printf("  ");
    for (int j = 0; j < t->c; j++)
    {
        printf("%s%c%s ", COORDINATE_COLOR, 'a' + j, RESET);
    }
    printf("\n");

    for (int i = 0; i < t->l; i++)
    {
        // Print row number
        printf("%s%d%s ", COORDINATE_COLOR, i + 1, RESET);

        for (int j = 0; j < t->c; j++)
        {
            char cell = t->tabela[i][j];
            if (cell == '#')
            {
                printf("%s%c%s ", HASHED_CELL, cell, RESET);
            }
            else if (isupper(cell))
            {
                // Create a square background for uppercase letters
                printf("%s[%c]%s", CAPITAL_LETTER, cell, RESET);
            }
            else
            {
                printf("%s%c%s ", NORMAL_LETTER, cell, RESET);
            }
        }
        printf("\n");
    }
    printf("\n");
}
