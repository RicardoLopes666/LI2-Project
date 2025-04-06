#ifndef PARTE1_H
#define PARTE1_H

#include <stdbool.h> // Para o tipo bool
#include <stdio.h>   // Para printf

// Estrutura que representa o tabuleiro
typedef struct Tabela
{
    int l;         // Número de linhas
    int c;         // Número de colunas
    char **tabela; // Ponteiro para o tabuleiro (array bidimensional)
} *TABELA;

// Funções para manipular o tabuleiro
void initTabela(TABELA t, int l, int c);
void freeTabela(TABELA t);
TABELA lerFicheiro(const char *nome);
bool pintarBranco(TABELA t, int linha, int coluna);
bool riscar(TABELA t, int linha, int coluna);
void mostrarTabela(TABELA t);

#endif // PARTE1_H