#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "../tipos.h"
#include "../parte1/parte1.h"
#include "../parte2/parte2.h" // para outras funções de tabela
#include "parte3.h"           // declara funções de parte3

// Stubs para funções de tabela
static TABELA criarTabela(int l, int c)
{
    TABELA t = malloc(sizeof(*t));
    t->l = l;
    t->c = c;
    t->tabela = malloc(l * sizeof(char *));
    for (int i = 0; i < l; i++)
    {
        t->tabela[i] = malloc(c * sizeof(char));
        memset(t->tabela[i], '.', c);
    }
    return t;
}

// Capture de stdout para silenciar funções que escrevem
static FILE *orig_stdout;
static void _begin_capture(void)
{
    orig_stdout = stdout;
    stdout = fopen("/dev/null", "w");
}

static void _end_capture(void)
{
    fclose(stdout);
    stdout = orig_stdout;
}

// Testa devolvePrimeiroNRiscado: retorna NULL quando tudo está riscado
void test_devolvePrimeiroNRiscado_tudo_riscado()
{
    TABELA t = criarTabela(2, 2);
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            t->tabela[i][j] = '#';
    COORDENADA p = devolvePrimeiroNRiscado(t);
    CU_ASSERT_PTR_NULL(p);
    freeTabela(t);
}

// Testa devolvePrimeiroNRiscado: encontra o primeiro livre
void test_devolvePrimeiroNRiscado_encontra()
{
    TABELA t = criarTabela(3, 3);
    for (int i = 0; i < 4; i++)
        t->tabela[i / 3][i % 3] = '#';
    COORDENADA p = devolvePrimeiroNRiscado(t);
    CU_ASSERT_PTR_NOT_NULL(p);
    CU_ASSERT_EQUAL(p->l, 1);
    CU_ASSERT_EQUAL(p->c, 1);
    free(p);
    freeTabela(t);
}

// Testa initVisited
void test_initVisited()
{
    TABELA t = criarTabela(2, 3);
    t->tabela[0][1] = '#';
    t->tabela[1][2] = '#';
    int count = 0;
    int **v = initVisited(t, &count);
    CU_ASSERT_EQUAL(v[0][0], 0);
    CU_ASSERT_EQUAL(v[0][1], 1);
    CU_ASSERT_EQUAL(v[1][2], 1);
    CU_ASSERT_EQUAL(count, 2);
    for (int i = 0; i < 2; i++)
        free(v[i]);
    free(v);
    freeTabela(t);
}

// Testa insertQueue e deleteQueue (inclui realloc)
void test_queue_insert_delete()
{
    CQUEUE q = malloc(sizeof(*q));
    initQueue(q);
    CU_ASSERT_EQUAL(q->capacidade, 3);
    CU_ASSERT_EQUAL(q->tamanho, 0);

    COORDENADA arr[5];
    for (int i = 0; i < 5; i++)
    {
        arr[i] = malloc(sizeof(struct Coordenada));
        arr[i]->l = i;
        arr[i]->c = i;
    }
    CU_ASSERT_TRUE(insertQueue(q, arr, 5));
    CU_ASSERT(q->capacidade >= 5);
    CU_ASSERT_EQUAL(q->tamanho, 5);

    for (int i = 0; i < 5; i++)
    {
        COORDENADA p = NULL;
        deleteQueue(q, &p);
        CU_ASSERT_PTR_NOT_NULL(p);
        CU_ASSERT_EQUAL(p->l, i);
        CU_ASSERT_EQUAL(p->c, i);
        free(p);
    }
    CU_ASSERT_EQUAL(q->tamanho, 0);
    free(q->list);
    free(q);
}

// Testa devolveNaoVisitados
void test_devolveNaoVisitados()
{
    int rows = 2, cols = 2, length = 2;
    int **v = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        v[i] = malloc(cols * sizeof(int));
    v[0][0] = 1;
    v[0][1] = 0;
    v[1][0] = 0;
    v[1][1] = 1;
    COORDENADA *nv = devolveNaoVisitados(v, rows, cols, length);
    bool found01 = false, found10 = false;
    for (int i = 0; i < length; i++)
    {
        if (nv[i]->l == 0 && nv[i]->c == 1)
            found01 = true;
        if (nv[i]->l == 1 && nv[i]->c == 0)
            found10 = true;
        free(nv[i]);
    }
    CU_ASSERT_TRUE(found01 && found10);
    free(nv);
    for (int i = 0; i < rows; i++)
        free(v[i]);
    free(v);
}

// Testa existeCaminhoOrtogonal (2x2 livre)
void test_existeCaminhoOrtogonal_simples()
{
    TABELA t = criarTabela(2, 2);
    CQUEUE q = malloc(sizeof(*q));
    initQueue(q);
    COORDENADA s = malloc(sizeof(struct Coordenada));
    s->l = 0;
    s->c = 0;
    insertQueue(q, (COORDENADA[]){s}, 1);
    int count = 0;
    int **v = initVisited(t, &count);
    CU_ASSERT_TRUE(existeCaminhoOrtogonal(q, t, v, &count));
    CU_ASSERT_EQUAL(count, 4);
    for (int i = 0; i < 2; i++)
        free(v[i]);
    free(v);
    free(q->list);
    free(q);
    freeTabela(t);
}

// Testa escrita e conta em trataCaminhoOrtogonal (erro)
void test_trataCaminhoOrtogonal_erro()
{
    TABELA t = criarTabela(3, 3);
    t->tabela[1][0] = '#';
    t->tabela[0][1] = '#';
    bool temR = false;
    int conta = 0;
    _begin_capture();
    bool r = trataCaminhoOrtogonal(t, &conta, &temR, false);
    _end_capture();
    CU_ASSERT_TRUE(r);
    CU_ASSERT_TRUE(temR);
    CU_ASSERT_EQUAL(conta, 1);
    freeTabela(t);
}

// Testa trataCaminhoOrtogonal sem restrições
void test_trataCaminhoOrtogonal_ok()
{
    TABELA t = criarTabela(2, 2);
    bool temR = false;
    int conta = 0;
    _begin_capture();
    bool r = trataCaminhoOrtogonal(t, &conta, &temR, true);
    _end_capture();
    CU_ASSERT_TRUE(r);
    CU_ASSERT_FALSE(temR);
    CU_ASSERT_EQUAL(conta, 0);
    freeTabela(t);
}
