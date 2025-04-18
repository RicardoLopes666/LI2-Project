#ifndef TESTES_PARTE2_H
#define TESTES_PARTE2_H

#include "parte2.h"
#include <CUnit/Basic.h>

// Função auxiliar
TABELA criarTabela(int l, int c);

// Funções de teste
void test_initStackTabs(void);
void test_insereTabela(void);
void test_deleteTabela(void);
void test_copiarTabela(void);
void test_freeStackTabs(void);
void test_dentroDosLimites(void);
void test_verificaRiscadaVizinhasBrancas(void);
void test_verificaRestrições(void);

#endif // TESTES_PARTE2_H
