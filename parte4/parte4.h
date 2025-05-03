#ifndef PARTE4_H
#define PARTE4_H

#include <stdbool.h> // Para usar o tipo bool

#include "../tipos.h" // Para o tipo TABELA

// Funções declaradas em parte3.c

void verificaSeQuebraCaminho(TABELA aux, int l, int c, bool *changed, bool escreve);
TABELA ajuda(TABELA t, bool escreve, bool *changed);
void trataAA_A_NasLinhas(TABELA aux);
void trataAA_A_NasColunas(TABELA aux);
void risca_AA_A(TABELA aux);
void trataABA_linhas(TABELA aux);
void trataABA_colunas(TABELA aux);
void riscaABA(TABELA aux);
int aplicaA(TABELA *aux);
void comandoA(TABELA *aux, bool *continuar);
int existemMinusculas(TABELA t);
int jogoResolvido(TABELA aux);
void tentaRiscarColunas(int l, int c1, int c2, TABELA *t, bool *continuar);
bool tentaColunas(TABELA *t);
void tentaRiscarLinhas(int c, int l1, int l2, TABELA *t, bool *continuar);
bool tentaLinhas(TABELA *t);
TABELA resolve(TABELA t);

#endif // PARTE4_H
