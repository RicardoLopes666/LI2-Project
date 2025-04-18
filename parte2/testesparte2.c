#include <CUnit/Basic.h>
#include "parte2.h" // Inclui o ficheiro com as funções a testar
#include <stdlib.h>

// Função auxiliar para testar se uma tabela foi alocada corretamente
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
    initStackTabs(s);
    CU_ASSERT_EQUAL(s->capacidade, 3);
    CU_ASSERT_EQUAL(s->comprimento, 0);
    freeStackTabs(s);
}

// Teste da função insereTabela
void test_insereTabela()
{
    STACKTABS s = malloc(sizeof(struct StackTabs));
    initStackTabs(s);

    TABELA t1 = criarTabela(3, 3);
    CU_ASSERT_TRUE(insereTabela(s, t1)); // Inserir primeira tabela

    TABELA t2 = criarTabela(4, 4);
    CU_ASSERT_TRUE(insereTabela(s, t2)); // Inserir segunda tabela

    CU_ASSERT_EQUAL(s->comprimento, 2); // Verifica se o comprimento aumentou para 2

    freeStackTabs(s);
    free(t1);
    free(t2);
}

// Teste da função deleteTabela
void test_deleteTabela()
{
    GAME game;
    game.stackTabs = malloc(sizeof(STACKTABS));
    initStackTabs(game.stackTabs);

    TABELA t1 = criarTabela(3, 3);
    insereTabela(game.stackTabs, t1);

    CU_ASSERT_TRUE(deleteTabela(&game)); // Verifica se a tabela pode ser deletada

    freeStackTabs(game.stackTabs);
    free(t1);
}

// Teste da função copiarTabela
void test_copiarTabela()
{
    TABELA t1 = criarTabela(3, 3);
    t1->tabela[0][0] = 'X'; // Altera um valor para testar a cópia

    TABELA t2 = copiarTabela(t1);

    CU_ASSERT_PTR_NOT_NULL(t2);             // Verifica se a cópia foi bem-sucedida
    CU_ASSERT_EQUAL(t2->tabela[0][0], 'X'); // Verifica se o valor foi copiado corretamente

    free(t1->tabela);
    free(t1);
    free(t2->tabela);
    free(t2);
}

// Teste da função freeStackTabs
void test_freeStackTabs()
{
    STACKTABS s = malloc(sizeof(struct StackTabs));
    initStackTabs(s);

    TABELA t1 = criarTabela(3, 3);
    insereTabela(s, t1);

    freeStackTabs(s); // Verifica se a memória é libertada sem erros
}

// Teste da função dentroDosLimites
void test_dentroDosLimites()
{
    TABELA t = criarTabela(3, 3);
    CU_ASSERT_TRUE(dentroDosLimites(t, 1, 1));  // Dentro dos limites
    CU_ASSERT_FALSE(dentroDosLimites(t, 3, 3)); // Fora dos limites

    free(t->tabela);
    free(t);
}

// Teste da função verificaRiscadaVizinhasBrancas
void test_verificaRiscadaVizinhasBrancas()
{
    TABELA t = criarTabela(3, 3);
    t->tabela[1][1] = '#'; // Casa riscada no meio

    int restricoes[4][2] = {{0}};
    int numRestricoes = verificaRiscadaVizinhasBrancas(t, 1, 1, restricoes);

    CU_ASSERT_EQUAL(numRestricoes, 4);    // Verifica se há 4 vizinhos
    CU_ASSERT_EQUAL(restricoes[0][0], 0); // Verifica se as coordenadas estão corretas

    free(t->tabela);
    free(t);
}

// Teste da função verificaRestrições
void test_verificaRestrições()
{
    TABELA t = criarTabela(3, 3);
    t->tabela[0][0] = '#'; // Casa riscada

    CU_ASSERT_TRUE(verificaRestrições(t)); // Verifica se há restrições

    free(t->tabela);
    free(t);
}
