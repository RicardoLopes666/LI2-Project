#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../tipos.h"
#include <ctype.h> // Para toupper
#include "../parte2/parte2.h"
#include "../parte4/parte4.h"
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
            fprintf(stderr, "%sErro: o comando gravar precisa de um argumento!%s\n", ERROR_COLOR, RESET);
            return false;
        }
        if (game->tab == NULL)
        {
            fprintf(stderr, "%sErro: a tabela não deve ser nula antes de a gravar!%s\n", ERROR_COLOR, RESET);
            return false;
        }
        FILE *f = fopen(arg, "w"); // Abre o ficheiro (caso existe o que estiver lá escrito vai ser sobrescrito)
        if (f == NULL)
        {
            fprintf(stderr, "%sErro: a abrir o ficheiro%s\n", ERROR_COLOR, RESET);
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
                fprintf(stderr, "%sErro: na leitura do conteúdo do tabuleiro%s\n", ERROR_COLOR, RESET);
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
        fprintf(stderr, "%sErro: na criação da stack de tabuleiros%s\n", ERROR_COLOR, RESET);
        return false;
    }
    return true;
}

void colocaSolucao(GAME *game)
{
    TABELA aux = resolve(game->stackTabs->tabelas[0]); // Recebe a primeira tabela carregada (em principio so com letras minusculas)

    TABELA temp = game->solution;
    game->solution = aux;
    if (temp != NULL)
        freeTabela(temp);
}
bool leTabuleirosDoFicheiro(int *linhas, int *colunas, FILE *file, GAME *game, STACKTABS s)
{
    while (fscanf(file, "%d %d", linhas, colunas) == 2)
    {
        TABELA t = malloc(sizeof(struct Tabela));

        initTabela(t, *linhas, *colunas);

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
        printf("%sErro: o ficheiro não contem nenhum tabuleiro%s", ERROR_COLOR, RESET);
        fclose(file);
        return false;
    }
}

// Comando para ler o tabuleiro de um ficheiro
bool lerCmd(char cmd, char *arg, GAME *game)
{
    if (cmd != 'l' || arg == NULL)
        return false;

    FILE *file = fopen(arg, "r"); // r é para leitura
    if (!file)
    {
        fprintf(stderr, "%sErro: ao abrir o ficheiro %s para leitura%s\n", ERROR_COLOR, arg, RESET);
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
    // Loop que le os vários tabuleiros enquanto o ficheiro não estiver vazio e coloca-os na stackTabs

    if (!leTabuleirosDoFicheiro(&linhas, &colunas, file, game, s))
        return false;

    fclose(file);
    if (game->tab != NULL)
        freeTabela(game->tab);
    TABELA temp = copiarTabela(game->stackTabs->tabelas[game->stackTabs->comprimento - 1]);

    if (temp != NULL)
        game->tab = temp;
    else
        return false;
    colocaSolucao(game);

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
bool pintarBranco(TABELA t, int linha, int coluna, TABELA p) // p é a primeira tabela carregada
{
    if (linha >= 0 && linha < t->l && coluna >= 0 && coluna < t->c)
    {
        if (t->tabela[linha][coluna] != '#')
            t->tabela[linha][coluna] = toupper(t->tabela[linha][coluna]);
        else
            t->tabela[linha][coluna] = toupper(p->tabela[linha][coluna]);
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

// Função responsavél por desenhar a parte interior da tabela
void desenhaConteudo(TABELA t, bool isC, TABELA s)
{
    for (int i = 0; i < t->l; i++)
    {
        // Escreve o numero da linha
        printf("%s%2d%s ┃", COORDINATE_COLOR, i + 1, RESET);

        for (int j = 0; j < t->c; j++)
        {
            char cell = t->tabela[i][j];
            int igual = t->tabela[i][j] == s->tabela[i][j];
            if (cell == '#')
            {
                isC ? printf("%s # %s", igual ? CAPITAL_GREEN : CAPITAL_RED, RESET) : printf("%s # %s", HASHED_CELL, RESET);
            }
            else if (isupper(cell))
            {
                isC ? printf("%s[%c]%s", igual ? CAPITAL_GREEN : CAPITAL_RED, cell, RESET) : printf("%s[%c]%s", CAPITAL_LETTER, cell, RESET);
            }
            else
                printf(" %s%c%s ", NORMAL_LETTER, cell, RESET);
            printf("┃");
        }
        printf("\n");

        if (i < t->l - 1)
        {
            // Desenha linha entre as diferentes linhas do tabuleiro
            printf("   ┣");
            for (int j = 0; j < t->c; j++)
                printf("━━━%s", (j == t->c - 1) ? "┫\n" : "╋");
        }
    }
}

// Função que mostra o tabuleiro
void mostrarTabela(GAME game, int isC) // isC é uma flag que se for positiva quer dizer que estou a invocar o comando c
{
    TABELA t = game.tab;
    TABELA s = game.solution;
    if (!isC)
        printf("\n%sEstado atual do tabuleiro:%s\n", COMMAND_COLOR, RESET);
    if (t == NULL)
    {
        printf("%sTabuleiro não inicializado.%s\n", ERROR_COLOR, RESET);
        return;
    }

    // Desenha os headers (a, b, c, ..)
    printf("    ");
    for (int j = 0; j < t->c; j++)
        printf(" %s%c%s  ", COORDINATE_COLOR, 'a' + j, RESET);
    printf("\n");

    // Desenha a primeira linha da tabela
    printf("   ┏");
    for (int j = 0; j < t->c; j++)
        printf("━━━%s", (j == t->c - 1) ? "┓\n" : "┳");

    // Função responsavél por desenhar a parte interior da tabela
    desenhaConteudo(t, isC, s);

    // Desenha a linha inferior
    printf("   ┗");
    for (int j = 0; j < t->c; j++)
        printf("━━━%s", (j == t->c - 1) ? "┛\n" : "┻");
    printf("\n");
}

void i(GAME *game, bool *comandoProcessado)
{
    TABELA new = copiarTabela(game->stackTabs->tabelas[0]);
    if (new)
    {
        TABELA temp = game->tab;
        game->tab = new;
        freeTabela(temp);
    }
    *comandoProcessado = true;
}

void escreveEstatisticas(float percentagem, GAME g, int resolvidas, int total, int corretas, bool *comandoProcessado, int b, int r)
{
    printf("\n%s=== ESTATÍSTICAS DO JOGO ===%s\n", HINT_COLOR, RESET);

    printf("\n%sEstado do Tabuleiro%s\n", BLUE, RESET);
    printf("  Dimensões: %dx%d (%d casas)\n", g.tab->l, g.tab->c, total);
    printf("  Casas resolvidas: %d/%d (%.1f%%)\n", resolvidas, total, percentagem);
    printf("  Casas por resolver: %d\n", total - resolvidas);
    printf("  Casas corretamente colocadas: %d (para mais informações use o comando 'c')\n", corretas);

    printf("\n%sDistribuição:%s\n", BLUE, RESET);
    printf("  Casas brancas: %d\n", b);
    printf("  Casas riscadas: %d\n", r);

    bool temResticoes = verificaRestricoes(g.tab, false);

    printf("\n%sValidação:%s\n", BLUE, RESET);
    if (!temResticoes)
        printf("  Estado: Válido\n");
    else
        printf("  Foram encontradas violações das regras (para mais informações utilize o comando 'v')\n");

    *comandoProcessado = true;
}

void e(GAME g, bool *comandoProcessado)
{
    int total = g.tab->c * g.tab->l;
    int corretas = 0;
    int r = 0, b = 0;
    for (int i = 0; i < g.tab->l; i++)
    {
        for (int j = 0; j < g.tab->c; j++)
        {
            int cel = g.tab->tabela[i][j];
            if (!islower(cel))
            {
                if (cel == g.solution->tabela[i][j])
                    corretas++;
                if (isupper(cel))
                    b++;
                else if (cel == '#')
                    r++;
            }
        }
    }

    int resolvidas = r + b;

    float percentagem = ((float)resolvidas / (float)total) * (float)100;

    escreveEstatisticas(percentagem, g, resolvidas, total, corretas, comandoProcessado, b, r);
}