// bfs.h

#ifndef BFS_H
#define BFS_H

#include "../tipos.h"

// Função que devolve a primeira coordenada não riscada
COORDENADA devolvePrimeroNRiscado(TABELA t);

// Inicializa o array de visitados e conta as casas riscas
int **initVisited(TABELA t, int *count);

// Inicializa a queue
void initQueue(CQUEUE q);

// Insere um array de coordenadas na queue
int insertQueue(CQUEUE q, COORDENADA *coordenadas, int coordSize);

// Remove o primeiro elemento da queue
void deleteQueue(CQUEUE q, COORDENADA *pos);

// Verifica se existe um caminho ortogonal entre casas válidas
int existeCaminhoOrtogonal(CQUEUE q, TABELA t, int **visited, int *count);

// Trata do imprimir no terminal a restrição
bool trataCaminhoOrtogonal(TABELA t, int *contaRestricoes, bool *temRestrições, bool escreve);

#endif
