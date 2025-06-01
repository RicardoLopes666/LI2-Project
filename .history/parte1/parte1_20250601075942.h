#ifndef PARTE1_H
#define PARTE1_H

#include <stdbool.h>
#include <stdio.h>
#include "../tipos.h"

// Tipo de função para comandos
typedef bool (*COMANDO)(char cmd, char *arg, GAME *game);

// Funções de manipulação de tabuleiros
void initTabela(TABELA t, int l, int c);
void freeTabela(TABELA t);
void escreveTabela(TABELA tabela, FILE *f);
void mostrarTabela(TABELA t);
bool leTabuleiro(TABELA *t, int linhas, int colunas, FILE *file);
bool colocaTabelaNaStack(GAME *game, TABELA tabela);
bool gravar(char cmd, char *arg, GAME *game);
bool sair(char cmd, char *arg, GAME *game);
void colocaSolucao(GAME *game);
bool lerCmd(char cmd, char *arg, GAME *game);
bool coordenadaParaIndice(const char *coord, int *linha, int *coluna);
bool pintarBranco(TABELA t, int linha, int coluna, TABELA p);
bool riscar(TABELA t, int linha, int coluna);
<<<<<<< HEAD

// Comandos
bool gravar(char cmd, char *arg, GAME *game);
bool sair(char cmd, char *arg, GAME *game);
bool lerCmd(char cmd, char *arg, GAME *game);
=======
void mostrarTabela(GAME game, int isC);
<<<<<<< HEAD
void i(GAME *game, bool *comandoProcessado);
void e(GAME g, bool *comandoProcessado);
=======
>>>>>>> main
>>>>>>> c47ba4a83e1231ed4058a89d1452409c5c7536f5

#endif // PARTE1_H