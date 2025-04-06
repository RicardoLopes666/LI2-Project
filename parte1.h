#ifndef PARTE1_H
#define PARTE1_H

#include <stdbool.h>

// Constantes
#define LINE_SIZE 1024

// Struct para controlar o estado do programa (se ele deve continuar ou parar)
typedef struct estado
{
    bool looping;
} ESTADO;

// Struct que representa o tabuleiro
typedef struct Tabela
{
    int l;
    int c;
    char **tabela;
} *TABELA;

// Estrutura que agrupa o estado do jogo e o tabuleiro
typedef struct
{
    ESTADO estado;
    TABELA tab;
} GAME;

// Declarações de funções
void initTabela(TABELA t, int l, int c);
void freeTabela(TABELA t);
TABELA lerFicheiro(const char *nome);
bool gravar(char cmd, char *arg, GAME *game);
bool sair(char cmd, char *arg, GAME *game);
bool lerCmd(char cmd, char *arg, GAME *game);
bool coordenadaParaIndice(const char *coord, int *linha, int *coluna);
bool pintarBranco(TABELA t, int linha, int coluna);
bool riscar(TABELA t, int linha, int coluna);
void mostrarTabela(TABELA t);

#endif // PARTE1_H