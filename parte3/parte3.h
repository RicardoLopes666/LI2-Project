#ifndef PARTE3_H
#define PARTE3_H

#include <stdbool.h>
#include "../tipos.h"
#include "../parte2/parte2.h"

// Função que devolve a primeira coordenada não riscada em uma tabela
COORDENADA devolvePrimeiroNRiscado(TABELA t);

// Função que inicializa o array de visitados, com 0 para não visitado e 1 para visitado
int **initVisited(TABELA t, int *count);

// Função que inicializa a fila (queue)
void initQueue(CQUEUE q);

// Função que efetivamente coloca os elementos novos na queue
void enqueue(CQUEUE q, COORDENADA *coordenadas, int coordSize);

// Função para inserir coordenadas na fila
int insertQueue(CQUEUE q, COORDENADA *coordenadas, int coordSize);

// Função que remove o elemento da frente da fila
void deleteQueue(CQUEUE q, COORDENADA *pos);

// Função que devolve um array de coordenadas não visitadas
COORDENADA *devolveNaoVisitados(int **visited, int l, int c, int length);

// Função que atualiza o estado (dos vizinhos e acrescenta o novo vizinho) para uma posição que é válida
void atualizaEstado(int linha, int coluna, int **visited, COORDENADA *vizinhos, int *numeroVizinhosValidos);

// Algoritmo de busca em largura para verificar caminho ortogonal
int existeCaminhoOrtogonal(CQUEUE q, TABELA t, int **visited, int *count);

// Função que escreve as restrições encontradas quando não há caminho ortogonal
void escreveInexistenciaCaminho(bool *temRestricoes, bool escreve, int *contaRestricoes, TABELA t, int **visited, int colunaI, int linhaI, int count);

// Função que liberta o espaço alocado previamente
void libertaEspaco(COORDENADA *primeiro, int **visited, TABELA t, CQUEUE queue);

// Função que trata da verificação do caminho ortogonal
bool trataCaminhoOrtogonal(TABELA t, int *contaRestricoes, bool *temRestricoes, bool escreve);

#endif // PARTE3_H
