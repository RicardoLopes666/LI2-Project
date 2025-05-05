#ifndef PARTE4_H
#define PARTE4_H

#include <stdbool.h>

typedef struct Tabela
{
    int l;         // Número de linhas
    int c;         // Número de colunas
    char **tabela; // Tabela de caracteres
} *TABELA;

// Funções
void verificaSeQuebraCaminho(TABELA t, int linha, int coluna, bool *changed, bool restricao);
void riscaLetrasRepetidas(TABELA t, TABELA aux, int linha, int coluna, bool muda, bool *changed);
void pintaVizinhosDeRiscadas(TABELA t, TABELA aux, int linha, int coluna, bool muda, bool *changed);
int aplicaA(TABELA *t);
void comandoA(TABELA *t, bool *continuar);
void trataAA_A_NasLinhas(TABELA t);
void trataAA_A_NasColunas(TABELA t);
void riscaABA(TABELA t);
bool jogoResolvido(TABELA t);

#endif // PARTE4_H
