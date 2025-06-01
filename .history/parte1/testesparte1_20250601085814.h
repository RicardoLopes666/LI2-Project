#ifndef TESTES_PARTE1_H
#define TESTES_PARTE1_H

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../tipos.h"
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

#endif // TESTES_PARTE1_H
