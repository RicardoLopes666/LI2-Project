<<<<<<< HEAD
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
=======
#ifndef TESTES_PARTE4_H
#define TESTES_PARTE4_H

#include "parte4.h"
#include <CUnit/Basic.h>

/* Declarações das funções de teste */
void test_verificaSeQuebraCaminho(void);
void test_riscaLetrasRepetidas(void);
void test_pintaVizinhosDeRiscadas(void);
void test_aplicaA(void);
void test_comandoA(void);
void test_trataAA_A_NasLinhas(void);
void test_trataAA_A_NasColunas(void);
void test_riscaABA(void);
void test_jogoResolvido(void);

#endif /* TESTES_PARTE4_H */
>>>>>>> main
