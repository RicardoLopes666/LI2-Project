#ifndef PARTE2_H
#define PARTE2_H
#include "../tipos.h"

void initStackTabs(STACKTABS s);

int insereTabela(STACKTABS s, TABELA tab);

int deleteTabela(GAME *game);

TABELA copiarTabela(TABELA t);

void freeStackTabs(STACKTABS s);

#endif // PARTE2_H