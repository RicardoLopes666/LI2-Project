#ifndef TESTESPARTE2_H
#define TESTESPARTE2_H

#include "../tipos.h"

// Função auxiliar para criar uma tabela para testes
TABELA criarTabela(int l, int c);

// Testes das funções da parte 2
void test_initStackTabs(void);
void test_insereTabela(void);
void test_copiarTabela(void);
void test_deleteTabela(void);
void test_freeStackTabs(void);
void test_dentroDosLimites(void);
void test_verificaRiscadaVizinhasBrancas(void);
void test_verificaLetraIgualLinhaColuna(void);
void test_verificaRestricoes(void);

#endif // TESTESPARTE2_H