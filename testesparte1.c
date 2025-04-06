#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "parte1.h"
#include <stdlib.h>
#include <stdio.h>

// Teste para a função pintarBranco
void testarPintarBranco()
{
    TABELA t = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);

    // Pinta a célula (1, 1) de branco
    bool resultado = pintarBranco(t, 1, 1);
    CU_ASSERT_TRUE(resultado);
    CU_ASSERT_EQUAL(t->tabela[1][1], ' ');

    // Testa índices fora dos limites
    resultado = pintarBranco(t, -1, 1);
    CU_ASSERT_FALSE(resultado);

    resultado = pintarBranco(t, 3, 3);
    CU_ASSERT_FALSE(resultado);

    freeTabela(t);
}

// Teste para a função riscar
void testarRiscar()
{
    TABELA t = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);

    // Risca a célula (0, 0)
    bool resultado = riscar(t, 0, 0);
    CU_ASSERT_TRUE(resultado);
    CU_ASSERT_EQUAL(t->tabela[0][0], 'X');

    // Testa índices fora dos limites
    resultado = riscar(t, -1, 0);
    CU_ASSERT_FALSE(resultado);

    resultado = riscar(t, 3, 3);
    CU_ASSERT_FALSE(resultado);

    freeTabela(t);
}

// Teste para a função mostrarTabela
void testarMostrarTabela()
{
    TABELA t = malloc(sizeof(struct Tabela));
    initTabela(t, 3, 3);

    // Preenche o tabuleiro com valores
    t->tabela[0][0] = 'A';
    t->tabela[1][1] = 'B';
    t->tabela[2][2] = 'C';

    // Mostra o tabuleiro (não há asserts aqui, apenas visualização)
    printf("Tabuleiro esperado:\n");
    mostrarTabela(t);

    freeTabela(t);
}

// Teste para a função lerFicheiro
void testarLerFicheiro()
{
    TABELA t = lerFicheiro("testes.txt");
    CU_ASSERT_PTR_NOT_NULL(t);

    // Verifica as dimensões do tabuleiro
    CU_ASSERT_EQUAL(t->l, 3);
    CU_ASSERT_EQUAL(t->c, 3);

    // Verifica o conteúdo do tabuleiro
    CU_ASSERT_EQUAL(t->tabela[0][0], 'A');
    CU_ASSERT_EQUAL(t->tabela[1][1], 'B');
    CU_ASSERT_EQUAL(t->tabela[2][2], 'C');

    freeTabela(t);
}

int main()
{
    // Inicializa o registro de testes
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }

    // Cria o conjunto de testes
    CU_pSuite suite = CU_add_suite("Testes para parte1.c", NULL, NULL);
    if (NULL == suite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Adiciona os testes ao conjunto
    if ((NULL == CU_add_test(suite, "Teste de pintarBranco", testarPintarBranco)) ||
        (NULL == CU_add_test(suite, "Teste de riscar", testarRiscar)) ||
        (NULL == CU_add_test(suite, "Teste de mostrarTabela", testarMostrarTabela)) ||
        (NULL == CU_add_test(suite, "Teste de lerFicheiro", testarLerFicheiro)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Executa os testes usando a interface básica
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Limpa o registro de testes
    CU_cleanup_registry();
    return CU_get_error();
}