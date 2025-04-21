#include <CUnit/Basic.h>
#include "parte2.h"
#include <stdlib.h>
#include <string.h>

// Função auxiliar para criar uma tabela
TABELA criarTabela(int l, int c)
{
    TABELA t = malloc(sizeof(struct Tabela));
    t->l = l;
    t->c = c;
    t->tabela = malloc(l * sizeof(char *));
    for (int i = 0; i < l; i++)
    {
        t->tabela[i] = malloc((c + 1) * sizeof(char));
    }
    return t;
}

// Teste da função initStackTabs
void test_initStackTabs()
{
    STACKTABS s = malloc(sizeof(struct StackTabs));
    CU_ASSERT_TRUE(initStackTabs(s));
    CU_ASSERT_EQUAL(s->capacidade, 0); // começa com 0
    CU_ASSERT_EQUAL(s->comprimento, 0);
    freeStackTabs(s);
}

// Teste da função insereTabela
void test_insereTabela()
{
    STACKTABS s = malloc(sizeof(struct StackTabs));
    initStackTabs(s);

    TABELA t1 = criarTabela(3, 3);
    CU_ASSERT_TRUE(insereTabela(s, t1));

    TABELA t2 = criarTabela(4, 4);
    CU_ASSERT_TRUE(insereTabela(s, t2));

    CU_ASSERT_EQUAL(s->comprimento, 2);

    freeStackTabs(s);
}

// Teste da função copiarTabela
void test_copiarTabela()
{
    TABELA t1 = criarTabela(3, 3);
    t1->tabela[0][0] = 'X';

    TABELA t2 = copiarTabela(t1);

    CU_ASSERT_PTR_NOT_NULL(t2);
    CU_ASSERT_EQUAL(t2->l, t1->l);
    CU_ASSERT_EQUAL(t2->c, t1->c);
    CU_ASSERT_EQUAL(t2->tabela[0][0], 'X');

    freeTabela(t1);
    freeTabela(t2);
}

// Teste da função deleteTabela
void test_deleteTabela()
{
    GAME g;
    g.stackTabs = malloc(sizeof(struct StackTabs));
    initStackTabs(g.stackTabs);

    TABELA t1 = criarTabela(2, 2);
    insereTabela(g.stackTabs, t1);

    g.tab = copiarTabela(t1);
    CU_ASSERT_FALSE(deleteTabela(&g)); // Só uma tabela, não pode apagar

    TABELA t2 = criarTabela(2, 2);
    insereTabela(g.stackTabs, t2);
    CU_ASSERT_TRUE(deleteTabela(&g)); // Agora já pode

    freeStackTabs(g.stackTabs);
    freeTabela(g.tab);
}

// Teste da função freeStackTabs
void test_freeStackTabs()
{
    STACKTABS s = malloc(sizeof(struct StackTabs));
    initStackTabs(s);
    TABELA t = criarTabela(2, 2);
    insereTabela(s, t);
    freeStackTabs(s);
}

// Teste da função dentroDosLimites
void test_dentroDosLimites()
{
    TABELA t = criarTabela(3, 3);
    CU_ASSERT_TRUE(dentroDosLimites(t, 2, 2));
    CU_ASSERT_FALSE(dentroDosLimites(t, 3, 3));
    freeTabela(t);
}

// Teste da função verificaRiscadaVizinhasBrancas
void test_verificaRiscadaVizinhasBrancas()
{
    TABELA t = criarTabela(3, 3);
    t->tabela[1][1] = '#';
    t->tabela[0][1] = 'A';
    t->tabela[2][1] = 'B';
    t->tabela[1][0] = 'C';
    t->tabela[1][2] = 'd'; // violação

    int restricoes[4][2] = {{0}};
    int num = verificaRiscadaVizinhasBrancas(t, 1, 1, restricoes);

    CU_ASSERT_EQUAL(num, 1);
    CU_ASSERT_EQUAL(restricoes[0][0], 1);
    CU_ASSERT_EQUAL(restricoes[0][1], 2);
    freeTabela(t);
}

// Teste da função verificaLetraIgualLinhaColuna
void test_verificaLetraIgualLinhaColuna()
{
    TABELA t = criarTabela(3, 3);
    t->tabela[0][0] = 'A';
    t->tabela[0][1] = 'a'; // mesma letra em minúscula na linha

    CU_ASSERT_TRUE(verificaLetraIgualLinhaColuna(t, 0, 0));
    freeTabela(t);
}

// Teste da função verificaRestricoes
void test_verificaRestricoes()
{
    TABELA t = criarTabela(3, 3);
    t->tabela[0][0] = '#';
    t->tabela[0][1] = 'a'; // vizinho inválido
    t->tabela[1][1] = 'B';
    t->tabela[1][2] = 'b'; // letra repetida em linha

    CU_ASSERT_TRUE(verificaRestricoes(t));
    freeTabela(t);
}