#ifndef PARTE2_H
#define PARTE2_H

#include <stdbool.h>
#include "tipos.h"

// Função auxiliar para criar uma tabela
TABELA criarTabela(int l, int c);

// Funções utilizadas para voltar atrás -> comando 'd'
bool initStackTabs(STACKTABS s);
bool insereTabela(STACKTABS s, TABELA tab);
TABELA copiarTabela(TABELA t);
bool deleteTabela(GAME *game);
void freeStackTabs(STACKTABS s);
void d(GAME *game, bool *comandoProcessado);

// Funções utilizadas para verificar as restrições -> comando 'v'
bool dentroDosLimites(TABELA t, int linha, int coluna);
int verificaRiscadaVizinhasBrancas(TABELA t, int linha, int coluna, int restricoes[][2]);
bool verificaLetraIgualLinhaColuna(TABELA t, int linha, int coluna);
void verificaRiscadasERepetidas(TABELA t, int i, int j, int *contaRestricoes, bool *temRestricoes, bool escreve);
bool verificaRestricoes(TABELA t, bool escreve);
void v(GAME game, bool *continuar, bool *comandoProcessado);

// Testes
void test_initStackTabs();
void test_insereTabela();
void test_copiarTabela();
void test_deleteTabela();
void test_freeStackTabs();
void test_dentroDosLimites();
void test_verificaRiscadaVizinhasBrancas();
void test_verificaLetraIgualLinhaColuna();
void test_verificaRestricoes();

#endif // PARTE2_H
