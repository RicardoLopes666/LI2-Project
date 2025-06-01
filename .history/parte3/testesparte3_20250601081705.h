#ifndef TESTES_PARTE3_H
#define TESTES_PARTE3_H

#include <stdbool.h>
#include <stdio.h>
#include "../tipos.h"

// Funções auxiliares
bool dentroDosLimites(TABELA t, int linha, int coluna);
TABELA criarTabela(int l, int c);
void freeTabela(TABELA t);

// Funções de captura de output (stdout)
void _begin_capture(void);
void _end_capture(void);

// Funções de teste (para CUnit)
void test_devolvePrimeiroNRiscado_tudo_riscado(void);
void test_devolvePrimeiroNRiscado_encontra(void);
void test_initVisited(void);
void test_queue_insert_delete(void);
void test_devolveNaoVisitados(void);
void test_existeCaminhoOrtogonal_simples(void);
void test_trataCaminhoOrtogonal_erro(void);
void test_trataCaminhoOrtogonal_ok(void);

#endif // TESTES_PARTE3_H
