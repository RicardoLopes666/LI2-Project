#ifndef CAMINHO_ORTOGONAL_H
#define CAMINHO_ORTOGONAL_H

#include <stdbool.h>
#include "../tipos.h"
#include "../parte2/parte2.h"

// Função que devolve a primeira coordenada não riscada em uma tabela
COORDENADA devolvePrimeroNRiscado(TABELA t);

// Função que inicializa o array de visitados, com 0 para não visitado e 1 para visitado
int **initVisited(TABELA t, int *count);

// Função que inicializa a fila (queue)
void initQueue(CQUEUE q);

// Função para inserir coordenadas na fila
int insertQueue(CQUEUE q, COORDENADA *coordenadas, int coordSize);

// Função que remove o elemento da frente da fila
void deleteQueue(CQUEUE q, COORDENADA *pos);

// Função que devolve um array de coordenadas não visitadas
COORDENADA *devolveNaoVisitados(int **visited, int l, int c, int length);

// Algoritmo de busca em largura para verificar caminho ortogonal
int existeCaminhoOrtogonal(CQUEUE q, TABELA t, int **visited, int *count);

// Função que escreve as restrições encontradas quando não há caminho ortogonal
void escreveInexistenciaCaminho(bool *temRestricoes, bool escreve, int *contaRestricoes, TABELA t, int **visited, int colunaI, int linhaI, int count);

// Função que trata da verificação do caminho ortogonal
bool trataCaminhoOrtogonal(TABELA t, int *contaRestricoes, bool *temRestricoes, bool escreve);

#endif // CAMINHO_ORTOGONAL_H
