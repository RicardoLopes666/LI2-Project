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
bool leTabuleiro(TABELA *t, int linhas, int colunas, FILE *file);
bool colocaTabelaNaStack(GAME *game, TABELA tabela);
void colocaSolucao(GAME *game);
bool coordenadaParaIndice(const char *coord, int *linha, int *coluna);
bool pintarBranco(TABELA t, int linha, int coluna, TABELA p);
bool riscar(TABELA t, int linha, int coluna);
bool gravar(char cmd, char *arg, GAME *game);
bool sair(char cmd, char *arg, GAME *game);
void leTabuleirosDoFicheiro(int *linhas, int *colunas, FILE *file, GAME *game, STACKTABS s);
bool lerCmd(char cmd, char *arg, GAME *game);
void desenhaConteudo(TABELA t, bool isC, TABELA s);
void mostrarTabela(GAME game, int isC);
void i(GAME *game, bool *comandoProcessado);
void escreveEstatisticas(int percentagem, GAME g, int resolvidas, int total, int corretas, bool *comandoProcessado, int b, int r);
void e(GAME g, bool *comandoProcessado);

#endif // PARTE1_H