#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> // Para toupper
#define LINE_SIZE 1024

// Struct para controlar o estado do programa (se ele deve continuar ou parar)
typedef struct estado
{
    bool looping;
} ESTADO;

// Struct que representa o tabuleiro
typedef struct Tabela
{
    int l;         // Número de linhas
    int c;         // Número de colunas
    char **tabela; // Ponteiro para o tabuleiro (array bidimensional)
} *TABELA;

// Estrutura que agrupa o estado do jogo e o tabuleiro
typedef struct
{
    ESTADO estado; // Estado do programa
    TABELA tab;    // Tabuleiro do jogo
} GAME;

// Função que cria e aloca o tabuleiro com as dimensões fornecidas
void initTabela(TABELA t, int l, int c)
{
    t->l = l;
    t->c = c;
    t->tabela = malloc(l * sizeof(char *));
    for (int i = 0; i < l; i++)
    {
        t->tabela[i] = malloc((c + 1) * sizeof(char));
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
            fprintf(stderr, "Erro: a tabela não ser nula antes de a gravar!\n");
            return false;
        }
        FILE *f = fopen(arg, "w"); // Abre o ficheiro (caso existe o que estiver lá escrito vai ser sobrescrito)
        if (f == NULL)
        {
            fprintf(stderr, "Erro a abrir o ficheiro");
            return false;
        }
        fprintf(f, "%d %d\n", game->tab->l, game->tab->c);
        for (int i = 0; i < game->tab->l; i++)
        {
            fprintf(f, "%s\n", game->tab->tabela[i]);
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

// Comando para ler o tabuleiro de um ficheiro
bool lerCmd(char cmd, char *arg, GAME *game)
{
    if (cmd != 'l' || arg == NULL)
        return false;

    FILE *file = fopen(arg, "r"); // r é para leitura
    if (!file)
    {
        fprintf(stderr, "Erro ao abrir o ficheiro %s para leitura\n", arg);
        return false;
    }

    int linhas, colunas;
    // Tenta ler as dimensões do tabuleiro
    if (fscanf(file, "%d %d", &linhas, &colunas) != 2)
    {
        fprintf(stderr, "Erro na leitura das dimensões do tabuleiro\n");
        fclose(file);
        return false;
    }

    TABELA t = malloc(sizeof(struct Tabela));
    if (!t)
    {
        fprintf(stderr, "Erro ao alocar memória para o tabuleiro\n");
        fclose(file);
        return false;
    }

    initTabela(t, linhas, colunas);

    // Lê o conteúdo do tabuleiro do ficheiro e passa-o para t
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            if (fscanf(file, " %c", &t->tabela[i][j]) != 1)
            {
                fprintf(stderr, "Erro na leitura do conteúdo do tabuleiro\n");
                freeTabela(t);
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);
    game->tab = t;
    return true;
}

// Converte coordenada do formato "a3" para índices numéricos (linha e coluna)
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
    if (t == NULL)
    {
        printf("Tabuleiro não inicializado.\n");
        return;
    }
    for (int i = 0; i < t->l; i++)
    {
        for (int j = 0; j < t->c; j++)
        {
            printf("%c ", t->tabela[i][j]);
        }
        printf("\n");
    }
}
