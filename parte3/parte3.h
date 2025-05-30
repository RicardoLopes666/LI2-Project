#ifndef PARTE3_H
#define PARTE3_H

#include "../tipos.h"
#include <stdbool.h> // Para o tipo bool

// Função que devolve a primeira coordenada não riscada na tabela.
// Retorna NULL se todas as casas estiverem riscadas.
COORDENADA devolvePrimeiroNRiscado(TABELA t);

// Inicializa o array de visitados e conta as casas riscadas.
// Parâmetros:
// - t: Tabuleiro atual.
// - count: Ponteiro para o contador de casas riscadas.
// Retorna um array bidimensional indicando as casas visitadas.
int **initVisited(TABELA t, int *count);

// Inicializa a fila (queue) com capacidade inicial.
// Parâmetros:
// - q: Ponteiro para a estrutura da fila.
void initQueue(CQUEUE q);

// Insere um array de coordenadas na fila.
// Parâmetros:
// - q: Ponteiro para a fila.
// - coordenadas: Array de coordenadas a serem inseridas.
// - coordSize: Número de coordenadas no array.
// Retorna 1 em caso de sucesso, 0 em caso de falha.
int insertQueue(CQUEUE q, COORDENADA *coordenadas, int coordSize);

// Remove o primeiro elemento da fila.
// Parâmetros:
// - q: Ponteiro para a fila.
// - pos: Ponteiro para armazenar a coordenada removida.
void deleteQueue(CQUEUE q, COORDENADA *pos);

// Verifica se existe um caminho ortogonal entre todas as casas não riscadas.
// Parâmetros:
// - q: Ponteiro para a fila.
// - t: Tabuleiro atual.
// - visited: Array bidimensional indicando as casas visitadas.
// - count: Ponteiro para o contador de casas visitadas.
// Retorna 1 se todas as casas não riscadas estiverem conectadas, 0 caso contrário.
int existeCaminhoOrtogonal(CQUEUE q, TABELA t, int **visited, int *count);

// Verifica se existe um caminho ortogonal entre todas as casas não riscadas.
// Caso não exista, escreve as restrições no terminal.
// Parâmetros:
// - t: Tabuleiro atual.
// - contaRestricoes: Ponteiro para o contador de restrições.
// - temRestricoes: Ponteiro para indicar se há restrições.
// - escreve: Indica se as restrições devem ser escritas no terminal.
// Retorna true se todas as casas não riscadas estiverem conectadas, false caso contrário.
bool trataCaminhoOrtogonal(TABELA t, int *contaRestricoes, bool *temRestricoes, bool escreve);

// Função que, dado um array de visitados, devolve um array com as posições que não foram visitadas.
// Parâmetros:
// - visited: Array bidimensional indicando as casas visitadas (1 -> visitada, 0 -> não visitada).
// - l: Número de linhas do tabuleiro.
// - c: Número de colunas do tabuleiro.
// - length: Número de casas não visitadas (calculado previamente).
// Retorna um array de coordenadas das casas não visitadas.
COORDENADA *devolveNaoVisitados(int **visited, int l, int c, int length);

#endif // PARTE3_H