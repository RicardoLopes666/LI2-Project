#ifndef PARTE4_H
#define PARTE4_H

#include <stdbool.h>
#include "../tipos.h"

// ————— Funções do comando de ajuda (‘a’) —————
void verificaSeQuebraCaminho(TABELA aux, int l, int c, bool *changed, bool escreve);
void riscaLetrasRepetidas(TABELA t, TABELA aux, int i, int j, bool escreve, bool *changed);
void pintaVizinhosDeRiscadas(TABELA t, TABELA aux, int i, int j, bool escreve, bool *changed);
TABELA ajuda(TABELA t, bool escreve, bool *changed);
int aplicaA(TABELA *aux);
void comandoA(TABELA *aux, bool *continuar);

// ————— Funções do comando de riscar (‘R’) —————
void trataAA_A_NasLinhas(TABELA aux);
void trataAA_A_NasColunas(TABELA aux);
void risca_AA_A(TABELA aux);
void trataABA_linhas(TABELA aux);
void trataABA_colunas(TABELA aux);
void riscaABA(TABELA aux);

// ————— Funções de verificação/resolução —————
int existemMinusculas(TABELA t);
int jogoResolvido(TABELA aux);
void tentaRiscarColunas(int l, int c1, int c2, TABELA *t, bool *continuar);
bool tentaColunas(TABELA *t);
void tentaRiscarLinhas(int c, int l1, int l2, TABELA *t, bool *continuar);
bool tentaLinhas(TABELA *t);
TABELA resolve(TABELA t);

#endif /* PARTE4_H */
