#ifndef TESTES_PARTE1_H
#define TESTES_PARTE1_H

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parte1.h" // Certifica-te de que este header contém as declarações das funções e tipos usados
#include "../parte2/parte2.h"

#define LINE_SIZE 1024

// Declaração das funções utilizadas nos testes
void test_pintarBranco(void);
void test_riscar(void);
void test_mostrarTabela(void);
void test_coordenadaParaIndice(void);
void test_sair(void);
void test_gravar(void);
void test_lerCmd(void);

// Definições dos tipos de dados usados
typedef struct Tabela
{
    int l;               // número de linhas
    int c;               // número de colunas
    char tabela[10][10]; // tamanho máximo do tabuleiro
} *TABELA;

typedef struct Estado
{
    int looping; // Flag para controle do loop principal
} Estado;

typedef struct StackTabs
{
    int comprimento; // número de elementos na pilha
    TABELA *tab;     // ponteiro para as tabelas na pilha
} StackTabs;

typedef struct Game
{
    TABELA tab;           // Tabuleiro atual
    Estado estado;        // Estado do jogo
    StackTabs *stackTabs; // Pilha de tabuleiros
} GAME;

// Declaração das funções manipuladoras do tabuleiro
void initTabela(TABELA t, int linhas, int colunas);
void freeTabela(TABELA t);
int pintarBranco(TABELA t, int linha, int coluna);
int riscar(TABELA t, int linha, int coluna);
void mostrarTabela(TABELA t);
int coordenadaParaIndice(const char *coordenada, int *linha, int *coluna);
int sair(char cmd, const char *argumento, GAME *game);
int gravar(char cmd, const char *filename, GAME *game);
int lerCmd(char cmd, const char *filename, GAME *game);
int initStackTabs(StackTabs *stackTabs);
int insereTabela(StackTabs *stackTabs, TABELA t);
TABELA copiarTabela(TABELA t);
void freeStackTabs(StackTabs *stackTabs);

#endif // TESTES_PARTE1_H
