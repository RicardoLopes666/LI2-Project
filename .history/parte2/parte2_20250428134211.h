#ifndef PARTE2_H
#define PARTE2_H
#include "../tipos.h"
#include <stdbool.h>

bool initStackTabs(STACKTABS s);

bool insereTabela(STACKTABS s, TABELA tab);

bool deleteTabela(GAME *game);

TABELA copiarTabela(TABELA t);

void freeTabela(TABELA t);

bool verificaLetraIgualLinhaColuna(TABELA t, int linha, int coluna);

void freeStackTabs(STACKTABS s);

bool dentroDosLimites(TABELA t, int linha, int coluna);

int verificaRiscadaVizinhasBrancas(TABELA t, int linha, int coluna, int restricoes[][2]);

bool verificaRestricoes(TABELA t, bool escreve);

#endif // PARTE2_H
