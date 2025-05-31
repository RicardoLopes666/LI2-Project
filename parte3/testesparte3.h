<<<<<<< HEAD
#ifndef TESTES_CAMINHO_ORTOGONAL_H
#define TESTES_CAMINHO_ORTOGONAL_H

#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "parte3.h"
#include "../parte2/parte2.h"
#include "../parte2/testesparte2.h"

// Função de teste para a função devolvePrimeroNRiscado
void test_devolvePrimeroNRiscado();

// Função de teste para a função initVisited
void test_initVisited();

// Função de teste para a função insertQueue
void test_insertQueue();

// Função de teste para a função deleteQueue
void test_deleteQueue();

// Função de teste para a função existeCaminhoOrtogonal
void test_existeCaminhoOrtogonal();

#endif // TESTES_CAMINHO_ORTOGONAL_H
=======
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
>>>>>>> main
