#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "parte3.h"
#include "../parte2/parte2.h"
#include "../parte2/testesparte2.h"

// Teste da função devolvePrimeroNRiscado
void test_devolvePrimeroNRiscado()
{
    TABELA t = criarTabela(3, 3);
    t->tabela[0][0] = '#'; // Riscado
    t->tabela[0][1] = '#'; // Riscado
    t->tabela[0][2] = '.'; // Não riscado

    COORDENADA pos = devolvePrimeroNRiscado(t);
    CU_ASSERT_PTR_NOT_NULL(pos);
    CU_ASSERT_EQUAL(pos->l, 0);
    CU_ASSERT_EQUAL(pos->c, 2);

    free(pos);
    freeTabela(t);
}

// Teste da função initVisited
void test_initVisited()
{
    TABELA t = criarTabela(3, 3);
    t->tabela[0][0] = '#'; // Riscado

    int count = 0;
    int **visited = initVisited(t, &count);

    CU_ASSERT_EQUAL(visited[0][0], 1); // Riscado
    CU_ASSERT_EQUAL(visited[0][1], 0); // Não riscado
    CU_ASSERT_EQUAL(count, 1);         // Apenas uma casa riscada

    for (int i = 0; i < t->l; i++)
    {
        free(visited[i]);
    }
    free(visited);
    freeTabela(t);
}

// Teste da função insertQueue
void test_insertQueue()
{
    CQUEUE q = malloc(sizeof(struct CQueue));
    initQueue(q);

    COORDENADA c1 = malloc(sizeof(struct Coordenada));
    c1->l = 0;
    c1->c = 0;

    COORDENADA c2 = malloc(sizeof(struct Coordenada));
    c2->l = 1;
    c2->c = 1;

    COORDENADA coords[] = {c1, c2};
    CU_ASSERT_TRUE(insertQueue(q, coords, 2));
    CU_ASSERT_EQUAL(q->tamanho, 2);

    free(c1);
    free(c2);
    free(q->list);
    free(q);
}

// Teste da função deleteQueue
void test_deleteQueue()
{
    CQUEUE q = malloc(sizeof(struct CQueue));
    initQueue(q);

    COORDENADA c1 = malloc(sizeof(struct Coordenada));
    c1->l = 0;
    c1->c = 0;

    COORDENADA coords[] = {c1};
    CU_ASSERT_TRUE(insertQueue(q, coords, 1)); // Insere um elemento na fila

    COORDENADA pos = NULL; // Inicializa o ponteiro como NULL
    deleteQueue(q, &pos);  // Passa o endereço de pos

    CU_ASSERT_PTR_NOT_NULL(pos); // Verifica se o ponteiro foi atualizado
    CU_ASSERT_EQUAL(pos->l, 0);
    CU_ASSERT_EQUAL(pos->c, 0);
    CU_ASSERT_EQUAL(q->tamanho, 0);

    free(pos);     // liberta o elemento removido
    free(q->list); // liberta a lista da fila
    free(q);       // liberta a estrutura da fila
}

// Teste da função existeCaminhoOrtogonal
void test_existeCaminhoOrtogonal()
{
    TABELA t = criarTabela(3, 3);
    t->tabela[0][0] = '#'; // Riscado

    CQUEUE q = malloc(sizeof(struct CQueue));
    initQueue(q);

    COORDENADA inicial = malloc(sizeof(struct Coordenada));
    inicial->l = 0;
    inicial->c = 1;

    COORDENADA coords[] = {inicial};
    insertQueue(q, coords, 1);

    int count = 0;
    int **visited = initVisited(t, &count);

    CU_ASSERT_TRUE(existeCaminhoOrtogonal(q, t, visited, &count));

    for (int i = 0; i < t->l; i++)
    {
        free(visited[i]);
    }
    free(visited);
    free(q->list);
    free(q);
    freeTabela(t);
}
