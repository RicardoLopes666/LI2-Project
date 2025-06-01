#ifndef PARTE2_H
#define PARTE2_H

#include <stdbool.h>
#include "../tipos.h"
#include "../parte1/parte1.h"
#include "../parte3/parte3.h"

// Funções para voltar atrás -> comando 'd'
bool initStackTabs(STACKTABS s);
bool insereTabela(STACKTABS s, TABELA tab);
TABELA copiarTabela(TABELA t);
bool deleteTabela(GAME *game);
void freeStackTabs(STACKTABS s);
void d(GAME *game, bool *comandoProcessado);
void u(GAME *game, bool *comandoProcessado);

// Funções para verificar as restrições -> comando 'v'
bool dentroDosLimites(TABELA t, int linha, int coluna);
int verificaRiscadaVizinhasBrancas(TABELA t, int linha, int coluna, int restricoes[][2]);
bool verificaLetraIgualLinhaColuna(TABELA t, int linha, int coluna);
void verificaRiscadasERepetidas(TABELA t, int i, int j, int *contaRestricoes, bool *temRestricoes, bool escreve);
bool verificaRestricoes(TABELA t, bool escreve);
void v(GAME game, bool *continuar, bool *comandoProcessado);

#endif // PARTE2_H
