#ifndef PARTE1_H
#define PARTE1_H

#include <stdbool.h>
#include "../tipos.h"

// Constantes
#define LINE_SIZE 1024

// Tipo de funções
typedef bool (*COMANDO)(char cmd, char *arg, GAME *game);

// Declarações de funções
void initTabela(TABELA t, int l, int c);
void freeTabela(TABELA t);
bool gravar(char cmd, char *arg, GAME *game);
bool sair(char cmd, char *arg, GAME *game);
void colocaSolucao(GAME *game);
bool lerCmd(char cmd, char *arg, GAME *game);
bool coordenadaParaIndice(const char *coord, int *linha, int *coluna);
bool pintarBranco(TABELA t, int linha, int coluna, TABELA p);
bool riscar(TABELA t, int linha, int coluna);
void mostrarTabela(GAME game, int isC);
void i(GAME *game, bool *comandoProcessado);

#endif // PARTE1_H